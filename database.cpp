#include "database.h"

DataBase::DataBase(QObject* parent)
{
    db = db.QSqlDatabase::addDatabase("QSQLITE");
    db.QSqlDatabase::setDatabaseName(QDir::currentPath() + "/notes.db");

    //якщо БД ще не інує то створюємо
    db.open();
    QSqlQuery q;
    q.exec("CREATE TABLE IF NOT EXISTS notes ("
           "id       INTEGER PRIMARY KEY AUTOINCREMENT,"
           "name     STRING,"
           "text     TEXT,"
           "archive  BOOLEAN DEFAULT (false),"
           "contekst STRING,"
           "time     TIME );");
}

//запис/перезапис
void DataBase::write_note(int id, bool flag, QString name, QString text, QString contekst, QString time)
{
    QSqlQuery q;
    if(id == -1){
        q.prepare("INSERT INTO  notes (name, text, archive, contekst, time)"
                  "VALUES (:NAME, :TEXT, :ARCHIVE,  :CONTEKST, :TIME);");
        q.bindValue(":NAME", name);
        q.bindValue(":TEXT", text);
        q.bindValue(":ARCHIVE", flag);
        q.bindValue(":CONTEKST", contekst);
        q.bindValue(":TIME", time);
    }
    else{
        q.prepare("UPDATE notes SET name = :N, text = :TEXT, contekst = :C, time = :T WHERE id = :ID");
        q.bindValue(":N", name);
        q.bindValue(":TEXT", text);
        q.bindValue(":C", contekst);
        q.bindValue(":T", time);
        q.bindValue(":ID", id);
    }
    q.exec();
}

//отримання наявних тем
QList<QString> DataBase::get_conteksts(bool flag)
{
    QList<QString> conteksts;
    QSqlQuery q;
    q.prepare("SELECT DISTINCT contekst FROM notes WHERE archive = :B");
    q.bindValue(":B", flag);
    q.exec();
    while(q.next())
        conteksts.append(q.value("contekst").toString());
    return conteksts;
}

//отримання запису по id
QHash<QString, QString> DataBase::get_node(int id)
{
    QHash<QString, QString> node;

    QSqlQuery q;
    q.prepare("SELECT \"name\", \"text\", \"contekst\", \"time\""
              "FROM notes WHERE id = :ID");
    q.bindValue(":ID", id);
    if(q.exec())
    {
        q.first();
        node.insert("name", q.value("name").toString());
        node.insert("text", q.value("text").toString());
        node.insert("contekst", q.value("contekst").toString());
        node.insert("time", q.value("time").toString());
    }
    return node;
}

//отримання списку записів по деякій темі
QList<int> DataBase::get_nodes(QString contekst, bool flag, int key)
{
    QList<int> list;
    QSqlQuery q;
    QString q_text;
    if(contekst == "Без теми" || contekst == "Всі нотатки")
    {
        q_text = "SELECT id FROM notes WHERE archive = ";
    }
    else
    {
        q_text = "SELECT id FROM notes WHERE contekst = \"" +contekst+ "\" AND archive = ";
    }
    //СОРТУВАННЯ
    q_text += flag?"1":"0";
    q_text += " ORDER BY ";
    if(key == 0){
        q_text += " name ";
    }
    else if(key == 1){
        q_text += " name DESC";
    }
    else if(key == 2){
        q_text += " time DESC ";
    }
    else if(key == 3){
        q_text += " time ";
    }
    q.exec(q_text);
    for(int i = 0; q.next(); i++)
    {
        list.append(q.value("id").toInt());
    }

    return list;
}

//видалення запису
void DataBase::del_node(int id)
{
    QSqlQuery q;
    q.prepare("DELETE FROM notes WHERE id = :ID;");
    q.bindValue(":ID", id);
    q.exec();
}

//переміщення до/з архіву
void DataBase::archivated(bool b, int id)
{
    QSqlQuery q;
    q.prepare("UPDATE notes SET archive = :B WHERE id = :ID");
    q.bindValue(":B", b);
    q.bindValue(":ID", id);
    q.exec();
}

DataBase::~DataBase()
{
    db.close();
}

