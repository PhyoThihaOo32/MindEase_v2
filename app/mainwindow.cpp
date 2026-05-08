#include "app/mainwindow.h" // Main window class
#include "core/lucideicons.h" // Lucide icon helper functions
#include "core/screen.h" // Base abstract screen class
#include "screens/home.h" // Home screen
#include "screens/recommendations.h" // BMCC resources screen
#include "screens/toolkit.h" // Mental health toolkit screen
#include "screens/journal.h" // Journal screen

#include <QHBoxLayout> // Horizontal layouts
#include <QVBoxLayout> // Vertical layouts
#include <QPushButton> // Push button widgets
#include <QLabel> // Text labels
#include <QFrame> // Frame containers and separators

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("MindEase — BMCC Wellness Companion"); // Sets window title
    setMinimumSize(1120, 760); // Minimum allowed window size
    resize(1500, 960); // Default starting size
    setWindowState(Qt::WindowMaximized); // Opens window maximized

    applyStyle(); // Applies global stylesheet/theme

    centralWidget = new QWidget(this); // Main root widget
    centralWidget->setObjectName("appShell"); // Object name for styling
    setCentralWidget(centralWidget); // Sets root widget into main window

    mainLayout = new QVBoxLayout(centralWidget); // Main vertical layout
    mainLayout->setContentsMargins(24, 20, 24, 20); // Outer spacing
    mainLayout->setSpacing(18); // Space between widgets

    // Build screens (polymorphic — stored as Screen*)
    Home *home = new Home(this); // Creates Home screen
    screens.append(home); // Adds Home screen to screen list
    screens.append(new Recommendations(this)); // Adds BMCC resources screen
    screens.append(new Toolkit(this)); // Adds toolkit screen
    screens.append(new Journal(this)); // Adds journal screen

    // Stack
    stack = new QStackedWidget(this); // Stack container for screen switching
    stack->setObjectName("screenStack"); // Object name for styling

    for (Screen *s : screens)
        stack->addWidget(s); // Adds each screen into stacked widget

    buildNavigationBar(); // Builds top navigation bar

    mainLayout->addWidget(navShell); // Adds navigation bar
    mainLayout->addWidget(stack, 1); // Adds stacked screens

    connect(home, &Home::requestScreen, this, [this](const QString &screenId) {
        for (int i = 0; i < screens.size(); ++i) {
            if (screens[i]->screenId() == screenId) {
                switchScreen(i); // Switches to requested screen
                return;
            }
        }
    });

    switchScreen(0); // Opens Home screen first
}

MainWindow::~MainWindow() {}

void MainWindow::buildNavigationBar() {
    navShell = new QWidget(this); // Navigation container
    navShell->setObjectName("navShell"); // Object name for styling

    QHBoxLayout *shellLayout = new QHBoxLayout(navShell); // Horizontal layout
    shellLayout->setContentsMargins(10, 6, 10, 6); // Inner margins
    shellLayout->setSpacing(24); // Space between sections

    QWidget *logo = new QWidget(); // Logo container
    logo->setObjectName("logoBox");

    QVBoxLayout *ll = new QVBoxLayout(logo); // Logo layout
    ll->setContentsMargins(0, 0, 0, 0);
    ll->setSpacing(0);

    QLabel *appName = new QLabel("MindEase"); // App title label
    appName->setObjectName("appName");
    ll->addWidget(appName);

    QWidget *navRow = new QWidget(); // Navigation buttons row
    navRow->setObjectName("navRow");

    QHBoxLayout *navLayout = new QHBoxLayout(navRow); // Navigation layout
    navLayout->setContentsMargins(0, 0, 0, 0);
    navLayout->setSpacing(24);

    struct NavDef { QString label; }; // Navigation item structure

    const QList<NavDef> navDefs = {
                                   { "Home"                 },
                                   { "BMCC Resources"       },
                                   { "Mental Health Toolkit"},
                                   { "My Journal"           },
                                   };

    for (int i = 0; i < navDefs.size(); i++) {
        QPushButton *btn = new QPushButton(navDefs[i].label); // Navigation button
        btn->setObjectName("navBtn"); // Object name for styling
        btn->setCheckable(true); // Makes button selectable
        btn->setChecked(i == 0); // Home button selected by default
        btn->setCursor(Qt::PointingHandCursor); // Hand cursor on hover
        btn->setMinimumHeight(40); // Minimum button height

        connect(btn, &QPushButton::clicked, this, [this, i]() {
            switchScreen(i); // Switches screen when clicked
        });

        navButtons.append(btn); // Stores button
        navLayout->addWidget(btn); // Adds button to layout
    }

    navLayout->addStretch(); // Pushes buttons to left side

    shellLayout->addWidget(logo, 0, Qt::AlignLeft | Qt::AlignTop); // Adds logo
    shellLayout->addSpacing(28); // Space between logo and nav
    shellLayout->addWidget(navRow, 1); // Adds nav row
}

