#include "screens/recommendations.h"   // Includes the Recommendations screen class
#include "core/lucideicons.h"          // Provides Lucide icon helper functions
#include "core/screen.h"               // Base Screen class
#include <QVBoxLayout>                 // Vertical layout manager
#include <QHBoxLayout>                 // Horizontal layout manager
#include <QLabel>                      // Text/image label widget
#include <QFrame>                      // Frame widget used for cards, dividers, and styled boxes
#include <QScrollArea>                 // Scrollable container widget
#include <QPushButton>                 // Clickable button widget
#include <QStackedWidget>              // Widget stack for switching between pages
#include <QSizePolicy>                 // Controls widget resizing behavior
#include <QLineEdit>                   // Single-line text input widget
#include <QGridLayout>                 // Grid layout manager
#include <QDesktopServices>            // Opens URLs/files using system default apps
#include <QDir>                        // Directory handling
#include <QFile>                       // File handling
#include <QFileInfo>                   // File path and file metadata handling
#include <QStandardPaths>              // Access system folders like Cache/Desktop/Documents
#include <QUrl>                        // URL handling
#include <QColor>                      // Color handling
#include <functional>                  // std::function callback support

// ─────────────────────────────────────────────────────────────────────────────
// Data
// These structs and lists store the resource information shown in the UI.
// ─────────────────────────────────────────────────────────────────────────────

// Stores one resource card's information
struct ResourceInfo {
    QString icon;                      // Icon name or emoji-style icon key
    QString color;                     // Accent/background color for icon/card
    QString title;                     // Resource title
    QString description;               // Short description shown on the card
    QString details;                   // Contact details, hours, location, etc.
    QString url;                       // Main link or target
    QStringList tags;                  // Small category chips shown on card
    QString url2 = {};                 // Optional second link
    QString url2Label = {};            // Optional label for second link
    QStringList extraLinks = {};       // Label/url pairs for cards with many actions
};

// Stores one main topic card's information
struct TopicInfo {
    QString icon;                      // Topic icon
    QString accent;                    // Background color for icon badge
    QString title;                     // Topic title
    QString subtitle;                  // Subtitle shown on landing card
};

// Stores information for internal subpages
struct ResourceSubpageInfo {
    QString key;                       // Internal key used to open this subpage
    int parentTopicIndex;              // Which main topic this subpage belongs to
    QString icon;                      // Subpage icon
    QString color;                     // Accent color
    QString title;                     // Subpage title
    QString subtitle;                  // Short subpage subtitle
    QString details;                   // Details/contact text
    QString url;                       // Main external URL
    QString urlLabel;                  // Button label for main URL
    QStringList tags;                  // Tags shown on subpage
    QStringList childLinks = {};       // Label/url pairs for hub page tool buttons
};

// Main topic list shown on the landing page
static const QList<TopicInfo> TOPICS = {
    { "📚", "#dcfff1", "Exam & Study Stress",        "Tutoring, writing help, academic coaching" },
    { "💰", "#f4ffb8", "Finance",                    "Financial aid, emergency funds, free food"  },
    { "🌐", "#dff6ff", "Immigration",                "Legal help, DACA, undocumented support"     },
    { "💬", "#e9fff3", "Relationships & Family",     "Personal counseling, peer support"          },
    { "🏥", "#e7fbff", "Health & Wellness",          "Medical care, mental health, insurance"     },
    { "💼", "#f7efdf", "Job & Career",               "Career center, resumes, internships, jobs"  },
    };

// ─────────────────────────────────────────────────────────────────────────────
// Resource cards
// Each inner list belongs to one topic index from TOPICS.
// Example: RESOURCES[0] belongs to Exam & Study Stress.
// ─────────────────────────────────────────────────────────────────────────────

