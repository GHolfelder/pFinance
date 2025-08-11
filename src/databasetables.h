#ifndef DATABASETABLES_H
#define DATABASETABLES_H

#include <QObject>
#include <QSqlDatabase>

// Forward declarations
class VendorTable;

class DatabaseTables : public QObject {
    Q_OBJECT
    VendorTable* m_vendortable;   // Pointer to vendor access object

public:
    explicit DatabaseTables(QSqlDatabase db, QObject *parent = nullptr);

    VendorTable *vendortable() const;
};

#endif // DATABASETABLES_H
