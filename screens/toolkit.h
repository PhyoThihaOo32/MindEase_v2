#ifndef TOOLKIT_H // Prevents multiple inclusion of this header file
#define TOOLKIT_H

#include "core/screen.h" // Base Screen class
#include <QLabel> // QLabel UI element
#include <QLineEdit> // Search input widget
#include <QStackedWidget> // Widget stack for page navigation
#include <QVector> // Dynamic array container

// ─────────────────────────────────────────────────────────────────────────────
// Toolkit — Mental Health Toolkit screen
//
// Demonstrates:
// Inheritance — Extends the custom Screen base class
// Polymorphism — Overrides virtual methods from Screen
// ─────────────────────────────────────────────────────────────────────────────

class Toolkit : public Screen { // Toolkit screen inherits from Screen
    Q_OBJECT // Enables Qt signals and slots system

public:
    explicit Toolkit(QWidget *parent = nullptr); // Constructor

    QString screenId() const override { return "toolkit"; } // Returns unique screen ID

    void onActivated() override; // Called when screen becomes active

private:
    void applyFolderSearch(const QString &query); // Filters toolkit folders using search
    void openBestFolderMatch(); // Opens best matching toolkit folder
    void showFolder(int index); // Displays selected folder page
    void showHome(); // Returns to home page

    QStackedWidget *m_stack = nullptr; // Handles multi-page navigation
    QLineEdit *m_searchInput = nullptr; // Search bar input
    QLabel *m_searchStatus = nullptr; // Displays search feedback
    QVector<QWidget*> m_folderCards; // Stores folder card widgets
    QVector<QLabel*> m_folderSubtitleLabels; // Stores folder subtitle labels
};

#endif // TOOLKIT_H