static const QList<QList<ResourceInfo>> RESOURCES = {

// 0 — Exam & Study
{
    { "📚", "#edf7ee", "Learning Resource Center — Tutoring",
     "Free peer tutoring for Math, English, Science, Business, and more. Walk-in or book online. No appointment needed.",
     "📍  199 Chambers St., Room S-510\n"
     "    245 Greenwich St., Fiterman Hall Room F-511\n"
     "🕐  In-person (Chambers): Mon–Thu 10am–6pm  |  Fri–Sat 10am–5pm\n"
     "    In-person (Fiterman): Mon–Wed 10am–3pm  |  Thu 10am–2pm\n"
     "    Remote: Mon–Thu 9am–9pm\n"
     "📞  (212) 220-1383  |  lrctutoring@bmcc.cuny.edu",
     "https://www.bmcc.cuny.edu/students/lrc/in-person-tutoring/",
     {"Free", "Walk-in", "In-person & Remote"} },

    { "🤝", "#eef8f1", "Peer Assisted Learning (PAL)",
     "Small-group study sessions (max 5 students) led by a trained PAL leader for supported courses. Great for processing difficult course material together.",
     "📍  199 Chambers St., Room S-510\n"
     "📞  (212) 220-1383  |  lrctutoring@bmcc.cuny.edu",
     "https://www.bmcc.cuny.edu/students/lrc/in-person-tutoring/peer-assisted-learning/",
     {"Small Group", "Free", "Collaborative"} },

    { "✏️", "#e2f4e8", "Writing Center",
     "One-on-one help with any writing assignment — essays, research papers, reports, and more. All levels welcome.",
     "📍  199 Chambers St., Room S-510\n"
     "🕐  Mon–Thu 10am–6pm  |  Sat 10am–3pm  |  Sun: online only\n"
     "📞  (212) 220-1384  |  writingcenter@bmcc.cuny.edu",
     "https://www.bmcc.cuny.edu/students/writing-center/",
     {"Writing", "Free", "In-person & Online"} },

    { "🎯", "#f1f8e8", "Academic Coaching",
     "Personalized one-on-one coaching for study strategies, time management, organization, and test preparation.",
     "📍  199 Chambers St., Room S-510\n"
     "🕐  Mon–Thu 10am–6pm  |  Fri 10am–5pm\n"
     "📧  AcademicCoaching@bmcc.cuny.edu",
     "https://www.bmcc.cuny.edu/students/lrc/academic-coaching/",
     {"Study Skills", "Test Prep", "Free"} },

    { "💻", "#e8f7ef", "Online Tutoring",
     "Join a live remote tutoring session from home. Covers the same subjects as in-person — no commute needed.",
     "📞  (212) 220-8359  |  onlinetutoring@bmcc.cuny.edu\n"
     "🕐  Mon–Thu 9am–9pm  (extended remote hours)",
     "https://www.bmcc.cuny.edu/students/lrc/online-tutoring/tutoring-schedule/",
     {"Remote", "Flexible Hours", "Free"},
     "https://bmcc.upswing.io/up/dashboard/home", "Upswing →" },

    { "🧰", "#eef8f1", "Free Software for Students — Digital Education Center",
     "Access free BMCC and CUNY digital learning tools for classes, online meetings, assignments, and skill building.",
     "CONTACT: Digital Education Center\n"
     "📧  digitaleducation@bmcc.cuny.edu\n"
     "📞  (212) 220-1243\n"
     "📍  199 Chambers St, Room S-510a, New York, NY 10007\n"
     "🕐  Monday – Friday: 9 a.m.–5 p.m.\n"
     "Includes Brightspace, Zoom, Microsoft Office 365, LinkedIn Learning, VoiceThread, and CUNY Technology Services.",
     "",
     {"Free Software", "Digital Tools", "Student Support"},
     "", "",
     {
         "View Software Tools →", "subpage:free-software"
     } },
},

    // 1 — Finance
    {
     { "💰", "#e2f4e8", "Financial Aid Office",
      "Grants, loans, work-study, and scholarships. Speak with an advisor.",
      "📞 (212) 220-1430  |  finaid@bmcc.cuny.edu",
      "https://www.bmcc.cuny.edu/finaid/",
      {"Grants", "Scholarships"} },

     { "🆘", "#f1f8e8", "Student Emergency Fund",
      "Emergency grants for textbooks, rent, utilities, and urgent needs.",
      "📍 Room S-230  |  Mon–Fri 8am–6:30pm  |  Sat 10am–1pm\n📞 (212) 220-8195",
      "https://www.bmcc.cuny.edu/student-affairs/arc/student-emergency-fund/",
      {"Emergency", "Fast Support"} },

     { "🍎", "#edf7ee", "Panther Pantry — Free Food",
      "Free food for students facing food insecurity. Walk in, no questions asked.",
      "📍 Room S-230  |  Mon–Fri 8am–6:30pm  |  Sat 10am–1pm",
      "https://www.bmcc.cuny.edu/student-affairs/arc/panther-pantry/",
      {"Free Food", "Walk-in"} },

     { "🏛️", "#e2f4e8", "Advocacy & Resource Center",
      "SNAP assistance, tax prep, housing referrals, and financial counseling.",
      "📍 Room S-230  |  Mon–Fri 8am–6:30pm  |  Sat 10am–1pm",
      "https://www.bmcc.cuny.edu/student-affairs/arc/",
      {"SNAP", "Housing", "Tax Help"} },
     },

    // 2 — Immigration
    {
     { "🌐", "#edf7ee", "Immigrant Resource Center",
      "Confidential support for immigrant and undocumented students.",
      "📞 Albert Lee: (212) 776-6252  |  allee@bmcc.cuny.edu\n📍 Room S-230  |  Mon–Fri 8am–6:30pm",
      "https://www.bmcc.cuny.edu/student-affairs/center-for-belonging-and-inclusion/immigrant-resource-center/",
      {"Confidential", "All Statuses"} },

     { "⚖️", "#e2f4e8", "Free Legal Services — CUNY Citizenship Now!",
      "On-site attorneys monthly for DACA, TPS, family petitions, and naturalization.",
      "📍 Room S-230",
      "https://www.bmcc.cuny.edu/student-affairs/arc/legal-services/",
      {"DACA", "Legal Aid", "Free"} },

     { "🎓", "#f1f8e8", "Scholarships for Immigrant Students",
      "TheDream.US and other scholarships available regardless of status.",
      "",
      "https://www.bmcc.cuny.edu/immigration/scholarships-and-financial-aid/",
      {"Scholarships", "DREAMers"} },

     { "🔒", "#edf7ee", "Know Your Rights at BMCC",
      "CUNY will not share your records with immigration enforcement without a court order.",
      "",
      "qrc:/documents/2025-KYR-Final-01.13.202592.pdf",
      {"Privacy", "Your Rights"} },
     },

    // 3 — Relationships & Family
    {
     { "💬", "#edf7ee", "Personal Counseling — Counseling Center",
      "Free, confidential counseling for relationship, family, and personal issues.",
      "📍 Room S-343  |  Mon, Tue, Thu 9am–6pm  |  Wed, Fri 9am–5pm\n"
      "📞 (212) 220-8140  |  counselingcenter@bmcc.cuny.edu",
      "https://www.bmcc.cuny.edu/student-affairs/counseling/counseling-services/",
      {"Free", "Confidential", "Telehealth OK"} },

     { "🤝", "#e2f4e8", "Togetherall — 24/7 Peer Support",
      "Free anonymous online community for students to support each other.",
      "",
      "https://togetherall.com/en-us/",
      {"Anonymous", "24/7", "Online"} },

     { "📋", "#f1f8e8", "Book a Counseling Appointment",
      "Schedule in person, by phone, or on Zoom. Crisis support also available.",
      "📞 (212) 220-8140",
      "https://www.bmcc.cuny.edu/student-affairs/counseling/request-an-appointment/",
      {"Appointment", "Crisis"} },

     { "🏘️", "#edf7ee", "Community Referrals",
      "Referrals to outside help for housing, domestic support, and legal aid.",
      "",
      "https://www.bmcc.cuny.edu/student-affairs/counseling/counseling-resources/community-resources/",
      {"Housing", "Legal Aid"} },
     },

    // 4 — Health & Wellness
    {
     { "🏥", "#e2f4e8", "Health & Wellness Services",
      "First aid, medical evaluation, health counseling, and wellness workshops.",
      "📍  Room N-380, 199 Chambers St.\n"
      "🕐  Mon – Fri: 9 a.m. – 5 p.m.\n"
      "📞  Tel: (212) 220-8256  |  Fax: (212) 220-2367\n"
      "📧  healthservices@bmcc.cuny.edu",
      "https://www.bmcc.cuny.edu/student-affairs/health-services/",
      {"Medical", "On-campus", "Free"} },

     { "🧠", "#edf7ee", "Mental Health Counseling",
      "Short-term counseling, crisis support, and referrals to community providers.",
      "📍  Room S-343\n"
      "🕐  Mon, Tue, Thu 9am–6pm  |  Wed, Fri 9am–5pm\n"
      "📞  (212) 220-8140  |  counselingcenter@bmcc.cuny.edu",
      "https://www.bmcc.cuny.edu/student-affairs/counseling/",
      {"Mental Health", "Free", "Crisis"} },

     { "🏋️", "#eef7f1", "Campus Fitness",
      "Fitness Center, Swimming Pool, and Health & Wellness Services — accessible exercise options for every student.",
      "CONTACT: BMCC Athletics\n"
      "📞  (212) 220-8260",
      "https://www.bmcc.cuny.edu/students/fitness-center/",
      {"Fitness", "On-campus", "Wellness"},
      "https://bmccathletics.com/sports/2012/4/2/RecreationSchedule.aspx",
      "Schedule →" },

     { "🎖️", "#f3faef", "Veterans Resource Center — Stress & Crisis Support",
      "Dedicated support for veteran students dealing with stress, PTSD, and mental health challenges. Request a virtual or in-person appointment.",
      "📍  199 Chambers St., Room S-115M\n"
      "📞  (212) 220-8035  |  saruiz@bmcc.cuny.edu\n"
      "👤  Samantha Ruiz, LCSW — Manager for Veterans Affairs & Student Development",
      "https://www.bmcc.cuny.edu/student-affairs/veterans-resource-center/dealing-with-stress/",
      {"Veterans", "PTSD", "Free"} },

     { "☎️", "#e9f5ef", "Samaritans — 24-Hour Crisis Hotline",
      "Free and confidential support if you are feeling stressed, overwhelmed, or suicidal — or if you're worried about a friend or family member. Available around the clock.",
      "📞  (212) 673-3000  —  free, confidential, 24 hours a day",
      "",
      {"24/7", "Free", "Confidential"} },

     { "🪖", "#dff1e5", "National Center for PTSD",
      "Information and resources from the U.S. Department of Veterans' Affairs for veterans and their families dealing with PTSD.",
      "",
      "https://www.ptsd.va.gov/",
      {"Veterans", "PTSD", "U.S. VA"} },

     { "💪", "#eef7f1", "Combat Stress Recovery Program",
      "Support program from the Wounded Warrior Project specifically designed to help veterans recover from combat stress and related mental health challenges.",
      "",
      "https://www.woundedwarriorproject.org/programs/mental-wellness",
      {"Veterans", "Combat Stress", "Wounded Warrior"} },

     { "🩺", "#f1f8e8", "Health Insurance Enrollment",
      "Help enrolling in NY State health insurance — open to all students including international and undocumented.",
      "📍  Room S-230",
      "https://www.bmcc.cuny.edu/student-affairs/arc/health-insurance/",
      {"Insurance", "All Students"} },

     },

    // 5 — Job & Career
    {
        { "💼", "#f7efdf", "Center for Career Development",
         "Start here for career planning, job search support, internships, resume help, and interview preparation.",
         "📍  199 Chambers Street, Room S-103\n"
         "    New York, NY 10007\n"
         "📧  career@bmcc.cuny.edu\n"
         "📞  (212) 220-8170\n"
         "🕐  Drop-in Hours: 2 p.m.–4 p.m.",
         "https://www.bmcc.cuny.edu/student-affairs/career/",
         {"Career Center", "Drop-in", "Jobs"},
         "https://bit.ly/ccdvfrontdesk",
         "Virtual Front Desk →" },

        { "🧰", "#eef8f1", "Career Toolkit",
         "Use BMCC's career toolkit to organize your job search, prepare application materials, and build confidence before applying.",
         "Helpful for choosing a direction, preparing for interviews, finding opportunities, and understanding what employers expect.",
         "https://www.bmcc.cuny.edu/student-affairs/career/student-resources/develop-a-career-toolkit/",
         {"Career Planning", "Job Search", "Interview Prep"} },

        { "📄", "#e7fbff", "Resume & Cover Letter Tools",
         "Build a cleaner resume or cover letter quickly, then bring your draft to BMCC Career Development for feedback.",
         "Use this as a starting point for first drafts, class assignments, internship applications, and part-time job applications.",
         "https://quick-resume-builder-three.vercel.app/",
         {"Resume", "Cover Letter", "Builder"} },

        { "🚀", "#edf7ee", "Internships — Vault",
         "Explore internship and career research tools through BMCC's Vault access. Useful for learning about industries, companies, and application pathways.",
         "Vault can help you compare career fields, prepare for interviews, and research employers before applying.",
         "https://www.bmcc.cuny.edu/student-affairs/career/vault/",
         {"Internships", "Vault", "Career Research"} },
        },
    };

