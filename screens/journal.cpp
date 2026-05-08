#include "screens/journal.h"      // Includes the Journal class header file
#include "core/lucideicons.h"     // Provides Lucide icon helper functions

#include <QColor>                 // Used for handling colors in Qt
#include <QDateTime>              // Used for current date and time operations
#include <QFileDialog>            // Opens file save/open dialog windows
#include <QFrame>                 // Frame container widget for styling sections
#include <QHBoxLayout>            // Horizontal layout manager
#include <QMenu>                  // Dropdown/context menu support
#include <QMessageBox>            // Popup message dialogs
#include <QShortcut>              // Keyboard shortcut handling
#include <QPrinter>               // Printing and PDF export support
#include <QPrintDialog>           // Printer selection dialog
#include <QPushButton>            // Clickable button widget
#include <QRegularExpression>     // Regular expression support for text parsing
#include <QScrollArea>            // Scrollable container widget
#include <QSizePolicy>            // Controls widget resizing behavior
#include <QStandardPaths>         // Access system folders like Documents/Desktop
#include <QTextDocument>          // Rich text and HTML document handling
#include <QTimer>                 // Timer and delayed execution support

// ─────────────────────────────────────────────────────────────────────────────
// Anonymous-namespace helpers
// Helper code inside this namespace is only used in this .cpp file.
// It keeps these helper functions private and avoids name conflicts.
// ─────────────────────────────────────────────────────────────────────────────
namespace {

// Holds the separated display parts of a journal entry
struct EntryDisplayParts {
    QString title;     // Title shown on the journal card
    QString body;      // Full body text of the journal entry
    QString excerpt;   // Short preview text shown in the card
};

// Clears all widgets/items inside a layout
void clearLayout(QLayout *layout) {
    if (!layout) return;

    // Keep removing items until the layout is empty
    while (QLayoutItem *item = layout->takeAt(0)) {

        // If this item is a widget, delete it
        if (QWidget *w = item->widget()) delete w;

        // If this item contains another layout, clear that layout too
        if (QLayout *l = item->layout()) clearLayout(l);

        // Delete the layout item itself
        delete item;
    }
}

// Builds title, body, and excerpt from a saved journal entry
EntryDisplayParts buildEntryDisplay(const JournalEntry &entry) {
    EntryDisplayParts parts;

    // Get the journal body and remove extra spaces from beginning/end
    const QString raw = entry.body().trimmed();

    // If entry has no text, use a default title
    if (raw.isEmpty()) { parts.title = "Untitled reflection"; return parts; }

    // Start with the full raw text as the body
    QString fullBody = raw;

    // Regular expression for detecting paragraph breaks
    const QRegularExpression paragraphBreak("\n\\s*\n");

    // Split the journal text into paragraphs
    QStringList paragraphs = raw.split(paragraphBreak, Qt::SkipEmptyParts);

    // If there are at least two paragraphs, try using the first paragraph as title
    if (paragraphs.size() >= 2) {
        const QString candidateTitle = paragraphs.first().trimmed();

        // Only use it as title if it is not empty and not too long
        if (!candidateTitle.isEmpty() && candidateTitle.size() <= 90) {
            parts.title = candidateTitle;

            // Remove first paragraph from the body because it became the title
            paragraphs.takeFirst();

            // Join the remaining paragraphs as the body
            fullBody = paragraphs.join("\n\n").trimmed();
        }
    }

    // If title was not found from paragraph logic, create one
    if (parts.title.isEmpty()) {

        // Try using the first line as title
        const QString firstLine = raw.section('\n', 0, 0).trimmed();

        if (!firstLine.isEmpty() && firstLine.size() <= 90) {
            parts.title = firstLine;
        } else {

            // If first line is too long, simplify the whole text for a fallback title
            parts.title = raw.simplified();

            // Shorten very long titles
            if (parts.title.size() > 56)
                parts.title = parts.title.left(56).trimmed() + "...";
        }
    }

    // If body somehow becomes empty, restore original raw text
    if (fullBody.isEmpty()) fullBody = raw;

    // Store final body text
    parts.body    = fullBody;

    // Create simplified preview text
    parts.excerpt = fullBody.simplified();

    // Shorten preview text if it is too long
    if (parts.excerpt.size() > 180)
        parts.excerpt = parts.excerpt.left(180).trimmed() + "...";

    return parts;
}

} // namespace

