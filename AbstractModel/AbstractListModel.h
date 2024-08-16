#ifndef ABSTRACTLISTMODEL_H
#define ABSTRACTLISTMODEL_H

#include <QAbstractListModel>


class Item;

class AbstractListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit AbstractListModel(QObject *parent = nullptr);
    ~AbstractListModel();
    QList<Item*> m_list;
    QList<Item*> m_insertItems;
    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent=QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;


    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const override;

    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    virtual Qt::DropActions supportedDropActions() const override;

    virtual QStringList mimeTypes() const override;
};

#endif // ABSTRACTLISTMODEL_H
