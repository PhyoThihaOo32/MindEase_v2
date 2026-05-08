#include "screens/toolkit.h"      // Includes the Toolkit screen class
#include "core/lucideicons.h"     // Provides helper functions for Lucide-style icons

#include <QColor>                 // Handles colors in Qt
#include <QDesktopServices>       // Opens URLs using the system browser
#include <QFrame>                 // Frame widget used for cards and dividers
#include <QGridLayout>            // Grid layout for folder cards
#include <QHBoxLayout>            // Horizontal layout manager
#include <QLabel>                 // Text/icon label widget
#include <QPushButton>            // Clickable button widget
#include <QScrollArea>            // Scrollable page area
#include <QUrl>                   // URL handling
#include <QVBoxLayout>            // Vertical layout manager
#include <functional>             // Allows std::function callback usage

// Holds one toolkit item inside a folder/detail page
struct ToolItem {
    QString icon;                  // Icon shown beside the tool item
    QString title;                 // Tool item title
    QString body;                  // Description or instructions
    QString url;                   // Main link
    QString urlLabel;              // Label for main link button
    QString url2 = {};             // Optional second link
    QString url2Label = {};        // Label for optional second link button
};

// Holds one toolkit folder/category
struct FolderDef {
    QString id;                    // Internal folder ID
    QString icon;                  // Folder icon
    QString accent;                // Accent background color
    QString accentBorder;          // Accent border color
    QString name;                  // Folder name shown on card
    QString tagline;               // Short subtitle shown under folder name
    QString intro;                 // Intro text shown on detail page
    QList<ToolItem> items;         // List of tools/resources inside this folder
};