// ─────────────────────────────────────────────────────────────────────────────
// Constructor
// Builds the full Journal screen UI and sets up storage.
// ─────────────────────────────────────────────────────────────────────────────
Journal::Journal(QWidget *parent)
    : Screen("My Journal", parent),

    // Store journal files inside the user's Documents/MindEase_Journal folder
    m_storage(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
              + "/MindEase_Journal") {

    // Root layout for the whole Journal screen
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setContentsMargins(0, 0, 0, 0);
    root->setSpacing(0);

    // Scroll area allows the journal page to scroll
    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setObjectName("screenSurface");
    scroll->viewport()->setObjectName("screenViewport");

    // Main page widget inside the scroll area
    QWidget *page = new QWidget();
    page->setObjectName("screenSurface");

    // Layout for the page
    QVBoxLayout *pageLayout = new QVBoxLayout(page);
    pageLayout->setContentsMargins(72, 32, 72, 56);
    pageLayout->setSpacing(0);

    // Inner page container used to control width and spacing
    QWidget *pageInner = new QWidget();
    pageInner->setStyleSheet("background:transparent; border:none;");
    pageInner->setMaximumWidth(1680);
    pageInner->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Main content layout
    QVBoxLayout *inner = new QVBoxLayout(pageInner);
    inner->setContentsMargins(0, 0, 0, 0);
    inner->setSpacing(0);

    // ── Header ───────────────────────────────────────────────────────────────
    // Small top label
    auto *eyebrowLbl = new QLabel("ZEN SPACE");
    eyebrowLbl->setStyleSheet(
        "font-size:12px; font-weight:700; color:#5e6d85; ");

    // Main title label
    auto *titleLbl = new QLabel("My Journal");
    titleLbl->setStyleSheet(
        "font-size:40px; font-weight:800; color:#111111; ");

    // Subtitle text under the main title
    auto *subLbl = new QLabel(
        "Create entries to reflect on events, and return to past reflections.");
    subLbl->setStyleSheet("font-size:15px; color:#4c5e78; border:none;");

    // Add header widgets to the main layout
    inner->addWidget(eyebrowLbl, 0, Qt::AlignLeft);
    inner->addSpacing(10);
    inner->addWidget(titleLbl, 0, Qt::AlignLeft);
    inner->addSpacing(12);
    inner->addWidget(subLbl, 0, Qt::AlignLeft);
    inner->addSpacing(34);

    // ── Composer card ────────────────────────────────────────────────────────
    // Card where the user writes a new journal entry
    auto *composerCard = new QFrame();
    composerCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    composerCard->setMinimumHeight(640);
    composerCard->setStyleSheet(
        "QFrame {"
        "  background:#ffffff;"
        "  border:1px solid #c7d8c1;"
        "  border-radius:8px;"
        "}");

    // Layout inside the composer card
    QVBoxLayout *composerLayout = new QVBoxLayout(composerCard);
    composerLayout->setContentsMargins(44, 38, 44, 30);
    composerLayout->setSpacing(0);

    // Title input for the journal entry
    m_titleEdit = new QLineEdit();
    m_titleEdit->setPlaceholderText("Entry title...");
    m_titleEdit->setStyleSheet(
        "QLineEdit {"
        "  border:none; background:transparent;"
        "  font-size:27px; font-weight:700; color:#173c2c; padding:0 0 8px 0;"
        "}"
        "QLineEdit::placeholder { color:#9eaa9d; }");
    composerLayout->addWidget(m_titleEdit);
    composerLayout->addSpacing(10);

    // Main text editor for writing the journal body
    m_editor = new QTextEdit();

    // Plain text only, no rich formatting
    m_editor->setAcceptRichText(false);
    m_editor->setPlaceholderText("How are you feeling today? What's on your mind?");
    m_editor->setMinimumHeight(430);
    m_editor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_editor->setStyleSheet(
        "QTextEdit {"
        "  border:none; background:transparent;"
        "  font-size:17px; color:#365143; padding:12px 0 0 0; line-height:170%;"
        "}"
        "QTextEdit::placeholder { color:#8fa09a; }");
    composerLayout->addWidget(m_editor);
    composerLayout->addSpacing(20);

    // Divider line between editor and footer row
    auto *composerDivider = new QFrame();
    composerDivider->setFrameShape(QFrame::HLine);
    composerDivider->setStyleSheet("background:#dce8d8; border:none;");
    composerDivider->setFixedHeight(1);
    composerLayout->addWidget(composerDivider);
    composerLayout->addSpacing(18);

    // Footer row holds date, status, shortcut hint, and save button
    QWidget *footerRow = new QWidget();
    footerRow->setStyleSheet("background:transparent; border:none;");
    QHBoxLayout *footerLayout = new QHBoxLayout(footerRow);
    footerLayout->setContentsMargins(0, 0, 0, 0);
    footerLayout->setSpacing(16);

    // Shows today's date
    m_dateLbl = new QLabel();
    m_dateLbl->setStyleSheet("font-size:14px; font-weight:600; color:#5a6f62; border:none;");

    // Shows save/delete/export status messages
    m_statusLbl = new QLabel();
    m_statusLbl->setStyleSheet("font-size:12px; color:#2E7D32; border:none;");

    // ── Keyboard shortcut hint ───────────────────────────────────────────────
    // Small label showing user they can press Ctrl+S to save
    auto *shortcutHint = new QLabel("Ctrl+S to save");
    shortcutHint->setStyleSheet("font-size:11px; color:#9eaa9d; border:none;");

    // Save button for saving the current journal entry
    QPushButton *saveBtn = new QPushButton("Save Entry");
    saveBtn->setCursor(Qt::PointingHandCursor);
    saveBtn->setMinimumHeight(44);
    saveBtn->setMinimumWidth(150);
    saveBtn->setStyleSheet(
        "QPushButton {"
        "  background:#2f5f3a;"
        "  color:#fffdf5; border:1px solid #2f523b; border-radius:8px;"
        "  font-size:14px; font-weight:700; padding:10px 24px;"
        "}"
        "QPushButton:hover { background:#274f31; }");

    // Connect save button click to saveEntry function
    connect(saveBtn, &QPushButton::clicked, this, &Journal::saveEntry);

    // Ctrl+S shortcut
    // Allows user to save by pressing Ctrl+S / Command+S depending on system
    auto *saveShortcut = new QShortcut(QKeySequence::Save, this);
    connect(saveShortcut, &QShortcut::activated, this, &Journal::saveEntry);

    // Add footer widgets
    footerLayout->addWidget(m_dateLbl);
    footerLayout->addWidget(m_statusLbl, 1);
    footerLayout->addWidget(shortcutHint);
    footerLayout->addWidget(saveBtn, 0, Qt::AlignRight);

    // Add footer row to composer card
    composerLayout->addWidget(footerRow);

    // Add composer card to main layout
    inner->addWidget(composerCard);
    inner->addSpacing(42);

    // ── Past Reflections header ──────────────────────────────────────────────
    // Header label for saved entries section
    auto *pastLbl = new QLabel("PAST REFLECTIONS");
    pastLbl->setStyleSheet(
        "font-size:12px; font-weight:700; color:#5e6d85; ");
    inner->addWidget(pastLbl, 0, Qt::AlignLeft);
    inner->addSpacing(14);

    // ── Feature 4: Search + Date-filter row ──────────────────────────────────
    // Row that contains search box and time period filter
    QWidget *filterRow = new QWidget();
    filterRow->setStyleSheet("background:transparent; border:none;");
    QHBoxLayout *filterLayout = new QHBoxLayout(filterRow);
    filterLayout->setContentsMargins(0, 0, 0, 0);
    filterLayout->setSpacing(12);

    // Search input for filtering journal entries by text
    m_searchEdit = new QLineEdit();
    m_searchEdit->setPlaceholderText("Search reflections...");

    // Shows an X button inside the input to clear search
    m_searchEdit->setClearButtonEnabled(true);
    m_searchEdit->setStyleSheet(
        "QLineEdit {"
        "  border:1px solid #c7d8c1; border-radius:8px;"
        "  background:#ffffff; padding:10px 16px;"
        "  font-size:14px; color:#365143;"
        "}"
        "QLineEdit:focus { border-color:#7aac6e; }"
        "QLineEdit::placeholder { color:#9eaa9d; }");

    // Dropdown filter for date period
    m_periodFilter = new QComboBox();
    m_periodFilter->addItems({ "All Time", "Today", "This Week", "This Month" });
    m_periodFilter->setMinimumHeight(42);
    m_periodFilter->setMinimumWidth(140);
    m_periodFilter->setCursor(Qt::PointingHandCursor);
    m_periodFilter->setStyleSheet(
        "QComboBox {"
        "  border:1px solid #c7d8c1; border-radius:8px;"
        "  background:#ffffff; padding:8px 14px;"
        "  font-size:14px; color:#365143; font-weight:600;"
        "}"
        "QComboBox:focus { border-color:#7aac6e; }"
        "QComboBox::drop-down { border:none; width:28px; }"
        "QComboBox QAbstractItemView {"
        "  background:#ffffff; border:1px solid #c7d8c1;"
        "  selection-background-color:#edf5eb; selection-color:#173c2c;"
        "}");

    // Add search and dropdown to filter row
    filterLayout->addWidget(m_searchEdit, 1);
    filterLayout->addWidget(m_periodFilter);

    // When search text changes, apply filter again
    connect(m_searchEdit,   &QLineEdit::textChanged,
            this,           &Journal::applyFilter);

    // When date filter changes, apply filter again
    connect(m_periodFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,           &Journal::applyFilter);

    // Add filter row to main layout
    inner->addWidget(filterRow);
    inner->addSpacing(18);

    // ── Entry cards host ─────────────────────────────────────────────────────
    // Container where saved journal entry cards will be displayed
    m_entriesHost = new QWidget();
    m_entriesHost->setStyleSheet("background:transparent; border:none;");
    m_entriesHost->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // Layout that holds all entry cards
    m_entriesLayout = new QVBoxLayout(m_entriesHost);
    m_entriesLayout->setContentsMargins(0, 0, 0, 0);
    m_entriesLayout->setSpacing(18);

    // Add entries host to main layout
    inner->addWidget(m_entriesHost);
    inner->addStretch();

    // Add inner page content to page layout
    pageLayout->addWidget(pageInner);
    pageLayout->addStretch();

    // Put page inside scroll area
    scroll->setWidget(page);

    // Add scroll area to root layout
    root->addWidget(scroll);

    // Initialize date label and load saved entries
    refreshDateLabel();
    refreshEntryList();
}

