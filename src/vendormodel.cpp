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

/**
 * @brief Vendor model constructor
 *
 * @param db Database object where connection has been made
 * @param columnNames List of column names in model
 * @param columnTitles List of column titles for display
 * @param parent Reference to parent class.
 */
VendorModel::VendorModel(QSqlDatabase db, QObject *parent) : ModelBase(db, COLUMN_NAMES, COLUMN_TITLES, parent) {
    setObjectName("VendorModel");
}

/**
 * @brief Retrieve the vendor data for the display
 *
 * @param index Row and column requested
 * @param role Type of data to be retrieved
 * @returns Requested data or an empty QVariant
 */
QVariant VendorModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case CellDataRole:
        return m_data.at(index.row()).at(index.column());
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

/**
 * @brief Retrieve the vendor header data (column titles) for the display
 *
 * @param index Row and column requested
 * @param role Type of data to be retrieved
 * @returns Requested data or an empty QVariant
 */
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

/**
 * @brief Return the hash table of roles
 *
 * The hash table maps the various roles for data and header
 * data retrieval to a symbol.
 *
 * @returns Hash table of roles
 */
QHash<int, QByteArray> VendorModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[CellDataRole] = "celldata";
    roles[CellNameRole] = "cellname";
    roles[RowRole] = "row";
    roles[ColumnRole] = "column";
    return roles;
}

/**
 * @brief Reload the model in the requested order
 *
 * @param columnName Column on which data is to be sorder
 */
void VendorModel::sortBy(const QString columnName) {
    setSort(columnName);

    // Load/Reload the data
    beginResetModel();
    m_data.clear();

    QSqlQuery query(m_db);
    QString select = QString("SELECT " + COLUMN_NAMES.join(", ") + " FROM vendors ORDER BY %1 %2")
        .arg(sortColumn())
        .arg(sortOrder() == Qt::AscendingOrder ? "ASC" : "DESC");
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
        m_data.append(v);
    }
    endResetModel();
}
