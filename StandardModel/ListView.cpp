#include "ListView.h"
#include "StandardItemListModel.h"
#include "StyledItemDelegate.h"
#include <QListView>
#include <QSaveFile>
#include <QFile>

ListView::ListView()
{
    setDragDropMode(QListView::DragDrop);
    setDefaultDropAction(Qt::MoveAction);
    StandardItemListModel *model = new StandardItemListModel();
    QStandardItem *i1 = new QStandardItem();
    i1->setIcon(QIcon(":/images/1.jpg"));
    i1->setText("item0");
    i1->setFont(QFont("Times New Roman", 72));
    i1->setBackground(QBrush(QColor(105, 171, 197)));
    i1->setBackground(QBrush(QColor(237, 232, 150)));
    i1->setTextAlignment(Qt::AlignCenter|Qt::AlignLeft);
    i1->setToolTip("Tool Tip Neko1");
    i1->setCheckState(Qt::CheckState::Checked);
    i1->setSizeHint(QSize(150, 150));
    i1->setFlags(i1->flags()|Qt::ItemIsUserCheckable|Qt::ItemIsUserTristate);
    model->appendRow(i1);

    QStandardItem *i2 = new QStandardItem();
    i2->setIcon(QIcon(":/images/2.jpg"));
    i2->setText("item1");
    i2->setFont(QFont("Segoe UI Light", 36));
    i2->setBackground(QBrush(QColor(246, 241, 214)));
    i2->setBackground(QBrush(QColor(117, 101,  93)));
    i2->setTextAlignment(Qt::AlignCenter|Qt::AlignLeft);
    i2->setToolTip("Tool Tip Neko1");
    i2->setCheckState(Qt::CheckState::PartiallyChecked);
    i2->setSizeHint(QSize(150, 150));
    i2->setFlags(i2->flags()|Qt::ItemIsUserCheckable|Qt::ItemIsUserTristate);
    model->appendRow(i2);

    QStandardItem *i3 = new QStandardItem();
    i3->setIcon(QIcon(":/images/3.jpg"));
    i3->setText("item2");
    i3->setFont(QFont("Times New Roman", 18));
    i3->setBackground(QBrush(QColor(176, 229, 213)));
    i3->setBackground(QBrush(QColor(213, 187, 216)));
    i3->setTextAlignment(Qt::AlignCenter|Qt::AlignLeft);
    i3->setToolTip("Tool Tip Neko1");
    i3->setCheckState(Qt::CheckState::Unchecked);
    i3->setSizeHint(QSize(150, 150));
    i3->setFlags(i3->flags()|Qt::ItemIsUserCheckable|Qt::ItemIsUserTristate);
    model->appendRow(i3);

    StyledItemDelegate *delegate = new StyledItemDelegate();
    setModel(model);
    setItemDelegate(delegate);
}

void ListView::save()
{
    QSaveFile file = QSaveFile("dummy.dat");
    if (file.open(QIODevice::WriteOnly))

    {

        QDataStream out(&file);

        for (int row = 0; row < model()->rowCount(); row++)
        {

            StandardItemListModel *m = qobject_cast<StandardItemListModel*>(model());
            QModelIndex index = m->index(row, 0);
            QStandardItem *s = m->itemFromIndex(index);
            s->write(out);

        }
    }
    file.commit();
}

void ListView::load()
{
    QFile file("dummy.dat");
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QDataStream out(&file);
            StandardItemListModel *m = qobject_cast<StandardItemListModel*>(model());
            m->clear();
            while (!out.atEnd())
            {
                QStandardItem *s = new QStandardItem();
                s->read(out);
                m->appendRow(s);
            }
        }
    }
    file.close();
}
