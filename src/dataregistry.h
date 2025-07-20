#ifndef DATAREGISTRY_H
#define DATAREGISTRY_H

#include <QObject>
#include <QSqlDatabase>

// Forward declarations
class VendorAccess;

class DataRegistry : public QObject {
    Q_OBJECT
    VendorAccess* m_vendoraccess;   // Pointer to vendor access object

public:
    explicit DataRegistry(QSqlDatabase db, QObject *parent = nullptr);

    VendorAccess *vendoraccess() const;
};

#endif // DATAREGISTRY_H
