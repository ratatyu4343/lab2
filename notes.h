#ifndef NOTES_H
#define NOTES_H

#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QList>
#include <QScrollArea>
#include <QRadioButton>
#include <form.h>
#include <database.h>
#include <node.h>
#include <mainwindow.h>

class Notes :  public QWidget
{
    Q_OBJECT
public:
    Notes(QWidget *parent = nullptr);

    //при нажиманні кнопки додавання
    void add_clicked();

    //перестворення списку нотаток
    void update_all();
    void update_contekst();
    void update_nodes(QString contekst);

    //перезапис нотатки
    void update_node(int id);

    //зчитування режиму сортування
    int get_sort();

    //перехід в архів
    void archive_clicked();

    ~Notes();

signals:
    //сигнал переходу в/з архіву
    void archivated_change_signal(bool flag);

private:
    bool is_archived;
    DataBase* db;
    QComboBox* contekst;
    QPushButton* archive;
    QPushButton* add;
    QGridLayout *grid;
    QScrollArea* nodes_area;
    QRadioButton* b[4];
};

#endif // NOTES_H
