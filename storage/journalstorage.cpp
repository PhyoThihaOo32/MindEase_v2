#include "storage/journalstorage.h"     // Includes the JournalStorage class header

#include <QDir>                         // Directory handling
#include <QFile>                        // File reading/writing/removing
#include <QFileInfo>                    // File path, filename, suffix, and metadata handling
#include <QStandardPaths>               // Access system folders like Documents
#include <QTextStream>                  // Text input/output stream for reading and writing files

#include <algorithm>                    // Provides std::sort

// On-disk schema constants — single source of truth so the filename format
// and body separator never drift between save and parse.
namespace {
static const QString kEntryStampFormat = "yyyy-MM-dd_HH-mm-ss-zzz";  // Timestamp format used in journal filenames
static const QChar   kSeparatorChar    = QChar(0x2500);              // ─ (BOX DRAWINGS LIGHT HORIZONTAL)
static const int     kSeparatorWidth   = 30;                         // Number of separator characters
}

// Default constructor
JournalStorage::JournalStorage() {}

// Constructor that allows a custom storage directory
JournalStorage::JournalStorage(const QString &directoryPath)
    : m_directoryPath(directoryPath) {}

// Returns the folder path where journal entries are stored
QString JournalStorage::directoryPath() const {
    return resolveDirectoryPath();
}

// Saves one journal entry as a text file
bool JournalStorage::saveEntry(const JournalEntry &entry) const {

    // Do not save invalid entries
    if (!entry.isValid()) return false;

    // Open or create the journal storage directory
    QDir dir(resolveDirectoryPath());

    // If the folder does not exist, create it
    if (!dir.exists() && !dir.mkpath("."))
        return false;

    // Build a unique file path based on the entry date/time
    QFile file(buildEntryPath(entry.dateTime()));

    // Open file for writing text
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    // Write formatted journal content into the file
    QTextStream out(&file);
    out << formatEntry(entry);

    // Verify the write succeeded before closing
    const bool ok = (out.status() == QTextStream::Ok);
    file.close();
    return ok;
}

// Deletes one saved journal entry file
bool JournalStorage::deleteEntry(const QString &path) const {

    // Reject empty paths
    if (path.trimmed().isEmpty())
        return false;

    // Get the configured journal storage directory
    const QDir storageDir(resolveDirectoryPath());

    // Build absolute storage root path
    const QString storageRoot = storageDir.absolutePath() + QDir::separator();

    // Get information about the target file
    const QFileInfo targetInfo(path);

    // Only remove plain-text journal files that live inside the configured
    // MindEase journal directory. This keeps UI deletion from becoming a
    // generic file remover if a bad path ever reaches the list widget.
    if (!targetInfo.absoluteFilePath().startsWith(storageRoot)
        || targetInfo.suffix().compare("txt", Qt::CaseInsensitive) != 0)
        return false;

    // Open target file
    QFile file(targetInfo.absoluteFilePath());

    // Cannot delete a file that does not exist
    if (!file.exists())
        return false;

    // Remove file from disk
    return file.remove();
}

// Loads all valid journal entries from the storage folder
QVector<JournalEntry> JournalStorage::loadEntries() const {

    // List that stores loaded entries
    QVector<JournalEntry> entries;

    // Open storage directory
    QDir dir(resolveDirectoryPath());

    // If directory does not exist yet, return empty list
    if (!dir.exists())
        return entries;

    // Get all .txt files from the journal directory
    const QStringList files = dir.entryList({"*.txt"}, QDir::Files, QDir::Name);

    // Load each text file as a JournalEntry
    for (const QString &fileName : files) {
        const JournalEntry entry = loadEntry(dir.filePath(fileName));

        // Only keep valid entries
        if (entry.isValid())
            entries.append(entry);
    }

    // Sort entries using JournalEntry comparison logic
    std::sort(entries.begin(), entries.end());

    return entries;
}

// Loads one journal entry from a specific file path
JournalEntry JournalStorage::loadEntry(const QString &path) const {

    // Open file
    QFile file(path);

    // If file cannot be opened, return empty invalid JournalEntry
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return JournalEntry{};

    // Read entire file content
    QTextStream in(&file);
    const QString contents = in.readAll();
    const bool ok = (in.status() == QTextStream::Ok);
    file.close();

    if (!ok || contents.isEmpty())
        return JournalEntry{};

    return parseEntryFile(path, contents);
}

// Resolves the final directory path for journal storage
QString JournalStorage::resolveDirectoryPath() const {

    if (!m_directoryPath.isEmpty())
        return m_directoryPath;

    // Fallback to home directory if Documents location is unavailable
    const QString docs = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    return (docs.isEmpty() ? QDir::homePath() : docs) + "/MindEase_Journal";
}

// Builds a unique file path for a new journal entry
QString JournalStorage::buildEntryPath(const QDateTime &dateTime) const {

    // Base filename uses timestamp
    const QString basePath = resolveDirectoryPath() + "/"
                             + dateTime.toString(kEntryStampFormat);

    // First filename attempt
    QString candidate = basePath + ".txt";

    // If file already exists, add numbered suffix
    int suffix = 1;
    while (QFileInfo::exists(candidate)) {
        candidate = QString("%1_%2.txt")
        .arg(basePath)
            .arg(suffix++, 2, 10, QChar('0'));
    }

    return candidate;
}

// Formats a JournalEntry into text that will be saved on disk
QString JournalStorage::formatEntry(const JournalEntry &entry) const {
    return QString("=== MindEase Journal Entry ===\n"
                   "Date: %1\n"
                   "Time: %2\n"
                   "%3\n\n"
                   "%4\n")
        .arg(entry.dateTime().toString("dddd, MMMM d, yyyy"))
        .arg(entry.dateTime().toString("h:mm AP"))
        .arg(QString(kSeparatorWidth, kSeparatorChar))
        .arg(entry.body());
}

// Parses saved text file content back into a JournalEntry object
JournalEntry JournalStorage::parseEntryFile(const QString &path,
                                            const QString &content) const {

    // Get file metadata
    const QFileInfo fileInfo(path);

    // Get filename without extension
    const QString stem = fileInfo.completeBaseName();

    // Length of modern timestamp format
    const int modernStampLength = kEntryStampFormat.length();

    // Try parsing modern timestamp format from filename
    QDateTime dateTime = QDateTime::fromString(
        stem.left(modernStampLength), kEntryStampFormat);

    // Fallback for older filename timestamp format
    if (!dateTime.isValid()) {
        const QString datePart = stem.left(10);
        const QString timePart = stem.mid(11, 8).replace('-', ':');

        dateTime = QDateTime::fromString(
            datePart + " " + timePart, "yyyy-MM-dd HH:mm:ss");
    }

    // Build separator string used inside saved journal file
    const QString separator = QString(kSeparatorWidth, kSeparatorChar);

    // Find where the separator appears in file content
    const int separatorIndex = content.indexOf(separator);

    // Body is everything after separator; fallback is full content
    const QString body = (separatorIndex >= 0)
                             ? content.mid(separatorIndex + separator.length() + 2).trimmed()
                             : content.trimmed();

    // Return parsed journal entry with date, body, and file path
    return JournalEntry(dateTime, body, path);
}