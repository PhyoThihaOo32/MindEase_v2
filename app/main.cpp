#include "app/mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include <stdexcept>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("MindEase");
    app.setOrganizationName("MindEase");

    try {
        MainWindow w;
        w.show();
        return app.exec();
    } catch (const std::exception &e) {
        QMessageBox::critical(nullptr, "MindEase Error",
            QString("An unexpected error occurred:\n%1").arg(e.what()));
        return 1;
    } catch (...) {
        QMessageBox::critical(nullptr, "MindEase Error",
            "An unexpected error occurred.");
        return 1;
    }
}
