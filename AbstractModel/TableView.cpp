#include "TableView.h"
#include "AbstractTableModel.h"
#include <QDragEnterEvent>
#include <QMimeData>

#include <QSaveFile>
#include <QFile>

#include "Item.h"
TableView::TableView()
{

    m_tableModel = new AbstractTableModel();
    setModel(m_tableModel);
    setDragDropMode(QTableView::DragDrop);
    setDefaultDropAction(Qt::MoveAction);
    setSelectionMode(QTableView::ContiguousSelection);
}



void TableView::save()
{
    QSaveFile file("dummy_table.dat");
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        AbstractTableModel *m = qobject_cast<AbstractTableModel*>(model());
        int rows = m->rowCount();
        int cols = m->columnCount();
        out << rows << cols;
        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                Item *item = m->m_list[row][col];
                if (item == nullptr)
                {
                    out << false;
                    continue;
                }
                out << true;
                out << *item;

            }
        }
    }
    file.commit();
}

void TableView::load()
{
    QFile file("dummy_table.dat");
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            QDataStream out(&file);
            AbstractTableModel *m = qobject_cast<AbstractTableModel*>(model());
            int rows;
            int cols;
            out >> rows >> cols;
            m->m_rowCount = rows;
            m->m_columnCount = cols;
            m->init();
            for (int row = 0; row < rows; ++row)
            {
                for (int col = 0; col < cols; ++col)
                {
                    bool notEmpty;
                    out >> notEmpty;
                    if (notEmpty)
                    {
                        Item* item = new Item();
                        out >> *item;
                        QModelIndex index = m->index(row, col);
                        QVariant var = QVariant::fromValue(item);
                        m->setData(index, var, Qt::UserRole);
                    }
                }
            }
        }
    }
    file.close();
}





