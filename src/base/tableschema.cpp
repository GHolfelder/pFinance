#include "tableschema.h"

/**
 * @brief Table schema constructor
 *
 * @param tableName Name of table being defined
 * @param parent Reference to parent class.
 */
TableSchema::TableSchema(const QString &tableName, QObject *parent) : QObject(parent) {
    m_tableName = tableName;
    setObjectName(tableName);
}

/**
 * @brief Add column definition to table schema
 * @param column Column property values
 */
void TableSchema::addColumn(const ColumnDefinition &column) {
    m_columns.append(column);
}

/**
 * @brief Table name getter
 * @returns Name of table
 */
QString TableSchema::tableName() const {
    return m_tableName;
}

/**
 * @brief Table structure getter
 * @returns Table structure
 */
const QList<ColumnDefinition> &TableSchema::columns() const {
    return m_columns;
}

/**
 * @brief Initialize empty result variant map with default values for each field
 *
 * @returns QVariant map that can be used for adding a new record to the database
 */
QVariantMap TableSchema::initialize() {
    QVariantMap result;

    for (auto it = m_columns.constBegin(); it != m_columns.constEnd(); ++it) {
        if (!it->isPrimaryKey) switch (it->type) {
            case COLUMNTYPE::STRING:
                result[it->name] = it->defaultValue.isEmpty() ? "" : it->defaultValue;
                break;
            case COLUMNTYPE::INT:
                result[it->name] = it->defaultValue.isEmpty() ? 0 : it->defaultValue.toInt();
                break;
            case COLUMNTYPE::DATE:
                result[it->name] = it->defaultValue.isEmpty() ? 0 : it->defaultValue.toInt();
                break;
            case COLUMNTYPE::CURRENCY:
                result[it->name] = it->defaultValue.isEmpty() ? 0.0 : it->defaultValue.toFloat();
                break;
            case COLUMNTYPE::FLOAT:
                result[it->name] = it->defaultValue.isEmpty() ? 0.0 : it->defaultValue.toFloat();
                break;
        }
    }
    return result;
}

/**
 * @brief Get count of defined columns
 *
 * @returns Number of columns in table
 */
int TableSchema::columnCount() const {
    return m_columns.count();
}

/**
 * @brief Column names getter
 *
 * @param includePrimary When true, primary field is included, otherwise the primary key will not be in the result set.
 * @returns QString list of column names
 */
QStringList TableSchema::columnNames(bool includePrimary) const {
    QStringList names;
    for (const ColumnDefinition &column : m_columns) {
        if (includePrimary || !column.isPrimaryKey)
            names << column.name;
    }
    return names;
}

/**
 * @brief Column placeholder getter
 *
 * Placeholders are used to represent dynamic values that will be substituted when the SQL
 * statement is executed. The format of the generated placeholder is ":column-name" where
 * the column-name is the column name from the schema.
 *
 * @param includePrimary When true, primary field is included, otherwise the primary key will not be in the result set.
 * @returns QString list of column placeholders
 */
QStringList TableSchema::columnPlaceholders(bool includePrimary) const {
    QStringList placeholders;
    for (const ColumnDefinition &column : m_columns) {
        if (includePrimary || !column.isPrimaryKey)
            placeholders << ":" + column.name;
    }
    return placeholders;
}

/**
 * @brief Column titles getter
 *
 * @param includePrimary When true, primary field is included, otherwise the primary key will not be in the result set.
 * @returns QString list of column titles
 */
QStringList TableSchema::columnTitles(bool includePrimary) const {
    QStringList titles;
    for (const ColumnDefinition &column : m_columns) {
        if (includePrimary || !column.isPrimaryKey)
            titles << column.title;
    }
    return titles;
}

/**
 * @brief Column types getter
 *
 * @param includePrimary When true, primary field is included, otherwise the primary key will not be in the result set.
 * @returns QString list of column types represented as a string that corresponds to their enum value
 */
QStringList TableSchema::columnTypes(bool includePrimary) const {
    QStringList types;
    for (const ColumnDefinition &column : m_columns) {
        if (includePrimary || !column.isPrimaryKey) switch (column.type) {
            case COLUMNTYPE::STRING:   types << "STRING"; break;
            case COLUMNTYPE::INT:      types << "INT"; break;
            case COLUMNTYPE::DATE:     types << "DATE"; break;
            case COLUMNTYPE::CURRENCY: types << "CURRENCY"; break;
            case COLUMNTYPE::FLOAT:    types << "FLOAT"; break;
        }
    }
    return types;
}

/**
 * @brief Get default sort columns
 *
 * The first field tyhat is not a primary key will always be
 * the default sort column.
 *
 * @returns QString name of column to be sorted by default
 */
QString TableSchema::defaultSort() const {
    for (const ColumnDefinition &column : m_columns) {
        if (!column.isPrimaryKey)
            return column.name;
    }
    return "";
}

