#ifndef STANDARDITEMLISTMODEL_H
#define STANDARDITEMLISTMODEL_H

#include <QStandardItemModel>

class StandardItemListModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit StandardItemListModel(QObject *parent = nullptr);
    // QAbstractItemModel interface
public:
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const override;
    virtual QStringList mimeTypes() const override;
};

#endif // STANDARDITEMLISTMODEL_H
