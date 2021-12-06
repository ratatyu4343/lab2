#include "form.h"

Form::Form(QWidget *parent, QString n, QString content, int id, QString contekst, DataBase* db) : QDialog(parent)
{
    this->id = id;
    this->db = db;
    this->setMinimumSize(400, 600);
    this->setMaximumSize(400, 600);
    this->setWindowModality(Qt::WindowModal);
    this->setWindowTitle(n);

    auto vert = new QVBoxLayout(this);
    auto top = new QHBoxLayout();
    auto bottom = new QHBoxLayout();

    this->contekst = new QComboBox(this);
    QList<QString> conteksts = db->get_conteksts(false);
    conteksts.removeAll("Без теми");
    conteksts.push_front("Без теми");
    conteksts.removeAll(contekst);
    this->contekst->addItem(contekst);
    this->contekst->addItems(conteksts);
    this->contekst->setEditable(true);
    connect(this->contekst, &QComboBox::currentTextChanged, this, &Form::textChange);

    name = new QLineEdit(n, this);
    connect(name, &QLineEdit::textChanged, this, &Form::textChange);

    text = new QTextEdit(content, this);
    connect(text, &QTextEdit::textChanged, this, &Form::textChange);

    save = new QPushButton("Зберегти", this);
    save->setEnabled(false);
    connect(save, &QPushButton::clicked, this, &Form::save_cliked);

    top->addWidget(name);
    top->addWidget(this->contekst);

    bottom->addStretch(1);
    bottom->addWidget(save);

    vert->addLayout(top);
    vert->addWidget(text);
    vert->addLayout(bottom);
}

void Form::textChange(){
    save->setEnabled(true);
}

void Form::save_cliked(){
    this->time = QDateTime::currentDateTime().toString("yyyy-MM-dd  HH:mm:ss");
    db->write_note(id, false, name->text(), text->toPlainText(), contekst->currentText(), time);
    emit save_signal();
    this->close();
}

Form::~Form()
{
    delete name;
    delete contekst;
    delete save;
    delete text;
}

