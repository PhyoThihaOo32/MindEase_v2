#ifndef JOURNAL_H // Prevents multiple inclusion of this header file
#define JOURNAL_H

#include "core/screen.h" // Base Screen class
#include "models/journalentry.h" // JournalEntry data model
#include "storage/journalstorage.h" // JournalStorage file handling class
#include <QComboBox> // Dropdown filter widget
#include <QLabel> // QLabel UI element
#include <QLineEdit> // Single-line text input widget
#include <QTextEdit> // Multi-line text editor widget
#include <QVBoxLayout> // Vertical layout manager
#include <QVector> // Dynamic array container

class Journal : public Screen { // Journal screen inherits from Screen
    Q_OBJECT // Enables Qt signals and slots system

public:
    explicit Journal(QWidget *parent = nullptr); // Constructor
    QString screenId() const override { return "journal"; } // Returns unique screen ID
    void onActivated() override; // Called when Journal screen becomes active

private slots:
    void saveEntry(); // Saves journal entry
    void clearEditor(); // Clears editor fields
    void applyFilter(); // Applies search and date filtering

private:
    void refreshEntryList(); // Reloads entries from storage
    void refreshDateLabel(); // Updates current date label
    void deleteEntryAtPath(const QString &path); // Deletes entry file by path
    void showStatus(const QString &msg, bool ok = true); // Displays status message

    void buildEntryCards(const QVector<JournalEntry> &entries); // Builds UI cards for filtered entries

    void exportEntry(const JournalEntry &entry); // Handles export options
    void exportAsPdf(const JournalEntry &entry); // Exports entry as PDF
    void exportAsText(const JournalEntry &entry); // Exports entry as text file
    QString buildHtmlExport(const JournalEntry &entry) const; // Generates HTML export content

    // Composer widgets
    QLineEdit *m_titleEdit = nullptr; // Journal title input
    QTextEdit *m_editor = nullptr; // Main journal editor
    QLabel *m_dateLbl = nullptr; // Displays current date
    QLabel *m_statusLbl = nullptr; // Displays save/export status messages

    // Past reflections section
    QWidget *m_entriesHost = nullptr; // Container for entry cards
    QVBoxLayout *m_entriesLayout = nullptr; // Layout for entry cards

    // Search and filtering widgets
    QLineEdit *m_searchEdit = nullptr; // Search bar
    QComboBox *m_periodFilter = nullptr; // Time period filter dropdown
    QVector<JournalEntry> m_allEntries; // Cached journal entries list

    JournalStorage m_storage; // Handles journal file persistence
};

#endif // JOURNAL_H