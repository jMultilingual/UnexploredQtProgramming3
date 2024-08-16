#include "MyListView.h"
#include "AbstractListModel.h"
#include "Item.h"
#include <QGridLayout>
#include <QLabel>
#include <QScrollBar>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>


MyListView::MyListView(QWidget *widget)
{
    setSelectionMode(QAbstractItemView::ContiguousSelection);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m = new AbstractListModel(this);
    setModel(m);
    Item *i1 = new Item(this);
    i1->setData(QVariant(QIcon(":/images/1.jpg")), Qt::DecorationRole);
    m->m_list.append(i1);
    Item *i2 = new Item(this);
    i2->setData(QVariant(QIcon(":/images/2.jpg")), Qt::DecorationRole);
    m->m_list.append(i2);
    Item *i3 = new Item(this);
    i3->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i3);
    Item *i4 = new Item(this);
    i4->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i4);
    Item *i5 = new Item(this);
    i5->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i5);
    Item *i6 = new Item(this);
    i6->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i6);
    Item *i7 = new Item(this);
    i7->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i7);
    Item *i8 = new Item(this);
    i8->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i8);
    Item *i9 = new Item(this);
    i9->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i9);
    Item *i10 = new Item(this);
    i10->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i10);
    Item *i11 = new Item(this);
    i11->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i11);
    Item *i12 = new Item(this);
    i12->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i12);
    Item *i13 = new Item(this);
    i13->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i13);
    Item *i14 = new Item(this);
    i14->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i14);
    Item *i15 = new Item(this);
    i15->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i15);
    Item *i16 = new Item(this);
    i16->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i16);
    Item *i17 = new Item(this);
    i17->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i17);
    Item *i18 = new Item(this);
    i18->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i18);
    Item *i19 = new Item(this);
    i19->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i19);
    Item *i20 = new Item(this);
    i20->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i20);
    Item *i21 = new Item(this);
    i21->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i21);
    Item *i22 = new Item(this);
    i22->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i22);
    Item *i23 = new Item(this);
    i23->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i23);
    Item *i24 = new Item(this);
    i24->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i24);
    Item *i25 = new Item(this);
    i25->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i25);
    Item *i26 = new Item(this);
    i26->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i26);
    Item *i27 = new Item(this);
    i27->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i27);
    Item *i28 = new Item(this);
    i28->setData(QVariant(QIcon(":/images/3.jpg")), Qt::DecorationRole);
    m->m_list.append(i28);




}


QRect MyListView::visualRect(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QRect();
    }
    if (m_rectForView.count() > index.row())
    {
        return m_rectForView.at(index.row());
    }
    return rect();
}

void MyListView::scrollTo(const QModelIndex &index, ScrollHint hint)
{
    QRect v = viewport()->rect();
    QRect r = visualRect(index);
    if (r.top() < v.top())
    {
        verticalScrollBar()->setValue(verticalScrollBar()->value()+ r.top() - v.top());

    }
    else if (r.bottom() > v.bottom())
    {
        verticalScrollBar()->setValue(verticalScrollBar()->value() + std::min(r.bottom() - v.bottom(), r.top() - v.top()));

    }
    viewport()->update();
    qDebug() << viewport()->size() << verticalScrollBar()->value() << index << hint;
}

QModelIndex MyListView::indexAt(const QPoint &point) const
{
    qDebug() << "indexAt" << point << currentIndex();
    int count = 0;
    for (QRect rect: m_rectForView)
    {
        if (rect.contains(point))
        {
            qDebug() << m->index(count, 0, QModelIndex());
            return m->index(count, 0, QModelIndex());
        }
        ++count;
    }
    return currentIndex();
}

