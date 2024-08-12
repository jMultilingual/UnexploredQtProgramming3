#include "StyledItemDelegate.h"
#include "ItemEditorFactory.h"
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>

StyledItemDelegate::StyledItemDelegate()
{
    m_itemEditorFactory = new ItemEditorFactory();
    setItemEditorFactory(m_itemEditorFactory);
}

StyledItemDelegate::~StyledItemDelegate()
{
    delete m_itemEditorFactory;
}


void StyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt(option);
    opt.decorationSize = sizeHint(option, index);
    if (index.row() == 0)
    {
        opt.font = QFont("Times New Roman", 18);
    }
    else if (index.row() == 1)
    {
        opt.font = QFont("Segoe UI Light", 36);
    }
    else if (index.row() == 2)
    {
        opt.font = QFont("Segoe UI Black", 54);
    }
    QStyledItemDelegate::paint(painter, opt, index);

}

QSize StyledItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant s = index.data(Qt::SizeHintRole);
    return s.value<QSize>();
    return QStyledItemDelegate::sizeHint(option, index);
}


QWidget *StyledItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);
    QWidget* w = nullptr;
    if (index.row() == 0)
    {
        w =  itemEditorFactory()->createEditor(
            QMetaType::QString, parent);

    }
    else if (index.row() == 1)
    {
        w = itemEditorFactory()->createEditor(
            QMetaType::User, parent);

    }
    else if (index.row() == 2)
    {
        w = itemEditorFactory()->createEditor(
            QMetaType::Bool, parent);

    }
    if (w != nullptr)
    {
        w->setFont(opt.font);
        return w;
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}


void StyledItemDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
    option->decorationSize = sizeHint(*option, index);
}


void StyledItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QVariant value = index.data(Qt::EditRole);

    const char *propertyName = editor->metaObject()->userProperty().name();
    if (propertyName)
    {
        editor->setProperty(propertyName, value);
    }
}

void StyledItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    const char *propertyName = editor->metaObject()->userProperty().name();
    if (!propertyName)
    {
        propertyName = itemEditorFactory()->valuePropertyName(
            model->data(index, Qt::EditRole).userType()
            );
    }
    if (propertyName)
    {
        model->setData(index, editor->property(propertyName), Qt::EditRole);
    }
}
