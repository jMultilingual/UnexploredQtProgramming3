#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QListView>

class ListView : public QListView
{
    Q_OBJECT
public:
    ListView();
    void save();
    void load();
};

#endif // LISTVIEW_H
