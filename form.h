#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QTextEdit>
#include <QDialog>
#include <QComboBox>
#include <QDateTime>
#include <QList>
#include <QLineEdit>
#include <database.h>
#include <notes.h>

//клас для додавання нового запису
class Form : public QDialog
{
   Q_OBJECT
public:
    //додавання запису
    Form(QWidget *parent = nullptr,
         QString name = "Новий запис",
         QString content = "",
         int id = -1,
         QString contekst = "Без теми",
         DataBase* db = nullptr);
    ~Form();
public slots:
    //при зміні форми активуємо кнопку зберегти
    void textChange();

    //при нажиманні кнопки зберегти записуємо в бд та закриваємо вікно
    void save_cliked();

signals:
    //сигнал кнопки збереження
    void save_signal();

private:
    int id;
    QLineEdit* name;
    QComboBox* contekst;
    QPushButton* save;
    QTextEdit* text;
    DataBase* db;
    QString time;
};
#endif // FORM_H
