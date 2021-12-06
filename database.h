#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QString>
#include <QSqlQuery>
#include <QObject>
#include <QList>
#include <QHash>
#include <QDir>
#include <QFile>

//клас для взаємодії з базою данних
class DataBase : public QObject
{
    Q_OBJECT;
public:
    DataBase(QObject* parent = nullptr);

    //запис/перезапис
    void write_note(int id, bool flag, QString name, QString text, QString contekst, QString time);

    //отримання наявних тем
    QList<QString> get_conteksts(bool flag);

    //отримання запису по id
    QHash<QString, QString> get_node(int id);

    //отримання списку записів по деякій темі
    QList<int> get_nodes(QString contekst, bool flag, int key);

    //видалення запису
    void del_node(int id);

    //переміщення до/з архіву
    void archivated(bool b, int id);

    ~DataBase();

private:
    //об'єкт бази даних
    QSqlDatabase db;
};
#endif // DATABASE_H
