#ifndef JOURNALSTORAGE_H // Prevents multiple inclusion of this header file
#define JOURNALSTORAGE_H

#include "models/journalentry.h" // JournalEntry data model
#include <QString> // QString text handling
#include <QVector> // Dynamic array container

// ─────────────────────────────────────────────────────────────────────────────
// JournalStorage — file I/O service for journal entries
//
// Responsibility split:
// Journal → UI and interaction
// JournalEntry → data model
// JournalStorage → saving/loading journal files
// ─────────────────────────────────────────────────────────────────────────────

class JournalStorage {
public:
    JournalStorage(); // Default constructor
    explicit JournalStorage(const QString &directoryPath); // Constructor with custom directory

    QString directoryPath() const; // Returns active journal storage directory

    bool saveEntry(const JournalEntry &entry) const; // Saves entry to disk
    bool deleteEntry(const QString &path) const; // Deletes entry file
    QVector<JournalEntry> loadEntries() const; // Loads all journal entries
    JournalEntry loadEntry(const QString &path) const; // Loads a single entry

private:
    QString resolveDirectoryPath() const; // Resolves final storage directory path
    QString buildEntryPath(const QDateTime &dateTime) const; // Builds unique filename path
    QString formatEntry(const JournalEntry &entry) const; // Formats entry text for saving
    JournalEntry parseEntryFile(const QString &path, const QString &content) const; // Parses file into JournalEntry object

    QString m_directoryPath; // Custom or default storage directory path
};

#endif // JOURNALSTORAGE_H