#include "TreeView.h"
#include "StandardItemTreeModel.h"
#include <QIODevice>
#include <QDataStream>
#include <QUuid>
#include <QSaveFile>
#include <QFile>

TreeView::TreeView() {

    setDragDropMode(QTreeView::DragDrop);
    setDefaultDropAction(Qt::MoveAction);
    setSelectionMode(QTreeView::ContiguousSelection);
    StandardItemTreeModel *model = new StandardItemTreeModel();
    setModel(model);
}

void TreeView::save()
{
    QSaveFile file("dummy_tree.dat");
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        StandardItemTreeModel* m = qobject_cast<StandardItemTreeModel*>(model());
        int count = m->rowCount(QModelIndex());
        QModelIndexList list;
        for (int c = 0; c < count ; c++)
        {
            list.append(m->index(c, 0, QModelIndex()));
        }
        for (const QModelIndex& index: list)
        {
            m->writeMimeDataRecur(index, out);
        }
    }
    file.commit();
}

void TreeView::load()
{
    QFile file("dummy_tree.dat");

    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream out(&file);
        StandardItemTreeModel* m = qobject_cast<StandardItemTreeModel*>(model());
        QMap<QString, QStandardItem*> map;
        while (!out.atEnd())
        {
            QStandardItem *item = new QStandardItem();
            item->read(out);
            Qt::ItemFlag flag;
            out >> flag;
            QString str;
            out >> str;
            map.insert(str, item);
            bool hasParent;
            out >> hasParent;
            item->setFlags(flag);
            item->setData(str, Qt::UserRole);

            if (hasParent)
            {

                QString pstr;
                out >> pstr;
                // bool parentIsContained;
                // out >> parentIsContained;

                if (!map.contains(pstr))
                {
                    m->appendRow(item);
                }
                if (map.contains(pstr))
                {
                    QStandardItem *pi = map.value(pstr);
                    pi->setChild(pi->rowCount(), item);

                }
            }
            else
            {
                m->appendRow(item);
            }
        }
    }

}
