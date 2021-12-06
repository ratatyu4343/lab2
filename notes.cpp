#include "notes.h"

Notes::Notes(QWidget *parent) : QWidget(parent)
{
    is_archived = false;
    this->setFixedWidth(600);
    db = new DataBase();

    contekst = new QComboBox(this);
    contekst->setMinimumHeight(50);

    archive = new QPushButton("Архів", this);
    archive->setMinimumHeight(50);
    archive->setMaximumWidth(160);
    connect(archive, &QPushButton::clicked, this, &Notes::archive_clicked);

    b[0] = new QRadioButton("А-Я", this);
    b[1] = new QRadioButton("Я-А", this);
    b[2] = new QRadioButton("Нові-Старі", this);
    b[3] = new QRadioButton("Старі-Нові", this);

    add = new QPushButton("+", this);
    add->setGeometry(450, 650, 50, 50);
    add->setStyleSheet( "QPushButton{font-size: 50px;"
                        "background-color: SandyBrown;"
                        "border-style: solid;"
                        "border-width: 5px;"
                        "border-radius: 50px;"
                        "border-color: black;"
                        "min-width: 90px;"
                        "min-height: 90px;}"
                        "QPushButton:hover{background-color: LightSkyBlue;}" );
    connect(add, &QPushButton::clicked, this, &Notes::add_clicked);

    auto vert = new QVBoxLayout(this);
    auto top = new QHBoxLayout();
    auto vert_b = new QGridLayout();
    grid = new QGridLayout();

    nodes_area = new QScrollArea(this);
    auto conteiner = new QWidget(nodes_area);

    conteiner->setLayout(grid);
    nodes_area->setWidget(conteiner);
    update_nodes("Всі теми");

    vert_b->addWidget(b[2], 0, 0);
    vert_b->addWidget(b[3], 1, 0);
    vert_b->addWidget(b[0], 0, 1);
    vert_b->addWidget(b[1], 1, 1);
    b[2]->setChecked(true);
    connect(b[0], &QRadioButton::clicked, this, &Notes::update_all);
    connect(b[1], &QRadioButton::clicked, this, &Notes::update_all);
    connect(b[2], &QRadioButton::clicked, this, &Notes::update_all);
    connect(b[3], &QRadioButton::clicked, this, &Notes::update_all);

    top->addWidget(contekst, Qt::AlignLeft);
    top->addLayout(vert_b);
    top->addWidget(archive, Qt::AlignRight);

    vert->addLayout(top);
    vert->addWidget(nodes_area);

    setLayout(vert);
    update_all();
    connect(contekst, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &Notes::update_all);
    connect(static_cast<MainWindow*>(parent), &MainWindow::add_kays_clicked, this, &Notes::add_clicked);
}

//при нажиманні кнопки додавання
void Notes::add_clicked()
{
    auto q = new Form(this, "Новий запис", "", -1, "Без теми", db);
    q->show();
    connect(q, &Form::save_signal, this, &Notes::update_all);
}

//перестворення списку нотаток
void Notes::update_all()
{
    contekst->blockSignals(true);
    update_contekst();
    update_nodes(contekst->currentText());
    contekst->blockSignals(false);
}
void Notes::update_contekst()
{
    QString curent_contekst = contekst->currentText();
    contekst->clear();
    QList<QString> conteksts = db->get_conteksts(is_archived);
    if(curent_contekst != "Всі нотатки" && !db->get_nodes(curent_contekst, is_archived, get_sort()).isEmpty())
    {
        conteksts.removeAll(curent_contekst);
        contekst->addItem(curent_contekst);
    }
    conteksts.removeAll("Всі нотатки");
    conteksts.removeAll("Без теми");
    contekst->addItem("Всі нотатки");
    contekst->addItems(conteksts);
}
void Notes::update_nodes(QString contekst)
{
    if(grid->count())
    {
        while(QLayoutItem* item = grid->takeAt(0))
        {
            delete item->widget();
        }
    }
    QList<int> list = db->get_nodes(contekst, is_archived, get_sort());
    int n = list.size();
    for(int g = 0; g < n; g++)
    {
        Node *f = new Node(nodes_area, db, list.takeFirst(), is_archived);
        grid->addWidget(f, g/3, g%3);
        connect(f, &Node::del_clicked_signal, this, &Notes::update_all);
        connect(f, &Node::archivated_signal, this, &Notes::update_all);
        connect(f, &Node::update_node_signal, this, &Notes::update_node);
    }
    int haigh = (n/3)*300 + 300*((n%3)?1:0);
    grid->parentWidget()->setFixedSize(550, haigh);
    add->raise();
}

//перезапис нотатки
void Notes::update_node(int id)
{
    QHash<QString, QString> h = this->db->get_node(id);
    Form* f = new Form(this, h["name"], h["text"], id, h["contekst"], this->db);
    f->show();
    connect(f, &Form::save_signal, this, &Notes::update_all);
}

//зчитування режиму сортування
int Notes::get_sort()
{
    for(int i = 0; i < 4; i++)
    {
        if(b[i]->isChecked()) return i;
    }
    return 0;
}

//перехід в/з архіву
void Notes::archive_clicked()
{
    if(archive->text() == "Архів")
    {
        archive->setText("Нотатки");
        is_archived = true;
        add->hide();
    }
    else
    {
        archive->setText("Архів");
        is_archived = false;
        add->show();
    }
    update_all();
}

Notes::~Notes()
{
    delete db;
    delete contekst;
    delete archive;
    delete add;
    delete grid;
    delete nodes_area;
    for(int i = 0; i < 4; i++)
        delete b[i];
}

