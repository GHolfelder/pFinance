#ifndef STATETABLE_H
#define STATETABLE_H

#include <QObject>
#include "base/tableschema.h"

class StateTable : public TableSchema
{
    Q_OBJECT
public:
    explicit StateTable(QObject *parent = nullptr);

signals:
};

#endif // STATETABLE_H
