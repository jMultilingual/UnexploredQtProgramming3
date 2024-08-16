
#ifndef ABSTRACTTABLEMODEL_H
#define ABSTRACTTABLEMODEL_H

#include <QAbstractTableModel>

class Item;

class AbstractTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit AbstractTableModel(QObject *parent = nullptr);
    ~AbstractTableModel();
    int m_rowCount = 12;
    int m_columnCount = 5;
    QList<QList<Item*>> m_list;
    QList<QPair<Item*, QPair<int, int>>> m_insertItems;
    void init();


    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    virtual QMimeData *mimeData(const QModelIndexList &indexes) const override;
    // virtual QStringList mimeTypes() const override;

    virtual bool insertColumns(int column, int count, const QModelIndex &parent) override;
     virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    virtual Qt::DropActions supportedDropActions() const override;


    virtual QStringList mimeTypes() const override;
};

#endif // ABSTRACTTABLEMODEL_H
