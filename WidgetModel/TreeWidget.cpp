#include "TreeWidget.h"
#include <QTreeWidgetItem>
#include <QMimeData>
#include <QIODevice>
#include <QListWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QSaveFile>
#include <QFile>
#include <QUuid>

TreeWidget::TreeWidget()
{
    setDragDropMode(QTreeWidget::DragDrop);
    setDefaultDropAction(Qt::MoveAction);
    setSelectionMode(QTreeWidget::ContiguousSelection);
}

void TreeWidget::save()
{
    QTreeWidgetItemIterator it(this);

    QSaveFile f("dummy_tree.dat");
    if (f.open(QIODevice::WriteOnly))
    {
        QDataStream out(&f);
        while (*it)
        {
            QTreeWidgetItem *treeItem = *it;
            treeItem->write(out);
            out << treeItem->data(0, Qt::UserRole).toString();
            out << treeItem->isExpanded();
            if (treeItem->parent() != nullptr)
            {
                out << true;
                out << treeItem->parent()->data(0, Qt::UserRole).toString();
            }
            else
            {
                out << false;
            }
            ++it;
        }
    }
    f.commit();
}

void TreeWidget::load()
{
    clear();
    QMap<QString, QTreeWidgetItem*> map;
    QList<QTreeWidgetItem*> expand;
    QFile f("dummy_tree.dat");
    if (f.exists())
    {
        if (f.open(QIODevice::ReadOnly))
        {
            QDataStream out(&f);
            while (!out.atEnd())
            {
                QTreeWidgetItem *treeItem = new QTreeWidgetItem();
                treeItem->read(out);
                QString uuid;
                out >> uuid;

                map.insert(uuid, treeItem);
                bool isExpanded;
                out >> isExpanded;
                if (isExpanded)
                {
                    expand.append(treeItem);
                }
                bool hasParent;
                out >> hasParent;
                if (hasParent)
                {
                    QString pid;
                    out >> pid;
                    QTreeWidgetItem *p = map.value(pid);
                    p->addChild(treeItem);

                }
                else
                {
                    addTopLevelItem(treeItem);
                }

            }
        }
    }
    for (QTreeWidgetItem *treeItem : expand)
    {
        treeItem->setExpanded(true);
    }
    f.close();
}



bool TreeWidget::dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action)
{

    if (data->hasFormat(mimeTypes().at(0)))
    {
        QByteArray qb(data->data(mimeTypes().at(0)));

        QDataStream out(&qb, QIODevice::ReadOnly);

        while (!out.atEnd())
        {
            int column = 0;
            QListWidgetItem* litem = new QListWidgetItem();
            litem->read(out);
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setIcon(column, litem->icon());
            item->setText(column, litem->text());
            item->setFont(column, litem->font());
            item->setBackground(column, litem->background());
            item->setForeground(column, litem->foreground());
            item->setCheckState(column, litem->checkState());
            item->setSizeHint(column, litem->sizeHint());
            item->setToolTip(column, litem->toolTip());
            item->setData(0, Qt::UserRole, QUuid::createUuid().toString());
            Qt::ItemFlag flag;
            out >> flag;
            item->setFlags(flag);

            if (topLevelItemCount() == 0)
            {
                addTopLevelItem(item);
            }
            else
            {
                if (topLevelItem(column) != nullptr && parent != nullptr)
                {
                    topLevelItem(column)->addChild(item);
                }
                else
                {
                    addTopLevelItem(item);
                }
            }
            delete litem;
        }

        if (action == Qt::CopyAction)
        {
            return false;
        }
        return true;
    }
    return false;
}



QMimeData *TreeWidget::mimeData(const QList<QTreeWidgetItem *> &items) const
{
    QMimeData * mimeData = QTreeWidget::mimeData(items);

    QByteArray qb;
    QDataStream out(&qb, QIODevice::WriteOnly);

    QList<QTreeWidgetItem*> treeItems;

    for (QTreeWidgetItem * item: items)
    {
        if (item->child(0) != nullptr)
        {
            QTreeWidgetItemIterator it(item);
            while (*it) {
                if (!(*it)->isSelected())
                    (*it)->setSelected(true);
                treeItems.append(*it);
                ++it;
            }
        }
    }

    for (QTreeWidgetItem *item : treeItems)
    {
        item->write(out);
        out << item->flags();
    }

    mimeData->setData("application/x-qabstractitemmodeldatatree", qb);

    return mimeData;
}
