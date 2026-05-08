#ifndef LUCIDEICONS_H // Prevents multiple inclusions of this header file
#define LUCIDEICONS_H

#include <QColor> // QColor class for icon color handling
#include <QIcon> // QIcon class for icon objects
#include <QLabel> // QLabel class for displaying icons in labels
#include <QPixmap> // QPixmap class for image rendering
#include <QString> // QString class for string handling

QString lucideIconNameForToken(const QString &token); // Maps a token to a Lucide icon name

QPixmap lucidePixmap(const QString &tokenOrName, // Generates a QPixmap icon
                     const QColor &stroke = QColor("#173c2c"), // Default icon color
                     int size = 24); // Default icon size

QIcon lucideIcon(const QString &tokenOrName, // Generates a QIcon object
                 const QColor &stroke = QColor("#173c2c"), // Default icon color
                 int size = 24); // Default icon size

void setLucideIcon(QLabel *label, // Applies icon directly to a QLabel
                   const QString &tokenOrName, // Icon token or name
                   const QColor &stroke = QColor("#173c2c"), // Default icon color
                   int size = 24); // Default icon size

QString lucideCleanText(QString text); // Cleans or formats text for Lucide icon usage

#endif // LUCIDEICONS_H