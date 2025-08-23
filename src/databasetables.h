#ifndef DATABASETABLES_H
#define DATABASETABLES_H

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QString>
#include "base/tableschema.h"

// Forward declarations
class VendorTable;

class DatabaseTables : public QObject {
    Q_OBJECT
    QMap<QString, TableSchema*> m_tables;           // Map of database access classes

public:
    explicit DatabaseTables(QObject *parent = nullptr);

    TableSchema* fetch(const QString &tableName) const;
    QVector<TableSchema*> getTableSchemasVector() const;
};

#endif // DATABASETABLES_H