// Main folder/category data shown on the Toolkit home page
static const QList<FolderDef> FOLDERS = {
    {   "academic", "📚", "#dcfff1", "#8ee2a8",
        "Academic Stress",
        "Deadlines, studying, tutoring, and staying focused.",
        "School can feel heavy fast. This section brings together study tools, academic support, and small strategies for handling pressure with more structure and less panic.",
        {
         { "🗓️", "Weekly Planner",
          "Break tasks into manageable chunks so you always know what to prioritize. A weekly planner can reduce semester overwhelm by giving you a clear, day-by-day roadmap.",
          "https://www.bmcc.cuny.edu/student-affairs/counseling/your-mental-health-toolkit/",
          "Open Toolkit →" },
         { "🧩", "Procrastination Worksheet",
          "Identify what is holding you back and create actionable steps to move forward. Pinpoint your procrastination triggers and build a plan that actually works for you.",
          "https://freetodolist.com/", "Open Worksheet →" },
         { "☕", "Smart Study Break Ideas",
          "• 5-minute refresh: stretch or take a short walk\n"
          "• Creative break: doodle or free-write in a journal\n"
          "• Snack smart: fruit, nuts, or a protein bar\n"
          "• Mindful moment: try a 5-minute guided meditation\n"
          "• Social recharge: check in with a friend or study buddy", "", "" },
         { "🏫", "BMCC Academic Support",
          "Free tutoring, Writing Center, academic coaching, library quiet spaces, and the Study Space Guide are all available on campus.",
          "https://www.bmcc.cuny.edu/learning-assistance/", "Visit →" },
         { "🧠", "BMCC Counseling — Academic Stress",
          "Counselors can help with stress reduction, time management, and academic success strategies.\n📍 Room S-343   📞 (212) 220-8140",
          "https://www.bmcc.cuny.edu/student-affairs/counseling/academics/", "Book Appointment →" },
         }
    },
    {   "connection", "🤝", "#e7fbff", "#8bdff2",
        "Connection & Community",
        "Peer support, groups, clubs, and finding belonging.",
        "You do not have to do college alone. This section is for support groups, workshops, clubs, and community resources that can help you feel more connected.",
        {
         { "🫂", "Counseling Groups & Workshops",
          "BMCC offers group spaces and workshops where students can share experiences, practice coping skills, and build connection.",
          "https://www.bmcc.cuny.edu/student-affairs/counseling/groups-and-workshops/groups/",
          "View Groups →" },
         { "📅", "Upcoming Workshops",
          "Drop-in and pop-up workshops on stress management, art-based stress relief, mindfulness, and more.",
          "https://www.bmcc.cuny.edu/student-affairs/counseling/groups-and-workshops/workshops/",
          "View Schedule →" },
         { "🎓", "Student Clubs & Activities",
          "Join or start a club to meet people who share your interests and build community on campus.",
          "https://www.bmcc.cuny.edu/student-affairs/student-activities/clubs/",
          "Find a Club →" },
         { "📞", "NYC Crisis Resources (24/7)",
          "Hotlines and texting services for emergency mental health support, available 24/7.",
          "https://www.bmcc.cuny.edu/student-affairs/counseling/counseling-resources/nyc-resources/",
          "See All Hotlines →" },
         { "🌐", "Community Referrals",
          "Outside referrals for housing support, domestic assistance, legal aid, and substance use services curated by BMCC.",
          "https://www.bmcc.cuny.edu/student-affairs/counseling/counseling-resources/community-resources/",
          "Visit →" },
         }
    },
    {   "identity", "🌈", "#fbf4d0", "#f1e8ad",
        "Empowered Identity & Care",
        "Racial trauma, LGBTQIA+, disability justice, and identity support.",
        "This section gathers affirming resources for students navigating identity, discrimination, and belonging. Support should make room for every part of who you are.",
        {
         { "✊", "Racial Trauma Resources",
          "Resources for learning, validation, and healing around racial trauma and repeated racism.",
          "https://www.bmcc.cuny.edu/about-bmcc/race-equity-and-inclusion/resources/racial-trauma-resources/",
          "Open BMCC Resources →" },
         { "📖", "Anti-Racist Resources",
          "Articles, talks, and infographics that support learning, allyship, and reflection.", "", "" },
         { "🏳️‍🌈", "LGBTQIA+ Support & Hotlines",
          "Trans Lifeline: (877) 565-8860\nLGBT National Hotline: (888) 843-4564\nTrevor Lifeline: (866) 488-7386", "", "" },
         { "♿", "Disability Justice — 10 Principles",
          "A framework for understanding disability in relation to race, gender, and other intersecting identities.",
          "https://www.sinsinvalid.org/blog/10-principles-of-disability-justice", "Read →" },
         { "💬", "BMCC Counseling — Identity Support",
          "BMCC counselors provide affirming, culturally competent support for all students.\n📍 Room S-343   📞 (212) 220-8140",
          "https://www.bmcc.cuny.edu/student-affairs/counseling/counseling-services/", "Visit →" },
         }
    },
    {   "gratitude", "✏️", "#fbf4d0", "#f1e8ad",
        "Gratitude & Journaling",
        "Prompts, reflection, and gentle daily writing.",
        "A few minutes of reflection can help slow racing thoughts and make room for perspective. This section offers prompts and simple journaling ideas you can start right away.",
        {
         { "🔬", "Why It Works",
          "Gratitude and journaling are linked to lower stress, improved sleep, stronger relationships, and greater resilience.", "", "" },
         { "📝", "Journal Prompts to Try Today",
          "1. What are three things that happened today that brought you peace?\n"
          "2. What went well this week, even if something else was hard?\n"
          "3. What everyday object near you are you grateful for?\n"
          "4. Who made a positive difference in your life this month?", "", "" },
         { "🗓️", "7-Day Gratitude Challenge",
          "A small guided challenge to help you reflect on what is steady, good, or meaningful in your week.",
          "https://www.bmcc.cuny.edu/student-affairs/counseling/your-mental-health-toolkit/",
          "Open Toolkit →" },
         { "📚", "Recommended Reading",
          "The Artist's Way — Julia Cameron\n"
          "The Great Book of Journaling — Eric Maisel\n"
          "Thanks! — Robert Emmons", "", "" },
         }
    },
    {   "mindfulness", "🧘", "#dcfff1", "#8ee2a8",
        "Mindfulness & Meditation",
        "Breathing, guided meditation, and grounding practices.",
        "Mindfulness does not need to be perfect or long. Even a short pause can help settle your body and make the next moment feel more manageable.",
        {
         { "💛", "Lovingkindness Meditation",
          "A practical guide with recorded sessions and beginner-friendly support.",
          "https://www.elliemillertherapy.com/blog/loving-kindness-meditation-a-complete-guide",
          "Open Guide →" },
         { "🌿", "Calm — Free Guided Meditations",
          "Open-access guided meditations for stress, anxiety, focus, and sleep.",
          "https://www.youtube.com/@calm", "Watch on YouTube →" },
         { "🎧", "Headspace Meditation Classes",
          "Structured meditation classes for stress, sleep, focus, and anxiety.",
          "https://www.headspace.com/meditation", "Visit →" },
         { "▶️", "Free YouTube Guided Meditation",
          "A simple guided meditation that is easy to try even if this is new for you.",
          "https://www.youtube.com/watch?v=8x-QmEB4REg", "Watch →" },
         { "📖", "Verywell Mind — Meditation Library",
          "A curated library of guided meditations for anxiety, sleep, grief, and more.",
          "https://www.verywellmind.com/guided-meditations-for-anxiety-stress-and-more-4692781",
          "Visit →" },
         }
    },
    {   "music", "🎵", "#dff6ff", "#8bdff2",
        "Music for Your Mind",
        "Lo-fi, calm music, and playlists for focus or rest.",
        "The right sound can shift your energy fast. This section collects calming and focus-friendly music resources you can use while studying, resting, or decompressing.",
        {
         { "😴", "Calming & Sleep Music",
          "For winding down, reducing anxiety, or falling asleep with something soft in the background.",
          "https://www.youtube.com/watch?v=1ZYbU82GVz4", "YouTube: Sleep Music →" },
         { "☕", "Study & Focus Music",
          "Background sound for concentration: lo-fi beats, coffee shop ambiance, and mellow instrumental sets.",
          "https://www.youtube.com/watch?v=5qap5aO4i9A", "YouTube: Lo-Fi Hip Hop →" },
         { "🎵", "Spotify — Peaceful Piano",
          "A calm instrumental playlist for studying or slowing down after a long day.",
          "https://open.spotify.com/playlist/37i9dQZF1DX3rxVfibe1L0", "Open Spotify →" },
         { "🌿", "Lofi Station",
          "A clean, simple lofi music station for studying, relaxing, and staying focused.",
          "https://www.lofi.cafe/", "Open Lofi Station →" },
         }
    },
    {   "nutrition", "🥗", "#e9fff3", "#8ee2a8",
        "Nutrition & Energy",
        "Food access, campus fitness, and steady energy.",
        "Food and movement shape energy, mood, and focus. This section brings together practical campus resources for nourishment, movement, and everyday energy support.",
        {
         { "🧠", "Food & Your Mind",
          "Leafy greens, protein, hydration, and steadier meals can support energy, concentration, and mood balance.", "", "" },
         { "🍎", "Panther Pantry (Free, On Campus)",
          "Non-perishable, refrigerated, and frozen food items for students facing food insecurity.\n📍 Room S-230   🕐 Mon–Fri 8am–6:30pm  |  Sat 10am–1pm",
          "https://www.bmcc.cuny.edu/student-affairs/arc/panther-pantry/", "Visit →" },
         { "🏋️", "Campus Fitness",
          "On campus: Fitness Center, Swimming Pool, and Health & Wellness Services.\nCONTACT: BMCC Athletics\n📞 (212) 220-8260",
          "https://bmccathletics.com/sports/2012/4/2/RecreationSchedule.aspx",
          "Schedule →",
          "https://www.bmcc.cuny.edu/students/fitness-center/",
          "Visit →" },
         { "🌳", "NYC Free Outdoor Fitness",
          "Free outdoor fitness classes in NYC parks, adaptive programs, and community exercise options.",
          "https://www.nycgovparks.org/programs/recreation/free-fitness-classes",
          "NYC Parks →" },
         }
    },
    {   "sleep", "🌙", "#e7fbff", "#8bdff2",
        "Sleep & Relaxation",
        "Better rest, bedtime tools, and winding down.",
        "Rest affects everything: mood, focus, memory, and patience. This section offers practical sleep tools and wind-down ideas that fit student life.",
        {
         { "⏰", "Sleep Calculator",
          "Find a bedtime based on your wake-up time so you can line up fuller sleep cycles.",
          "https://sleepeducation.org/healthy-sleep/bedtime-calculator/", "Calculate →" },
         { "📘", "Young Adult Sleep Toolkit",
          "Covers sleep cycles, routines, and habits that support more consistent rest.",
          "https://www.thensf.org/sleep-in-adolescents/", "Visit →" },
         { "💡", "Tips for Better Sleep",
          "• Get daylight early in the day\n"
          "• Reduce bright screens before bed\n"
          "• Keep meals and sleep times steadier\n"
          "• Make the room cool, dark, and quiet",
          "https://www.sleepfoundation.org/sleep-hygiene", "Sleep Foundation →" },
         { "📵", "Stop Doom-Scrolling",
          "Blue light and endless scrolling can make it harder to fall asleep. Try a phone-free wind-down for 30 minutes before bed.", "", "" },
         { "🎧", "Relaxation Audio",
          "Rain sounds, white noise, or guided sleep meditations can help quiet a busy mind.",
          "https://www.youtube.com/watch?v=1ZYbU82GVz4", "YouTube: Sleep Music →" },
         }
    },
    };

