#ifndef DATAREGISTRY_H
#define DATAREGISTRY_H

#include <QObject>
#include <QSqlDatabase>

// Forward declarations
class VendorTable;

class DataRegistry : public QObject {
    Q_OBJECT
    VendorTable* m_vendortable;   // Pointer to vendor access object

public:
    explicit DataRegistry(QSqlDatabase db, QObject *parent = nullptr);

    VendorTable *vendortable() const;
};

#endif // DATAREGISTRY_H
