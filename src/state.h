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

    Qt::SortOrder restoreSortOrder(const QString propertyName, const Qt::SortOrder defaultValue);
    QString restoreString(const QString propertyName, const QString defaultValue);
    QStringList restoreStringList(const QString propertyName, const QStringList defaultValue);

    bool save(const QString propertyName, const Qt::SortOrder propertyValue);
    bool save(const QString propertyName, const QString propertyValue);
    bool save(const QString propertyName, const QStringList propertyValue);

private:
    QString restoreValue(const QString propertyName);
    bool saveValue(const QString propertyName, const QString propertyValue);

    QString m_object;                               // Name of object whose properties are being saved
};

#endif // STATE_H