// Normalizes text for search by converting to lowercase,
// replacing symbols with spaces, and simplifying extra spaces.
static QString normalizedText(const QString &text) {
    QString out = text.toLower();
    for (QChar &c : out) {
        if (!c.isLetterOrNumber()) c = ' ';
    }
    return out.simplified();
}

// Builds one searchable text string from a folder and its tool items
static QString folderSearchText(const FolderDef &fd) {
    QString text = fd.name + " " + fd.tagline + " " + fd.intro;

    // Add all tool titles and descriptions to the searchable text
    for (const ToolItem &item : fd.items)
        text += " " + item.title + " " + item.body;

    return normalizedText(text);
}

// Gives a search score to a folder based on how well it matches the query
static int folderSearchScore(const FolderDef &fd, const QString &normalizedQuery) {

    // Empty query should not match anything
    if (normalizedQuery.isEmpty())
        return -1;

    // Split query into individual words
    const QStringList tokens = normalizedQuery.split(' ', Qt::SkipEmptyParts);

    // Normalize folder name and tagline
    const QString name = normalizedText(fd.name);
    const QString tagline = normalizedText(fd.tagline);

    // Full searchable content from folder and all tool items
    const QString haystack = folderSearchText(fd);

    int score = 0;

    // Strong score if folder name contains the whole query
    if (name.contains(normalizedQuery))
        score += 80;

    // Medium score if tagline contains the whole query
    if (tagline.contains(normalizedQuery))
        score += 55;

    // General score if full folder text contains the whole query
    if (haystack.contains(normalizedQuery))
        score += 40;

    int matchedTokens = 0;

    // Score individual search words
    for (const QString &token : tokens) {
        if (name.contains(token)) {
            score += 20;
            matchedTokens++;
        } else if (tagline.contains(token)) {
            score += 14;
            matchedTokens++;
        } else if (haystack.contains(token)) {
            score += 8;
            matchedTokens++;
        }
    }

    // No matched words means no search result
    if (matchedTokens == 0)
        return -1;

    // Bonus when every search word matched
    if (matchedTokens == tokens.size())
        score += 16;

    return score;
}