// Internal subpages opened from resource cards
static const QList<ResourceSubpageInfo> RESOURCE_SUBPAGES = {
    { "free-software", 0, "🧰", "#eef8f1", "Free Software for Students",
     "BMCC Digital Education Center software support and student tool directory.",
     "Use this page first when you need the official BMCC list of free and supported student software.\n"
     "Contact Digital Education Center if you are blocked by login, access, setup, or class technology problems.\n"
     "Email: digitaleducation@bmcc.cuny.edu\nPhone: (212) 220-1243\nLocation: 199 Chambers St, Room S-510a\nHours: Monday-Friday 9 a.m.-5 p.m.",
     "https://www.bmcc.cuny.edu/academics/digitaleducation/student-support/software-for-students/",
     "Open Free Software Page →",
     {"Software", "DEC", "Student Support"},
     {
         "Brightspace", "https://brightspace.cuny.edu/d2l/home",
         "Zoom", "https://www.bmcc.cuny.edu/academics/digitaleducation/student-support/zoom-for-students/",
         "Office 365", "https://www.cuny.edu/about/administration/offices/cis/technology-services/microsoft-office-365-for-education/",
         "LinkedIn Learning", "https://www.linkedin.com/learning-login/",
         "VoiceThread", "https://voicethread.com/howto/",
         "CUNY Tech Services", "https://www.cuny.edu/about/administration/offices/cis/technology-services/#student"
     } },
};

// Creates a small rounded information chip/tag label
static QLabel* makeInfoChip(const QString &text,
                            const QString &bg = "#f5f0e2",
                            const QString &fg = "#3b5645",
                            const QString &border = "#c9d7c2") {
    QLabel *chip = new QLabel(text);
    chip->setStyleSheet(
        QString("font-size:11px; font-weight:600; color:%1; background:%2; "
                "border:1px solid %3; border-radius:8px; padding:5px 10px;")
            .arg(fg, bg, border));
    return chip;
}

// Opens a resource target.
// It supports qrc embedded files, local files, and regular web URLs.
static bool openResourceTarget(const QString &target) {

    // If target is a Qt resource file
    if (target.startsWith("qrc:/")) {

        // Convert qrc:/ path into Qt resource path format
        const QString resourcePath = ":" + target.mid(4);

        // Get writable cache directory
        QString cacheDirPath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);

        // Fallback to temp path if cache path is unavailable
        if (cacheDirPath.isEmpty())
            cacheDirPath = QDir::tempPath() + "/MindEase";

        QDir cacheDir(cacheDirPath);

        // Create cache directory if it does not exist
        if (!cacheDir.exists() && !cacheDir.mkpath("."))
            return false;

        // Output path where embedded resource will be copied
        const QString outputPath = cacheDir.filePath(QFileInfo(resourcePath).fileName());

        // Remove old cached file if it exists
        if (QFile::exists(outputPath))
            QFile::remove(outputPath);

        // Copy embedded resource to local cache
        if (!QFile::copy(resourcePath, outputPath))
            return false;

        // Open copied file using system default app
        return QDesktopServices::openUrl(QUrl::fromLocalFile(outputPath));
    }

    // Check if target is a local file
    const QFileInfo localFile(target);
    if (localFile.exists())
        return QDesktopServices::openUrl(QUrl::fromLocalFile(localFile.absoluteFilePath()));

    // Otherwise treat target as web URL
    return QDesktopServices::openUrl(QUrl(target));
}

// Normalizes text for search matching
static QString normalizedText(const QString &text) {
    return text.simplified().toLower();
}

// Stores search result data for one topic
struct TopicSearchResult {
    int score = -1;                    // Search score, -1 means no match
    QStringList matchedSubtopics;      // Resource/subpage titles that matched
};

// Scores how well a text field matches the query
static int scoreTextMatch(const QString &text,
                          const QString &query,
                          const QStringList &terms,
                          int exact,
                          int prefix,
                          int contains,
                          int allTermsBase) {
    // Normalize input text before comparing
    const QString normalized = normalizedText(text);

    // Empty text cannot match
    if (normalized.isEmpty()) return -1;

    int score = -1;

    // Strongest match: exact text match
    if (normalized == query) score = qMax(score, exact);

    // Medium match: text starts with query
    else if (normalized.startsWith(query)) score = qMax(score, prefix);

    // Lower match: text contains query
    else if (normalized.contains(query)) score = qMax(score, contains);

    int termHits = 0;

    // Count how many query terms are found
    for (const QString &term : terms) {
        if (normalized.contains(term))
            termHits++;
    }

    // Bonus if all query terms appear
    if (!terms.isEmpty() && termHits == terms.size())
        score = qMax(score, allTermsBase + termHits * 10);

    return score;
}