// ─────────────────────────────────────────────────────────────────────────────
// Screen interface
// Called when the Journal screen becomes active.
// ─────────────────────────────────────────────────────────────────────────────
void Journal::onActivated() {

    // Update today's date
    refreshDateLabel();

    // Reload and display saved entries
    refreshEntryList();
}

// ─────────────────────────────────────────────────────────────────────────────
// Status / date helpers
// ─────────────────────────────────────────────────────────────────────────────
void Journal::showStatus(const QString &msg, bool ok) {

    // Show status message
    m_statusLbl->setText(msg);

    // Green for success, red for error
    m_statusLbl->setStyleSheet(
        ok ? "font-size:12px; color:#2E7D32; border:none;"
           : "font-size:12px; color:#C62828; border:none;");

    // Clear the message after 4 seconds if it has not changed
    QTimer::singleShot(4000, m_statusLbl, [this, msg]() {
        if (m_statusLbl->text() == msg) m_statusLbl->clear();
    });
}

// Updates the label that shows today's date
void Journal::refreshDateLabel() {
    const QDate today = QDate::currentDate();
    m_dateLbl->setText(QString("Today, %1").arg(today.toString("MMMM d, yyyy")));
}

// ─────────────────────────────────────────────────────────────────────────────
// Save / clear
// Handles saving a journal entry and clearing the editor.
// ─────────────────────────────────────────────────────────────────────────────
void Journal::saveEntry() {

    // Get title and body from input fields
    const QString titleText = m_titleEdit->text().trimmed();
    const QString bodyText  = m_editor->toPlainText().trimmed();

    // Prevent saving empty entries
    if (bodyText.isEmpty()) {
        showStatus("Write something before saving.", false);
        return;
    }

    // Start with body text
    QString combined = bodyText;

    // If title exists, place title above body with a blank line
    if (!titleText.isEmpty())
        combined = titleText + "\n\n" + bodyText;

    // Create new journal entry with current date/time
    JournalEntry entry(QDateTime::currentDateTime(), combined);

    // Save entry to storage
    if (!m_storage.saveEntry(entry)) {
        showStatus("Could not save this entry.", false);
        return;
    }

    // Show success message and refresh UI
    showStatus("Entry saved");
    clearEditor();
    refreshDateLabel();
    refreshEntryList();
}

