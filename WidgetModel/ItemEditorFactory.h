#ifndef ITEMEDITORFACTORY_H
#define ITEMEDITORFACTORY_H

#include <QItemEditorFactory>

class ItemEditorCreatorBase;

class ItemEditorFactory : public QItemEditorFactory
{
public:
    ItemEditorFactory();
    ~ItemEditorFactory();
    ItemEditorCreatorBase *m_itemEditorCreatorBase;
};

#endif // ITEMEDITORFACTORY_H
