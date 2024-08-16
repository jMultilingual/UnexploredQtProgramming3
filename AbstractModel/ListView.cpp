#include "ListView.h"
#include "AbstractListModel.h"
#include "Item.h"
#include "StyledItemDelegate.h"
#include <QSaveFile>
#include <QIODevice>
#include <QFile>

ListView::ListView()
{
    setDragDropMode(QListView::DragDrop);
    setDefaultDropAction(Qt::MoveAction);

    m_listModel = new AbstractListModel();

    setSelectionMode(QListView::ContiguousSelection);

    setModel(m_listModel);
    StyledItemDelegate *delegate = new StyledItemDelegate();
    setItemDelegate(delegate);
    Item *i1 = new Item();
    i1->setData(QVariant(QIcon(":/images/1.jpg")), Qt::DecorationRole);
    m_listModel->m_list.append(i1);
    Item *i2 = new Item();
    i2->setData(QVariant(QIcon(":/images/2.jpg")), Qt::DecorationRole);
    m_listModel->m_list.append(i2);
    Item *i3 = new Item();
    i3->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m_listModel->m_list.append(i3);
}

void ListView::save()
{
    QSaveFile file("abstract_dummy.dat");
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        AbstractListModel *m = qobject_cast<AbstractListModel*>(model());
        out << m->rowCount();
        for (Item *item :  m->m_list)
        {
            out << *item;
        }
    }
    file.commit();
}

void ListView::load()
{
    QFile file("abstract_dummy.dat");
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QDataStream out(&file);
            AbstractListModel *m = qobject_cast<AbstractListModel*>(model());
            m->removeRows(0, m->rowCount());
            int rowCount;
            out >> rowCount;
            m->m_insertItems.clear();
            for (int i = 0; i < rowCount; i++)
            {
                Item *item = new Item();
                out >> *item;
                m->m_insertItems.append(item);
            }
            m->insertRows(0, m->m_insertItems.size());
        }
    }
    file.close();
}
