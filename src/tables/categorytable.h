#ifndef CATEGORYTABLE_H
#define CATEGORYTABLE_H

#include <QObject>
#include "base/tableschema.h"

class CategoryTable : public TableSchema
{
    Q_OBJECT
    EnumConstraint TypeConstraint;

public:
    explicit CategoryTable(QObject *parent = nullptr);

signals:
};

#endif // CATEGORYTABLE_H
