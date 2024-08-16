#include "AbstractTreeModel.h"
#include "Item.h"
#include "TreeView.h"
#include <QMimeData>
#include <QIODevice>
#include <QUuid>

AbstractTreeModel::AbstractTreeModel(QObject *parent)
    : QAbstractItemModel{parent}
{
    m_parent = qobject_cast<TreeView*>(parent);
    m_rootItem = new Item(this);
    m_rootItem = new Item(this);
    Item *item = new Item(this);
    item->addChild(new Item(this));
    item->addChild(new Item(this));
    item->addChild(new Item(this));
    m_rootItem->addChild(item);
    Item *item2 = new Item(this);
    m_rootItem->addChild(item2);

}



AbstractTreeModel::~AbstractTreeModel()
{
    QList<Item*> deleteChildren;
    createChildChain(deleteChildren, m_rootItem->children());
    for (Item* deleteChild: deleteChildren)
    {
        delete deleteChild;
    }
    delete m_rootItem;
}


void AbstractTreeModel::insertSubDirectories(const QModelIndex &idx, QList<Item *> children)
{
    for (int i= 0; i < children.length(); i++)
    {
        QModelIndex a = index(i, 0, idx);
        m_newItems = children.at(i)->children();
        insertRows(0, m_newItems.length(), a);
        insertSubDirectories(a, m_newItems);
    }
}

void AbstractTreeModel::createChildChain(QList<Item*> &seq, QList<Item*> children)
{
    for (Item* child : children)
    {
        seq.append(child);
        createChildChain(seq, child->children());
    }
}

Item *AbstractTreeModel::itemFromIndex(const QModelIndex &index) const
{
    void *ip = index.internalPointer();
    Item *item = static_cast<Item*>(ip);
    if (item != nullptr)
    {
        return item;
    }
    return m_rootItem;
}

QModelIndex AbstractTreeModel::indexFromItem(Item *item)
{
    return item->modelIndex();
}


QModelIndex AbstractTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        //rootitem is parent...
        Item* item = m_rootItem->m_children.at(row);
        return createIndex(row, column, item);
    }
    else
    {
        void *ip = parent.internalPointer();
        Item *item = static_cast<Item*>(ip);
        if (item != nullptr)
        {
            Item* child = item->childAt(row);
            QModelIndex index = createIndex(row, column, child);
            child->m_modelIndex = index;
            return index;
        }
    }
    return QModelIndex();
}

QModelIndex AbstractTreeModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
    {
        return QModelIndex();
    }
    else
    {
        if (child.isValid())
        {
            void *childItem = child.internalPointer();
            Item* item = static_cast<Item*>(childItem);
            Item* parentItem = item->parent();

            return createIndex(parentItem->at(), 0, parentItem);
        }
        return QModelIndex();
    }
}

int AbstractTreeModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
    {
        return m_rootItem->m_children.length();
    }
    else
    {
        void *ip = parent.internalPointer();
        Item* item = static_cast<Item*>(ip);
        if (item != nullptr)
        {
            return item->m_children.count();
        }
    }
    return 0;
}

int AbstractTreeModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant AbstractTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    if (role == Qt::DisplayRole)
    {
        void *ip = index.internalPointer();
        Item *item = static_cast<Item*>(ip);
        return item->m_uuid;
    }
    return QVariant();
}


Qt::ItemFlags AbstractTreeModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsDragEnabled|
           Qt::ItemIsDropEnabled;
}


Qt::DropActions AbstractTreeModel::supportedDropActions() const
{
    return Qt::MoveAction|Qt::CopyAction;
}



QMimeData *AbstractTreeModel::mimeData(const QModelIndexList &indexes) const
{
    QModelIndexList firstIndexes;
    for (QModelIndex index: indexes)
    {
        if (index.column() == 0) //columnが０のインデックスだけを集める。
        {
            firstIndexes.append(index);
        }
    }
    m_dropMimeDataIndexes = firstIndexes;
    QList<Item*> internalPointers;
    for (const QModelIndex &index: firstIndexes)
    {
        if (index.isValid())
        { //column0のインデックスを全てインターナルポインタからItem*型へ変換
            void * ip = index.internalPointer();
            Item* item = static_cast<Item*>(ip);
            internalPointers.append(item);
        }
    }
    QModelIndexList confinedIndexes;
    for (const QModelIndex &index: firstIndexes)
    {//親が選択された場合はすでに子が含まれているので、その場合はリストへ加えない。
        if (index.isValid())
        {
            //parentInternalPointer
            QModelIndex pindex = index.parent();
            void* pip = pindex.internalPointer();
            Item* pipItem = static_cast<Item*>(pip);
            if (pipItem && !internalPointers.contains(pipItem))
            {
                confinedIndexes.append(index);
            }
        }
    }
    m_independentChildren = confinedIndexes;
    QByteArray qb; //後は同じ流れでシリアライズ
    QDataStream out(&qb, QIODevice::WriteOnly);
    for (const QModelIndex &index: confinedIndexes)
    {
        Item* item = itemFromIndex(index);
        out << *item;
        }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData(
        mimeTypes().at(0), qb
        );
    return mimeData;
}

