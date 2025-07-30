#include "modelbase.h"

/**
 * @brief Model base constructor
 *
 * @param db Database object where connection has been made
 * @param columnNames List of column names in model
 * @param columnTitles List of column titles for display
 * @param parent Reference to parent class.
 */
ModelBase::ModelBase(
    QSqlDatabase db,
    const QStringList &columnNames,
    const QStringList &columnTitles,
    QObject *parent
    )
    : QAbstractTableModel(parent), m_db(db)
{
    m_columnNames = columnNames;
    m_columnTitles = columnTitles;
    setObjectName("ModelBase");
}

/**
 * @brief Get the number of columns for the given parent.
 *
 * @param parent Parent for which column count is desired (not used)
 * @returns Number of columns based on the number of columns extracted
 */
int ModelBase::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_columnNames.size();
}

/**
 * @brief Get the number of rows under the given parent.
 *
 * @param parent Parent for which row count is desired (not used)
 * @returns Number of loaded rows
 */
int ModelBase::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_data.size();
}

/**
 * @brief Get the current sort order (direction).
 *
 * @returns Sort direction, ascending or descending
 */
Qt::SortOrder ModelBase::sortOrder() {
    return m_sortOrder;
}

/**
 * @brief Get the current sort column.
 *
 * @returns Name of column on which data is to be sorted
 */
QString ModelBase::sortColumn() {
    return m_sortColumn;
}

/**
 * @brief Set sort properties
 *
 * If the same column is selected to be sorted, the sort order/direction
 * will be toggled. If a new column is selected, it will be sorted in
 * ascending order.
 *
 * This method just updates the properties. Use the getter methods
 * to use in building the SQL query to retrieve the records.
 *
 * Once the new sort properties have been set, the following signals are emitted:
 *      sortColumnChanged - Column has changed
 *      sortOrderChanged - Sort direction has changed
 *
 * @param sortColumn Name of column on which data is to be sorted
 * @param columnNames List of available column names
 */
void ModelBase::setSort(QString sortColumn) {
    // Toggle sort order if same column selected
    if (sortColumn != "" && sortColumn == m_sortColumn)
        m_sortOrder = (m_sortOrder == Qt::AscendingOrder) ? Qt::DescendingOrder : Qt::AscendingOrder;
    else {
        // Validate sort order
        if (!m_columnNames.contains(sortColumn)) {
            m_sortColumn = m_columnNames.at(0);
            qWarning() << "Unknown column used for sort order:" << sortColumn;
        } else
            m_sortColumn = sortColumn;
        // Default to ascending order
        m_sortOrder = Qt::AscendingOrder;
    }

    // Notify interface that order has changed
    emit sortOrderChanged();
}
