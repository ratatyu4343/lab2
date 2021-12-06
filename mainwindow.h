#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QDir>
#include <notes.h>
#include "windows.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

    //при закритті згортаємо програму або закриваємо
    void closeEvent(QCloseEvent * event) override;

    //обробляємо нажаття гарячих клавіш
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    ~MainWindow();

signals:
    //сигнал для відкриття вікна додавання нової нотатки
    void add_kays_clicked();

    //сигнал про закриття програми
    void exited();

};
#endif // MAINWINDOW_H
