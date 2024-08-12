#include "StandardItemListModel.h"
#include <QIODevice>
#include <QMimeData>

StandardItemListModel::StandardItemListModel(QObject *parent)
    : QStandardItemModel{parent}
{



}



bool StandardItemListModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{

    if (data->hasFormat(mimeTypes().at(0)))
    {
        QByteArray qb = data->data(mimeTypes().at(0));
        QDataStream out(&qb, QIODevice::ReadOnly);

        QList<QStandardItem*> sl;
        while (!out.atEnd())
        {
            QStandardItem *s = new QStandardItem();
            s->read(out);
            Qt::ItemFlag flag;
            out >> flag;
            s->setFlags(flag);
            sl.append(s);

        }

        if (row == -1 && column == -1)
        {
            for (QStandardItem *s : sl)
            {
                int _row = parent.row();
                int _col = parent.column();
                if (action == Qt::MoveAction)
                {
                    insertRow(_row, s);
                }
                else
                {
                    setItem(_row, _col, s);
                }
            }
        }
        else
        {
            for (QStandardItem *s : sl)
            {
                insertRow(row, s);
            }
        }
    }
    else if (data->hasFormat(mimeTypes().at(1)))
    {

        QByteArray qb = data->data(mimeTypes().at(1));
        QDataStream out(&qb, QIODevice::ReadOnly);
        if (parent.isValid())
        {
            row = parent.row();
        }
        else
        {

            if (row == -1)
            {
                row = rowCount();
            }
        }

        while (!out.atEnd())
        {
            QStandardItem *s = new QStandardItem();
            s->read(out);
            Qt::ItemFlag flag;
            out >> flag;
            s->setFlags(flag);
            int _;
            out >> _ >> _;
            insertRow(row, s);
        }
    }

    if (action == Qt::CopyAction)
    {
        return false;
    }
    return true;
}


QMimeData *StandardItemListModel::mimeData(const QModelIndexList &indexes) const
{
    QByteArray qb;
    QDataStream out(&qb, QIODevice::WriteOnly);
    for (const QModelIndex& index : indexes)
    {
        QStandardItem *item = itemFromIndex(index);
        item->write(out);
        out << item->flags();
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData(mimeTypes().at(0), qb);

    return mimeData;
}


QStringList StandardItemListModel::mimeTypes() const
{
    return {"application/x-qabstractitemmodeldatalist",
            "application/x-qabstractitemmodeldatatable",
            "application/x-qabstractitemmodeldatatree"};
}