// Clears editor fields after saving
void Journal::clearEditor() {
    m_titleEdit->clear();
    m_editor->clear();
    m_titleEdit->setFocus();
}

// ─────────────────────────────────────────────────────────────────────────────
// Feature 4: Search + Date Filter
// Loads entries and applies search/date filtering.
// ─────────────────────────────────────────────────────────────────────────────
void Journal::refreshEntryList() {
    // Load all entries from disk into cache, then let applyFilter render them.
    m_allEntries = m_storage.loadEntries();
    applyFilter();
}

void Journal::applyFilter() {

    // Get search query from search box
    const QString query = m_searchEdit ? m_searchEdit->text().trimmed().toLower()
                                       : QString();

    // Get selected period filter index
    const int period = m_periodFilter ? m_periodFilter->currentIndex() : 0;

    // Today's date used for filtering
    const QDate today = QDate::currentDate();

    // Stores entries that match filter rules
    QVector<JournalEntry> filtered;
    filtered.reserve(m_allEntries.size());

    // Loop through all saved entries
    for (const JournalEntry &e : m_allEntries) {

        // ── Period filter ────────────────────────────────────────────────────
        bool periodOk = true;

        // Check selected time period
        switch (period) {
        case 1: periodOk = (e.dateTime().date() == today);               break;
        case 2: periodOk = (e.dateTime().date() >= today.addDays(-6));   break;
        case 3: periodOk = (e.dateTime().date() >= today.addDays(-29));  break;
        default: break;
        }

        // Skip this entry if it does not match date filter
        if (!periodOk) continue;

        // ── Text search ──────────────────────────────────────────────────────
        // Skip this entry if search query does not exist in entry body
        if (!query.isEmpty() && !e.body().toLower().contains(query))
            continue;

        // Entry passed all filters
        filtered.append(e);
    }

    // Rebuild cards using filtered entries
    buildEntryCards(filtered);
}

