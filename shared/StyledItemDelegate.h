#ifndef STYLEDITEMDELEGATE_H
#define STYLEDITEMDELEGATE_H

#include <QStyledItemDelegate>


class ItemEditorFactory;

class StyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    StyledItemDelegate();
    ~StyledItemDelegate();

    ItemEditorFactory *m_itemEditorFactory;
    // QAbstractItemDelegate interface
public:
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    // QStyledItemDelegate interface
protected:
    virtual void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;

    // QAbstractItemDelegate interface


};

#endif // STYLEDITEMDELEGATE_H
