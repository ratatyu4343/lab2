#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Нотатки");
    setWindowIcon(QIcon(QPixmap(QDir::currentPath() + "\\title.png")));

    auto notes = new Notes(this);
    setCentralWidget(notes);

    auto tray = new QSystemTrayIcon(this);
    tray ->setIcon(QIcon(QPixmap(QDir::currentPath() + "\\unarchive.png")));

    QMenu * menu = new QMenu(this);
    QAction* viewWindow = new QAction("Відкрити нотатки", this);
    QAction* quitAction = new QAction("Вийти", this);

    menu->addAction(viewWindow);
    menu->addAction(quitAction);

    connect(viewWindow, &QAction::triggered, this, &QMainWindow::show);
    connect(quitAction, &QAction::triggered, this, &QMainWindow::close);

    tray->setContextMenu(menu);
    tray->show();

    RegisterHotKey((HWND)winId(), 100, MOD_CONTROL|MOD_ALT, 'N');
}

//при закритті згортаємо програму або закриваємо
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(this->isVisible())
    {
        event->ignore();
        this->hide();
    }
    else
    {
        emit exited();
    }
}

//обробляємо нажимання гарячих клавіш
bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)
    MSG* msg = reinterpret_cast<MSG*>(message);
    if(msg->message == WM_HOTKEY)
    {
        if(msg->wParam == 100)
        {
            emit add_kays_clicked();
            return true;
        }
    }
    return false;
}

MainWindow::~MainWindow()
{

}