// ─────────────────────────────────────────────────────────────────────────────
// Build entry cards
// This function creates the UI cards for saved journal entries.
// It is called by applyFilter with the filtered subset.
// ─────────────────────────────────────────────────────────────────────────────
void Journal::buildEntryCards(const QVector<JournalEntry> &entries) {

    // Clear existing cards before rebuilding
    clearLayout(m_entriesLayout);

    // If there are no entries to show, display empty state card
    if (entries.isEmpty()) {
        QFrame *emptyCard = new QFrame();
        emptyCard->setStyleSheet(
            "QFrame {"
            "  background:#e8f1e4;"
            "  border:1px solid #c7d8c1; border-radius:8px;"
            "}");

        // Layout inside empty card
        QVBoxLayout *emptyLayout = new QVBoxLayout(emptyCard);
        emptyLayout->setContentsMargins(30, 26, 30, 26);

        // Different message when search/filter is active vs. truly empty
        const bool filtering = (m_searchEdit && !m_searchEdit->text().isEmpty())
                               || (m_periodFilter && m_periodFilter->currentIndex() != 0);

        // Empty state title
        QLabel *emptyTitle = new QLabel(filtering ? "No matching reflections"
                                                  : "No reflections yet");
        emptyTitle->setStyleSheet(
            "font-size:18px; font-weight:700; color:#173c2c; border:none;");

        // Empty state body message
        QLabel *emptyBody = new QLabel(
            filtering ? "Try a different search term or time period."
                      : "Your saved entries will appear here after you write and save "
                        "your first journal reflection.");
        emptyBody->setWordWrap(true);
        emptyBody->setStyleSheet(
            "font-size:14px; color:#5a6f62; border:none; line-height:160%;");

        // Add empty state labels
        emptyLayout->addWidget(emptyTitle);
        emptyLayout->addSpacing(6);
        emptyLayout->addWidget(emptyBody);

        // Add empty card to entries layout
        m_entriesLayout->addWidget(emptyCard);
        m_entriesLayout->addStretch();
        return;
    }

    // Create a card for each saved entry
    for (const JournalEntry &entry : entries) {

        // Build title/body/excerpt for display
        const EntryDisplayParts parts = buildEntryDisplay(entry);

        // Card frame for one journal entry
        QFrame *card = new QFrame();
        card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        card->setStyleSheet(
            "QFrame {"
            "  background:#e8f1e4;"
            "  border:1px solid #c7d8c1; border-left:4px solid #93ad89;"
            "  border-radius:8px;"
            "}");

        // Tooltip shows full body when hovering over card
        card->setToolTip(parts.body);

        // Layout inside card
        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(30, 22, 30, 24);
        cardLayout->setSpacing(12);

        // ── Top row: title + action buttons ──────────────────────────────────
        // Row that contains entry title, export button, and delete button
        QWidget *topRow = new QWidget();
        topRow->setStyleSheet("background:transparent; border:none;");
        QHBoxLayout *topLayout = new QHBoxLayout(topRow);
        topLayout->setContentsMargins(0, 0, 0, 0);
        topLayout->setSpacing(10);

        // Entry title label
        QLabel *titleLabel = new QLabel(parts.title);
        titleLabel->setWordWrap(true);
        titleLabel->setStyleSheet(
            "font-size:18px; font-weight:800; color:#173c2c; border:none;");

        // ── Feature 6: Export button with dropdown menu ───────────────────────
        // Button that opens export menu
        QPushButton *exportBtn = new QPushButton("Export ▾");
        exportBtn->setCursor(Qt::PointingHandCursor);
        exportBtn->setStyleSheet(
            "QPushButton {"
            "  background:#f0f8ed; border:1px solid #b8d4b0;"
            "  border-radius:9px; color:#2d5c39;"
            "  font-size:12px; font-weight:700; padding:6px 12px;"
            "}"
            "QPushButton:hover { background:#e4f0e0; border-color:#9ac094; }"
            "QPushButton::menu-indicator { image:none; }");

        // Dropdown menu for export options
        QMenu *exportMenu = new QMenu(exportBtn);
        exportMenu->setStyleSheet(
            "QMenu {"
            "  background:#ffffff; border:1px solid #c7d8c1; border-radius:8px; padding:4px;"
            "}"
            "QMenu::item { padding:8px 20px; font-size:13px; color:#173c2c; border-radius:6px; }"
            "QMenu::item:selected { background:#edf5eb; }");

        // PDF export option
        exportMenu->addAction(lucideIcon("file-text", QColor("#173c2c"), 18),
                              "Save as PDF", this, [this, entry]() { exportAsPdf(entry); });

        // Plain text export option
        exportMenu->addAction(lucideIcon("pencil", QColor("#173c2c"), 18),
                              "Save as Plain Text", this, [this, entry]() { exportAsText(entry); });

        // Attach menu to export button
        exportBtn->setMenu(exportMenu);

        // Delete button
        QPushButton *deleteBtn = new QPushButton("Delete");
        deleteBtn->setCursor(Qt::PointingHandCursor);
        deleteBtn->setStyleSheet(
            "QPushButton {"
            "  background:#fff9f6; border:1px solid #efd3cc; border-radius:9px;"
            "  color:#a33a2e; font-size:12px; font-weight:700; padding:6px 12px;"
            "}"
            "QPushButton:hover { background:#fff0ec; border-color:#e1b9b0; }");

        // Save file path for this entry so delete button knows what to delete
        const QString path = entry.filePath();

        // Connect delete button to delete function
        connect(deleteBtn, &QPushButton::clicked, this, [this, path]() {
            deleteEntryAtPath(path);
        });

        // Add title and buttons to top row
        topLayout->addWidget(titleLabel, 1);
        topLayout->addWidget(exportBtn, 0, Qt::AlignTop);
        topLayout->addWidget(deleteBtn, 0, Qt::AlignTop);

        // Add top row to card
        cardLayout->addWidget(topRow);

        // Entry preview/excerpt label
        QLabel *bodyLbl = new QLabel(parts.excerpt);
        bodyLbl->setWordWrap(true);
        bodyLbl->setStyleSheet(
            "font-size:14px; color:#4f6255; border:none; line-height:165%;");
        cardLayout->addWidget(bodyLbl);

        // Date/time label for the entry
        QLabel *dateLbl = new QLabel(entry.dateTime().toString("MMM d, yyyy · h:mm AP"));
        dateLbl->setStyleSheet(
            "font-size:13px; font-weight:600; color:#71856f; border:none;");
        cardLayout->addWidget(dateLbl);

        // Add completed card to entries layout
        m_entriesLayout->addWidget(card);
    }

    // Push cards upward and fill remaining space
    m_entriesLayout->addStretch();
}

