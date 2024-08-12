#ifndef STANDARDITEMTABLEMODEL_H
#define STANDARDITEMTABLEMODEL_H

#include <QStandardItemModel>

class StandardItemTableModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit StandardItemTableModel(int rows, int columns, QObject *parent = nullptr);

    // QAbstractItemModel interface
public:
    virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) override;
    virtual QStringList mimeTypes() const override;
    virtual QMimeData *mimeData(const QModelIndexList &indexes) const override;
};

#endif // STANDARDITEMTABLEMODEL_H
