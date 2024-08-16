#include "AbstractListModel.h"
#include "Item.h"
#include <QMimeData>
#include <QIODevice>

AbstractListModel::AbstractListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

AbstractListModel::~AbstractListModel()
{
    for (Item *v : m_list)
    {
        delete v;
    }
}

int AbstractListModel::rowCount(const QModelIndex &parent) const
{
    return m_list.length();
}

QVariant AbstractListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    if (role == Qt::UserRole)
    {
        Item* item = m_list.at(index.row());
        QVariant var = QVariant::fromValue(item);
        return var;
    }
    if (index.row() < m_list.length())
    {
        Item* var = m_list.at(index.row());
        if (var->m_var.contains(role))
        {
            return var->m_var[role];
        }
    }
    return QVariant();
}


bool AbstractListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
    {
        return false;
    }
    if (role == Qt::EditRole)
    {
        role = Qt::DisplayRole;
    }
    Item *var = m_list.at(index.row());
    var->m_var[role] = value;
    emit dataChanged(index, index);
    return true;
}


Qt::ItemFlags AbstractListModel::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index)|Qt::ItemIsEditable|
           Qt::ItemIsUserCheckable|Qt::ItemIsUserTristate|
           Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled;
}


bool AbstractListModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    int _row = row;
    int _column = column;

    if (row == -1 && column == -1)
    {
        row = parent.row();
        column = parent.column();
    }
    if (row == -1 && column == -1)
    {
        row = rowCount() - 1;
        column = 0;
    }
    if (data->hasFormat(mimeTypes().at(0)))
    {
        QByteArray qb = data->data(mimeTypes().at(0));
        QDataStream out(&qb, QIODevice::ReadOnly);
        m_insertItems.clear(); // QList<Item*>型でリストを作っておく
        while (!out.atEnd())
        {
            Item *item = new Item();
            out >> *item;
            m_insertItems.append(item);
        }
        bool success;
        success = insertRows(row, m_insertItems.size());

        if (!success)
        {
            return false;
        }
    }


    else if (data->hasFormat(mimeTypes().at(1)))
    {

        QByteArray qb = data->data(mimeTypes().at(1));
        QDataStream out(&qb, QIODevice::ReadOnly);
        m_insertItems.clear();
        int _;
        bool success;

        while (!out.atEnd())
        {

            Item* item = new Item();
            out >> *item;
            m_insertItems.append(item);
            out >> _ >> _; //要らないデータ
        }
        if (_row == -1 && _column == -1)
        {
            success = insertRows(row, m_insertItems.size());
        }
        else if (_row != -1 && _column != -1)
        {
            success = insertRows(row, m_insertItems.size());
        }
        else
        {
            for (Item* item: m_insertItems)
            {
                delete item;
            }
            return false;
        }
        if (success)
        {
            return true;
        }
    }
    if (action == Qt::CopyAction)
        return false;
    return true;
}


QMimeData *AbstractListModel::mimeData(const QModelIndexList &indexes) const
{
    QByteArray qb;
    QDataStream out(&qb, QIODevice::WriteOnly);
    for (const QModelIndex& index : indexes)
    {
        QVariant var = data(index, Qt::UserRole);
        Item *item = var.value<Item*>();
        out << *item;
    }
    QMimeData *mimeData = new QMimeData();
    mimeData->setData(mimeTypes().at(0), qb);
    qDebug() << mimeTypes().at(0);
    return mimeData;
}


bool AbstractListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || count <= 0 || row  > m_list.size())
        return false;

    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; i++)
    {
        m_list.insert(row + i, m_insertItems[i]);
    }
    endInsertRows();
    return true;
}

bool AbstractListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || count <= 0 || row + count > m_list.size())
        return false;
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        delete m_list[row];
        m_list.removeAt(row);
    }
    endRemoveRows();
    return true;
}


Qt::DropActions AbstractListModel::supportedDropActions() const
{
    return Qt::MoveAction;
}


QStringList AbstractListModel::mimeTypes() const
{
    return {"application/x-qabstractitemmodeldatalist",
            "application/x-qabstractitemmodeldatatable"};
}
