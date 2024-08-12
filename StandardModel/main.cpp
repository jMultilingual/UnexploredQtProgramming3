#include "MainWindow.h"
#include "ListView.h"
#include "TableView.h"
#include "TreeView.h"


#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QListView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QPushButton>
#include <QStatusBar>
#include <QHBoxLayout>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "StandardModel_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    ListView *l = new ListView();
    TableView *t = new TableView();
    TreeView *tr = new TreeView();
    QHBoxLayout *h = new QHBoxLayout();
    h->addWidget(l);
    h->addWidget(t);
    h->addWidget(tr);
    QPushButton *saveButton = new QPushButton("Save");
    saveButton->setObjectName("saveButton");
    QPushButton *loadButton = new QPushButton("Load");
    loadButton->setObjectName("loadButton");
    w.statusBar()->addPermanentWidget(saveButton);
    w.statusBar()->addPermanentWidget(loadButton);
    QObject::connect(saveButton, &QPushButton::clicked, l, &ListView::save);
    QObject::connect(loadButton, &QPushButton::clicked, l, &ListView::load);
    QObject::connect(saveButton, &QPushButton::clicked, t, &TableView::save);
    QObject::connect(loadButton, &QPushButton::clicked, t, &TableView::load);
    QObject::connect(saveButton, &QPushButton::clicked, tr, &TreeView::save);
    QObject::connect(loadButton, &QPushButton::clicked, tr, &TreeView::load);
    QWidget *widget = new QWidget();
    widget->setLayout(h);
    w.setCentralWidget(widget);
    w.show();
    return a.exec();
}