bool AbstractTreeModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    int _row = row;
    int _col = column;

    if (action == Qt::IgnoreAction)
    {
        return true;
    }

    if (!data->hasFormat(mimeTypes().at(0)))
    {
        return false;
    }

    if (column > 0)
    {
        return false;
    }

    QByteArray d = data->data(mimeTypes().at(0));
    QDataStream out(&d, QIODevice::ReadOnly);
    m_newItems.clear();
    if (row == -1 && column == -1)
    {
        _row = rowCount(parent);
        _col = 0;
    }
    if (row == -1 && column == -1)
    {
        _row = rowCount(parent) - 1;
        _col = 0;
    }
    int beginRow;
    if (row != -1)
    {
        beginRow = row;
    }
    else if (_row == -1 && _col == -1)
    {
        beginRow = rowCount(parent);
    }
    else
    {
        beginRow = rowCount(parent);
    }

    int rows = 0;

    while (!out.atEnd())
    {
        Item *item = new Item();
        out >> *item;
        m_newItems.append(item);
        rows++;
    }

    int startRow = beginRow;

    insertRows(beginRow, rows, parent);

    beginRow = startRow;
    for (Item *insertItem : m_newItems)
    {
        QModelIndex idx = index(beginRow, 0, parent);
        insertItem->setModelIndex(idx);
        QVariant it = QVariant::fromValue(insertItem);
        setData(
            idx, it, Qt::EditRole);
        ++beginRow;
    }
    m_parent->clearSelection();
    if (!parent.isValid())
    {
        QModelIndex idx = index(rowCount(QModelIndex())-1, 0, parent);
        void *originalPointer = m_independentChildren.at(0).internalPointer();
        Item *op = static_cast<Item*>(originalPointer);
        m_newItems = op->children();
        insertRows(0, m_newItems.size(), idx);
        if (!m_newItems.isEmpty())
            insertSubDirectories(
                idx, m_newItems);
    }
    else
    {
        for (int i = 0; i < m_independentChildren.size() ; ++i)
        {
            QModelIndex idx;
            if (parent.isValid())
            {
                idx = index(i, 0, parent);
            }

            void *originalPointer = m_independentChildren.at(i).internalPointer();
            Item *op = static_cast<Item*>(originalPointer);
            m_newItems = op->children();
            if (!m_newItems.isEmpty())
            {
                if (_row == -1 && _col == -1)
                {
                    insertRows(0, m_newItems.size(), idx);
                    insertSubDirectories(
                        idx, m_newItems);
                }
                else
                {
                    if (i == 0 && !parent.isValid())
                    {
                        idx = index(rowCount(parent)-1, 0, parent);
                    }
                    insertRows(startRow, m_newItems.size(), idx);
                    insertSubDirectories(
                        idx, m_newItems);
                }
            }
        }
    }
    for (int i = 0; i < m_dropMimeDataIndexes.size() ; i++)
    {
        QModelIndex index = m_dropMimeDataIndexes.at(i);
        Item* item = itemFromIndex(index);
        QModelIndex parent = index.parent();
        Item *pitem = itemFromIndex(parent);

        if (pitem->contains(item))
        {
            int row = pitem->indexOfChild(item);
            removeRows(row, 1, parent);
        }
    }
    m_dropMimeDataIndexes.clear();
    return false;
}

QStringList AbstractTreeModel::mimeTypes() const
{
    return {"application/x-qabstractitemmodeldatatree"};//他ビューからのドロップを防ぐ
}



bool AbstractTreeModel::insertRows(int row, int count, const QModelIndex &parent)
{

    bool success = false;
    Item* parentItem;
    if (!parent.isValid())
    {

        parentItem = m_rootItem;
    }
    else
    {
        void *v = parent.internalPointer();
        parentItem = static_cast<Item*>(v);

    }
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; i++)
    {
        Item* item = new Item(this);
        if (!m_newItems.isEmpty())
        {

            item->m_uuid = m_newItems.at(i)->uuid();
        }
        else
        {

            item->m_uuid = QUuid::createUuid().toString();
        }
        if (row == -1)
        {
            success = parentItem->addChild(item);

        }
        else
        {
            success = parentItem->insertChild(row+i, item);

        }

    }
    endInsertRows();
    return success;




}

bool AbstractTreeModel::removeRows(int row, int count, const QModelIndex &parent)
{

    bool success = false;
    Item* parentItem;
    if (!parent.isValid())
    {
        parentItem = m_rootItem;
    }
    else
    {
        void *p = parent.internalPointer();
        parentItem = static_cast<Item*>(p);
    }

    QList<Item*> items = parentItem->children().mid(row, count);

    if (!items.isEmpty())
    {
        beginRemoveRows(parent, row, row + count -1);

        success = parentItem->removeChildren(row, count);

        endRemoveRows();
    }

    return success;

}


void AbstractTreeModel::writeDataRecur(const QModelIndex &_index, QDataStream &out) const
{
    if (!_index.isValid())
        return;
    Item *item = itemFromIndex(_index);
    out << *item;
    Item* p = item->parent();
    if (p != nullptr)
    {
        out << true;
        out << p->uuid();
    }
    else
    {
        out << false;
    }
    if (hasChildren(_index))
    {
        for (int r = 0; r < rowCount(_index) ; r++)
        {
            const QModelIndex &child = index(r, 0, _index);
            writeDataRecur(child, out);
        }
    }
}