// Searches one topic and its resources/subpages
static TopicSearchResult searchTopic(int topicIndex, const QString &queryText) {

    // Normalize search text
    const QString query = normalizedText(queryText);

    // Empty query returns empty result
    if (query.isEmpty()) return {};

    TopicSearchResult result;

    // Get topic data
    const TopicInfo &topic = TOPICS[topicIndex];

    // Split search into words
    const QStringList terms = query.split(' ', Qt::SkipEmptyParts);

    // Search topic title
    result.score = qMax(result.score,
                        scoreTextMatch(topic.title, query, terms, 420, 320, 240, 260));

    // Search topic subtitle
    result.score = qMax(result.score,
                        scoreTextMatch(topic.subtitle, query, terms, 220, 170, 120, 190));

    // Search all resource cards under this topic
    for (const ResourceInfo &resource : RESOURCES[topicIndex]) {
        int resourceScore = -1;

        // Search resource title
        resourceScore = qMax(resourceScore,
                             scoreTextMatch(resource.title, query, terms, 260, 220, 190, 210));

        // Search resource description
        resourceScore = qMax(resourceScore,
                             scoreTextMatch(resource.description, query, terms, 170, 150, 130, 150));

        // Search resource details
        resourceScore = qMax(resourceScore,
                             scoreTextMatch(resource.details, query, terms, 110, 100, 90, 100));

        // Search resource tags
        for (const QString &tag : resource.tags) {
            resourceScore = qMax(resourceScore,
                                 scoreTextMatch(tag, query, terms, 210, 180, 150, 170));
        }

        // Search extra link labels
        for (int i = 0; i + 1 < resource.extraLinks.size(); i += 2) {
            resourceScore = qMax(resourceScore,
                                 scoreTextMatch(resource.extraLinks[i], query, terms, 190, 160, 130, 150));
        }

        // If resource matched, update topic result
        if (resourceScore >= 0) {
            result.score = qMax(result.score, resourceScore);
            result.matchedSubtopics.append(resource.title);
        }
    }

    // Search internal subpages connected to this topic
    for (const ResourceSubpageInfo &subpage : RESOURCE_SUBPAGES) {

        // Skip subpages from other topics
        if (subpage.parentTopicIndex != topicIndex)
            continue;

        int subpageScore = -1;

        // Search subpage title
        subpageScore = qMax(subpageScore,
                            scoreTextMatch(subpage.title, query, terms, 240, 200, 170, 190));

        // Search subpage subtitle
        subpageScore = qMax(subpageScore,
                            scoreTextMatch(subpage.subtitle, query, terms, 160, 140, 120, 140));

        // Search subpage details
        subpageScore = qMax(subpageScore,
                            scoreTextMatch(subpage.details, query, terms, 110, 100, 90, 100));

        // Search subpage tags
        for (const QString &tag : subpage.tags) {
            subpageScore = qMax(subpageScore,
                                scoreTextMatch(tag, query, terms, 190, 160, 130, 150));
        }

        // Search child link labels
        for (int i = 0; i + 1 < subpage.childLinks.size(); i += 2) {
            subpageScore = qMax(subpageScore,
                                scoreTextMatch(subpage.childLinks[i], query, terms, 190, 160, 130, 150));
        }

        // If subpage matched, update topic result
        if (subpageScore >= 0) {
            result.score = qMax(result.score, subpageScore);
            result.matchedSubtopics.append(subpage.title);
        }
    }

    // Remove duplicate matched resource/subpage titles
    result.matchedSubtopics.removeDuplicates();

    return result;
}

// Builds a short readable summary of matched subtopics
static QString summarizedSubtopics(const QStringList &subtopics) {
    if (subtopics.isEmpty()) return {};
    if (subtopics.size() == 1)
        return subtopics.first();
    if (subtopics.size() == 2)
        return subtopics.join(", ");
    return QString("%1, %2, +%3 more")
        .arg(subtopics[0], subtopics[1])
        .arg(subtopics.size() - 2);
}

// ─────────────────────────────────────────────────────────────────────────────
// Builders
// These functions build reusable UI components.
// ─────────────────────────────────────────────────────────────────────────────

// Builds one resource card widget
static QWidget* makeResourceCard(const ResourceInfo &r,
                                 std::function<void(const QString&)> onInternalTarget = {}) {
    // Outer wrapper: border + left accent bar.
    // Using border-left keeps the accent bar inside the rounded card shape.
    QFrame *wrapper = new QFrame();
    wrapper->setStyleSheet(
        QString("QFrame {"
                "  border: 1px solid #dfece1;"
                "  border-left: 4px solid %1;"
                "  border-radius:8px;"
                "  background:#ffffff;"
                "}").arg(r.color));
    wrapper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    // Layout inside resource card
    QVBoxLayout *cl = new QVBoxLayout(wrapper);
    cl->setContentsMargins(20, 18, 20, 18);
    cl->setSpacing(11);

    // ── Header: icon + title ──────────────────────────────────────────────
    QWidget *header = new QWidget();
    header->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *hl = new QHBoxLayout(header);
    hl->setContentsMargins(0, 0, 0, 0);
    hl->setSpacing(12);

    // Resource icon label
    QLabel *iconLbl = new QLabel();
    iconLbl->setFixedSize(38, 38);
    setLucideIcon(iconLbl, r.icon, QColor("#173c2c"), 24);
    iconLbl->setStyleSheet(
        QString("background:%1; border-radius:8px; border:none;").arg(r.color));

    // Resource title label
    QLabel *titleLbl = new QLabel(r.title);
    titleLbl->setWordWrap(true);
    titleLbl->setStyleSheet(
        "font-size:15px; font-weight:800; color:#173c2c; border:none; "
        "");

    // Add icon and title to header row
    hl->addWidget(iconLbl, 0, Qt::AlignTop);
    hl->addWidget(titleLbl, 1);
    cl->addWidget(header);

    // ── Description ───────────────────────────────────────────────────────
    QLabel *descLbl = new QLabel(r.description);
    descLbl->setWordWrap(true);
    descLbl->setStyleSheet(
        "font-size:13px; color:#4f6255; border:none; line-height:165%;");
    cl->addWidget(descLbl);

    // ── Contact / Details block ───────────────────────────────────────────
    // Only show this box if details text exists
    if (!r.details.isEmpty()) {
        QFrame *detBox = new QFrame();
        detBox->setStyleSheet(
            "QFrame {"
            "  background: #f8f4ea;"
            "  border: 1px solid #d5e0cf;"
            "  border-radius:8px;"
            "}");

        // Layout inside details box
        QVBoxLayout *dl = new QVBoxLayout(detBox);
        dl->setContentsMargins(12, 10, 12, 10);
        dl->setSpacing(4);

        // Add each detail line as a separate label
        for (const QString &line : r.details.split('\n')) {
            if (line.trimmed().isEmpty()) continue;

            QLabel *lbl = new QLabel(lucideCleanText(line.trimmed()));
            lbl->setWordWrap(true);
            lbl->setStyleSheet(
                "font-size:12px; color:#4f6255; border:none; background:transparent; "
                "line-height:155%;");
            dl->addWidget(lbl);
        }

        cl->addWidget(detBox);
    }

    // ── Footer: tags + buttons ────────────────────────────────────────────
    QWidget *footer = new QWidget();
    footer->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *fl = new QHBoxLayout(footer);
    fl->setContentsMargins(0, 4, 0, 0);
    fl->setSpacing(6);

    // Rotating styles for tag chips
    static const QStringList tagStyles = {
        "background:#edf4e8; color:#355141; border:1px solid #bfd0bb;",
        "background:#f7f1e3; color:#6a7054; border:1px solid #ddd2bb;",
        "background:#eef1e6; color:#597062; border:1px solid #ccd9c8;"
    };

    // Add tag chips
    for (int i = 0; i < r.tags.size(); i++) {
        QLabel *t = new QLabel(r.tags[i]);
        t->setStyleSheet(
            QString("font-size:10px; font-weight:700; padding:3px 10px; "
                    "border-radius:8px;  %1")
                .arg(tagStyles[i % tagStyles.size()]));
        fl->addWidget(t);
    }

    // Push tags to left
    fl->addStretch();

    cl->addWidget(footer);

    // Store button labels and URLs
    QStringList buttonLabels;
    QStringList buttonUrls;

    // Add main URL button if available
    if (!r.url.isEmpty()) {
        buttonLabels << (r.url.startsWith("qrc:/") ? "Open PDF →" : "Visit →");
        buttonUrls << r.url;
    }

    // Add optional second URL button
    if (!r.url2.isEmpty()) {
        buttonLabels << (r.url2Label.isEmpty() ? "Open →" : r.url2Label);
        buttonUrls << r.url2;
    }

    // Add extra link buttons
    for (int i = 0; i + 1 < r.extraLinks.size(); i += 2) {
        buttonLabels << r.extraLinks[i];
        buttonUrls << r.extraLinks[i + 1];
    }

    // Build button area only if there are links
    if (!buttonUrls.isEmpty()) {
        QWidget *buttonWrap = new QWidget();
        buttonWrap->setStyleSheet("border:none; background:transparent;");
        QGridLayout *buttonGrid = new QGridLayout(buttonWrap);
        buttonGrid->setContentsMargins(0, 0, 0, 0);
        buttonGrid->setHorizontalSpacing(8);
        buttonGrid->setVerticalSpacing(8);

        // Helper lambda for creating one URL button
        auto addUrlBtn = [&](const QString &url, const QString &label, int row, int column) {
            QPushButton *btn = new QPushButton(label);
            const QString u = url;

            // Internal subpage navigation
            if (u.startsWith("subpage:") && onInternalTarget) {
                QObject::connect(btn, &QPushButton::clicked, [u, onInternalTarget]() {
                    onInternalTarget(u.mid(QString("subpage:").length()));
                });

                // External URL or file target
            } else {
                QObject::connect(btn, &QPushButton::clicked, [u]() {
                    openResourceTarget(u);
                });
            }

            // Button styling
            btn->setStyleSheet(
                "QPushButton { font-size:12px; font-weight:800; color:#2a4434; "
                "  background:#e8f1e4;"
                "  border:1px solid #c7d8c1; border-radius:8px; padding:7px 16px; }"
                "QPushButton:hover { background:#e8f1e4; color:#234030; border-color:#a8bf9d; }");
            btn->setCursor(Qt::PointingHandCursor);

            // Add button into grid
            buttonGrid->addWidget(btn, row, column);
        };

        // Use 3 columns for many buttons, otherwise 2
        const int columns = buttonUrls.size() > 4 ? 3 : 2;

        // Add all buttons to grid
        for (int i = 0; i < buttonUrls.size(); ++i)
            addUrlBtn(buttonUrls[i], buttonLabels[i], i / columns, i % columns);

        cl->addWidget(buttonWrap);
    }

    return wrapper;
}

