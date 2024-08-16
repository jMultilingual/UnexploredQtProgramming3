#include "MainWindow.h"
#include "ListView.h"
#include "TableView.h"
#include "TreeView.h"
#include "MyListView.h"
#include <QApplication>
#include <QHBoxLayout>

#include <QPushButton>
#include <QStatusBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    ListView *l = new ListView();
    TableView *t = new TableView();
    TreeView *tr = new TreeView();
    MyListView *mv = new MyListView();
    QHBoxLayout *h = new QHBoxLayout();
    h->addWidget(l);
    h->addWidget(t);
    h->addWidget(tr);
    h->addWidget(mv);
    QWidget *widget = new QWidget();
    widget->setLayout(h);
    w.setCentralWidget(widget);

    QPushButton *saveButton = new QPushButton("save");
    QPushButton *loadButton = new QPushButton("load");
    // QObject::connect(saveButton, &QPushButton::clicked, l, &ListView::save);
    // QObject::connect(loadButton, &QPushButton::clicked, l, &ListView::load);
    // QObject::connect(saveButton, &QPushButton::clicked, t, &TableView::save);
    // QObject::connect(loadButton, &QPushButton::clicked, t, &TableView::load);
    QObject::connect(saveButton, &QPushButton::clicked, tr, &TreeView::save);
    QObject::connect(loadButton, &QPushButton::clicked, tr, &TreeView::load);
    w.statusBar()->addWidget(saveButton);
    w.statusBar()->addWidget(loadButton);
    w.show();
    return a.exec();
}
