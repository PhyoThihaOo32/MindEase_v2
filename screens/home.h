#ifndef HOME_H // Prevents multiple inclusion of this header file
#define HOME_H

#include "core/screen.h" // Base Screen class

class Home : public Screen { // Home screen inherits from Screen
    Q_OBJECT // Enables Qt signals and meta-object features

public:
    explicit Home(QWidget *parent = nullptr); // Constructor

    QString screenId() const override { return "home"; } // Returns unique screen ID

signals:
    void requestScreen(const QString &screenId); // Signal used to request navigation to another screen
};

#endif // HOME_H