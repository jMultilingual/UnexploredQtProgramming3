#ifndef ABSTRACTTREEMODEL_H
#define ABSTRACTTREEMODEL_H

#include <QAbstractItemModel>

class Item;
class TreeView;

class AbstractTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit AbstractTreeModel(QObject *parent = nullptr);
    ~AbstractTreeModel();
    void createChildChain(QList<Item*> &seq, QList<Item*> children);
    TreeView *m_parent;
    Item *m_rootItem;
    QList<Item*> m_newItems;
    mutable QModelIndexList m_dropMimeDataIndexes;
    mutable QModelIndexList m_independentChildren;
    Item *itemFromIndex(const QModelIndex &index) const;
    QModelIndex indexFromItem(Item *item);
    void insertSubDirectories(const QModelIndex &idx, QList<Item *> children);
    void writeDataRecur(const QModelIndex &_index, QDataStream &out) const;
    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    virtual QModelIndex parent(const QModelIndex &child) const override;
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual int columnCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual Qt::DropActions supportedDropActions() const override;
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const override;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    virtual QStringList mimeTypes() const override;
    virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent) override;
};

#endif // ABSTRACTTREEMODEL_H