// Large clickable topic card for the landing page
static QWidget* makeTopicCard(const TopicInfo &t,
                              std::function<void()> onClick,
                              QLabel **subtitleLabelOut = nullptr) {

    // Topic card is a QPushButton so the whole card is clickable
    QPushButton *card = new QPushButton();
    card->setFixedSize(268, 268);
    card->setCursor(Qt::PointingHandCursor);
    card->setStyleSheet(
        "QPushButton {"
        "  border: 1px solid #c7d8c1;"
        "  border-radius:8px;"
        "  background:#ffffff;"
        "}"
        "QPushButton:hover {"
        "  border: 1px solid #afc5aa;"
        "  background:#ffffff;"
        "}"
        "QPushButton:pressed {"
        "  background: #f5f8ef;"
        "}"
        );

    // Layout inside clickable card
    QHBoxLayout *hl = new QHBoxLayout(card);
    hl->setContentsMargins(24, 22, 24, 22);
    hl->setSpacing(0);
    hl->setAlignment(Qt::AlignCenter);

    // Topic icon
    QLabel *iconLbl = new QLabel();
    iconLbl->setFixedSize(60, 60);
    setLucideIcon(iconLbl, t.icon, QColor("#173c2c"), 34);
    iconLbl->setStyleSheet(
        "background:transparent; border:none;");

    // Let mouse clicks pass through label to the button
    iconLbl->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Text block for title and subtitle
    QWidget *textBlock = new QWidget();
    textBlock->setStyleSheet("border:none; background:transparent;");
    textBlock->setAttribute(Qt::WA_TransparentForMouseEvents);

    QVBoxLayout *vl = new QVBoxLayout(textBlock);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(9);
    vl->setAlignment(Qt::AlignCenter);

    // Topic title
    QLabel *titleLbl = new QLabel(t.title);
    titleLbl->setWordWrap(true);
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setStyleSheet(
        "font-size:16px; font-weight:800; color:#1a1a1a; border:none; "
        "");

    // Topic subtitle
    QLabel *subLbl = new QLabel(t.subtitle);
    subLbl->setWordWrap(true);
    subLbl->setAlignment(Qt::AlignCenter);
    subLbl->setMaximumWidth(190);
    subLbl->setStyleSheet("font-size:11px; color:#6d8272; border:none;");
    subLbl->setVisible(true);

    // Add title and subtitle to text block
    vl->addWidget(titleLbl);
    vl->addWidget(subLbl);

    // Return subtitle label pointer if caller needs to update it during search
    if (subtitleLabelOut)
        *subtitleLabelOut = subLbl;

    // Vertical column to center icon and text inside card
    QVBoxLayout *cardColumn = new QVBoxLayout();
    cardColumn->setContentsMargins(0, 0, 0, 0);
    cardColumn->setSpacing(14);
    cardColumn->setAlignment(Qt::AlignCenter);
    cardColumn->addStretch();
    cardColumn->addWidget(iconLbl, 0, Qt::AlignHCenter);
    cardColumn->addWidget(textBlock, 0, Qt::AlignHCenter);
    cardColumn->addStretch();

    // Add column to card layout
    hl->addLayout(cardColumn);

    // Connect card click to callback
    QObject::connect(card, &QPushButton::clicked, onClick);

    return card;
}

// Scrollable list of resource cards for one topic
static QWidget* makeResourcePage(int topicIndex,
                                 std::function<void(const QString&)> onInternalTarget = {}) {

    // Scroll area for resource cards
    QScrollArea *scroll = new QScrollArea();
    scroll->setObjectName("screenSurface");
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->viewport()->setObjectName("screenViewport");

    // Inner widget that holds all resource cards
    QWidget *inner = new QWidget();
    inner->setObjectName("screenSurface");

    // Layout for resource cards
    QVBoxLayout *il = new QVBoxLayout(inner);
    il->setSpacing(14);
    il->setContentsMargins(2, 8, 10, 10);

    // Add each resource card for selected topic
    for (const ResourceInfo &r : RESOURCES[topicIndex])
        il->addWidget(makeResourceCard(r, onInternalTarget));

    // Push cards upward
    il->addStretch();

    // Put inner widget into scroll area
    scroll->setWidget(inner);

    return scroll;
}

