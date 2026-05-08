# MindEase - BMCC Wellness Companion

MindEase is a desktop wellness support application built for Borough of Manhattan Community College students. It combines curated BMCC resources, a mental health toolkit, and a private local journal in one simple Qt interface.

This project was created for **CSC211H Advanced Programming Techniques (Honors)** at BMCC / CUNY.

> MindEase is not a therapist, doctor, counselor, diagnosis tool, or emergency service. For emergencies or immediate danger, call 911. In the U.S., call or text 988 for crisis support.

## Features

- **BMCC Resources:** Students can search and browse support topics such as study stress, finance, immigration, health, family, and wellness.
- **Resource Search:** Search can match main topics and subtopics, helping students find support faster.
- **Mental Health Toolkit:** Curated self-care folders for academic stress, mindfulness, connection, sleep, music, identity support, nutrition, and journaling.
- **My Journal:** Private local journal entries saved as plain text files on the user's machine.
- **Journal Tools:** Saved entries can be searched, filtered, exported, or deleted.
- **Local-first Storage:** Journal files are stored locally, not in a database or cloud account.

## Tech Stack

| Area | Technology |
|---|---|
| Desktop App | C++17, Qt 6 Widgets |
| UI Architecture | `QMainWindow`, `QStackedWidget`, custom `Screen` subclasses |
| Storage | Local text files using Qt file I/O |
| Build System | qmake |

## Application Screens

- **Home:** Simple landing screen with navigation.
- **BMCC Resources:** Guided resource finder with search and direct links.
- **Mental Health Toolkit:** Self-care and campus wellness tools.
- **My Journal:** Local journal with save, search, filter, export, and delete support.

## Overall Project Flow

```mermaid
flowchart TD
    A[Start MindEase Desktop App] --> B[MainWindow]
    B --> C[Sidebar Navigation]
    C --> D[Home]
    C --> E[BMCC Resources]
    C --> F[Mental Health Toolkit]
    C --> G[My Journal]

    E --> E1[Choose or search support topic]
    E1 --> E2[View curated BMCC resources]
    E2 --> E3[Open campus links or local PDF]

    F --> F1[Choose wellness folder]
    F1 --> F2[Read self-care tools and resources]
    F2 --> F3[Open external links when needed]

    G --> G1[Write private journal entry]
    G1 --> G2[Save as local text file]
    G --> G3[Search or filter past entries]
    G3 --> G4[Export or delete entry]
```

## UML Class Diagram

```mermaid
classDiagram
    direction LR

    class MainWindow {
        -QStackedWidget screenStack
        -QButtonGroup navGroup
        +switchScreen(index)
        +applyStyle()
    }

    class Screen {
        <<abstract>>
        +QString title
        +screenId() QString
        +onActivated()
    }

    class Home {
        +screenId() QString
    }

    class Recommendations {
        -QLineEdit searchInput
        +screenId() QString
        +showHome()
        +applySearch()
    }

    class Toolkit {
        -QLineEdit searchInput
        +screenId() QString
        +showHome()
        +applySearch()
    }

    class Journal {
        -QLineEdit titleEdit
        -QTextEdit editor
        -JournalStorage storage
        -QVector~JournalEntry~ allEntries
        +screenId() QString
        +saveEntry()
        +applyFilter()
        +refreshEntryList()
        +exportAsPdf(entry)
        +exportAsText(entry)
    }

    class JournalEntry {
        -QString body
        -QDateTime dateTime
        -QString filePath
        +body() QString
        +dateTime() QDateTime
        +filePath() QString
    }

    class JournalStorage {
        -QString storageDir
        +saveEntry(entry) bool
        +loadEntries() QVector~JournalEntry~
        +loadEntry(path) JournalEntry
        +deleteEntry(path) bool
    }

    class LucideIcons {
        +lucideIcon(name, color, size) QIcon
        +setLucideIcon(label, name, color, size)
    }

    MainWindow --> Screen : displays
    Screen <|-- Home
    Screen <|-- Recommendations
    Screen <|-- Toolkit
    Screen <|-- Journal
    Journal --> JournalStorage : persists entries
    JournalStorage --> JournalEntry : loads/saves
    MainWindow ..> LucideIcons : navigation icons
    Recommendations ..> LucideIcons : resource icons
    Toolkit ..> LucideIcons : toolkit icons
```

## Project Structure

```text
MindEase/
|-- MindEase.pro
|-- README.md
|-- app/
|   |-- main.cpp
|   `-- mainwindow.h / mainwindow.cpp
|-- core/
|   |-- lucideicons.h / lucideicons.cpp
|   `-- screen.h / screen.cpp
|-- models/
|   `-- journalentry.h / journalentry.cpp
|-- resources/
|   |-- resources.qrc
|   `-- 2025-KYR-Final-01.13.202592.pdf
|-- screens/
|   |-- home.h / home.cpp
|   |-- journal.h / journal.cpp
|   |-- recommendations.h / recommendations.cpp
|   `-- toolkit.h / toolkit.cpp
`-- storage/
    `-- journalstorage.h / journalstorage.cpp
```

## Setup

### 1. Clone the repository

```bash
git clone https://github.com/PhyoThihaOo32/MindEase_v2.git
cd MindEase_v2
```

### 2. Build and run the Qt app

#### Qt Creator

1. Open Qt Creator.
2. Open `MindEase.pro`.
3. Select a Qt 6 desktop kit.
4. Build and run.

#### Command line

```bash
mkdir -p build
cd build
/Users/phyothihaoo/Qt/6.11.0/macos/bin/qmake ../MindEase.pro -spec macx-clang CONFIG+=debug CONFIG+=qml_debug
make -j8
open MindEase.app
```

Your Qt path may be different. If `qmake` is on your PATH, you can use:

```bash
qmake MindEase.pro
make -j4
```

## Journal Storage

Journal entries are saved as plain text files in:

```text
~/Documents/MindEase_Journal/
```

This keeps the journal local and readable without a database or account.

## C++ Concepts Demonstrated

- Object-oriented programming with classes such as `MainWindow`, `Screen`, `Recommendations`, `Toolkit`, and `Journal`.
- Inheritance through the shared abstract `Screen` base class.
- Polymorphism through `Screen*` storage and virtual screen activation.
- Encapsulation through private UI and storage members.
- File I/O through `JournalStorage`.
- Qt signals and slots for navigation, saving, deleting, exporting, and searching.
- Dynamic UI construction from resource data.

## Safety Notes

MindEase provides resource information and journaling tools only. It is not a therapist, doctor, counselor, diagnosis tool, or emergency service.

Emergency resources:

- Call 911 for immediate danger.
- Call or text 988 in the U.S. for crisis support.
- BMCC Counseling Center: Room S-343, (212) 220-8140, counselingcenter@bmcc.cuny.edu.

## Resources

- [BMCC Counseling Center](https://www.bmcc.cuny.edu/student-affairs/counseling/)
- [BMCC Mental Health Toolkit](https://www.bmcc.cuny.edu/student-affairs/counseling/your-mental-health-toolkit/)
- [BMCC Learning Resource Center](https://www.bmcc.cuny.edu/students/lrc/)
- [BMCC Advocacy & Resource Center](https://www.bmcc.cuny.edu/student-affairs/arc/)
- [988 Suicide & Crisis Lifeline](https://988lifeline.org/)

## License

Built for academic purposes at BMCC / CUNY. Resource information belongs to the respective BMCC, CUNY, and external organizations.
