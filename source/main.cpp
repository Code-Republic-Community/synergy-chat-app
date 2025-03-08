#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/resources/icon.ico"));
    MainWindow w;
    QObject::connect(&w, &MainWindow::theme_changed, &w, &MainWindow::changeTheme);
    w.show();
    return a.exec();
}
