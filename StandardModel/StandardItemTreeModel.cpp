#include "StandardItemTreeModel.h"
#include <QMimeData>
#include <QIODevice>
#include <QDataStream>
#include <QUuid>
#include <QSaveFile>
#include <QFile>

StandardItemTreeModel::StandardItemTreeModel(QObject *parent)
    : QStandardItemModel{parent}
{}

void StandardItemTreeModel::writeMimeDataRecur(const QModelIndex &_index, QDataStream &out) const
{
    if (!_index.isValid())
        return;


    QStandardItem *item = itemFromIndex(_index);

    item->write(out);
    out << item->flags();
    out << item->data(Qt::UserRole).toString();
    QStandardItem* p = item->parent();
    if (p != nullptr)
    {

        out << true;
        out << p->data(Qt::UserRole).toString();
    }
    else
    {
        out << false;
        qDebug() << "parent doesn't have...";
    }

    if (hasChildren(_index))
    {
        for (int r = 0; r < rowCount() ; r++)
        {
            const QModelIndex &child = index(r, 0, _index);
            writeMimeDataRecur(child, out);
        }
    }
}




QMimeData *StandardItemTreeModel::mimeData(const QModelIndexList &indexes) const
{
    QByteArray qb;
    QDataStream out(&qb, QIODevice::WriteOnly);
    QModelIndexList parentIndexes;
    for (const QModelIndex& index: indexes)
    {
        if (!indexes.contains(index.parent()))
        {
            parentIndexes.append(index);
        }
    }

    for (const QModelIndex& index: parentIndexes)
    {
        writeMimeDataRecur(index, out);
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData(mimeTypes().at(2), qb);
    return mimeData;
}


bool StandardItemTreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (data->hasFormat(mimeTypes().at(0)))
    {
        //list type
        QByteArray qb = data->data(mimeTypes().at(0));
        QDataStream out(&qb, QIODevice::ReadOnly);
        int _row = row;
        int _column = column;
        while (!out.atEnd())
        {
            QStandardItem *item = new QStandardItem();
            item->read(out);
            Qt::ItemFlag flag;
            out >> flag;
            item->setFlags(flag);
            item->setData(QUuid::createUuid().toString(), Qt::UserRole);
            if (parent.isValid() && _row != -1 && _column != -1)
            {
                //子として途中挿入
                QStandardItem *parentItem = itemFromIndex(parent);

                parentItem->insertRow(row, item);
            }
            if (!parent.isValid())
            {

                if (_row == -1 && _column == -1)
                {
                    //アイテムが全くない場合あるいは範囲外
                    setItem(rowCount(parent), item);
                }
                else
                {
                    //アイテムとアイテムの間（ただしトップレベルのみ）
                    insertRow(row, item);
                }
            }
            else if (_row == -1 && _column == -1)
            {
                //オンアイテムドロップ 子として加える。最後に加える。

                QStandardItem *parentItem = itemFromIndex(parent);
                parentItem->setChild(parentItem->rowCount(), 0, item);
            }
            ++row;
        }

    }
    else if (data->hasFormat(mimeTypes().at(1)))
    {
        //table type
        QByteArray qb = data->data(mimeTypes().at(1));
        QDataStream out(&qb, QIODevice::ReadOnly);
        int _row = row;
        int _column = column;
        while (!out.atEnd())
        {
            QStandardItem *item = new QStandardItem();
            item->read(out);
            Qt::ItemFlag flag;
            out >> flag;
            item->setFlags(flag);
            item->setData(QUuid::createUuid().toString(), Qt::UserRole);
            int _;
            out >> _ >> _;
            if (parent.isValid() && _row != -1 && _column != -1)
            {
                //子として途中挿入
                QStandardItem *parentItem = itemFromIndex(parent);
                parentItem->insertRow(row, item);
            }
            if (!parent.isValid())
            {

                if (_row == -1 && _column == -1)
                {
                    //アイテムが全くない場合あるいは範囲外
                    setItem(rowCount(parent), item);
                }
                else
                {
                    //アイテムとアイテムの間（ただしトップレベルのみ）
                    insertRow(row, item);
                }
            }
            else if (_row == -1 && _column == -1)
            {
                //オンアイテムドロップ 子として加える。最後に加える。

                QStandardItem *parentItem = itemFromIndex(parent);
                parentItem->setChild(parentItem->rowCount(), 0, item);
            }
            ++row;
        }

    }
    else if (data->hasFormat(mimeTypes().at(2)))
    {

        QByteArray qb = data->data(mimeTypes().at(2));
        QDataStream out(&qb, QIODevice::ReadOnly);
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

                if (parent.isValid() && row == -1 && column == -1 && !map.contains(pstr))
                {
                    QStandardItem *pi = itemFromIndex(parent);
                    if (pi != nullptr)
                    {
                        pi->setChild(pi->rowCount(), item);
                    }
                }
                else if (parent.isValid() && row == -1 && column == -1 && !map.contains(pstr))
                {
                    QStandardItem *pi = itemFromIndex(parent);
                    if (pi != nullptr)
                    {
                        pi->setChild(pi->rowCount(), item);
                    }
                }
                if (!parent.isValid() && row == -1 && column == -1 && !map.contains(pstr))
                {
                    appendRow(item);
                }


                if (map.contains(pstr))
                {

                    if (parent.isValid() && row == -1 && column == -1)
                    {
                        QStandardItem *pi = map.value(pstr);
                        pi->setChild(pi->rowCount(), item);
                    }
                    else if (!parent.isValid() && row == -1 && column == -1)
                    {
                        QStandardItem *pi = map.value(pstr);
                        pi->setChild(pi->rowCount(), item);
                    }
                    else
                    {
                        qDebug() << 214;
                        QStandardItem *pi = map.value(pstr);
                        pi->insertRow(rowCount(), item);
                    }

                }
            }
            else
            {
                if (parent.isValid() && row == -1 && column == -1)
                {
                    QStandardItem *pi = itemFromIndex(parent);
                    pi->setChild(pi->rowCount(), item);
                }
                else if (!parent.isValid() && row == -1 && column == -1)
                {
                    appendRow(item);
                }

            }
        }

    }
    if (action == Qt::CopyAction)
    {
        return false;
    }
    else
    {
        return true;
    }
    return false;
}


QStringList StandardItemTreeModel::mimeTypes() const
{
    return {"application/x-qabstractitemmodeldatalist",
            "application/x-qabstractitemmodeldatatable",
            "application/x-qabstractitemmodeldatatree"};
}
