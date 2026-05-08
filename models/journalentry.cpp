#include "models/journalentry.h"

// ─────────────────────────────────────────────────────────────────────────────
// JournalEntry implementation
// ─────────────────────────────────────────────────────────────────────────────

// Default constructor
// Creates an empty invalid journal entry.
JournalEntry::JournalEntry() {}

// Main constructor
// Stores journal timestamp, body text, and optional file path.
JournalEntry::JournalEntry(const QDateTime &dt,
                           const QString &body,
                           const QString &filePath)
    : m_dateTime(dt),
    m_body(body),
    m_filePath(filePath) {}

// Returns the journal entry date/time
QDateTime JournalEntry::dateTime() const {
    return m_dateTime;
}

// Returns the journal body text
QString JournalEntry::body() const {
    return m_body;
}

// Returns saved file path
QString JournalEntry::filePath() const {
    return m_filePath;
}

// Checks whether the journal entry is valid
// Entry must contain a valid timestamp and non-empty text.
bool JournalEntry::isValid() const {
    return m_dateTime.isValid()
    && !m_body.trimmed().isEmpty();
}

// Operator overload for sorting
// Reversed intentionally so newer entries appear first when sorted.
bool JournalEntry::operator<(const JournalEntry &other) const {
    return m_dateTime > other.m_dateTime;
}

// Operator overload for equality comparison
// Two entries are equal if both timestamp and body match.
bool JournalEntry::operator==(const JournalEntry &other) const {
    return m_dateTime == other.m_dateTime
           && m_body == other.m_body;
}