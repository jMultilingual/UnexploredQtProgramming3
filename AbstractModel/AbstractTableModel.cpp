#include "AbstractTableModel.h"
#include "Item.h"
#include <QtGui>
#include <QMimeData>
#include <QIODevice>

AbstractTableModel::AbstractTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    init();

    Item *i1 = new Item();
    i1->setData(QVariant(QIcon(":/images/1.jpg")), Qt::DecorationRole);
    m_list[0].insert(0, i1);
    Item *i2 = new Item();
    i2->setData(QVariant(QIcon(":/images/2.jpg")), Qt::DecorationRole);
    m_list[1].insert(0, i2);
    Item *i3 = new Item();
    i3->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m_list[2].insert(0, i3);
}

AbstractTableModel::~AbstractTableModel()
{
    for (int row = 0; row < m_rowCount; ++row)
    {

        for (int column = 0; column < m_columnCount; ++column)
        {
            if (m_list[row][column] != nullptr)
            {
                delete m_list[row][column];
            }

        }
    }
}

void AbstractTableModel::init()
{
    for (int row = 0; row < m_rowCount; ++row)
    {
        m_list.append(QList<Item*>());
        for (int column = 0; column < m_columnCount; ++column)
        {
            m_list[row].append(nullptr);
        }
    }
}




int AbstractTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_rowCount;
}

int AbstractTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_columnCount;
}

QVariant AbstractTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    Item *item = m_list.at(index.row()).at(index.column());
    if (item != nullptr)
    {
        if (role == Qt::UserRole)
        {
            QVariant var = QVariant::fromValue(item);
            return var;
        }

        return item->m_var[role];
    }
    return QVariant();
}


Qt::ItemFlags AbstractTableModel::flags(const QModelIndex &index) const
{

    return QAbstractTableModel::flags(index)
           |Qt::ItemIsEditable
           |Qt::ItemIsSelectable
        |Qt::ItemIsDragEnabled
        |Qt::ItemIsDropEnabled
        |Qt::ItemIsUserCheckable
        |Qt::ItemIsUserTristate;
}


bool AbstractTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int row = index.row();
    int col = index.column();
    if (role == Qt::EditRole)
    {
        role = Qt::DisplayRole; //roleの変換は重要
    }
    if (role == Qt::UserRole)
    {

        if (m_list.at(row).at(col) != nullptr)
        {
            delete m_list[row][col]; //メモリリークを防止
        }
        m_list[row][col] = value.value<Item*>();
        emit dataChanged(index, index);
        return true;

    }
    else if (m_list.at(row).at(col) != nullptr)
    {
        m_list[row][col]->m_var[role] = value;
        emit dataChanged(index, index);
        return true;
    }
    else
    {
        m_list[row][col] = new Item();//空セルを編集した場合
        m_list[row][col]->m_var[role] = value;
        emit dataChanged(index, index);
        return true;
    }
    return false;

}


QMimeData *AbstractTableModel::mimeData(const QModelIndexList &indexes) const
{
    QByteArray qb;
    QDataStream out(&qb, QIODevice::WriteOnly);
    int count =0;
    int relativeRow;
    int relativeColumn;
    int row;
    int column;
    for (const QModelIndex& index: indexes)
    {
        if (count == 0)
        {
            row = index.row();
            column = index.column();
        }
        QVariant var = data(index, Qt::UserRole);
        Item *item = var.value<Item*>();
        if (item == nullptr)
        {
            continue;
        }
        out << *item;
        relativeRow = index.row() - row;
        relativeColumn = index.column() - column;
        out << relativeRow << relativeColumn;
        ++count;
    }

    QMimeData *mimeData = new QMimeData();
    mimeData->setData(mimeTypes().at(1), qb);

    return mimeData;
}


bool AbstractTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row < 0 || count <= 0 || row > m_list.size())
        return false;

    beginInsertRows(parent, row, row + count - 1);
    qDebug() << "original" << m_list.size();
    for (int i = 0; i < count; ++i)
    {
        QList<Item*> newRow(m_columnCount, nullptr); // 列数に合わせて初期化
        m_list.insert(row, newRow);
        qDebug() << m_list.size();
    }

    endInsertRows();
    emit layoutAboutToBeChanged();
    m_rowCount = m_list.size();
    emit layoutChanged();
    return true;
}

bool AbstractTableModel::insertColumns(int column, int count, const QModelIndex &parent)
{

    beginInsertColumns(parent, column, column + count - 1);
    m_columnCount = columnCount() + count;
    for (int c = 0; c < count; c++)
    {
        for (int row = 0; row < rowCount(); row++)
        {
            m_list[row].append(nullptr);
        }
    }
    endInsertColumns();
    return true;

}


bool AbstractTableModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
    {
        return false;
    }

    if (row == -1)
    {
        row = parent.row();
    }
    if (column == -1)
    {
        column = parent.column();
    }

    QByteArray qb = data->data(mimeTypes().at(0));
    QDataStream in(&qb, QIODevice::ReadOnly);
    m_insertItems.clear();

    while (!in.atEnd())
    {
        Item* item = new Item();
        int relativeRow, relativeColumn;
        in >> *item >> relativeRow >> relativeColumn;
        m_insertItems.append(qMakePair(item, qMakePair(relativeRow, relativeColumn)));
    }

    if (row == -1 && column == -1)
    {
        row = 0; // 適切なデフォルト値を設定
        column = 0;
    }

    for (const auto &itemPair : m_insertItems)
    {
        Item *item = itemPair.first;
        int dropRow = row + itemPair.second.first;
        int dropColumn = column + itemPair.second.second;

        if (dropRow >= rowCount(QModelIndex()))
        {
            insertRows(dropRow, 1, QModelIndex());
        }
        if (dropColumn >= columnCount(QModelIndex()))
        {
            insertColumns(dropColumn, 1, QModelIndex());
        }

        QVariant var = QVariant::fromValue(item);
        QModelIndex x = index(dropRow, dropColumn, QModelIndex());
        if (x.isValid())
        {
            setData(x, var, Qt::UserRole);
        }
    }

    if (action == Qt::CopyAction)
    {
        return false;
    }
    return true;
}




Qt::DropActions AbstractTableModel::supportedDropActions() const
{
    return Qt::MoveAction|Qt::CopyAction;
}


QStringList AbstractTableModel::mimeTypes() const
{
    return {"application/x-qabstractitemmodeldatalist",
            "application/x-qabstractitemmodeldatatable"};
}
