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
TableModel::TableModel(QSqlDatabase db, DatabaseTables *tables, QString tableName, QObject *parent) : QAbstractTableModel(parent), TableMixin<TableModel>(db, tables->fetch(tableName)) {
    m_table = tables->fetch(tableName);
    setObjectName(m_table->tableName() + "TableModel");
    // Initialize state
    m_state = new State(db, tables, objectName(), parent);
    // Get/set default sort column, sort order, visible columns
    m_sortColumn        = m_state->restoreString("sortColumn", m_table->defaultSort());
    m_sortOrder         = m_state->restoreSortOrder("sortOrder", Qt::AscendingOrder);
    m_visibleColumns    = m_state->restoreStringList("visibleColumns", m_table->columnAliases(false, true));
    // Verify that the properties are still correct in case there's been a database change
    if (!m_table->isAliasListValid(m_visibleColumns, true)) {
        qInfo() << (m_table->tableName() + " visible columns were invalid and reset");
        m_visibleColumns = m_table->columnAliases(false, true);
    }
    if (!m_table->isAliasValid(m_sortColumn, true)) {
        qInfo() << (m_table->tableName() + " sorted column was invalid and reset");
        m_sortColumn = m_table->defaultSort();
        m_sortOrder = Qt::AscendingOrder;
    }
}

/**
 * @brief Retrieve data for the display
 *
 * @param index Row and column requested
 * @param role Type of data to be retrieved
 * @returns Requested data or an empty QVariant
 */
QVariant TableModel::data(const QModelIndex &index, int role) const {
    int columnIndex = -1;

    // Retrieve requested information
    switch (role) {
    case CellDataRole:
        columnIndex = columnToIndex(index.column());
        return m_data.at(index.row()).at(columnIndex);
    case CellNameRole:
        return m_visibleColumns.at(index.column());
    case RowRole:
        return index.row();
    case ColumnRole:
        return index.column();
    case IdRole:
        return m_data.at(index.row()).at(m_table->columnAliases(true, true).indexOf(m_table->primaryKey()));
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
    int columnIndex = -1;

    // Retrieve requested information
    switch (role) {
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal) {
            columnIndex = columnToIndex(section);
            return m_table->columnTitles().at(columnIndex);
        } else if (orientation == Qt::Vertical) {
            return QVariant::fromValue(section + 1);
        }
    }
    case CellNameRole:
        return m_visibleColumns.at(section);
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
 * @returns Number of columns based on the number of visiblke columns
 */
int TableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_visibleColumns.size();
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
 * @brief Get list of all column names
 *
 * @returns List of column names
 */
QStringList TableModel::columnNames() const {
    return m_table->columnAliases(true, true);
}

/**
 * @brief Get list of all column titles
 *
 * @returns List of column titles
 */
QStringList TableModel::columnTitles() const {
    return m_table->columnTitles(true);
}

/**
 * @brief Get list of column types
 *
 * @returns List of column types represented as a string that corresponds to their enum value
 */
QStringList TableModel::columnTypes() const {
    return m_table->columnTypes();
}

/**
 * @brief Get list of visible column names
 *
 * @returns List of visible column names
 */
QStringList TableModel::visibleColumns() const {
    return m_visibleColumns;
}

/**
 * @brief Set list of visible column names
 *
 * @param columns List of visible column names
 */
void TableModel::setVisibleColumns(const QStringList &columns) {
    const QStringList allColumns = m_table->columnAliases(true, true);
    QStringList newColumns;

    // Validate and transfer columns to new column list
    for (const QString &name : allColumns) {
        if (columns.contains(name)) newColumns << name;
    }

    // On change in list
    if (m_visibleColumns != newColumns) {
        m_visibleColumns = newColumns;
        m_state->save("visibleColumns", m_visibleColumns);
        emit visibleColumnsChanged();
        // Trigger layout change
        beginResetModel();
        endResetModel();
    }
}

/**
 * @brief Reload the model in the requested order
 *
 * @param sortColumn Column on which data is to be sorted
 * @param id Value of id of record to be located
 * @returns Index to found record or -1 if not found
 */
int TableModel::sortBy(const QString sortColumn, const QString &id) {
    const QStringList allColumns = m_table->columnAliases(true, true);

    // Toggle sort order if same column selected
    if (sortColumn != "" && sortColumn == m_sortColumn)
        m_sortOrder = (m_sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    else {
        // Validate sort order
        if (!allColumns.contains(sortColumn)) {
            m_sortColumn = allColumns.at(0);
            qWarning() << "Unknown column used for sort order:" << sortColumn;
        } else
            m_sortColumn = sortColumn;
        // Default to ascending order
        m_sortOrder = Qt::AscendingOrder;
    }

    // Persist sort column and sort order
    m_state->save("sortColumn", m_sortColumn);
    m_state->save("sortOrder", m_sortOrder);
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
    const QStringList allColumns = m_table->columnAliases(true, true);
    const QString pKey = m_table->primaryKey();
    QSqlQuery query(m_db);
    int foundIdx = -1;
    int index = -1;

    // Prepare to load result set
    beginResetModel();
    m_data.clear();

    // Prepare query
    const QString sql = m_table->selectSql({}, m_sortColumn, m_sortOrder, true);
    query.prepare(sql);
    if (!query.exec()) {
        qDebug() << sql;
        fail( "failed query:" + query.lastError().text());
        endResetModel();
        return foundIdx;
    }

    // Save result set
    while (query.next()) {
        QList<QString> v;
        index += 1;
        for (const QString &name : allColumns) {
            if (name == pKey && query.value(name) == id) {
                foundIdx = index;
            }
            v << query.value(name).toString();
        }
        m_data.append(v);
    }
    endResetModel();
    success("successful query by", m_sortColumn);

    return foundIdx;
}

/**
 * @brief Convert column index to data index
 *
 * This uses the visible column list to convert the column
 * index from the browse to the data index.
 *
 * @param column Index to column in browse
 * @returns Data column index or -1
 */
int TableModel::columnToIndex(const int column) const {
    if (column < m_visibleColumns.size()) {
        QString columnName = m_visibleColumns.at(column);
        return m_table->columnAliases(true, true).indexOf(columnName);
    }
    return -1;
}
