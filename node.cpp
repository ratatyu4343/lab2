#include "node.h"

Node::Node(QWidget *parent, DataBase* db, int id, bool arch) : QWidget(parent)
{
    is_archivated = arch;
    this->db = db;
    this->id = id;
    this->setFixedSize(185, 300);

    auto vert = new QVBoxLayout(this);
    auto top = new QHBoxLayout();

    QHash<QString, QString> node = db->get_node(id);
    auto name = new QLabel(node["name"], this);

    this->setStyleSheet("QPushButton {background-color: white}");

    archivated = new QPushButton("", this);
    if(is_archivated)
    {
        archivated->setIcon(QIcon(QPixmap(QDir::currentPath() + "\\unarchive.png")));
    }
    else
    {
        archivated->setIcon(QIcon(QPixmap(QDir::currentPath() + "\\archive.png")));
    }
    archivated->hide();
    connect(archivated, &QPushButton::clicked, this, &Node::archivated_clicked);

    del = new QPushButton("", this);
    del->setIcon(QIcon(QPixmap(QDir::currentPath() + "\\del.png")));
    del->hide();
    connect(del, &QPushButton::clicked, this, &Node::del_clicked);

    top->addWidget(name);
    top->addWidget(archivated);
    top->addWidget(del);

    auto text = new TextBrowser(this, db, id);
    text->setText(node["text"]);
    text->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(text, &TextBrowser::update_clicked_signal, this, &Node::update_node_signal);

    auto time = new QLabel(node["time"], this);
    time->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    vert->addLayout(top);
    vert->addWidget(text);
    vert->addWidget(time);

    setLayout(vert);
    this->setAttribute(Qt::WA_Hover, true);
}

//при нажиманні видалити
void Node::del_clicked()
{
    db->del_node(id);
    this->close();
    emit del_clicked_signal();
}

//при нажиманні архівувати
void Node::archivated_clicked()
{
    db->archivated(!is_archivated, id);
    this->close();
    emit archivated_signal();
}

//при наведенні на нотатку показуємо кнопки
void Node::enterEvent(QEvent * event)
{
    del->show();
    archivated->show();
}
void Node::leaveEvent(QEvent * event)
{
    del->hide();
    archivated->hide();
}


Node::~Node()
{
    delete archivated;
    delete del;
}

//при нажиманні на блок відкривати вікно для редагування
void TextBrowser::mousePressEvent(QMouseEvent *event)
{
    emit update_clicked_signal(this->id);
}


