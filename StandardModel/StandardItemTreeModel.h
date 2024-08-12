#ifndef STANDARDITEMTREEMODEL_H
#define STANDARDITEMTREEMODEL_H

#include <QStandardItemModel>

class StandardItemTreeModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit StandardItemTreeModel(QObject *parent = nullptr);
    void writeMimeDataRecur(const QModelIndex& index, QDataStream &out) const;


    // QAbstractItemModel interface
public:
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const override;
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    virtual QStringList mimeTypes() const override;
};

#endif // STANDARDITEMTREEMODEL_H
