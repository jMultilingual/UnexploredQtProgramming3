#include "MainWindow.h"
#include "StyledItemDelegate.h"
#include "ListWidget.h"
#include "TableWidget.h"
#include "TreeWidget.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStatusBar>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "WidgetModel_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    ListWidget *l = new ListWidget();
    TableWidget *t = new TableWidget(12, 10);
    TreeWidget *tr = new TreeWidget();
    QHBoxLayout *h = new QHBoxLayout();
    QWidget *widget = new QWidget();
    h->addWidget(l);
    h->addWidget(t);
    h->addWidget(tr);
    widget->setLayout(h);
    w.setCentralWidget(widget);
    QPushButton *saveButton = new QPushButton("Save");
    saveButton->setObjectName("saveButton");
    QPushButton *loadButton = new QPushButton("Load");
    loadButton->setObjectName("loadButton");
    w.statusBar()->addPermanentWidget(saveButton);
    w.statusBar()->addPermanentWidget(loadButton);
    QObject::connect(saveButton, &QPushButton::clicked, l, &ListWidget::save);
    QObject::connect(loadButton, &QPushButton::clicked, l, &ListWidget::load);
    QObject::connect(saveButton, &QPushButton::clicked, t, &TableWidget::save);
    QObject::connect(loadButton, &QPushButton::clicked, t, &TableWidget::load);
    QObject::connect(saveButton, &QPushButton::clicked, tr, &TreeWidget::save);
    QObject::connect(loadButton, &QPushButton::clicked, tr, &TreeWidget::load);
    w.show();
    return a.exec();
}
