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
    "Unpaid\nBalance"
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


VendorModel::VendorModel(QSqlDatabase db, QObject *parent) : QAbstractTableModel(parent) {
    m_db = db;
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
    case CellDataRole:
        return m_vendors.at(index.row()).at(index.column());
    case CellNameRole:
        return COLUMN_NAMES.at(index.column());
    case RowRole:
        return index.row();
    case ColumnRole:
        return index.column();
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
    case CellNameRole:
        return COLUMN_NAMES.at(section);
    case ColumnRole:
        return section;
    default:                break;
    }

    return QVariant();
}

QHash<int, QByteArray> VendorModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[CellDataRole] = "celldata";
    roles[CellNameRole] = "cellname";
    roles[RowRole] = "row";
    roles[ColumnRole] = "column";
    return roles;
}

QVariant VendorModel::get_display_data(const QModelIndex &index) {
    return data(index, VendorModel::CellDataRole);
}

void VendorModel::sortBy(const QString columnName) {
    // Toggle sort order if same column selected
    if (columnName == m_sortColumn)
        m_sortOrder = (m_sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    else
        m_sortOrder = Qt::AscendingOrder;
    m_sortColumn = columnName;

    // Notify interface that order has change
    emit sortColumnChanged();
    emit sortOrderChanged();

    // Load/Reload the data
    beginResetModel();
    m_vendors.clear();

    QSqlQuery query(m_db);
    QString select = QString("SELECT " + COLUMN_NAMES.join(", ") + " FROM vendors ORDER BY %1 %2")
        .arg(m_sortColumn)
        .arg(m_sortOrder == Qt::AscendingOrder ? "ASC" : "DESC");
    query.prepare(select);

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

Qt::SortOrder VendorModel::sortOrder() {
    return m_sortOrder;
}

QString VendorModel::sortColumn() {
    return m_sortColumn;
}