// Builds one tool item row inside a folder detail page
static QWidget* makeToolItem(const ToolItem &item) {

    // Row container
    QFrame *row = new QFrame();
    row->setStyleSheet("QFrame { border:none; background:transparent; }");

    // Horizontal layout for icon + text
    QHBoxLayout *hl = new QHBoxLayout(row);
    hl->setContentsMargins(0, 12, 0, 12);
    hl->setSpacing(14);

    // Icon label
    QLabel *iconLbl = new QLabel();
    iconLbl->setFixedSize(40, 40);
    setLucideIcon(iconLbl, item.icon, QColor("#173c2c"), 23);
    iconLbl->setStyleSheet(
        "background:#edf4e8; border:1px solid #c7d8c1; border-radius:8px;");

    // Text area beside icon
    QWidget *textBlock = new QWidget();
    textBlock->setStyleSheet("background:transparent; border:none;");

    // Vertical layout for title, body, and buttons
    QVBoxLayout *vl = new QVBoxLayout(textBlock);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(5);

    // Tool item title
    QLabel *titleLbl = new QLabel(item.title);
    titleLbl->setStyleSheet(
        "font-size:14px; font-weight:800; color:#173c2c; border:none;");
    vl->addWidget(titleLbl);

    // Tool item description/body
    if (!item.body.isEmpty()) {
        QLabel *bodyLbl = new QLabel(lucideCleanText(item.body));
        bodyLbl->setWordWrap(true);
        bodyLbl->setStyleSheet(
            "font-size:13px; color:#4f6255; border:none; line-height:168%;");
        vl->addWidget(bodyLbl);
    }

    // Add link buttons if this tool item has URLs
    if (!item.url.isEmpty() || !item.url2.isEmpty()) {
        QWidget *buttonRow = new QWidget();
        buttonRow->setStyleSheet("background:transparent; border:none;");

        QHBoxLayout *buttonLayout = new QHBoxLayout(buttonRow);
        buttonLayout->setContentsMargins(0, 2, 0, 0);
        buttonLayout->setSpacing(8);

        // Helper function to create one link button
        auto addLinkButton = [&](const QString &url, const QString &label) {
            if (url.isEmpty())
                return;

            QPushButton *linkBtn = new QPushButton(label.isEmpty() ? "Visit →" : label);
            const QString targetUrl = url;

            // Open URL in browser when clicked
            QObject::connect(linkBtn, &QPushButton::clicked, linkBtn, [targetUrl]() {
                QDesktopServices::openUrl(QUrl(targetUrl));
            });

            // Link button style
            linkBtn->setStyleSheet(
                "QPushButton { font-size:12px; font-weight:800; color:#2a4434; "
                "background:#e8f1e4; "
                "border:1px solid #c7d8c1; border-radius:8px; padding:6px 14px; margin-top:3px; }"
                "QPushButton:hover { background:#e8f1e4; color:#234030; border-color:#a8bf9d; }");
            linkBtn->setCursor(Qt::PointingHandCursor);

            // Add button to row
            buttonLayout->addWidget(linkBtn);
        };

        // Add first and second optional link buttons
        addLinkButton(item.url, item.urlLabel);
        addLinkButton(item.url2, item.url2Label);

        // Push buttons to the left
        buttonLayout->addStretch();

        // Add button row under body text
        vl->addWidget(buttonRow);
    }

    // Add icon and text block to row
    hl->addWidget(iconLbl, 0, Qt::AlignTop);
    hl->addWidget(textBlock, 1);

    return row;
}

// Creates a horizontal divider line between tool items
static QFrame* makeDivider() {
    QFrame *f = new QFrame();
    f->setFrameShape(QFrame::HLine);
    f->setStyleSheet("background:#dfece1; border:none;");
    f->setFixedHeight(1);
    return f;
}

