#include "Item.h"
#include <QtGui>

Item::Item(QObject *parent)
    : QObject{parent}, m_var()
{


    m_var[Qt::DisplayRole] = QVariant("string");
    m_var[Qt::FontRole] = QVariant(QFont("Times New Roman", 18));
    m_var[Qt::ToolTipRole] = QVariant("tooltip");
    m_var[Qt::SizeHintRole] = QVariant(QSize(50, 50));
    m_var[Qt::CheckStateRole] = QVariant(Qt::Checked);
    m_var[Qt::ItemDataRole::DecorationRole] = QVariant(QIcon());
    m_var[Qt::ItemDataRole::ForegroundRole] = QVariant(QBrush(QColor(0, 0, 0)));
    m_var[Qt::ItemDataRole::BackgroundRole] = QVariant(QBrush(QColor(255, 255, 255)));
    m_var[Qt::ItemDataRole::TextAlignmentRole] = QVariant(Qt::AlignLeft);
    m_children = {};
    m_uuid = QUuid::createUuid().toString();
    m_puuid = QUuid::createUuid().toString();
}

void Item::setData(QVariant value, Qt::ItemDataRole role)
{
    m_var[role] = value;
}

QVariant Item::data(Qt::ItemDataRole role)
{
    return m_var[role];
}

QDataStream& operator<<(QDataStream& out, const Item& item)
{
    // Serialize each QVariant in the QMap
    out << item.m_var[Qt::DisplayRole];
    out << item.m_var[Qt::FontRole];
    out << item.m_var[Qt::ToolTipRole];
    out << item.m_var[Qt::SizeHintRole];
    out << item.m_var[Qt::CheckStateRole];
    out << item.m_var[Qt::DecorationRole];
    out << item.m_var[Qt::ForegroundRole];
    out << item.m_var[Qt::BackgroundRole];
    out << item.m_var[Qt::TextAlignmentRole];
    out << item.m_uuid;
    out << item.m_puuid;

    return out;
}

// Implementation of operator>>
QDataStream& operator>>(QDataStream& in, Item& item)
{
    // Deserialize each QVariant in the QMap
    in >> item.m_var[Qt::DisplayRole];
    in >> item.m_var[Qt::FontRole];
    in >> item.m_var[Qt::ToolTipRole];
    in >> item.m_var[Qt::SizeHintRole];
    in >> item.m_var[Qt::CheckStateRole];
    in >> item.m_var[Qt::DecorationRole];
    in >> item.m_var[Qt::ForegroundRole];
    in >> item.m_var[Qt::BackgroundRole];
    in >> item.m_var[Qt::TextAlignmentRole];
    in >> item.m_uuid;
    in >> item.m_puuid;
    return in;
}

Item *Item::parent()
{
    return m_parent;
}

void Item::setParent(Item *parent)
{
    m_parent = parent;
}



int Item::childCount()
{
    return m_children.length();
}

Item *Item::childAt(int row)
{
    return m_children.at(row);
}

bool Item::contains(Item *item)
{
    return m_children.contains(item);

}

QString Item::uuid()
{
    return m_uuid;
}

QModelIndex Item::modelIndex()
{
    return m_modelIndex;
}

void Item::setModelIndex(const QModelIndex &index)
{
    m_modelIndex = index;
}

int Item::at()
{
    if (parent() != nullptr)
    {
        int row = parent()->m_children.indexOf(this);
        return row;
    }
    return -1;
}

bool Item::addChild(Item *child)
{
    child->setParent(this);
    m_children.append(child);
    return true;
}

bool Item::insertChild(int row, Item *child)
{
    child->setParent(this);
    m_children.insert(row, child);
    return true;
}

void Item::removeChild(Item *child)
{
    if (m_children.contains(child))
    {
        child->setParent(nullptr);
        m_children.removeOne(child);
    }
}

bool Item::removeChildren(int row, int count)
{
    if (row < 0 || row + count > childCount())
    {
        return false;
    }
    QList<Item*> removedItems;
    removedItems = children().mid(row, count);
    for (int i = 0; i < removedItems.size(); i++)
    {
        removeChild(removedItems.at(i));
    }
    return true;
}



int Item::indexOfChild(Item *child)
{
    return m_children.indexOf(child);
}

void Item::addChildren(QList<Item *> children)
{
    for (Item *child : children)
    {
        addChild(child);
    }
}

void Item::insertChildren(int row, QList<Item *> children)
{
    if (row < 0 || row > m_children.size()) {
        return;
    }
    for (Item *child: children)
    {
        insertChild(row, child);
        row++;
    }
}

QList<Item *> Item::children()
{
    return m_children;
}
