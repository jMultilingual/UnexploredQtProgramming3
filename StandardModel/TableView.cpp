#include "TableView.h"
#include "StandardItemTableModel.h"
#include "StyledItemDelegate.h"
#include <QListView>
#include <QSaveFile>
#include <QFile>


TableView::TableView()
{
    setDragDropMode(QListView::DragDrop);
    setDefaultDropAction(Qt::MoveAction);
    StandardItemTableModel *model = new StandardItemTableModel(12, 12);


    setModel(model);

}

void TableView::save()
{
    QSaveFile file = QSaveFile("dummy_table.dat");

    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        StandardItemTableModel *m = qobject_cast<StandardItemTableModel*>(model());
        int rows = m->rowCount();
        int columns = m->columnCount();
        out << rows << columns;
        for (int row = 0; row < rows; ++row)
        {
            for (int column = 0; column < columns; ++column)
            {
                QModelIndex index = m->index(row, column);
                QStandardItem *s = m->itemFromIndex(index);
                if (s != nullptr)
                {
                    out << true;
                    s->write(out);
                    out << s->flags();
                }
                else
                {
                    out << false;

                }

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
            int rows;
            int columns;
            out >> rows >> columns;

            StandardItemTableModel* m = qobject_cast<StandardItemTableModel*>(model());
            m->clear();
            for (int row = 0; row < rows; row++)
            {
                for (int col = 0; col < columns; col++)
                {
                    bool notNull;
                    out >> notNull;
                    if (notNull)
                    {
                        QStandardItem *item = new QStandardItem();
                        item->read(out);
                        Qt::ItemFlag flag;
                        out >> flag;
                        m->setItem(row, col, item);
                    }



                }
            }
        }
    }
    file.close();
}
