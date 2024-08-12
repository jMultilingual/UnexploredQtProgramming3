#ifndef ITEM_H
#define ITEM_H

#include <QObject>
#include <QVariant>

#include <QModelIndex>

class Item : public QObject
{
    Q_OBJECT

public:

    Item();
    QHash<int, QVariant> m_var;
    void setData( QVariant value, Qt::ItemDataRole role);
    QVariant data( Qt::ItemDataRole role);
    QList<Item*> m_children;
    QList<Item*> children();
    QString m_uuid;
    QString m_puuid;
    QString uuid();
    QString puuid();

    Item *m_parent = nullptr;
    Item * parent();
    void setParent(Item* parent);
    int childCount();
    Item* childAt(int row);
    bool contains(Item* item);

    QModelIndex m_modelIndex;
    QModelIndex modelIndex();
    void setModelIndex(const QModelIndex &index);

    int at();

    bool addChild(Item* child);
    bool insertChild(int row, Item *child);
    void removeChild(Item *child);
    bool removeChildren(int row, int count);

    int indexOfChild(Item * child);
    void addChildren(QList<Item*> children);
    void insertChildren(int row, QList<Item*> children);
    friend QDataStream& operator<<(QDataStream& out, const Item& item);
    friend QDataStream& operator>>(QDataStream& in, Item& item);
};

#endif // ITEM_H
