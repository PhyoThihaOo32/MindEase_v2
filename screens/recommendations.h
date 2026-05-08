#ifndef RECOMMENDATIONS_H // Prevents multiple inclusion of this header file
#define RECOMMENDATIONS_H

#include "core/screen.h" // Base Screen class
#include <QStackedWidget> // Widget stack for page navigation
#include <QLineEdit> // Search input widget
#include <QLabel> // QLabel UI element
#include <QVector> // Dynamic array container

// ─────────────────────────────────────────────────────────────────────────────
// Recommendations — BMCC Resources screen
//
// Demonstrates:
// Inheritance — Extends the custom Screen base class
// Polymorphism — Overrides virtual methods from Screen
// ─────────────────────────────────────────────────────────────────────────────

class Recommendations : public Screen { // Recommendations screen inherits from Screen
    Q_OBJECT // Enables Qt signals and slots system

public:
    explicit Recommendations(QWidget *parent = nullptr); // Constructor

    QString screenId() const override { return "resources"; } // Returns unique screen ID

    void onActivated() override; // Called when screen becomes active

private:
    void applyTopicSearch(const QString &query); // Filters topics using search input
    void openBestTopicMatch(); // Opens highest ranked search result
    void showTopic(int index); // Displays selected topic page
    void showHome(); // Returns to home page

    QStackedWidget *m_stack; // Handles multi-page navigation
    QLineEdit *m_searchInput; // Search bar input
    QLabel *m_searchStatus; // Displays search status or results
    QVector<QWidget*> m_topicCards; // Stores topic card widgets
    QVector<QLabel*> m_topicSubtitleLabels; // Stores topic subtitle labels
};

#endif // RECOMMENDATIONS_H