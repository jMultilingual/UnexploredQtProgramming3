#ifndef MINDMAPVIE_H
#define MINDMAPVIE_H

#include <QAbstractItemView>

class QLabel;
class AbstractListModel;


class MyListView : public QAbstractItemView
{
    Q_OBJECT
public:
    MyListView(QWidget *parent =nullptr);



    // QAbstractItemView interface
public:
    virtual QRect visualRect(const QModelIndex &index) const override;
    virtual void scrollTo(const QModelIndex &index, ScrollHint hint) override;
    virtual QModelIndex indexAt(const QPoint &point) const override;

protected:
    virtual QModelIndex moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers) override;
    virtual int horizontalOffset() const override;
    virtual int verticalOffset() const override;
    virtual bool isIndexHidden(const QModelIndex &index) const override;
    virtual void setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command) override;
    virtual QRegion visualRegionForSelection(const QItemSelection &selection) const override;
private:

    QList<QRect> m_rectForView;
    AbstractListModel *m = nullptr;
    QList<QRect> m_selectionRects;
    // QAbstractItemView interface
protected slots:
    virtual void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles) override;
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;

    // QAbstractScrollArea interface
protected:
    virtual void scrollContentsBy(int dx, int dy) override;
};

#endif // MINDMAPVIE_H