void MainWindow::switchScreen(int index) {
    stack->setCurrentIndex(index); // Displays selected screen

    for (int i = 0; i < navButtons.size(); i++)
        navButtons[i]->setChecked(i == index); // Updates active nav button

    // Polymorphic dispatch — each Screen subclass decides what to refresh.
    if (index >= 0 && index < screens.size())
        screens[index]->onActivated(); // Calls screen-specific refresh logic
}

void MainWindow::applyStyle() {
    setStyleSheet(R"(
        QMainWindow {
            background: #f6f6f2;
        }

        #appShell {
            background: #f6f6f2;
        }

        #navShell {
            background: transparent;
            border: none;
        }

        #brandRow, #navRow {
            background: transparent;
            border: none;
        }

        #logoBox {
            background: transparent;
            border: none;
        }

        #appName {
            font-size: 24px;
            font-weight: 800;
            color: #111111;
        }

        #navBtn {
            text-align: center;
            padding: 8px 8px;
            border: none;
            background: transparent;
            font-size: 15px;
            font-weight: 700;
            color: #171717;
        }

        #navBtn:hover {
            color: #3f5f46;
        }

        #navBtn:checked {
            color: #2f5f3a;
            font-weight: 700;
        }

        #navBtn:focus {
            background: transparent;
            color: #2f5f3a;
        }

        #screenStack {
            background: transparent;
        }

        QWidget#screenRoot,
        QWidget#screenSurface,
        QWidget#screenViewport {
            background: transparent;
        }

        QLabel#screenTitle {
            font-size: 30px;
            font-weight: 800;
            color: #173c2c;
        }

        QLabel#sectionLabel {
            font-size: 10px;
            font-weight: 700;
            color: #5f715f;
        }

        QPushButton#primaryBtn {
            background: #e8f1e4;
            color: #173c2c;
            border: 1px solid #b8c8b5;
            border-radius: 8px;
            padding: 10px 20px;
            font-size: 14px;
            font-weight: 700;
        }

        QPushButton#primaryBtn:hover {
            background: #ddebd8;
        }

        QPushButton#primaryBtn:pressed {
            background: #d2e0cd;
        }

        QPushButton#primaryBtn:disabled {
            background: #d8ddd2;
            color: #92a092;
        }

        QPushButton#outlineBtn {
            background: #ffffff;
            color: #173c2c;
            border: 1px solid #c7d8c1;
            border-radius: 8px;
            padding: 10px 18px;
            font-size: 14px;
            font-weight: 600;
        }

        QPushButton#outlineBtn:hover {
            background: #f2f5ef;
        }

        QScrollArea {
            border: none;
            background: transparent;
        }

        QScrollBar:vertical {
            width: 10px;
            background: transparent;
            margin: 6px 2px 6px 0;
        }

        QScrollBar::handle:vertical {
            background: #b8c8b5;
            border-radius: 5px;
            min-height: 30px;
        }

        QScrollBar::handle:vertical:hover {
            background: #96ad91;
        }

        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0;
            background: transparent;
        }

        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: transparent;
        }

        QSplitter::handle:horizontal {
            background: transparent;
            width: 10px;
        }
    )"); // Global Qt stylesheet
}