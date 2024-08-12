#ifndef ITEMEDITORCREATORBASE_H
#define ITEMEDITORCREATORBASE_H

#include <QItemEditorCreatorBase>

class ItemEditorCreatorBase : public QItemEditorCreatorBase
{
public:
    ItemEditorCreatorBase();

    QByteArray m_propertyName;

    // QItemEditorCreatorBase interface
public:
    virtual QWidget *createWidget(QWidget *parent) const override;
    virtual QByteArray valuePropertyName() const override;
};

#endif // ITEMEDITORCREATORBASE_H