// Builds an internal subpage, such as Free Software page
static QWidget* makeSubpage(const ResourceSubpageInfo &subpage,
                            std::function<void(int)> onBackToTopic) {

    // Main subpage widget
    QWidget *page = new QWidget();
    page->setObjectName("screenSurface");

    // Main subpage layout
    QVBoxLayout *pl = new QVBoxLayout(page);
    pl->setContentsMargins(48, 36, 48, 36);
    pl->setSpacing(18);

    // Top row contains back button, icon, and title
    QWidget *topRow = new QWidget();
    topRow->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *trl = new QHBoxLayout(topRow);
    trl->setContentsMargins(0, 0, 0, 0);
    trl->setSpacing(12);

    // Back button returns to parent topic page
    QPushButton *backBtn = new QPushButton(
        QString("←  Back to %1").arg(TOPICS[subpage.parentTopicIndex].title));
    backBtn->setStyleSheet(
        "QPushButton { font-size:13px; font-weight:600; color:#365143; "
        "  background:#f6f1e5; border:1px solid #c7d8c1; "
        "  border-radius:8px; padding:8px 16px; }"
        "QPushButton:hover { background:#edf4e8; border-color:#a8bf9d; }");
    backBtn->setCursor(Qt::PointingHandCursor);

    // Connect back button to parent topic
    QObject::connect(backBtn, &QPushButton::clicked, [onBackToTopic, subpage]() {
        onBackToTopic(subpage.parentTopicIndex);
    });

    // Subpage icon
    QLabel *iconLbl = new QLabel();
    iconLbl->setFixedSize(42, 42);
    setLucideIcon(iconLbl, subpage.icon, QColor("#173c2c"), 25);
    iconLbl->setStyleSheet(
        QString("background:%1; border-radius:8px; border:none;").arg(subpage.color));

    // Subpage title
    QLabel *titleLbl = new QLabel(subpage.title);
    titleLbl->setWordWrap(true);
    titleLbl->setStyleSheet(
        "font-size:24px; font-weight:700; color:#173c2c; border:none; "
        "");

    // Add top row widgets
    trl->addWidget(backBtn);
    trl->addSpacing(4);
    trl->addWidget(iconLbl);
    trl->addWidget(titleLbl, 1);

    // Main content card
    QFrame *content = new QFrame();
    content->setStyleSheet(
        "QFrame { background:#ffffff;"
        "         border:1px solid #c7d8c1; border-radius:8px; }");

    // Content card layout
    QVBoxLayout *cl = new QVBoxLayout(content);
    cl->setContentsMargins(24, 22, 24, 22);
    cl->setSpacing(14);

    // Subtitle inside content card
    QLabel *subtitle = new QLabel(subpage.subtitle);
    subtitle->setWordWrap(true);
    subtitle->setStyleSheet(
        "font-size:15px; font-weight:600; color:#355141; border:none; line-height:165%;");
    cl->addWidget(subtitle);

    // Details box
    QFrame *detailsBox = new QFrame();
    detailsBox->setStyleSheet(
        "QFrame { background:#f8f4ea; border:1px solid #d5e0cf; border-radius:8px; }");

    // Details layout
    QVBoxLayout *dl = new QVBoxLayout(detailsBox);
    dl->setContentsMargins(16, 14, 16, 14);
    dl->setSpacing(6);

    // Add each details line as separate label
    for (const QString &line : subpage.details.split('\n')) {
        if (line.trimmed().isEmpty())
            continue;

        QLabel *lineLbl = new QLabel(lucideCleanText(line.trimmed()));
        lineLbl->setWordWrap(true);
        lineLbl->setStyleSheet(
            "font-size:12px; color:#4f6255; border:none; background:transparent; line-height:165%;");
        dl->addWidget(lineLbl);
    }

    cl->addWidget(detailsBox);

    // If subpage has child links, create tool buttons
    if (!subpage.childLinks.isEmpty()) {
        QLabel *branchLabel = new QLabel("Choose a tool");
        branchLabel->setStyleSheet(
            "font-size:12px; font-weight:800; color:#6d8272; border:none; "
            " text-transform:uppercase;");
        cl->addWidget(branchLabel);

        // Wrapper for tool buttons
        QWidget *branchWrap = new QWidget();
        branchWrap->setStyleSheet("border:none; background:transparent;");

        // Grid layout for tool buttons
        QGridLayout *branchGrid = new QGridLayout(branchWrap);
        branchGrid->setContentsMargins(0, 0, 0, 0);
        branchGrid->setHorizontalSpacing(10);
        branchGrid->setVerticalSpacing(10);

        // childLinks are stored as label/url pairs
        for (int i = 0; i + 1 < subpage.childLinks.size(); i += 2) {
            const QString label = subpage.childLinks[i];
            const QString url = subpage.childLinks[i + 1];

            // Button for one child tool
            QPushButton *toolBtn = new QPushButton(label + " →");
            toolBtn->setCursor(Qt::PointingHandCursor);
            toolBtn->setStyleSheet(
                "QPushButton { font-size:13px; font-weight:800; color:#2a4434; "
                "  background:#e8f1e4;"
                "  border:1px solid #c7d8c1; border-radius:8px; padding:12px 16px; }"
                "QPushButton:hover { background:#e8f1e4; color:#234030; border-color:#a8bf9d; }");

            // Open child tool URL when clicked
            QObject::connect(toolBtn, &QPushButton::clicked, [url]() {
                openResourceTarget(url);
            });

            // Place button in grid
            const int itemIndex = i / 2;
            branchGrid->addWidget(toolBtn, itemIndex / 2, itemIndex % 2);
        }

        cl->addWidget(branchWrap);
    }

    // Tag row for subpage tags
    QWidget *tagRow = new QWidget();
    tagRow->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *tl = new QHBoxLayout(tagRow);
    tl->setContentsMargins(0, 0, 0, 0);
    tl->setSpacing(8);

    // Add tag chips
    for (const QString &tag : subpage.tags)
        tl->addWidget(makeInfoChip(tag, "#edf4e8", "#355141", "#bfd0bb"));

    tl->addStretch();
    cl->addWidget(tagRow);

    // Main open button for subpage URL
    QPushButton *openBtn = new QPushButton(subpage.urlLabel);
    const QString targetUrl = subpage.url;

    // Open main subpage URL
    QObject::connect(openBtn, &QPushButton::clicked, [targetUrl]() {
        openResourceTarget(targetUrl);
    });

    openBtn->setCursor(Qt::PointingHandCursor);
    openBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    openBtn->setStyleSheet(
        "QPushButton { font-size:13px; font-weight:800; color:#2a4434; "
        "  background:#e8f1e4;"
        "  border:1px solid #c7d8c1; border-radius:8px; padding:10px 18px; }"
        "QPushButton:hover { background:#e8f1e4; }");

    cl->addWidget(openBtn, 0, Qt::AlignLeft);

    // Add top row and content card to page
    pl->addWidget(topRow);
    pl->addWidget(content);
    pl->addStretch();

    return page;
}

// ─────────────────────────────────────────────────────────────────────────────
// Constructor
// ─────────────────────────────────────────────────────────────────────────────

// ── Activation hook ───────────────────────────────────────────────────────────
// Resets to the topic-picker home page whenever the user navigates to this
// screen. Overrides the virtual onActivated() from Screen.
void Recommendations::onActivated() {

    // Clear search when screen opens
    if (m_searchInput) {
        m_searchInput->clear();
        applyTopicSearch(QString{});
    }

    // Show landing/home page
    m_stack->setCurrentIndex(0);
}

// Applies topic search filter on the landing page
void Recommendations::applyTopicSearch(const QString &query) {

    // Normalize search query
    const QString normalized = normalizedText(query);

    // If search is empty, show all topic cards
    if (normalized.isEmpty()) {
        for (QWidget *card : m_topicCards)
            card->setVisible(true);

        // Reset subtitles back to original topic subtitles
        for (int i = 0; i < m_topicSubtitleLabels.size() && i < TOPICS.size(); ++i) {
            m_topicSubtitleLabels[i]->setText(TOPICS[i].subtitle);
            m_topicSubtitleLabels[i]->setStyleSheet(
                "font-size:13px; color:#6d8272; border:none;");
            m_topicSubtitleLabels[i]->setVisible(true);
        }

        // Hide search status message
        m_searchStatus->clear();
        m_searchStatus->setVisible(false);
        return;
    }

    int visibleCount = 0;              // Number of matching topic cards
    int bestIndex = -1;                // Best matching topic index
    int bestScore = -1;                // Best matching score
    QString bestSubtopicSummary;       // Best matched subtopic summary text

    // Search every topic card
    for (int i = 0; i < m_topicCards.size(); ++i) {
        const TopicSearchResult result = searchTopic(i, normalized);
        const bool matches = result.score >= 0;

        // Show only matching topic cards
        m_topicCards[i]->setVisible(matches);

        // Update topic subtitle visibility/styling
        if (i < m_topicSubtitleLabels.size()) {
            if (matches && !result.matchedSubtopics.isEmpty()) {
                m_topicSubtitleLabels[i]->setText(TOPICS[i].subtitle);
                m_topicSubtitleLabels[i]->setStyleSheet(
                    "font-size:11px; color:#6d8272; border:none;");
                m_topicSubtitleLabels[i]->setVisible(true);
            } else {
                m_topicSubtitleLabels[i]->setText(TOPICS[i].subtitle);
                m_topicSubtitleLabels[i]->setStyleSheet(
                    "font-size:11px; color:#6d8272; border:none;");
                m_topicSubtitleLabels[i]->setVisible(matches);
            }
        }

        // Track best match
        if (matches) {
            visibleCount++;
            if (result.score > bestScore) {
                bestScore = result.score;
                bestIndex = i;
                bestSubtopicSummary = summarizedSubtopics(result.matchedSubtopics);
            }
        }
    }

    // If no topic matched, show error/help message
    if (visibleCount == 0) {
        m_searchStatus->setText(
            "No topic matched that search. Try words like food, immigration, tutoring, Zoom, or counseling.");
        m_searchStatus->setStyleSheet(
            "font-size:11px; color:#C62828; border:none; background:transparent;");
        m_searchStatus->setVisible(true);
        return;
    }

    // Build status text when exactly one topic matches
    if (visibleCount == 1) {
        if (bestSubtopicSummary.isEmpty()) {
            m_searchStatus->setText("1 matching topic found. Press Enter to open it.");
        } else {
            m_searchStatus->setText(
                QString("1 matching topic found via subtopic: %1. Press Enter to open it.")
                    .arg(bestSubtopicSummary));
        }

        // Build status text when multiple topics match
    } else {
        if (bestSubtopicSummary.isEmpty()) {
            m_searchStatus->setText(
                QString("%1 matching topics found. Press Enter to open the best match.")
                    .arg(visibleCount));
        } else {
            m_searchStatus->setText(
                QString("%1 matching topics found. Best subtopic match: %2. Press Enter to open it.")
                    .arg(visibleCount)
                    .arg(bestSubtopicSummary));
        }
    }

    // Show neutral status message if there is a best match
    if (bestIndex >= 0) {
        m_searchStatus->setStyleSheet(
            "font-size:12px; color:#4c5e78; border:none; background:transparent;");
        m_searchStatus->setVisible(true);
    }
}

