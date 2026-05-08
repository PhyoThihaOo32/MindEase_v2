#ifndef SCREEN_H // Prevents multiple inclusion of this header file
#define SCREEN_H

#include <QWidget> // Base Qt widget class
#include <QVBoxLayout> // Vertical layout manager

// ─────────────────────────────────────────────────────────────────────────────
// Screen — abstract base class for all application screens
//
// OOP concepts demonstrated
// ─────────────────────────────────────────────────────────────────────────────
// Abstraction — Defines a common interface for all screens
// Inheritance — Other screens inherit shared functionality from Screen
// Polymorphism — Virtual methods allow runtime behavior changes
// Encapsulation — Private members protect internal data
// ─────────────────────────────────────────────────────────────────────────────

class Screen : public QWidget { // Base class for all UI screens
    Q_OBJECT // Enables Qt signals, slots, and meta-object features

public:
    explicit Screen(const QString &title, QWidget *parent = nullptr); // Constructor
    virtual ~Screen() = default; // Virtual destructor for safe inheritance cleanup

    QString title() const; // Returns screen title text

    // Pure virtual function — makes Screen an abstract class
    virtual QString screenId() const = 0; // Every screen must provide unique ID

    // Virtual activation hook called when screen becomes visible
    virtual void onActivated() {} // Default empty implementation

protected:
    // Shared helper for building consistent screen headers
    void buildHeader(QVBoxLayout *root);

private:
    QString m_title; // Stores screen title (encapsulated private member)
};

#endif // SCREEN_H