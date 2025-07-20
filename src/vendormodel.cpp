#include "VendorModel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

// Initialize static column titles and names
const QStringList VendorModel::COLUMN_TITLES = {
    "ID",
    "Name",
    "Address 1",
    "Address 2",
    "City",
    "State",
    "Postal Code",
    "Phone",
    "Unpaid Balance"
};
const QStringList VendorModel::COLUMN_NAMES = {
    "id",
    "name",
    "address1",
    "address2",
    "city",
    "state",
    "postal_code",
    "phone",
    "unpaid_balance"
};


VendorModel::VendorModel(QObject *parent) : QAbstractTableModel(parent) {
    setObjectName("VendorModel");
}

int VendorModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_vendors.size();
}

int VendorModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return COLUMN_NAMES.size();
}

QVariant VendorModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case TableDataRole:
        return m_vendors.at(index.row()).at(index.column());
    default:
        break;
    }

    return QVariant();
}

QVariant VendorModel::headerData(int section, Qt::Orientation orientation, int role) const {
    switch (role) {
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal) {
            return COLUMN_TITLES.at(section);
        } else if (orientation == Qt::Vertical) {
            return QVariant::fromValue(section + 1);
        }
    }
    default:                break;
    }

    return QVariant();
}

QHash<int, QByteArray> VendorModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[TableDataRole] = "tabledata";
    //roles[SomeOtherRole] = "someotherrole"; // You can set up multiple mappings if needed
    return roles;
}

QVariant VendorModel::get_display_data(const QModelIndex &index) {
    return data(index, VendorModel::TableDataRole);
}

void VendorModel::loadFromDatabase(QSqlDatabase db) {
    beginResetModel();
    m_vendors.clear();

    QSqlQuery query(db);
    query.prepare("SELECT " + COLUMN_NAMES.join(", ") + " FROM vendors ORDER BY name ASC");

    if (!query.exec()) {
        qWarning() << "Failed to load vendors:" << query.lastError().text();
        endResetModel();
        return;
    }

    while (query.next()) {
        QList<QString> v;
        for (const QString &name : COLUMN_NAMES) {
            v << query.value(name).toString();
        }
        m_vendors.append(v);
    }
    endResetModel();
}
