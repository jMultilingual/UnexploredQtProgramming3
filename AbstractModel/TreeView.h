#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QTreeView>


class AbstractTreeModel;

class TreeView : public QTreeView
{
    Q_OBJECT
public:
    TreeView();
    AbstractTreeModel *m_treeModel;
    void save();
    void load();
    void writeDataRecur(const QModelIndex &_index, QDataStream &out) const;
};

#endif // TREEVIEW_H
