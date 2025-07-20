#include "VendorModel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

VendorModel::VendorModel(QObject *parent) : QAbstractListModel(parent) {
    setObjectName("VendorModel");
}

int VendorModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_vendors.count();
}

QVariant VendorModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() < 0 || index.row() >= m_vendors.count())
        return QVariant();

    const Vendor &vendor = m_vendors.at(index.row());

    switch (role) {
    case IdRole:               return vendor.id;
    case NameRole:             return vendor.name;
    case Address1Role:         return vendor.address1;
    case Address2Role:         return vendor.address2;
    case CityRole:             return vendor.city;
    case StateRole:            return vendor.state;
    case Postal_CodeRole:      return vendor.postal_code;
    case PhoneRole:            return vendor.phone;
    case Unpaid_BalanceRole:   return vendor.unpaid_balance;
    default:                   return QVariant();
    }
}

QHash<int, QByteArray> VendorModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IdRole]              = "id";
    roles[NameRole]            = "name";
    roles[Address1Role]        = "address1";
    roles[Address2Role]        = "address2";
    roles[CityRole]            = "city";
    roles[StateRole]           = "state";
    roles[Postal_CodeRole]     = "postal_code";
    roles[PhoneRole]           = "phone";
    roles[Unpaid_BalanceRole]  = "unpaid_balance";
    return roles;
}

void VendorModel::loadFromDatabase(QSqlDatabase db) {
    beginResetModel();
    m_vendors.clear();

    QSqlQuery query(db);
    query.prepare("SELECT id, name, address1, address2, city, state, postal_code, phone, unpaid_balance FROM vendors ORDER BY name ASC");

    if (!query.exec()) {
        qWarning() << "Failed to load vendors:" << query.lastError().text();
        endResetModel();
        return;
    }

    while (query.next()) {
        Vendor v;
        v.id                   = query.value("id").toString();
        v.name                 = query.value("name").toString();
        v.address1             = query.value("address1").toString();
        v.address2             = query.value("address2").toString();
        v.city                 = query.value("city").toString();
        v.state                = query.value("state").toString();
        v.postal_code          = query.value("postal_code").toString();
        v.phone                = query.value("phone").toString();
        v.unpaid_balance       = query.value("unpaid_balance").toString();
        m_vendors.append(v);
    }
    endResetModel();
}
