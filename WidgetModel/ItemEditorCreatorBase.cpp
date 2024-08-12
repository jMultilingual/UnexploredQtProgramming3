#include "ItemEditorCreatorBase.h"
#include <QSpinBox>

ItemEditorCreatorBase::ItemEditorCreatorBase()
{
    m_propertyName = QByteArray(
        QSpinBox::staticMetaObject.userProperty().name());
}


QWidget *ItemEditorCreatorBase::createWidget(QWidget *parent) const
{
    QSpinBox *rangeSpinBox = new QSpinBox(parent);
    rangeSpinBox->setRange(0, 10);
    return rangeSpinBox;
}

QByteArray ItemEditorCreatorBase::valuePropertyName() const
{
    return m_propertyName;
}
