#include <QDate>
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
 *
 * @param filters Filter structure to be applied to select
 * @returns QString Sql statement
 */
QString TableSchema::selectSql(const QList<FilterCondition> &filters) const {
    QStringList const columns = columnNames(true);

    // Return generated statement
    return QString("SELECT %1 FROM %2 %3")
        .arg(columns.join(", "), m_tableName, whereClause(filters));
}

/**
 * @brief Create Sql statement for selecting a series of rows from table
 *
 * All columns of a table will be selected, including the primary key.
 * A sort column and order needs to be provided.
 *
 * @param filters Filter structure to be applied to select
 * @param sortColumn Name of column to be used to sort data
 * @param sortOrder Sort direction of returned data
 * @returns QString Sql statement
 */
QString TableSchema::selectSql(const QList<FilterCondition> &filters, const QString sortColumn, const Qt::SortOrder sortOrder) const {
    QStringList const columns = columnNames(true);

    // Return generated statement
    return QString("SELECT %1 FROM %2 %3 ORDER BY %4 %5")
        .arg(columns.join(", "), m_tableName, whereClause(filters), sortColumn, sortOrder == Qt::AscendingOrder ? "ASC" : "DESC");
}

/**
 * @brief Create Sql statement for updating a row by id in the table
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

    // Build assignments
    for (const ColumnDefinition &column : m_columns) {
        QString assign = QString("%1 = %2").arg(column.name, ":" + column.name);
        if (!column.isPrimaryKey && data.contains(column.name))
            assignments << assign;
    }

    // Return generated statement
    return QString("UPDATE %1 SET %2 WHERE %3 = %4")
        .arg(m_tableName, assignments.join(", "), primaryKey(false), primaryKey(true));
}


/**
 * @brief Create Sql statement for updating an existing row or inserting the row if it can't be found
 *
 * At a minimum, there must be the primary key for the row to be inserted
 * along with at least one column to be used to locate the row in the database
 * and one additional column to be updated for the generated sql to be correct.
 * Only valid fields will be added to the generated SQL.
 *
 * Note: This method will work even if no unique constraint has been assigned
 *       to the columns used for matching.
 *
 * @param data Variant map containing fields and values to be updated
 * @param matchColumns List of column to be used in locating the row
 * @returns QString Sql statement
 */
QString TableSchema::updateInsertSql(const QVariantMap &data, const QStringList matchColumns) const {
    QStringList sourceColumns;
    QStringList sourcePlaceholders;
    QStringList matches;
    QStringList assignments;

    // Build list of source columns, placeholders and assignments
    for (const ColumnDefinition &column : m_columns) {
        // If valid column
        if (data.contains(column.name)) {
            sourceColumns << column.name;
            sourcePlaceholders << (":" + column.name);
            // If the column is one that we are matching on
            if (matchColumns.contains(column.name)) {
                QString match = QString("%1 = %2").arg(column.name, ":" + column.name);
                matches << match;
            // else it'll be a column we update
            } else if (!column.isPrimaryKey) {
                QString assign = QString("%1 = %2").arg(column.name, ":" + column.name);
                assignments << assign;
            }
        }
    }

    // Return generated statement
    return QString("MERGE INTO %1 AS target"
        " USING (SELECT %2) AS source"
        " ON %3"
        " WHEN MATCHED THEN"
        " UPDATE SET %4"
        " WHEN NOT MATCHED THEN"
        " INSERT (%5)"
        " VALUES (%6)")
        .arg(m_tableName,                       // %1 - Name of table
            sourcePlaceholders.join(", "),      // %2 - List of all source placeholders in the data
            matches.join(" AND "),              // %3 - List of columns to match on
            assignments.join(", "),             // %4 - List of columns to be updated
            sourceColumns.join(", "),           // %5 - List of all source columns in the data
            sourcePlaceholders.join(", "));     // %6 - List of all source placeholders in the data
}

/**
 * @brief Format value for use in a SQL statement
 *
 * @param value Value to be used in statement
 * @param type Type of column
 * @return String to be used as value in statement
 */
QString TableSchema::formatValue(const QVariant &value, COLUMNTYPE type) const {
    switch (type) {
    case COLUMNTYPE::STRING:
    case COLUMNTYPE::CURRENCY:
        return QString("'%1'").arg(value.toString().replace("'", "''"));
    case COLUMNTYPE::DATE:
        return QString("'%1'").arg(value.toDate().toString("yyyy-MM-dd"));
    case COLUMNTYPE::INT:
    case COLUMNTYPE::FLOAT:
        return value.toString();
    }
    return "NULL";
}

/**
 * @brief Generate where clause to be used in SQL statement
 *
 * @param conditions Structure containing where clause to be applied to statement
 * @return String to be inserted as where clause
 */
QString TableSchema::whereClause(const QList<FilterCondition> &conditions) const {
    QStringList clauses;

    for (const auto &cond : conditions) {
        auto it = std::find_if(m_columns.begin(), m_columns.end(),
            [&](const ColumnDefinition &col) { return col.name == cond.columnName; });

        if (it == m_columns.end()) {
            qWarning() << "Unknown column:" << cond.columnName;
            continue;
        }

        const auto &col = *it;
        const QString opStr = operatorToSql(cond.op);

        if (cond.op == FILTEROPERATOR::ISNULL || cond.op == FILTEROPERATOR::ISNOTNULL) {
            clauses << QString("%1 %2").arg(col.name, opStr);
        } else if (cond.op == FILTEROPERATOR::IN && cond.value.canConvert<QVariantList>()) {
            QStringList values;
            const QList<QVariant> list = cond.value.toList();
            for (const auto &v : list) {
                values << formatValue(v, col.type);
            }
            clauses << QString("%1 IN (%2)").arg(col.name, values.join(", "));
        } else {
            QString valStr = formatValue(cond.value, col.type);
            clauses << QString("%1 %2 %3").arg(col.name, opStr, valStr);
        }
    }

    return clauses.isEmpty() ? "" : "WHERE " + clauses.join(" AND ");
}

/**
 * @brief Convert operator to rtelated SQL syntax
 *
 * @param op Operator value
 * @return String value associated with operator
 */
QString TableSchema::operatorToSql(FILTEROPERATOR op) const {
    switch (op) {
    case FILTEROPERATOR::EQUALS:                return "=";
    case FILTEROPERATOR::NOTEQUALS:             return "!=";
    case FILTEROPERATOR::LESSTHAN:              return "<";
    case FILTEROPERATOR::GREATERTHAN:           return ">";
    case FILTEROPERATOR::LESSTHANOREQUAL:       return "<=";
    case FILTEROPERATOR::GREATERTHANOREQUAL:    return ">=";
    case FILTEROPERATOR::LIKE:                  return "ILIKE";
    case FILTEROPERATOR::IN:                    return "IN";
    case FILTEROPERATOR::ISNULL:                return "IS NULL";
    case FILTEROPERATOR::ISNOTNULL:             return "IS NOT NULL";
    }
    return "";
}
