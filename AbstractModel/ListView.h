#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QListView>

class AbstractListModel;

class ListView : public QListView
{
    Q_OBJECT
public:
    ListView();
    AbstractListModel *m_listModel = nullptr;
    void save();
    void load();
};

#endif // LISTVIEW_H