// Builds one clickable folder card on the Toolkit home page
static QWidget* makeFolderCard(const FolderDef &fd,
                               std::function<void()> onClick,
                               QLabel **subtitleLabelOut = nullptr) {

    // The whole folder card is a QPushButton so it is clickable
    QPushButton *card = new QPushButton();
    card->setFixedSize(268, 268);
    card->setCursor(Qt::PointingHandCursor);

    // Card styling
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

    // Layout inside card
    QHBoxLayout *hl = new QHBoxLayout(card);
    hl->setContentsMargins(24, 22, 24, 22);
    hl->setSpacing(0);
    hl->setAlignment(Qt::AlignCenter);

    // Folder icon
    QLabel *iconLbl = new QLabel();
    iconLbl->setFixedSize(60, 60);
    setLucideIcon(iconLbl, fd.icon, QColor("#173c2c"), 34);
    iconLbl->setStyleSheet("background:transparent; border:none;");

    // Let clicks pass through icon to the button
    iconLbl->setAttribute(Qt::WA_TransparentForMouseEvents);

    // Text block inside the card
    QWidget *textBlock = new QWidget();
    textBlock->setStyleSheet("border:none; background:transparent;");

    // Let clicks pass through text block to the button
    textBlock->setAttribute(Qt::WA_TransparentForMouseEvents);

    QVBoxLayout *vl = new QVBoxLayout(textBlock);
    vl->setContentsMargins(0, 0, 0, 0);
    vl->setSpacing(9);
    vl->setAlignment(Qt::AlignCenter);

    // Folder title
    QLabel *titleLbl = new QLabel(fd.name);
    titleLbl->setWordWrap(true);
    titleLbl->setAlignment(Qt::AlignCenter);
    titleLbl->setStyleSheet(
        "font-size:16px; font-weight:800; color:#1a1a1a; border:none; ");

    // Folder subtitle/tagline
    QLabel *subLbl = new QLabel(fd.tagline);
    subLbl->setWordWrap(true);
    subLbl->setAlignment(Qt::AlignCenter);
    subLbl->setMaximumWidth(190);
    subLbl->setStyleSheet("font-size:11px; color:#6d8272; border:none;");
    subLbl->setVisible(true);

    // Add text labels to text block
    vl->addWidget(titleLbl);
    vl->addWidget(subLbl);

    // Save subtitle label pointer for search updates
    if (subtitleLabelOut)
        *subtitleLabelOut = subLbl;

    // Column layout centers icon and text vertically
    QVBoxLayout *cardColumn = new QVBoxLayout();
    cardColumn->setContentsMargins(0, 0, 0, 0);
    cardColumn->setSpacing(14);
    cardColumn->setAlignment(Qt::AlignCenter);
    cardColumn->addStretch();
    cardColumn->addWidget(iconLbl, 0, Qt::AlignHCenter);
    cardColumn->addWidget(textBlock, 0, Qt::AlignHCenter);
    cardColumn->addStretch();

    // Add centered column to card
    hl->addLayout(cardColumn);

    // Open folder when card is clicked
    QObject::connect(card, &QPushButton::clicked, card, [onClick]() { onClick(); });

    return card;
}