// ─────────────────────────────────────────────────────────────────────────────
// Delete
// Handles deleting a journal entry after user confirmation.
// ─────────────────────────────────────────────────────────────────────────────
void Journal::deleteEntryAtPath(const QString &path) {

    // Load selected entry from file path
    const JournalEntry entry = m_storage.loadEntry(path);

    // Build readable title/body parts for confirmation dialog
    const EntryDisplayParts parts = buildEntryDisplay(entry);

    // Confirmation dialog
    QMessageBox dialog(this);
    dialog.setWindowTitle("MindEase");
    dialog.setIcon(QMessageBox::NoIcon);
    dialog.setText("Delete this reflection?");

    // Dialog description includes entry title
    dialog.setInformativeText(
        QString("This will remove \"%1\" from your saved journal entries.").arg(parts.title));

    dialog.setTextFormat(Qt::PlainText);

    // Style the delete confirmation dialog
    dialog.setStyleSheet(
        "QMessageBox { background:#f4f5ec; border:1px solid #c8d5c4; }"
        "QLabel { color:#264437; }"
        "QLabel#qt_msgbox_label { font-size:20px; font-weight:700; min-width:320px; }"
        "QLabel#qt_msgbox_informativelabel { font-size:14px; color:#5a6f62; min-width:320px; }"
        "QPushButton { background:#edf3e6; color:#264437; border:1px solid #c3d2bf;"
        "  border-radius:8px; min-width:100px; min-height:38px;"
        "  padding:8px 16px; font-size:13px; font-weight:700; }"
        "QPushButton:hover { background:#e4ecde; border-color:#aebfa8; }");

    // Add Cancel and Delete buttons
    QPushButton *cancelBtn = dialog.addButton("Cancel", QMessageBox::RejectRole);
    QPushButton *deleteBtn = dialog.addButton("Delete", QMessageBox::DestructiveRole);

    // Style the delete button
    deleteBtn->setStyleSheet(
        "QPushButton { background:#e8f1e4; color:#234030;"
        "  border:1px solid #aac19f; border-radius:8px;"
        "  min-width:100px; min-height:38px; padding:8px 16px;"
        "  font-size:13px; font-weight:700; }"
        "QPushButton:hover { background:#d3e5cc; border-color:#96af8b; }");

    // Default and escape buttons are Cancel for safety
    dialog.setDefaultButton(cancelBtn);
    dialog.setEscapeButton(cancelBtn);

    // Show dialog
    dialog.exec();

    // If user did not click Delete, stop here
    if (dialog.clickedButton() != deleteBtn) return;

    // Delete entry from storage
    if (!m_storage.deleteEntry(path)) {
        showStatus("Could not delete that entry.", false);
        return;
    }

    // Show success message and refresh list
    showStatus("Entry deleted.");
    refreshEntryList();
}

