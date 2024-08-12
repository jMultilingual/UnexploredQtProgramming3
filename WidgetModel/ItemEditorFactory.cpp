#include "ItemEditorFactory.h"
#include "ItemEditorCreatorBase.h"

ItemEditorFactory::ItemEditorFactory()
{
    m_itemEditorCreatorBase = new ItemEditorCreatorBase();
    registerEditor(
        QMetaType::User,
        m_itemEditorCreatorBase
        ); //new
}

ItemEditorFactory::~ItemEditorFactory()
{
    delete m_itemEditorCreatorBase;
}
