#include "screens/home.h"

#include <QHBoxLayout> // Horizontal layouts
#include <QLabel>      // Text labels
#include <QPushButton> // Clickable buttons
#include <QVBoxLayout> // Vertical layouts

// ─────────────────────────────────────────────────────────────────────────────
// Home screen stylesheet
// Stores all custom UI styling in one place.
// ─────────────────────────────────────────────────────────────────────────────
namespace {

QString homeStylesheet() {
    return R"(

        /* Small top label */
        QLabel#homeEyebrow {
            color: #5e6d85;
            font-size: 13px;
            font-weight: 600;
            background: transparent;
            border: none;
        }

        /* Main hero title */
        QLabel#homeTitle {
            color: #111111;
            font-size: 42px;
            font-weight: 800;
            background: transparent;
            border: none;
        }

        /* Supporting body paragraph */
        QLabel#homeBody {
            color: #4c5e78;
            font-size: 18px;
            font-weight: 500;
            background: transparent;
            border: none;
        }

        /* Bottom credit line */
        QLabel#homeCredit {
            color: #7a8fa0;
            font-size: 12px;
            font-weight: 600;
            background: transparent;
            border: none;
        }

        /* Navigation action buttons */
        QPushButton#homeActionBtn {
            font-size: 15px;
            font-weight: 700;
            color: #171717;
            background: #ffffff;
            border: 1px solid #d7ddd2;
            border-radius: 8px;
            padding: 12px 20px;
        }

        /* Hover effect */
        QPushButton#homeActionBtn:hover {
            background: #f2f5ef;
        }

        /* Pressed effect */
        QPushButton#homeActionBtn:pressed {
            background: #e8ece4;
        }
    )";
}

} // namespace

// ─────────────────────────────────────────────────────────────────────────────
// Creates reusable home screen action button
// ─────────────────────────────────────────────────────────────────────────────
namespace {

QPushButton *makeHomeAction(const QString &text) {

    auto *button = new QPushButton(text);

    // Used for stylesheet targeting
    button->setObjectName("homeActionBtn");

    // Hand cursor on hover
    button->setCursor(Qt::PointingHandCursor);

    // Consistent button height
    button->setMinimumHeight(50);

    return button;
}

}

// ─────────────────────────────────────────────────────────────────────────────
// Home screen constructor
// ─────────────────────────────────────────────────────────────────────────────
Home::Home(QWidget *parent)
    : Screen("Home", parent) {

    // Apply custom stylesheet
    setStyleSheet(homeStylesheet());

    // Root page layout
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(48, 30, 48, 56);
    root->setSpacing(0);

    // Push content vertically toward center
    root->addStretch();

    // Horizontal centering row
    auto *contentRow = new QHBoxLayout();
    contentRow->setContentsMargins(0, 0, 0, 0);
    contentRow->addStretch();

    // Main centered content container
    auto *content = new QWidget();
    content->setObjectName("homeContent");
    content->setStyleSheet(
        "background: transparent;"
        "border: none;"
        );

    // Limits maximum content width
    content->setMaximumWidth(1160);

    // Main vertical content layout
    auto *contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(22);
    contentLayout->setAlignment(Qt::AlignHCenter);

    // Small top label
    auto *eyebrow = new QLabel("MINDEASE");
    eyebrow->setObjectName("homeEyebrow");
    eyebrow->setAlignment(Qt::AlignHCenter);

    // Main hero title
    auto *title = new QLabel(
        "A calm wellness companion for BMCC\nstudents"
        );
    title->setObjectName("homeTitle");
    title->setAlignment(Qt::AlignHCenter);
    title->setWordWrap(true);

    // Supporting description text
    auto *body = new QLabel(
        "MindEase helps you find campus resources, "
        "take care small self-care steps, engage calming\n"
        "tools, and write positive future plans."
        );
    body->setObjectName("homeBody");
    body->setAlignment(Qt::AlignHCenter);
    body->setWordWrap(true);

    // Action button row
    auto *actionsRow = new QHBoxLayout();
    actionsRow->setContentsMargins(0, 0, 0, 0);
    actionsRow->setSpacing(18);
    actionsRow->setAlignment(Qt::AlignHCenter);

    // Navigation buttons
    auto *resourcesBtn = makeHomeAction("BMCC Resources");
    auto *toolkitBtn   = makeHomeAction("Mental Health Toolkit");
    auto *journalBtn   = makeHomeAction("Journal");

    // Honors project credit line
    auto *credit = new QLabel(
        "CSC211H Honors Project  ·  "
        "Phyo Thiha Oo  ·  "
        "Instructor: Dr. Azhar"
        );

    credit->setObjectName("homeCredit");
    credit->setAlignment(Qt::AlignHCenter);
    credit->setWordWrap(true);

    // Add buttons into horizontal row
    actionsRow->addWidget(resourcesBtn);
    actionsRow->addWidget(toolkitBtn);
    actionsRow->addWidget(journalBtn);

    // Build final layout structure
    contentLayout->addWidget(eyebrow);
    contentLayout->addWidget(title);
    contentLayout->addWidget(body);

    contentLayout->addSpacing(8);

    contentLayout->addLayout(actionsRow);

    contentLayout->addSpacing(18);

    contentLayout->addWidget(credit);

    // Center content horizontally
    contentRow->addWidget(content);
    contentRow->addStretch();

    // Add centered row into page
    root->addLayout(contentRow);

    // Push content vertically toward center
    root->addStretch();

    // Navigation button actions
    connect(resourcesBtn, &QPushButton::clicked,
            this,
            [this]() {
                emit requestScreen("resources");
            });

    connect(toolkitBtn, &QPushButton::clicked,
            this,
            [this]() {
                emit requestScreen("toolkit");
            });

    connect(journalBtn, &QPushButton::clicked,
            this,
            [this]() {
                emit requestScreen("journal");
            });
}