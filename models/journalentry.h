#ifndef JOURNALENTRY_H // Prevents multiple inclusion of this header file
#define JOURNALENTRY_H

#include <QString> // QString class for text handling
#include <QDateTime> // QDateTime class for timestamps

// ─────────────────────────────────────────────────────────────────────────────
// JournalEntry — value class representing one saved wellness journal entry
//
// OOP concepts demonstrated
// ─────────────────────────────────────────────────────────────────────────────
// Encapsulation — Private data accessed only through public methods
// Operator Overloading — Custom comparison and equality operators
// Simple Model — Stores journal data only, no file handling logic
// ─────────────────────────────────────────────────────────────────────────────

class JournalEntry {
public:
    JournalEntry(); // Default constructor creates invalid empty entry

    JournalEntry(const QDateTime &dt, const QString &body, // Constructor with timestamp and body
                 const QString &filePath = QString()); // Optional file path

    // Accessor methods for encapsulated private data
    QDateTime dateTime() const; // Returns entry timestamp
    QString body() const; // Returns journal text body
    QString filePath() const; // Returns saved file path
    bool isValid() const; // Returns true if entry contains valid data

    // Operator overloads
    bool operator<(const JournalEntry &other) const; // Compares entries for newest-first sorting
    bool operator==(const JournalEntry &other) const; // Checks if two entries are identical

private:
    QDateTime m_dateTime; // Stores entry date and time
    QString m_body; // Stores journal body text
    QString m_filePath; // Stores absolute file path on disk
};

#endif // JOURNALENTRY_H