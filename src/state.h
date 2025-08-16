#ifndef STATE_H
#define STATE_H

#include <QObject>
#include <QSqlDatabase>
#include "databasetables.h"
#include "tableaccess.h"

class State : public TableAccess
{
    Q_OBJECT
public:
    explicit State(QSqlDatabase db, DatabaseTables *tables, QObject *parent = nullptr);

};

#endif // STATE_H
