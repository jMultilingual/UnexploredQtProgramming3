#include "TreeView.h"
#include "AbstractTreeModel.h"
#include <QSaveFile>
#include <QFile>
#include "Item.h"
TreeView::TreeView() {

    m_treeModel = new AbstractTreeModel(this);
    setModel(m_treeModel);
    setDefaultDropAction(Qt::MoveAction);
    setSelectionMode(QTreeView::ContiguousSelection);
    setDragDropMode(QTreeView::InternalMove);

}

void TreeView::save()
{
    QSaveFile file("dummy_tree.dat");
    AbstractTreeModel *m = qobject_cast<AbstractTreeModel*>(model());
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);

        int count = m->rowCount(QModelIndex());
        QModelIndexList list;

        for (int c = 0; c < count ; c++)
        {
            list.append(m->index(c, 0, QModelIndex()));
        }
        for (const QModelIndex& index: list)
        {
            m->writeDataRecur(index, out);
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
        qDebug() << 56;
        AbstractTreeModel* m = qobject_cast<AbstractTreeModel*>(model());

        m->removeRows(0, m->rowCount(QModelIndex()), QModelIndex());
        // qDebug() << 61;
        int count = 0;

        QMap<QString, Item*> map;
        while (!out.atEnd())
        {

            Item *item = new Item(this);
            out >> *item;
            map.insert(item->uuid(), item);
            bool hasParent;
            out >> hasParent;

            if (hasParent)
            {
                QString pstr;
                out >> pstr;

                if (!map.contains(pstr))
                {
                    m->m_rootItem->addChild(item);
                }
                else if (map.contains(pstr))
                {
                    Item *pi = map.value(pstr);
                    pi->addChild(item);
                }
            }
        }
    }
    file.close();
}




