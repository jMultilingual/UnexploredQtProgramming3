#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeWidget>

class TreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    TreeWidget();

    void save();
    void load();

    // QTreeWidget interface
protected:
    virtual bool dropMimeData(QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action) override;
    virtual QMimeData *mimeData(const QList<QTreeWidgetItem *> &items) const override;
};

#endif // TREEWIDGET_H