// Builds the detail page for one toolkit folder
static QWidget* makeFolderDetailPage(const FolderDef &fd, std::function<void()> onBack) {

    // Detail page widget
    QWidget *page = new QWidget();
    page->setObjectName("screenSurface");

    // Main layout for detail page
    QVBoxLayout *pl = new QVBoxLayout(page);
    pl->setContentsMargins(48, 28, 48, 40);
    pl->setSpacing(18);

    // Top row with back button, icon, and title
    QWidget *topRow = new QWidget();
    topRow->setStyleSheet("border:none; background:transparent;");
    QHBoxLayout *trl = new QHBoxLayout(topRow);
    trl->setContentsMargins(0, 0, 0, 0);
    trl->setSpacing(12);

    // Back button
    QPushButton *backBtn = new QPushButton("←  Back");
    backBtn->setObjectName("detailFolderBack");
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(
        "QPushButton { font-size:13px; font-weight:700; color:#1a1a1a; "
        "background:#ffffff; border:1px solid #d7ddd2; border-radius:8px; padding:10px 16px; }"
        "QPushButton:hover { background:#f7f7f4; border-color:#c4d1c0; }");

    // Go back to Toolkit home page
    QObject::connect(backBtn, &QPushButton::clicked, backBtn, [onBack]() { onBack(); });

    // Folder icon
    QLabel *iconLbl = new QLabel();
    iconLbl->setFixedSize(42, 42);
    setLucideIcon(iconLbl, fd.icon, QColor("#173c2c"), 25);
    iconLbl->setStyleSheet(
        QString("background:%1; border-radius:8px; border:none;")
            .arg(fd.accent));

    // Title block contains eyebrow, folder title, and tagline
    QWidget *titleBlock = new QWidget();
    titleBlock->setStyleSheet("border:none; background:transparent;");

    QVBoxLayout *titleLay = new QVBoxLayout(titleBlock);
    titleLay->setContentsMargins(0, 0, 0, 0);
    titleLay->setSpacing(2);

    // Small section label
    QLabel *eyebrow = new QLabel("MENTAL HEALTH TOOLKIT");
    eyebrow->setObjectName("detailFolderEyebrow");
    eyebrow->setStyleSheet(
        "font-size:12px; font-weight:700; color:#5e6d85; ");

    // Folder detail title
    QLabel *titleLbl = new QLabel(fd.name);
    titleLbl->setObjectName("detailFolderTitle");
    titleLbl->setStyleSheet(
        "font-size:28px; font-weight:800; color:#111111; ");

    // Folder tagline
    QLabel *tagLbl = new QLabel(fd.tagline);
    tagLbl->setObjectName("detailFolderTagline");
    tagLbl->setStyleSheet("font-size:14px; color:#4c5e78; border:none;");

    // Add title block labels
    titleLay->addWidget(eyebrow);
    titleLay->addWidget(titleLbl);
    titleLay->addWidget(tagLbl);

    // Add widgets to top row
    trl->addWidget(backBtn, 0, Qt::AlignTop);
    trl->addSpacing(4);
    trl->addWidget(iconLbl, 0, Qt::AlignTop);
    trl->addWidget(titleBlock, 1);

    // Add top row to detail page
    pl->addWidget(topRow);

    // Scroll area for folder content
    QScrollArea *scroll = new QScrollArea();
    scroll->setObjectName("screenSurface");
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->viewport()->setObjectName("screenViewport");

    // Inner scroll content
    QWidget *inner = new QWidget();
    inner->setObjectName("screenSurface");

    QVBoxLayout *il = new QVBoxLayout(inner);
    il->setContentsMargins(2, 8, 10, 10);
    il->setSpacing(14);

    // Intro card
    QFrame *introCard = new QFrame();
    introCard->setStyleSheet(
        QString("QFrame { background:#ffffff; border:1px solid %1; border-radius:8px; }")
            .arg(fd.accentBorder));

    QVBoxLayout *introLay = new QVBoxLayout(introCard);
    introLay->setContentsMargins(20, 18, 20, 18);
    introLay->setSpacing(8);

    // Intro text
    QLabel *introLbl = new QLabel(fd.intro);
    introLbl->setWordWrap(true);
    introLbl->setStyleSheet(
        "font-size:14px; color:#4f6255; border:none; line-height:170%;");
    introLay->addWidget(introLbl);

    // Add intro card to page
    il->addWidget(introCard);

    // List card holds all tool items
    QFrame *listCard = new QFrame();
    listCard->setStyleSheet(
        "QFrame { background:#ffffff; border:1px solid #c7d8c1; border-radius:8px; }");

    QVBoxLayout *listLay = new QVBoxLayout(listCard);
    listLay->setContentsMargins(22, 16, 22, 18);
    listLay->setSpacing(0);

    // Add each tool item with dividers between them
    for (int i = 0; i < fd.items.size(); ++i) {
        if (i > 0)
            listLay->addWidget(makeDivider());

        listLay->addWidget(makeToolItem(fd.items[i]));
    }

    // Add list card to scroll content
    il->addWidget(listCard);
    il->addStretch();

    // Put inner content into scroll area
    scroll->setWidget(inner);

    // Add scroll area to detail page
    pl->addWidget(scroll, 1);

    return page;
}

