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
    explicit State(QSqlDatabase db, DatabaseTables *tables, QString object, QObject *parent = nullptr);

    QString restore(const QString propertyName, const QString defaultValue);
    bool save(const QString propertyName, const QString propertyValue);

private:
    QString m_object;                               // Name of object whose properties are being saved
};

#endif // STATE_H
