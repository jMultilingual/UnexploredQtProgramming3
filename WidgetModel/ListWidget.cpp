#include "ListWidget.h"
#include "StyledItemDelegate.h"
#include <QSaveFile>
#include <QTreeWidgetItem>
#include <QFile>
#include <QMimeData>
#include <QUrl>

ListWidget::ListWidget()
{
    setDragEnabled(true);
    setAcceptDrops(true);
    setDefaultDropAction(Qt::MoveAction);
    addListWidgetItem(QIcon(":/images/1.jpg"),
                      "item0",
                      QFont("Times New Roman", 72),
                      QBrush(QColor(105, 171, 197)),
                      QBrush(QColor(237, 232, 150)),
                      Qt::AlignCenter|Qt::AlignLeft,
                      "Tool Tip Neko1",
                      Qt::CheckState::Checked,
                      QSize(150, 150),
                      Qt::ItemFlag::ItemIsEditable);
    addListWidgetItem(QIcon(":/images/2.jpg"),
                      "item1",
                      QFont("Segoe UI Light", 36),
                      QBrush(QColor(246, 241, 214)),
                      QBrush(QColor(117, 101,  93)),
                      Qt::AlignVCenter|Qt::AlignHCenter,
                      "Tool Tip Neko2",
                      Qt::CheckState::PartiallyChecked,
                      QSize(150, 150),
                      Qt::ItemFlag::ItemIsEditable);
    addListWidgetItem(QIcon(":/images/3.jpg"),
                      "item2",
                      QFont("Times New Roman", 18),
                      QBrush(QColor(176, 229, 213)),
                      QBrush(QColor(213, 187, 216)),
                      Qt::AlignVCenter|Qt::AlignRight,
                      "Tool Tip Neko3",
                      Qt::CheckState::Unchecked,
                      QSize(150, 150),
                      Qt::ItemFlag::ItemIsEditable);

    QSize size(150, 150);
    setIconSize(size);
    StyledItemDelegate *delegate = new StyledItemDelegate();
    setItemDelegate(delegate);

}

void ListWidget::addListWidgetItem(QIcon icon, const QString &text, QFont font, QBrush foreground, QBrush background, Qt::Alignment alignment, QString toolTip, Qt::CheckState checkState, QSize sizeHint, Qt::ItemFlags flags)
{
    QListWidgetItem *item = new QListWidgetItem(icon, text);
    item->setFont(font);
    item->setForeground(foreground);
    item->setBackground(background);
    item->setTextAlignment(alignment);
    item->setToolTip(toolTip);
    item->setCheckState(checkState);
    item->setSizeHint(sizeHint);
    item->setFlags(
        Qt::ItemIsEnabled|
        Qt::ItemIsUserCheckable|
        Qt::ItemIsUserTristate|
        Qt::ItemIsSelectable|
        Qt::ItemIsDragEnabled|
        Qt::ItemIsDropEnabled|
        flags
        );
    addItem(item);
}

void ListWidget::save()
{
    QSaveFile file("dummy.dat");
    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        for (int i = 0; i < count(); i++)
        {
            QListWidgetItem* l = item(i);
            l->write(out);
            out << l->flags();
        }
    }
    file.commit();
}

void ListWidget::load()
{
    QFile file("dummy.dat");
    if (file.exists())
    {
        if (file.open(QIODevice::ReadOnly))
        {
            clear();
            QDataStream out(&file);
            while (!out.atEnd())
            {
                QListWidgetItem* l = new QListWidgetItem();
                l->read(out);
                addItem(l);
                Qt::ItemFlags f;
                out >> f;
                l->setFlags(f);
            }
        }
    }
    file.close();
}




void ListWidget::dragEnterEvent(QDragEnterEvent *event)
{

    QListWidget::dragEnterEvent(event);
}

void ListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    QListWidget::dragMoveEvent(event);
}

void ListWidget::dropEvent(QDropEvent *event)
{
    QListWidget::dropEvent(event);
}


QStringList ListWidget::mimeTypes() const
{
    return {"application/x-qabstractitemmodeldatalist",
            "text/uri-list",
            "application/x-qabstractitemmodeldatatree"};
}


bool ListWidget::dropMimeData(int index, const QMimeData *data, Qt::DropAction action)
{
    if (data->hasFormat(mimeTypes().at(2)))
    {

        QByteArray qb(data->data(mimeTypes().at(2)));
        QDataStream out(&qb, QIODevice::ReadOnly);
        {
            int count = 0;
            QTreeWidgetItem *titem = new QTreeWidgetItem();
            titem->read(out);
            QFont font = titem->font(count);
            QIcon icon = titem->icon(count);
            QString text = titem->text(count);
            QBrush foreground = titem->foreground(count);
            QBrush background = titem->background(count);
            Qt::AlignmentFlag alignment = Qt::AlignLeft;
            Qt::CheckState checkState = titem->checkState(count);
            QString toolTip = titem->toolTip(count);
            QSize sizeHint = titem->sizeHint(count);

            Qt::ItemFlag flag;
            out >> flag;
            addListWidgetItem(icon,
                              text,
                              font,
                              foreground,
                              background,
                              alignment,
                              toolTip,
                              checkState,
                              sizeHint,
                              flag);
            delete titem;
        }
        return true;
    }
    else if (data->hasFormat(mimeTypes().at(0)))
    {
        QByteArray qb(data->data(mimeTypes().at(0)));

        if (qb.size() == 0)
        {
            return false;
        }
        if (dragDropOverwriteMode())
        {
            takeItem(index);
        }


        QDataStream out(&qb, QIODevice::ReadOnly);
        while (!out.atEnd())
        {
            QListWidgetItem* item = new QListWidgetItem();
            item->read(out);
            Qt::ItemFlag flag;
            out >> flag;
            item->setFlags(flag);
            addItem(item);
        }

        if (action == Qt::CopyAction)
        {
            return false;
        }
        return true;
    }

    if (data->hasUrls())
    {
        QUrl url = data->urls().at(0);
        QFont font("Arial", 72);
        QIcon icon(url.toLocalFile());
        QString text = "item3";
        QBrush foreground(QColor(230, 123, 249));
        QBrush background(QColor(123, 240, 111));
        Qt::AlignmentFlag alignment = Qt::AlignLeft;
        Qt::CheckState checkState = Qt::Checked;
        QString toolTip = "External Drop Neko";
        QSize sizeHint(200, 200);
        addListWidgetItem(icon,
                          text,
                          font,
                          foreground,
                          background,
                          alignment,
                          toolTip,
                          checkState,
                          sizeHint,
                          Qt::ItemFlag::ItemIsEditable);
        return true;
    }
    return false;
}


QMimeData *ListWidget::mimeData(const QList<QListWidgetItem *> &items) const
{
    QByteArray qb;
    QDataStream out(&qb, QIODevice::WriteOnly);
    for (const QListWidgetItem *item: items)
    {
        item->write(out);
        out << item->flags();
    }

    QMimeData *mimeData = new QMimeData();
    mimeData->setData(mimeTypes().at(0), qb);
    return mimeData;
}
