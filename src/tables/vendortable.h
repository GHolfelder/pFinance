#ifndef VENDORTABLE_H
#define VENDORTABLE_H

#include <QObject>
#include "base/tableschema.h"

class VendorTable : public TableSchema
{
    Q_OBJECT
public:
    explicit VendorTable(QObject *parent = nullptr);

signals:
};

#endif // VENDORTABLE_H
