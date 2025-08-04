#include "tablemodel.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

/**
 * @brief Table access constructor
 *
 * This method sets the name of the object to tablename + TableModel for easier
 * diagnosis of any errors encountered.
 *
 * @param db Database object where tables are located
 * @param table Table schema to be used for all access
 * @param parent Reference to parent class.
 */
TableModel::TableModel(QSqlDatabase db, TableSchema *table, QObject *parent) : QAbstractTableModel(parent), TableMixin<TableModel>(db, table) {
    setObjectName(m_table->tableName() + "TableModel");
}

/**
 * @brief Retrieve data for the display
 *
 * @param index Row and column requested
 * @param role Type of data to be retrieved
 * @returns Requested data or an empty QVariant
 */
QVariant TableModel::data(const QModelIndex &index, int role) const {
    switch (role) {
    case CellDataRole:
        return m_data.at(index.row()).at(index.column());
    case CellNameRole:
        return m_table->columnNames().at(index.column());
    case RowRole:
        return index.row();
    case ColumnRole:
        return index.column();
    case IdRole:
        return m_data.at(index.row()).at(0);
    default:
        break;
    }

    return QVariant();
}

/**
 * @brief Retrieve header data (column titles) for the display
 *
 * @param section Section requested
 * @param orientation Header direction; horizontal or vertical
 * @param role Type of data to be retrieved
 * @returns Requested data or an empty QVariant
 */
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    switch (role) {
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal) {
            return m_table->columnTitles().at(section);
        } else if (orientation == Qt::Vertical) {
            return QVariant::fromValue(section + 1);
        }
    }
    case CellNameRole:
        return m_table->columnNames().at(section);
    case ColumnRole:
        return section;
    default:
        break;
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
QHash<int, QByteArray> TableModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    roles[CellDataRole] = "celldata";
    roles[CellNameRole] = "cellname";
    roles[RowRole] = "row";
    roles[ColumnRole] = "column";
    roles[IdRole] = "id";
    return roles;
}

/**
 * @brief Get the number of columns for the given parent.
 *
 * @param parent Parent for which column count is desired (not used)
 * @returns Number of columns based on the number of columns in table
 */
int TableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_table->columnCount();
}

/**
 * @brief Get the number of rows under the given parent.
 *
 * @param parent Parent for which row count is desired (not used)
 * @returns Number of loaded rows
 */
int TableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_data.size();
}

/**
 * @brief Get the current sort order (direction).
 *
 * @returns Sort order direction, ascending or descending
 */
Qt::SortOrder TableModel::sortOrder() {
    return m_sortOrder;
}

/**
 * @brief Get the current sort column.
 *
 * @returns Name of column on which data is to be sorted
 */
QString TableModel::sortColumn() {
    return m_sortColumn;
}

/**
 * @brief Get the default sort column.
 *
 * @returns Name of column on which data is to be sorted by default
 */
QString TableModel::defaultSort() {
    return m_table->defaultSort();
}

/**
 * @brief Reload the model in the requested order
 *
 * @param sortColumn Column on which data is to be sorted
 * @param id Value of id of record to be located
 * @returns Index to found record or -1 if not found
 */
int TableModel::sortBy(const QString sortColumn, const QString &id) {
    const QStringList columnNames = m_table->columnNames();

    // Toggle sort order if same column selected
    if (sortColumn != "" && sortColumn == m_sortColumn)
        m_sortOrder = (m_sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    else {
        // Validate sort order
        if (!columnNames.contains(sortColumn)) {
            m_sortColumn = columnNames.at(0);
            qWarning() << "Unknown column used for sort order:" << sortColumn;
        } else
            m_sortColumn = sortColumn;
        // Default to ascending order
        m_sortOrder = Qt::AscendingOrder;
    }

    // Notify interface that order has changed
    emit sortOrderChanged();
    return refresh(id);
}

/**
 * @brief Reload the model using the current properties
 *
 * @param id Value of id of record to be located
 * @returns Index to found record or -1 if not found
 */
int TableModel::refresh(const QString &id) {
    const QStringList columnNames = m_table->columnNames(true);
    const QString pKey = m_table->primaryKey();
    QSqlQuery query(m_db);
    int foundIdx = -1;
    int index = -1;

    // Prepare to load result set
    beginResetModel();
    m_data.clear();

    // Prepare query
    query.prepare(m_table->selectSql(m_sortColumn, m_sortOrder));
    if (!query.exec()) {
        fail( "failed query:" + query.lastError().text());
        endResetModel();
        return foundIdx;
    }

    // Save result set
    while (query.next()) {
        QList<QString> v;
        index += 1;
        for (const QString &name : columnNames) {
            if (name == pKey && query.value(name) == id) {
                foundIdx = index;
            }
            v << query.value(name).toString();
        }
        m_data.append(v);
    }
    endResetModel();
    success("successful query", id);

    return foundIdx;
}
