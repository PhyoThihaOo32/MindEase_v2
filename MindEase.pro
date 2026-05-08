# Qt modules used by MindEase.
QT += core gui widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Build configuration.
CONFIG += c++17

# Qt 6.11 on recent macOS/Clang can warn from qyieldcpu.h unless the ARM
# intrinsic declarations are included before Qt headers.
QMAKE_CXXFLAGS += -include arm_acle.h

TARGET = MindEase
TEMPLATE = app

# Project include directories.
INCLUDEPATH += \
    . \
    app \
    core \
    screens \
    models \
    storage

# Application source files.
SOURCES += \
    app/main.cpp \
    app/mainwindow.cpp \
    core/lucideicons.cpp \
    core/screen.cpp \
    screens/home.cpp \
    screens/recommendations.cpp \
    screens/toolkit.cpp \
    screens/journal.cpp \
    models/journalentry.cpp \
    storage/journalstorage.cpp

# Application header files.
HEADERS += \
    app/mainwindow.h \
    core/lucideicons.h \
    core/screen.h \
    screens/home.h \
    screens/recommendations.h \
    screens/toolkit.h \
    screens/journal.h \
    models/journalentry.h \
    storage/journalstorage.h

# Embedded resources.
RESOURCES += \
    resources/resources.qrc
