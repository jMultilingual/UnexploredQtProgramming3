#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QTableWidget>

class TableWidget : public QTableWidget
{
    Q_OBJECT
public:
    TableWidget(int rows, int columns, QWidget *parent=nullptr);
    void save();
    void load();
    // QTableWidget interface
protected:
    virtual QMimeData *mimeData(const QList<QTableWidgetItem *> &items) const override;
    virtual bool dropMimeData(int row, int column, const QMimeData *data, Qt::DropAction action) override;
    virtual QStringList mimeTypes() const override;
};

#endif // TABLEWIDGET_H