// Opens the best matching topic based on the current search input
void Recommendations::openBestTopicMatch() {

    // Get current query from search input
    const QString query = m_searchInput ? m_searchInput->text() : QString{};

    // Normalize query
    const QString normalized = normalizedText(query);

    // Do nothing if search is empty
    if (normalized.isEmpty()) return;

    int bestIndex = -1;
    int bestScore = -1;

    // Find highest scoring topic
    for (int i = 0; i < TOPICS.size(); ++i) {
        const TopicSearchResult result = searchTopic(i, normalized);
        if (result.score > bestScore) {
            bestScore = result.score;
            bestIndex = i;
        }
    }

    // Open best matching topic page
    if (bestIndex >= 0)
        showTopic(bestIndex);
}

// ─────────────────────────────────────────────────────────────────────────────
// Constructor
// Builds the full BMCC Resources screen.
// ─────────────────────────────────────────────────────────────────────────────

Recommendations::Recommendations(QWidget *parent) : Screen("BMCC Resources", parent) {

    // Root layout for this screen
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Stacked widget holds home page, topic pages, and subpages
    m_stack = new QStackedWidget(this);
    m_stack->setObjectName("screenSurface");

    // ── Page 0: Landing / topic picker ───────────────────────────────────
    // Home page scroll area
    QScrollArea *homeScroll = new QScrollArea();
    homeScroll->setObjectName("screenSurface");
    homeScroll->setWidgetResizable(true);
    homeScroll->setFrameShape(QFrame::NoFrame);
    homeScroll->viewport()->setObjectName("screenViewport");

    // Home page content widget
    QWidget *homePage = new QWidget();
    homePage->setObjectName("screenSurface");

    // Home page layout
    QVBoxLayout *hl = new QVBoxLayout(homePage);
    hl->setContentsMargins(48, 28, 48, 48);
    hl->setSpacing(0);

    // Centered inner container
    QWidget *homeInner = new QWidget();
    homeInner->setStyleSheet("border:none; background:transparent;");
    homeInner->setMaximumWidth(1240);

    // Inner layout for landing page content
    QVBoxLayout *innerLay = new QVBoxLayout(homeInner);
    innerLay->setContentsMargins(0, 0, 0, 0);
    innerLay->setSpacing(16);

    // Small greeting label
    auto *greeting = new QLabel("WELCOME");
    greeting->setStyleSheet(
        "font-size:12px; font-weight:700; color:#5e6d85; "
        "");

    // Main heading
    auto *heading = new QLabel("What kind of support do you need right now?");
    heading->setStyleSheet(
        "font-size:40px; font-weight:800; color:#111111; "
        "");

    // Subtitle under heading
    auto *sub = new QLabel(
        "BMCC offers resources that can help you feel supported, connected, and heard.");
    sub->setWordWrap(false);
    sub->setMaximumWidth(900);
    sub->setStyleSheet("font-size:15px; color:#4c5e78; border:none;");

    // Header wrapper
    QWidget *headerWrap = new QWidget();
    headerWrap->setStyleSheet("border:none; background:transparent;");
    QVBoxLayout *headerLay = new QVBoxLayout(headerWrap);
    headerLay->setContentsMargins(0, 0, 0, 0);
    headerLay->setSpacing(10);

    // Add heading widgets to header
    headerLay->addWidget(greeting, 0, Qt::AlignLeft);
    headerLay->addWidget(heading,  0, Qt::AlignLeft);
    headerLay->addWidget(sub,      0, Qt::AlignLeft);

    // Add header to landing page
    innerLay->addWidget(headerWrap);
    innerLay->addSpacing(18);

    // Search card container
    QFrame *searchCard = new QFrame();
    searchCard->setStyleSheet(
        "QFrame { background:transparent; border:none; }");

    // Search card layout
    QVBoxLayout *searchOuter = new QVBoxLayout(searchCard);
    searchOuter->setContentsMargins(0, 0, 0, 0);
    searchOuter->setSpacing(8);

    // Search row contains icon, input, and button
    QWidget *searchRow = new QWidget();
    searchRow->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *searchLay = new QHBoxLayout(searchRow);
    searchLay->setContentsMargins(0, 0, 0, 0);
    searchLay->setSpacing(10);
    searchLay->setAlignment(Qt::AlignLeft);

    // Search icon label
    auto *searchIcon = new QLabel();
    searchIcon->setAlignment(Qt::AlignCenter);
    searchIcon->setFixedSize(38, 38);
    setLucideIcon(searchIcon, "search", QColor("#4c5e78"), 18);
    searchIcon->setStyleSheet(
        "font-size:16px; color:#4c5e78; background:#ffffff; border:1px solid #d7ddd2; "
        "border-radius:8px;");

    // Search input box
    m_searchInput = new QLineEdit();
    m_searchInput->setPlaceholderText(
        "Search topics or subtopics, e.g. career, resume, internship, food, Zoom, DACA");
    m_searchInput->setClearButtonEnabled(true);
    m_searchInput->setMaximumWidth(560);
    m_searchInput->setStyleSheet(
        "QLineEdit {"
        "  border:1px solid #d7ddd2;"
        "  border-radius:8px;"
        "  background:#ffffff;"
        "  padding:12px 14px;"
        "  font-size:14px;"
        "  color:#1a1a1a;"
        "}"
        "QLineEdit:focus {"
        "  border:1px solid #b8c8b5;"
        "  background:#ffffff;"
        "}");

    // Search button opens best matching topic
    auto *searchBtn = new QPushButton("Search");
    searchBtn->setMinimumHeight(42);
    searchBtn->setCursor(Qt::PointingHandCursor);
    searchBtn->setStyleSheet(
        "QPushButton { font-size:13px; font-weight:700; color:#1a1a1a; "
        "background:#ffffff; border:1px solid #d7ddd2; border-radius:8px; padding:10px 18px; }"
        "QPushButton:hover { background:#f7f7f4; border-color:#c4d1c0; }");

    // Add search row widgets
    searchLay->addWidget(searchIcon);
    searchLay->addWidget(m_searchInput);
    searchLay->addWidget(searchBtn);
    searchLay->addStretch();

    // Search status label shows match count or no-result message
    m_searchStatus = new QLabel();
    m_searchStatus->setWordWrap(true);
    m_searchStatus->setVisible(false);

    // Add search row and status to search card
    searchOuter->addWidget(searchRow);
    searchOuter->addWidget(m_searchStatus);
    innerLay->addWidget(searchCard);
    innerLay->addSpacing(18);

    // Live search while typing
    connect(m_searchInput, &QLineEdit::textChanged,
            this, &Recommendations::applyTopicSearch);

    // Press Enter to open best match
    connect(m_searchInput, &QLineEdit::returnPressed,
            this, &Recommendations::openBestTopicMatch);

    // Click Search button to open best match
    connect(searchBtn, &QPushButton::clicked,
            this, &Recommendations::openBestTopicMatch);

    // Topic grid container
    QWidget *topicList = new QWidget();
    topicList->setStyleSheet("border:none; background:transparent;");
    QGridLayout *topicListLay = new QGridLayout(topicList);
    topicListLay->setContentsMargins(0, 8, 0, 0);
    topicListLay->setHorizontalSpacing(28);
    topicListLay->setVerticalSpacing(28);

    // Build topic cards
    for (int i = 0; i < TOPICS.size(); i++) {
        int idx = i;
        QLabel *subtitleLabel = nullptr;

        // Create topic card and connect it to showTopic
        QWidget *card = makeTopicCard(
            TOPICS[i],
            [this, idx]() { showTopic(idx); },
            &subtitleLabel);

        // Store card and subtitle label for search filtering
        m_topicCards.append(card);
        m_topicSubtitleLabels.append(subtitleLabel);

        // Place topic card in 3-column grid
        topicListLay->addWidget(card, i / 3, i % 3, Qt::AlignCenter);
    }

    // Make all topic grid columns stretch evenly
    for (int col = 0; col < 3; ++col)
        topicListLay->setColumnStretch(col, 1);

    // Add topic grid to landing page
    innerLay->addWidget(topicList, 0, Qt::AlignHCenter);
    innerLay->addStretch();

    // Center the inner content horizontally
    QHBoxLayout *centerLay = new QHBoxLayout();
    centerLay->setContentsMargins(0, 0, 0, 0);
    centerLay->addStretch();
    centerLay->addWidget(homeInner);
    centerLay->addStretch();

    // Add centered layout to home page
    hl->addLayout(centerLay);
    hl->addStretch();

    // Add home page to scroll area
    homeScroll->setWidget(homePage);

    // Add home scroll as first stacked page
    m_stack->addWidget(homeScroll); // index 0

    // Initialize search state
    applyTopicSearch(QString{});

    // ── Pages 1–5: Resource detail pages ─────────────────────────────────
    // Build one detail page for each main topic
    for (int i = 0; i < TOPICS.size(); i++) {

        // Topic detail page
        QWidget *page = new QWidget();
        page->setObjectName("screenSurface");

        // Topic detail page layout
        QVBoxLayout *pl = new QVBoxLayout(page);
        pl->setContentsMargins(56, 42, 56, 42);
        pl->setSpacing(20);

        // Back button + topic title row
        QWidget *topRow = new QWidget();
        topRow->setStyleSheet("border:none;");
        QHBoxLayout *trl = new QHBoxLayout(topRow);
        trl->setContentsMargins(0, 0, 0, 0);
        trl->setSpacing(12);

        // Back button returns to landing page
        QPushButton *backBtn = new QPushButton("←  Back");
        backBtn->setStyleSheet(
            "QPushButton { font-size:13px; font-weight:600; color:#f1e8ad; "
            "  background:#0b2a3c; border:1px solid #8bdff2; "
            "  border-radius:8px; padding:8px 16px; }"
            "QPushButton:hover { background:#123f46; border-color:#f1e8ad; }");
        backBtn->setCursor(Qt::PointingHandCursor);

        // Connect back button to show home page
        connect(backBtn, &QPushButton::clicked, this, &Recommendations::showHome);

        // Topic icon
        QLabel *iconLbl = new QLabel();
        iconLbl->setFixedSize(40, 40);
        setLucideIcon(iconLbl, TOPICS[i].icon, QColor("#173c2c"), 24);
        iconLbl->setStyleSheet(
            QString("background:%1; border-radius:8px; border:none;")
                .arg(TOPICS[i].accent));

        // Topic title
        QLabel *titleLbl = new QLabel(TOPICS[i].title);
        titleLbl->setObjectName("detailTopicTitle");
        titleLbl->setStyleSheet(
            "font-size:23px; font-weight:700; color:#173c2c; border:none; "
            "");

        // Add top row widgets
        trl->addWidget(backBtn);
        trl->addSpacing(4);
        trl->addWidget(iconLbl);
        trl->addWidget(titleLbl, 1);

        // Divider line
        QFrame *div = new QFrame();
        div->setFrameShape(QFrame::HLine);
        div->setStyleSheet("color:#dfece1; background:#dfece1; border:none;");

        // Section label above resource list
        QLabel *sectionLbl = new QLabel("AVAILABLE AT BMCC");
        sectionLbl->setObjectName("sectionLabel");

        // Add top area to detail page
        pl->addWidget(topRow);
        pl->addWidget(div);
        pl->addSpacing(4);

        // Summary card
        QFrame *summary = new QFrame();
        summary->setStyleSheet(
            "QFrame { background:#ffffff;"
            "         border:1px solid #8bdff2; border-radius:8px; }");

        // Summary card layout
        QVBoxLayout *sl = new QVBoxLayout(summary);
        sl->setContentsMargins(16, 14, 16, 14);
        sl->setSpacing(8);

        // Summary text
        QLabel *summaryText = new QLabel(
            "This section gathers the most relevant BMCC and trusted external support options "
            "for this topic, including direct links, locations, hours, and contact details.");
        summaryText->setWordWrap(true);
        summaryText->setStyleSheet(
            "font-size:13px; color:#4f6255; border:none; line-height:170%;");

        // Metadata chip row
        QWidget *metaRow = new QWidget();
        metaRow->setStyleSheet("border:none; background:transparent;");
        QHBoxLayout *ml = new QHBoxLayout(metaRow);
        ml->setContentsMargins(0, 0, 0, 0);
        ml->setSpacing(8);

        // Number of resources chip
        ml->addWidget(makeInfoChip(QString("%1 resources").arg(RESOURCES[i].size()),
                                   "#e7fbff", "#073b4c", "#8bdff2"));

        // Topic subtitle chip
        ml->addWidget(makeInfoChip(TOPICS[i].subtitle, "#f8ffff", "#31535f", "#c7eef5"));
        ml->addStretch();

        // Add summary content
        sl->addWidget(summaryText);
        sl->addWidget(metaRow);

        // Add summary and resource list to detail page
        pl->addWidget(summary);
        pl->addWidget(sectionLbl);

        // Add scrollable resource cards page
        pl->addWidget(makeResourcePage(i, [this](const QString &key) {

                          // Open matching internal subpage by key
                          for (int pageIndex = 0; pageIndex < RESOURCE_SUBPAGES.size(); ++pageIndex) {
                              if (RESOURCE_SUBPAGES[pageIndex].key == key) {
                                  m_stack->setCurrentIndex(1 + TOPICS.size() + pageIndex);
                                  return;
                              }
                          }
                      }), 1);

        // Add this topic detail page to stack
        m_stack->addWidget(page); // detail pages start at index 1
    }

    // Add all internal resource subpages to stack
    for (const ResourceSubpageInfo &subpage : RESOURCE_SUBPAGES) {
        m_stack->addWidget(makeSubpage(subpage, [this](int topicIndex) {
            showTopic(topicIndex);
        }));
    }

    // Add stacked widget to root layout
    root->addWidget(m_stack);

    // Start on home page
    m_stack->setCurrentIndex(0);
}

// Opens selected topic detail page
void Recommendations::showTopic(int index) {
    m_stack->setCurrentIndex(index + 1); // +1 because index 0 is home
}

// Returns to landing/home page
void Recommendations::showHome() {
    m_stack->setCurrentIndex(0);
}