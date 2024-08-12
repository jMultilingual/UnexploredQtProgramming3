#include "TableWidget.h"
#include <QMimeData>
#include <QIODevice>
#include <QSaveFile>
#include <QFile>
#include <QTreeWidgetItem>

TableWidget::TableWidget(int rows, int columns, QWidget *parent)
    :QTableWidget(rows, columns, parent)
{
    setDragDropMode(QTableWidget::DragDrop);
    setDefaultDropAction(Qt::MoveAction);
    setSelectionMode(QTableWidget::ContiguousSelection);
}

void TableWidget::save()
{
    QSaveFile file("dummy_table.dat");
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        out << rowCount() << columnCount();
        for (int row = 0; row < rowCount(); row++)
        {
            for (int col = 0; col < columnCount(); col++)
            {

                QTableWidgetItem *i = item(row, col);

                out << (i == nullptr);
                if (i != nullptr)
                {
                    i->write(out);
                    out << i->flags();
                }
            }
        }
    }
    file.commit();
}

void TableWidget::load()
{

    QFile file("dummy_table.dat");
    if (file.open(QIODevice::ReadOnly))
    {
        QDataStream out(&file);
        int rows;
        int cols;
        out >> rows >> cols;
        for (int row = 0; row < rows; row++)
        {
            for (int col =0; col < cols; col++)
            {
                bool isNull;
                out >> isNull;
                if (!isNull)
                {
                    QTableWidgetItem *i = new QTableWidgetItem();
                    i->read(out);
                    Qt::ItemFlag flag;
                    out >> flag;
                    i->setFlags(flag);
                    setItem(row, col, i);
                }
            }
        }
    }
    file.close();

}


QMimeData *TableWidget::mimeData(const QList<QTableWidgetItem *> &items) const
{
    QByteArray qb;
    QDataStream out(&qb, QIODevice::WriteOnly);
    for (const QTableWidgetItem *item: items)
    {
        if (item != nullptr)
        {
            item->write(out);
            out << item->flags();
        }
    }

    QMimeData *mimeData = new QMimeData();
    mimeData->setData(mimeTypes().at(0), qb);
    return mimeData;
}

bool TableWidget::dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action)
{
    if (data->hasFormat(mimeTypes().at(1)))
    {

        QByteArray qb(data->data(mimeTypes().at(1)));
        QDataStream out(&qb, QIODevice::ReadOnly);
        while (!out.atEnd())

        {
            int count = 0;

            QTreeWidgetItem *titem = new QTreeWidgetItem();
            titem->read(out);
            QFont font = titem->font(count);
            QIcon icon = titem->icon(count);
            QString text = titem->text(count);
            QBrush foreground = titem->foreground(count);
            QBrush background = titem->background(count);
            Qt::AlignmentFlag alignment = Qt::AlignLeft;
            Qt::CheckState checkState = titem->checkState(count);
            QString toolTip = titem->toolTip(count);
            QSize sizeHint = titem->sizeHint(count);

            Qt::ItemFlag flag;
            out >> flag;
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText(text);
            item->setIcon(icon);
            item->setFont(font);
            item->setForeground(foreground);
            item->setBackground(background);
            item->setTextAlignment(alignment);
            item->setCheckState(checkState);
            item->setToolTip(toolTip);
            item->setSizeHint(sizeHint);
            item->setFlags(flag);
            setItem(row, column, item);

            delete titem;
            ++row;
        }
        return true;
    }
    else if (data->hasFormat(mimeTypes().at(0)))
    {
        QByteArray qb(data->data(mimeTypes().at(0)));

        if (dragDropOverwriteMode())
        {
            takeItem(row, column);
        }


        QDataStream out(&qb, QIODevice::ReadOnly);

        while (!out.atEnd())
        {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->read(out);
            Qt::ItemFlag flag;
            out >> flag;
            item->setFlags(flag);
            setItem(row, column, item);
            ++row;
        }

        if (action == Qt::CopyAction)
        {
            return false;
        }
        return true;
    }
    return false;
}


QStringList TableWidget::mimeTypes() const
{
    return {"application/x-qabstractitemmodeldatalist",
            "application/x-qabstractitemmodeldatatree"};
}
