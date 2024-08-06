#include "MainWindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QListWidget>
#include <QListWidgetItem>

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
    QListWidget *l = new QListWidget();
    QListWidgetItem *i1 = new QListWidgetItem(
                              QIcon(":/images/1.jpg"),
                              "item1"

        );
    QListWidgetItem *i2 = new QListWidgetItem(
        QIcon(":/images/2.jpg"),
        "item2"
        );
    QListWidgetItem *i3 = new QListWidgetItem(
        QIcon(":/images/3.jpg"),
        "item2"

        );
    l->addItem(i1);
    l->addItem(i2);
    l->addItem(i3);
    QSize size(150, 150);
    l->setIconSize(size);
    w.setCentralWidget(l);
    w.show();
    return a.exec();
}
