#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setFixedSize(600, 800);
    w.move(600, 100);
    w.show();

    a.setQuitOnLastWindowClosed(false);
    QObject::connect(&w, &MainWindow::exited, &a, &QApplication::quit);

    return a.exec();
}
