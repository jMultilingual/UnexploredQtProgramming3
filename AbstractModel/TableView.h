#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>

class AbstractTableModel;

class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView();
    AbstractTableModel *m_tableModel = nullptr;
    void save();
    void load();

    // QWidget interface

};

#endif // TABLEVIEW_H
