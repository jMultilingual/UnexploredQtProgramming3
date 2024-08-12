#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    ListWidget();
    void addListWidgetItem(QIcon icon,
                           const QString &text,
                           QFont font,
                           QBrush foreground,
                           QBrush background,
                           Qt::Alignment alignment,
                           QString toolTip,
                           Qt::CheckState checkState,
                           QSize sizeHint,
                           Qt::ItemFlags flags
                           );
    void save();
    void load();

    // QWidget interface
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;
    virtual QStringList mimeTypes() const override;
    virtual bool dropMimeData(int index, const QMimeData *data, Qt::DropAction action) override;
    virtual QMimeData *mimeData(const QList<QListWidgetItem *> &items) const override;
};

#endif // LISTWIDGET_H
