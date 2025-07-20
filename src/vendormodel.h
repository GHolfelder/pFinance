#ifndef VENDORMODEL_H
#define VENDORMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QAbstractListModel>

class VendorModel : public QAbstractListModel {
    Q_OBJECT
    struct Vendor {                 // Structure to be used to hold a vendor record
        QString id;
        QString name;
        QString address1;
        QString address2;
        QString city;
        QString state;
        QString postal_code;
        QString phone;
        QString unpaid_balance;
    };
    QList<Vendor> m_vendors;        // List of vendor structures

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        Address1Role,
        Address2Role,
        CityRole,
        StateRole,
        Postal_CodeRole,
        PhoneRole,
        Unpaid_BalanceRole
    };

    explicit VendorModel(QObject *parent = nullptr);

    // Required overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Custom API
    Q_INVOKABLE void loadFromDatabase(QSqlDatabase db);
};
#endif // VENDORMODEL_H