// Constructor for Toolkit screen
Toolkit::Toolkit(QWidget *parent) : Screen("Mental Health Toolkit", parent) {

    // Root layout for entire Toolkit screen
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Stack holds Toolkit home page and all folder detail pages
    m_stack = new QStackedWidget();
    m_stack->setObjectName("screenSurface");

    // Scroll area for Toolkit home page
    QScrollArea *homeScroll = new QScrollArea();
    homeScroll->setObjectName("screenSurface");
    homeScroll->setWidgetResizable(true);
    homeScroll->setFrameShape(QFrame::NoFrame);
    homeScroll->viewport()->setObjectName("screenViewport");

    // Home page widget
    QWidget *homePage = new QWidget();
    homePage->setObjectName("screenSurface");

    // Home page layout
    QVBoxLayout *hl = new QVBoxLayout(homePage);
    hl->setContentsMargins(48, 28, 48, 48);
    hl->setSpacing(0);

    // Inner centered content wrapper
    QWidget *homeInner = new QWidget();
    homeInner->setStyleSheet("border:none; background:transparent;");
    homeInner->setMaximumWidth(1240);

    // Inner layout
    QVBoxLayout *innerLay = new QVBoxLayout(homeInner);
    innerLay->setContentsMargins(0, 0, 0, 0);
    innerLay->setSpacing(16);

    // Small welcome label
    auto *greeting = new QLabel("WELCOME");
    greeting->setStyleSheet(
        "font-size:12px; font-weight:700; color:#5e6d85; ");

    // Main heading
    auto *heading = new QLabel("A quiet space to pause, reflect, and reset");
    heading->setStyleSheet(
        "font-size:40px; font-weight:800; color:#111111; ");

    // Subtitle
    auto *sub = new QLabel(
        "Choose a toolkit topic for calming tools, reflection prompts, and supportive practices you can try at your own pace.");
    sub->setWordWrap(false);
    sub->setMaximumWidth(900);
    sub->setStyleSheet("font-size:15px; color:#4c5e78; border:none;");

    // Header wrapper
    QWidget *headerWrap = new QWidget();
    headerWrap->setStyleSheet("border:none; background:transparent;");

    QVBoxLayout *headerLay = new QVBoxLayout(headerWrap);
    headerLay->setContentsMargins(0, 0, 0, 0);
    headerLay->setSpacing(10);

    // Add header texts
    headerLay->addWidget(greeting, 0, Qt::AlignLeft);
    headerLay->addWidget(heading,  0, Qt::AlignLeft);
    headerLay->addWidget(sub,      0, Qt::AlignLeft);

    // Add header to home page
    innerLay->addWidget(headerWrap);
    innerLay->addSpacing(18);

    // Search section container
    QFrame *searchCard = new QFrame();
    searchCard->setStyleSheet("QFrame { background:transparent; border:none; }");

    QVBoxLayout *searchOuter = new QVBoxLayout(searchCard);
    searchOuter->setContentsMargins(0, 0, 0, 0);
    searchOuter->setSpacing(8);

    // Search row holds icon, search input, and search button
    QWidget *searchRow = new QWidget();
    searchRow->setStyleSheet("border:none; background:transparent;");

    QHBoxLayout *searchLay = new QHBoxLayout(searchRow);
    searchLay->setContentsMargins(0, 0, 0, 0);
    searchLay->setSpacing(10);
    searchLay->setAlignment(Qt::AlignLeft);

    // Search icon
    auto *searchIcon = new QLabel();
    searchIcon->setAlignment(Qt::AlignCenter);
    searchIcon->setFixedSize(38, 38);
    setLucideIcon(searchIcon, "search", QColor("#4c5e78"), 18);
    searchIcon->setStyleSheet(
        "font-size:16px; color:#4c5e78; background:#ffffff; border:1px solid #d7ddd2; border-radius:8px;");

    // Search input box
    m_searchInput = new QLineEdit();
    m_searchInput->setPlaceholderText(
        "Search toolkit topics or subtopics, e.g. sleep, journaling, music, focus");
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

    // Search button opens the best matching folder
    auto *searchBtn = new QPushButton("Search");
    searchBtn->setMinimumHeight(42);
    searchBtn->setCursor(Qt::PointingHandCursor);
    searchBtn->setStyleSheet(
        "QPushButton { font-size:13px; font-weight:700; color:#1a1a1a; "
        "background:#ffffff; border:1px solid #d7ddd2; border-radius:8px; padding:10px 18px; }"
        "QPushButton:hover { background:#f7f7f4; border-color:#c4d1c0; }");

    // Add search widgets to row
    searchLay->addWidget(searchIcon);
    searchLay->addWidget(m_searchInput);
    searchLay->addWidget(searchBtn);
    searchLay->addStretch();

    // Search status label shows match count or no result message
    m_searchStatus = new QLabel();
    m_searchStatus->setWordWrap(true);
    m_searchStatus->setVisible(false);

    // Add search row and status label
    searchOuter->addWidget(searchRow);
    searchOuter->addWidget(m_searchStatus);
    innerLay->addWidget(searchCard);
    innerLay->addSpacing(18);

    // Folder card grid container
    QWidget *folderList = new QWidget();
    folderList->setStyleSheet("border:none; background:transparent;");

    QGridLayout *folderListLay = new QGridLayout(folderList);
    folderListLay->setContentsMargins(0, 8, 0, 0);
    folderListLay->setHorizontalSpacing(28);
    folderListLay->setVerticalSpacing(28);

    // Build one folder card for each folder definition
    for (int i = 0; i < FOLDERS.size(); ++i) {
        int idx = i;
        QLabel *subtitleLabel = nullptr;

        // Create clickable folder card
        QWidget *card = makeFolderCard(
            FOLDERS[i],
            [this, idx]() { showFolder(idx); },
            &subtitleLabel);

        // Store card and subtitle label for search filtering
        m_folderCards.append(card);
        m_folderSubtitleLabels.append(subtitleLabel);

        // Place cards in 3-column grid
        folderListLay->addWidget(card, i / 3, i % 3, Qt::AlignCenter);
    }

    // Make all grid columns stretch evenly
    for (int col = 0; col < 3; ++col)
        folderListLay->setColumnStretch(col, 1);

    // Add folder grid to home page
    innerLay->addWidget(folderList, 0, Qt::AlignHCenter);
    innerLay->addStretch();

    // Center homeInner horizontally
    QHBoxLayout *centerLay = new QHBoxLayout();
    centerLay->setContentsMargins(0, 0, 0, 0);
    centerLay->addStretch();
    centerLay->addWidget(homeInner);
    centerLay->addStretch();

    // Add centered content to home page layout
    hl->addLayout(centerLay);
    hl->addStretch();

    // Put home page inside scroll area
    homeScroll->setWidget(homePage);

    // Add home page as index 0 in stack
    m_stack->addWidget(homeScroll);

    // Connect live search while typing
    connect(m_searchInput, &QLineEdit::textChanged,
            this, &Toolkit::applyFolderSearch);

    // Press Enter to open best match
    connect(m_searchInput, &QLineEdit::returnPressed,
            this, &Toolkit::openBestFolderMatch);

    // Click Search button to open best match
    connect(searchBtn, &QPushButton::clicked,
            this, &Toolkit::openBestFolderMatch);

    // Add one detail page for each folder
    for (int i = 0; i < FOLDERS.size(); ++i) {
        m_stack->addWidget(makeFolderDetailPage(FOLDERS[i], [this]() { showHome(); }));
    }

    // Initialize search state
    applyFolderSearch(QString{});

    // Add stack to root layout
    root->addWidget(m_stack);
}