/**
 * @brief Retrieve primary key for table
 *
 * @param placeholder When true, the placeholder name is returned, otherwise the column name
 * @returns Name of primary key field or blank if there is none
 */
QString TableSchema::primaryKey(bool placeholder) const {
    for (const ColumnDefinition &column : m_columns) {
        if (column.isPrimaryKey)
            return (placeholder ? ":" : "") + column.name;
    }
    return "";
}

/**
 * @brief Convert placeholder name to column name
 *
 * @param placeholder Placeholder name
 * @returns column name
 */
QString TableSchema::toName(const QString placeholder) const {
    QString columnName = placeholder;
    columnName.remove(":");
    return columnName;
}

/**
 * @brief Create Sql statement for getting count of recors in table
 *
 * @returns QString Sql statement
 */
QString TableSchema::countSql() const {
    return "SELECT COUNT(*) FROM " + m_tableName;
}

/**
 * @brief Create Sql statement for creating table in database
 * @returns QString Sql statement
 */
QString TableSchema::createTableSql() const {
    QStringList columnDefs;

    for (const ColumnDefinition &column : m_columns) {
        QString def = QString("%1 %2").arg(column.name, column.sqlType);

        if (column.isPrimaryKey)
            def += " PRIMARY KEY";
        if (column.isAutoIncrement)
            def += " AUTOINCREMENT";
        if (column.isNullable)
            def += " NOT NULL";
        if (!column.defaultValue.isEmpty())
            def += QString(" DEFAULT %1").arg(column.defaultValue);

        columnDefs << def;
    }

    QString sql = QString("CREATE TABLE IF NOT EXISTS %1 (%2);")
        .arg(m_tableName, columnDefs.join(", "));
    return sql;
}

/**
 * @brief Create Sql statement for deletoing a row from the table
 *
 * This method assumes that there is a primary key.
 *
 * @returns QString Sql statement
 */
QString TableSchema::deleteSql() const {
    return QString("DELETE FROM %1 WHERE %2 = %3")
        .arg(m_tableName, primaryKey(false), primaryKey(true));
}

/**
 * @brief Create Sql statement for inserting row into table
 *
 * The insert will always include the primary key.
 *
 * @param data Variant map of data to be inserted
 * @returns QString Sql statement
 */
QString TableSchema::insertSql(const QVariantMap &data) const {
    QStringList columns;
    QStringList placeholders;

    // Add primary key and any field in the variant map to column and placeholder list
    for (auto it = m_columns.constBegin(); it != m_columns.constEnd(); ++it) {
        const QString name = it->name;
        if (it->isPrimaryKey || data.contains(name)) {
            columns << name;
            placeholders << ":" + name;
        }
    }

    // Return generated insert
    return QString("INSERT INTO %1 (%2) VALUES (%3)")
        .arg(m_tableName, columns.join(", "), placeholders.join(", "));
}

/**
 * @brief Create Sql statement for selecting a row from table
 *
 * All columns of a table will be selected, including the primary key.
 * This method assumes that there is a primary key.
 *
 * @returns QString Sql statement
 */
QString TableSchema::selectSql() const {
    QStringList const columns = columnNames(true);

    // Return generated statement
    return QString("SELECT %1 FROM %2 WHERE %3 = %4")
        .arg(columns.join(", "), m_tableName, primaryKey(false), primaryKey(true));
}

/**
 * @brief Create Sql statement for selecting a series of rows from table
 *
 * All columns of a table will be selected, including the primary key.
 * A sort column and order needs to be provided.
 *
 * @returns QString Sql statement
 */
QString TableSchema::selectSql(const QString sortColumn, const Qt::SortOrder sortOrder) const {
    QStringList const columns = columnNames(true);

    // Return generated statement
    return QString("SELECT %1 FROM %2 ORDER BY %3 %4")
        .arg(columns.join(", "), m_tableName, sortColumn, sortOrder == Qt::AscendingOrder ? "ASC" : "DESC");
}

/**
 * @brief Create Sql statement for updating a row in the table
 *
 * At a minimum, there must be the primary key of the row to be updated
 * along with at least one other value for the generated sql to be correct.
 * Only valid fields will be added to the update statement.
 *
 * @param data Variant map containing fields and values to be updated
 * @returns QString Sql statement
 */
QString TableSchema::updateSql(const QVariantMap &data) const {
    QStringList assignments;

    for (const ColumnDefinition &column : m_columns) {
        QString assign = QString("%1 = %2").arg(column.name, ":" + column.name);
        if (!column.isPrimaryKey && data.contains(column.name))
            assignments << assign;
    }

    // Return generated statement
    return QString("UPDATE %1 SET %2 WHERE %3 = %4")
        .arg(m_tableName, assignments.join(", "), primaryKey(false), primaryKey(true));
}
