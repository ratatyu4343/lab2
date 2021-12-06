#ifndef NODE_H
#define NODE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QLabel>
#include <database.h>
#include <QHash>
#include <QDebug>
#include <QTextBrowser>
#include <form.h>

class Node : public QWidget
{
    Q_OBJECT
public:
    Node(QWidget *parent = nullptr,
         DataBase* db = nullptr,
         int id = -1,
         bool archivated = false);

    //при наведенні на нотатку показуємо кнопки
    virtual void enterEvent(QEvent * event) override;
    virtual void leaveEvent(QEvent * event) override;

    ~Node();

public slots:
    //при нажиманні видалити
    void del_clicked();

    //при нажиманні архівувати
    void archivated_clicked();

signals:
    //сигнал нажимання видалення
    void del_clicked_signal();

    //сигнал нажимання архівування
    void archivated_signal();

    //сигнал вікна, яке хочуть редагувати
    void update_node_signal(int id);

private:
    int id;
    bool is_archivated = false;
    QPushButton* archivated;
    QPushButton* del;
    DataBase* db;
};

//клас QTextBrowser, перевизначена подія нажимання мишки
class TextBrowser : public QTextBrowser
{
Q_OBJECT;
public:
    TextBrowser(QWidget *parent = nullptr, DataBase* db = nullptr, int id = 0)
    {
        this->db = db;
        this->id = id;
    };

    //при нажиманні на блок відкривати вікно для редагування
    virtual void mousePressEvent(QMouseEvent * event) override;

signals:
    //сигнал вікна, яке хочуть редагувати
    void update_clicked_signal(int id);

private:
    int id;
    DataBase* db;
};

#endif // NODE_H