// Called when Toolkit screen becomes active
void Toolkit::onActivated() {

    // Clear search input and reset search results
    if (m_searchInput) {
        m_searchInput->clear();
        applyFolderSearch(QString{});
    }

    // Return to home page
    showHome();
}

// Filters folder cards based on the search query
void Toolkit::applyFolderSearch(const QString &query) {

    // Normalize search query
    const QString normalized = normalizedText(query);

    // If search is empty, show all folders
    if (normalized.isEmpty()) {
        for (QWidget *card : m_folderCards)
            card->setVisible(true);

        // Reset all subtitles
        for (int i = 0; i < m_folderSubtitleLabels.size() && i < FOLDERS.size(); ++i) {
            m_folderSubtitleLabels[i]->setText(FOLDERS[i].tagline);
            m_folderSubtitleLabels[i]->setStyleSheet(
                "font-size:11px; color:#6d8272; border:none;");
            m_folderSubtitleLabels[i]->setVisible(true);
        }

        // Hide search status
        m_searchStatus->clear();
        m_searchStatus->setVisible(false);
        return;
    }

    int visibleCount = 0;      // Number of matching folder cards
    int bestIndex = -1;        // Best matching folder index
    int bestScore = -1;        // Best matching folder score

    // Check each folder against search query
    for (int i = 0; i < m_folderCards.size(); ++i) {
        const int score = folderSearchScore(FOLDERS[i], normalized);
        const bool matches = score >= 0;

        // Show only matching cards
        m_folderCards[i]->setVisible(matches);

        // Update subtitle visibility
        if (i < m_folderSubtitleLabels.size()) {
            m_folderSubtitleLabels[i]->setText(FOLDERS[i].tagline);
            m_folderSubtitleLabels[i]->setStyleSheet(
                "font-size:11px; color:#6d8272; border:none;");
            m_folderSubtitleLabels[i]->setVisible(matches);
        }

        // Track best matching folder
        if (matches) {
            visibleCount++;
            if (score > bestScore) {
                bestScore = score;
                bestIndex = i;
            }
        }
    }

    // Show no-result message
    if (visibleCount == 0) {
        m_searchStatus->setText(
            "No toolkit topic matched that search. Try words like sleep, music, journaling, focus, or meditation.");
        m_searchStatus->setStyleSheet(
            "font-size:11px; color:#C62828; border:none; background:transparent;");
        m_searchStatus->setVisible(true);
        return;
    }

    // Show result count message
    if (visibleCount == 1) {
        m_searchStatus->setText("1 matching toolkit topic found. Press Enter to open it.");
    } else {
        m_searchStatus->setText(
            QString("%1 matching toolkit topics found. Press Enter to open the best match.")
                .arg(visibleCount));
    }

    // Show neutral status style
    if (bestIndex >= 0) {
        m_searchStatus->setStyleSheet(
            "font-size:12px; color:#4c5e78; border:none; background:transparent;");
        m_searchStatus->setVisible(true);
    }
}

// Opens the best folder match from current search input
void Toolkit::openBestFolderMatch() {

    // Normalize current search text
    const QString normalized = normalizedText(m_searchInput ? m_searchInput->text() : QString{});

    // Do nothing if search is empty
    if (normalized.isEmpty())
        return;

    int bestIndex = -1;
    int bestScore = -1;

    // Find highest-scoring folder
    for (int i = 0; i < FOLDERS.size(); ++i) {
        const int score = folderSearchScore(FOLDERS[i], normalized);
        if (score > bestScore) {
            bestScore = score;
            bestIndex = i;
        }
    }

    // Open best matching folder
    if (bestIndex >= 0)
        showFolder(bestIndex);
}

// Opens a folder detail page
void Toolkit::showFolder(int index) {

    // Prevent invalid folder index
    if (index < 0 || index >= FOLDERS.size())
        return;

    // +1 because stack index 0 is the home page
    m_stack->setCurrentIndex(index + 1);
}

// Shows Toolkit home page
void Toolkit::showHome() {
    if (m_stack)
        m_stack->setCurrentIndex(0);
}