// ─────────────────────────────────────────────────────────────────────────────
// Feature 6: Export
// Handles exporting journal entries as PDF or plain text.
// ─────────────────────────────────────────────────────────────────────────────

QString Journal::buildHtmlExport(const JournalEntry &entry) const {

    // Build display parts for export
    const EntryDisplayParts parts = buildEntryDisplay(entry);

    // Format date for export page
    const QString dateStr = entry.dateTime().toString("dddd, MMMM d, yyyy · h:mm AP");

    // Return HTML template used for PDF export
    return QString(R"(
<html><head><style>
  body { font-family: Georgia, serif; color: #1a2e1b; max-width:720px; margin:60px auto; padding:0 40px; }
  h1   { font-size:28px; font-weight:bold; color:#173c2c; margin-bottom:6px; }
  .meta{ font-size:13px; color:#71856f; margin-bottom:28px; }
  hr   { border:none; border-top:1px solid #c7d8c1; margin:24px 0; }
  p    { font-size:16px; line-height:1.75; white-space:pre-wrap; color:#365143; }
  .footer { font-size:11px; color:#9eaa9d; margin-top:60px; }
</style></head><body>
  <h1>%1</h1>
  <div class="meta">%2</div>
  <hr>
  <p>%3</p>
  <div class="footer">Exported from MindEase · YangonDevs · BMCC</div>
</body></html>
)").arg(parts.title.toHtmlEscaped(), dateStr.toHtmlEscaped(), parts.body.toHtmlEscaped());
}

void Journal::exportAsPdf(const JournalEntry &entry) {

    // Create filename using entry date
    const QString dateTag = entry.dateTime().toString("yyyy-MM-dd");

    // Default save location is Desktop
    const QString defaultPath =
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
        + "/" + dateTag + "_journal.pdf";

    // Ask user where to save the PDF
    const QString path = QFileDialog::getSaveFileName(
        this, "Export Journal Entry as PDF", defaultPath, "PDF Files (*.pdf)");

    // If user cancels, stop
    if (path.isEmpty()) return;

    // Configure printer as PDF output
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(path);
    printer.setPageMargins(QMarginsF(20, 20, 20, 20), QPageLayout::Millimeter);

    // Create document from HTML and print it as PDF
    QTextDocument doc;
    doc.setHtml(buildHtmlExport(entry));
    doc.print(&printer);

    // Show success message
    showStatus("Exported PDF to Desktop");
}

void Journal::exportAsText(const JournalEntry &entry) {

    // Create filename using entry date
    const QString dateTag = entry.dateTime().toString("yyyy-MM-dd");

    // Default save location is Desktop
    const QString defaultPath =
        QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)
        + "/" + dateTag + "_journal.txt";

    // Ask user where to save the text file
    const QString path = QFileDialog::getSaveFileName(
        this, "Export Journal Entry as Text", defaultPath, "Text Files (*.txt)");

    // If user cancels, stop
    if (path.isEmpty()) return;

    // Open file for writing
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        showStatus("Could not write file.", false);
        return;
    }

    // Write journal content to text file
    QTextStream out(&f);
    out << "MindEase Journal Entry\n";
    out << "======================\n\n";
    out << entry.dateTime().toString("dddd, MMMM d, yyyy  h:mm AP") << "\n\n";
    out << QString(30, QChar(0x2500)) << "\n\n";
    out << entry.body() << "\n\n";
    out << QString(30, QChar(0x2500)) << "\n";
    out << "Exported from MindEase · YangonDevs · BMCC\n";

    // Close file after writing
    f.close();

    // Show success message
    showStatus("Exported plain text to Desktop");
}