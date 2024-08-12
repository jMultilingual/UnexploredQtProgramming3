#include "StandardItemTableModel.h"
#include <QMimeData>
#include <QIODevice>

StandardItemTableModel::StandardItemTableModel(int rows, int columns, QObject *parent)
    : QStandardItemModel{rows, columns, parent}
{}


bool StandardItemTableModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (data->hasFormat(mimeTypes().at(0)))
    {
        //list type
        if (parent.isValid())
        {
            row = parent.row();
            column = parent.column();
            QByteArray qb = data->data(mimeTypes().at(0));
            QDataStream out(&qb, QIODevice::ReadOnly);
            while (!out.atEnd())
            {
                QStandardItem *s = new QStandardItem();
                s->read(out);
                Qt::ItemFlag flag;
                out >> flag;
                s->setFlags(flag);
                setItem(row, column, s);
                ++row;

            }
        }
        else
        {
            return false;
        }

        return true;

    }
    else if (data->hasFormat(mimeTypes().at(1)))
    {
        //table type


        if (parent.isValid())
        {
            row = parent.row();
            column = parent.column();
            QByteArray qb = data->data(mimeTypes().at(1));
            QDataStream out(&qb, QIODevice::ReadOnly);

            while (!out.atEnd())
            {
                QStandardItem *s = new QStandardItem();
                s->read(out);
                Qt::ItemFlag flag;
                out >> flag;
                int relativeRow;
                int relativeColumn;
                out >> relativeRow;
                out >> relativeColumn;
                setItem(row + relativeRow, column + relativeColumn, s);
            }
        }
        else
        {
            return false;
        }
        return true;
    }
    else if (data->hasFormat(mimeTypes().at(2)))
    {
        //tree  type
    }

    return false;
}


QStringList StandardItemTableModel::mimeTypes() const
{
    return {"application/x-qabstractitemmodeldatalist",
            "application/x-qabstractitemmodeldatatable",
            "application/x-qabstractitemmodeldatatree"};
}


QMimeData *StandardItemTableModel::mimeData(const QModelIndexList &indexes) const
{
    QByteArray qb;
    QDataStream out(&qb, QIODevice::WriteOnly);
    int count = 0;
    int startRow;
    int startColumn;
    for (const QModelIndex& index: indexes)
    {
        if (index.isValid())
        {
            if (count == 0)
            {
                startRow = index.row();
                startColumn = index.column();
            }
            QStandardItem *s = itemFromIndex(index);
            s->write(out);
            out << s->flags();

            out << index.row() - startRow;
            out << index.column() - startColumn;
        }
        ++count;
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData(mimeTypes().at(1), qb);
    return mimeData;
}
