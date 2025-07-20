#ifndef VENDORACCESS_H
#define VENDORACCESS_H

#include "accessbase.h"
#include <QObject>
#include <QSqlDatabase>
#include <QVariantMap>

class VendorAccess : public AccessBase
{
    Q_OBJECT
public:
    explicit VendorAccess(QSqlDatabase db, QObject *parent = nullptr);

    Q_INVOKABLE int count() const;
    Q_INVOKABLE bool add(const QVariantMap &vendorData);
    Q_INVOKABLE bool get(const QString &id, QVariantMap &result);
    Q_INVOKABLE bool update(const QString &id, const QVariantMap &vendorData);
    Q_INVOKABLE bool remove(const QString &id);
    Q_INVOKABLE bool generateSample(int count);
};

#endif // VENDOR_H