QModelIndex MyListView::moveCursor(CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    qDebug() << "moveCursor" << cursorAction << modifiers;
    if (cursorAction == CursorAction::MoveDown)
    {
        QModelIndex index = currentIndex();
        qDebug() << index;
        QModelIndex nextIndex = m->index(index.row()+1, 0, QModelIndex());
        if (nextIndex.isValid())
        {
            return nextIndex;
        }
        else
        {
            return index;
        }
    }
    else if (cursorAction == CursorAction::MoveUp)
    {
        QModelIndex index = currentIndex();
        QModelIndex previousIndex = m->index(index.row()-1, 0, QModelIndex());
        if (previousIndex.isValid())
        {
            return previousIndex;
        }
        else
        {
            return index;
        }
    }
    return currentIndex();
}

int MyListView::horizontalOffset() const
{
    return horizontalScrollBar()->value();
}

int MyListView::verticalOffset() const
{
    return verticalScrollBar()->value();
}

bool MyListView::isIndexHidden(const QModelIndex &index) const
{
    return false;
}

void MyListView::setSelection(const QRect &rect, QItemSelectionModel::SelectionFlags command)
{
    QModelIndex firstIndex = indexAt(rect.topLeft());
    QModelIndex endIndex = currentIndex();

    QItemSelection selection(firstIndex, endIndex);
    selectionModel()->select(selection, command);

}

QRegion MyListView::visualRegionForSelection(const QItemSelection &selection) const
{
    QRegion region;
    foreach (const QModelIndex &index, selection.indexes())
    {
        QRect rect = visualRect(index);
        if (!rect.isNull())
            region += rect;
    }
    qDebug() << region << "region";
    return region;
}



void MyListView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QList<int> &roles)
{
    qDebug() << topLeft << bottomRight;

}


void MyListView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    qDebug() << selected << deselected;

}





void MyListView::paintEvent(QPaintEvent *event)
{
    QPainter p(viewport());
    int allWidth = viewport()->width();
    int startY = -verticalScrollBar()->value();
    int startX = 0;
    int extraX = 40;
    m_rectForView.clear();

    // コンテンツの全体の高さを計算
    int totalHeight = 0;

    for (int row = 0; row < m->rowCount(); ++row )
    {
        QModelIndex index = m->index(row, 0);
        QString text = m->data(index, Qt::DisplayRole).toString();
        QFont font = m->data(index, Qt::FontRole).value<QFont>();

        QFontMetrics fm(font);
        int height = fm.height();
        int width  = allWidth - 1;
        QIcon icon = m->data(index, Qt::DecorationRole).value<QIcon>();

        p.drawRect(QRect(startX, startY , width, height));
        m_rectForView.append(QRect(startX, startY , width, height));



        p.drawText(QPoint(startX + extraX, startY + height), text);
        p.drawPixmap(QRect(startX, startY, 32, height ), icon.pixmap(height ));
        startY += height;
        totalHeight += height;
    }

    for (QModelIndex in : selectionModel()->selection().indexes())
    {
        QRect rect = visualRect(in);
        if (!rect.isNull())
        {
            p.save();
            QBrush brush(QColor(0, 0, 255, 50)); // 青みがかった透明な色
            p.setBrush(brush);
            QRect r = rect;
            r.setLeft(0);

            r.setWidth(allWidth);

            p.drawRect(r);
            p.restore();
        }
    }

    if (visualRect(currentIndex()).isValid())
    {
        QRect r = visualRect(currentIndex());
        p.save();
        QBrush b = p.brush();
        b.setStyle(Qt::SolidPattern);
        b.setColor(QColor(128,128,128,50));
        p.setBrush(b);
        p.drawRect(r);
        p.restore();
    }
    // スクロールバーの設定
    verticalScrollBar()->setRange(0, std::max(0, totalHeight - viewport()->height()));
    verticalScrollBar()->setPageStep(viewport()->height());
    verticalScrollBar()->setSingleStep(10); // スクロールステップを設定

    p.end();
    qDebug() << "paintEvent called";
}


void MyListView::scrollContentsBy(int dx, int dy)
{

    scrollDirtyRegion(dx, dy);
    viewport()->scroll(dx, dy);

    viewport()->update(); // ビューポートを再描画
}
