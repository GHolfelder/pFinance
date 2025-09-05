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
 *
 * @param column Column property values
 */
void TableSchema::addColumn(const ColumnDefinition &column) {
    m_columns.append(column);
}

/**
 * @brief Add foreign key definition to table schema
 *
 * @param fk Foreign key propertgy values
 */
void TableSchema::addForeignKey(const ForeignKey &fk) {
    m_foreignKeys.append(fk);
}

/**
 * @brief Table name getter
 *
 * @returns Name of table
 */
QString TableSchema::tableName() const {
    return m_tableName;
}

/**
 * @brief Table structure getter
 *
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
            case ColumnType::String:
                result[it->name] = it->defaultValue.isEmpty() ? "" : it->defaultValue;
                break;
            case ColumnType::Int:
                result[it->name] = it->defaultValue.isEmpty() ? 0 : it->defaultValue.toInt();
                break;
            case ColumnType::Date:
                result[it->name] = it->defaultValue.isEmpty() ? 0 : it->defaultValue.toInt();
                break;
            case ColumnType::Currency:
                result[it->name] = it->defaultValue.isEmpty() ? 0.0 : it->defaultValue.toFloat();
                break;
            case ColumnType::Float:
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
 * Use the label option to indicate how columns with constraints are to be handled
 *  * False indicates that no special processing is associated with constraint columns
 *  * Label indicates that "_label" is to be added to the column name of any column with a column constraint
 *  * Expression adds a SQL expression converting the column constraint to a label as well as appending "_label" to the column name
 *
 * @param includePrimary When true, primary field is included, otherwise the primary key will not be in the result set.
 * @param labelOption Indicates how columns with constraints are to be handled
 * @returns QString list of column names
 */
QStringList TableSchema::columnNames(bool includePrimary, ColumnLabels labelOption) const {
    QStringList names;
    for (const ColumnDefinition &column : m_columns) {
        if (includePrimary || !column.isPrimaryKey) {
            switch (labelOption) {
            case ColumnLabels::Label:
                if (isEnumConstraint(column))
                    names << (column.name + "_label");
                else
                    names << column.name;
                break;
            case ColumnLabels::Expression:
                if (isEnumConstraint(column)) {
                    auto constraint = std::dynamic_pointer_cast<EnumConstraint>(column.constraint);
                    names << (QString(enumClause(column.name, *constraint)) + QString(" AS %1").arg(column.name + "_label"));
                } else
                    names << column.name;
                break;
            default:
                names << column.name;
            }
        }
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
            case ColumnType::String:   types << "STRING";   break;
            case ColumnType::Int:      types << "INT";      break;
            case ColumnType::Date:     types << "DATE";     break;
            case ColumnType::Currency: types << "CURRENCY"; break;
            case ColumnType::Float:    types << "FLOAT";    break;
        }
    }
    return types;
}

/**
 * @brief Column values getter
 *
 * This method returns a QVariantMap of enumerated integer values and their corresponding label.
 * This in turn can be used for validating values entered by the users.
 *
 * @param columnName Name of column whose values are to be retrieved
 * @returns QVariant map of values and labels or an empty list
 */
QVariantMap TableSchema::columnValues(const QString &columnName) const {
    QVariantMap emptyList;
    std::shared_ptr<EnumConstraint> values = enumConstraint(columnName);
    if (values)
        return values->values();
    return emptyList;
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
 * @brief Verify that list of column names are valid.
 *
 * This does not check for duplicate entries in the list.
 *
 * @param columnNames List of column names
 * @param useLabels True to use labeled enumerated columns
 * @returns True when list has all valid column names, otherwise false
 */
bool TableSchema::isColumnListValid(QStringList &nameList, bool useLabels) const {
    const QStringList allColumns = columnNames(true, useLabels ? ColumnLabels::Label : ColumnLabels::False);

    for (const auto &columnName : nameList) {
        if (!allColumns.contains(columnName))
            return false;
    }
    return true;
}

/**
 * @brief Verify that list of column name is valid.
 *
 * @param name Name of column
 * @param useLabels True to use labeled enumerated columns
 * @returns True when name is a valid column name, otherwise false
 */
bool TableSchema::isColumnValid(QString &name, bool useLabels) const {
    const QStringList allColumns = columnNames(true, useLabels ? ColumnLabels::Label : ColumnLabels::False);

    if (!allColumns.contains(name))
        return false;
    return true;
}

/**
 * @brief Create Sql statement for getting count of records in table
 *
 * @returns QString Sql statement
 */
QString TableSchema::countSql() const {
    return "SELECT COUNT(*) FROM " + m_tableName;
}

/**
 * @brief Create Sql statement for any column constraints
 *
 * Note: A case insensitive compare is used to verify that the constraint
 *       does not yet exist.
 *
 * @returns QString Sql statement
 */
QString TableSchema::createColumnConstraintSql() const {
    QStringList statements;

    // For each column
    for (const auto &col : m_columns) {
        if (col.type != ColumnType::Int || !col.constraint)
            continue;

        if (col.constraint->type != ConstraintType::EnumSet)
            continue;

        // Cast to underlying class and verify list is not empty
        auto enumConstraint = std::static_pointer_cast<EnumConstraint>(col.constraint);
        const QList<int> allowed = enumConstraint->allowedValues();
        if (allowed.isEmpty())
            continue;

        // Create arguments for generated SQL
        QString constraintName = QString("chk_%1_%2").arg(m_tableName, col.name);
        QStringList stringified;
        for (int val : allowed)
            stringified << QString::number(val);
        QString allowedList = stringified.join(", ");

        QString ddl = QString(R"(
DO $$
BEGIN
    IF NOT EXISTS (
        SELECT 1 FROM pg_constraint
        WHERE conname ILIKE '%1'
    ) THEN
        ALTER TABLE %2
        ADD CONSTRAINT %1
        CHECK (%3 IN (%4));
    END IF;
END
$$;
)").arg(constraintName,
    m_tableName,
    col.name,
    allowedList);
    statements << ddl.trimmed();
    }
    return statements.join("\n\n");
}

/**
 * @brief Create Sql statements for adding constraints to table
 *
 * @returns QString Sql statement
 */
QString TableSchema::createForeignKeySql() const {
    QStringList sql;

    // Process all defined foreign keys
    for (const auto &fk : m_foreignKeys) {
        QString constraintName = QString("fk_%1_%2").arg(m_tableName, fk.localColumn);
        QString onDeleteStr = constraintClause("ON DELETE", fk.onDelete);
        QString onUpdateStr = constraintClause("ON UPDATE", fk.onUpdate);
        QString ddl = QString(R"(
DO $$
BEGIN
    IF NOT EXISTS (
        SELECT 1 FROM pg_constraint
        WHERE conname = '%1'
    ) THEN
        ALTER TABLE %2
        ADD CONSTRAINT %1
        FOREIGN KEY (%3)
        REFERENCES %4 (%5)
        %6
        %7;
    END IF;
END
$$;
)").arg(constraintName,                         // %1 - Constraint name
        m_tableName,                            // %2 - Table name
        fk.localColumn,                         // %3 - Local column foreign key
        fk.referencedTable,                     // %4 - Parent table name
        fk.referencedColumn,                    // %5 - Parent primary key
        onDeleteStr,                            // %6 - Delete constraint
        onUpdateStr);                           // %7 - Update constraint
        // Append to prior constraint
        sql << ddl.trimmed();
    }
    // Return all constraints
    return sql.join("\n\n");
}

/**
 * @brief Create Sql statement for creating table in database
 *
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
 * @param useLabels True to use labeled enumerated columns
 * @returns QString Sql statement
 */
QString TableSchema::selectSql(bool useLabels) const {
    QStringList const columns = columnNames(true, useLabels ? ColumnLabels::Expression : ColumnLabels::False);

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
 * @param useLabels True to use labeled enumerated columns
 * @returns QString Sql statement
 */
QString TableSchema::selectSql(const QList<FilterCondition> &filters, bool useLabels) const {
    QStringList const columns = columnNames(true, useLabels ? ColumnLabels::Expression : ColumnLabels::False);

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
 * @param useLabels True to use labeled enumerated columns
 * @returns QString Sql statement
 */
QString TableSchema::selectSql(const QList<FilterCondition> &filters, const QString &sortColumn, const Qt::SortOrder sortOrder, bool useLabels) const {
    QStringList const columns = columnNames(true, useLabels ? ColumnLabels::Expression : ColumnLabels::False);

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
 * @brief Convert constraint to related SQL syntax
 *
 * @param policy Either "ON DELETE" or "ON UPDATE"
 * @param constraint Foreign key constraint value
 * @returns Constraint clause
 */
QString TableSchema::constraintClause(const QString policy, const ReferentialAction constraint) const {
    switch (constraint) {
    case ReferentialAction::NoAction:       return QString("%1 %2").arg(policy, "NO ACTION");
    case ReferentialAction::Cascade:        return QString("%1 %2").arg(policy, "CASCADE");
    case ReferentialAction::SetNull:        return QString("%1 %2").arg(policy, "SET NULL");
    case ReferentialAction::SetDefault:     return QString("%1 %2").arg(policy, "SET DEFAULT");
    case ReferentialAction::Restrict:       return QString("%1 %2").arg(policy, "RESTRICT");
        break;
    }
    return "";
}

/**
 * @brief Retrieve enumerated constraint for column
 *
 * @param columnName Name of column containing constraint
 * @returns Pointer to contraint function or null pointer if not found
 */
std::shared_ptr<EnumConstraint> TableSchema::enumConstraint(const QString &columnName) const {
    for (const auto &col : m_columns) {
        if (col.name == columnName && col.constraint &&
            col.constraint->type == ConstraintType::EnumSet) {
            return std::static_pointer_cast<EnumConstraint>(col.constraint);
        }
    }
    return nullptr;
}

/**
 * @brief Return SQL clause that will convert an enumerated column to a label value
 *
 * @param columnName Name of column
 * @param constraint Enumertated constraint. Contains label values
 * @returns SQL Case statement
 */
QString TableSchema::enumClause(const QString &columnName, const EnumConstraint &constraint) const {
    QString clause = QString("(CASE %1").arg(columnName);

    QList<int> values = constraint.allowedValues();
    for (int i = 0; i < values.size(); ++i) {
        int value = values.at(i);
        QString label = constraint.labelFor(value);
        clause += QString(" WHEN %1 THEN %2").arg(QString::number(value), formatValue(label, ColumnType::String));
        // ...
    }
    clause += QString(" ELSE 'Unknown' END)");
    return clause;
}

/**
 * @brief Format value for use in a SQL statement
 *
 * @param value Value to be used in statement
 * @param type Type of column
 * @return String to be used as value in statement
 */
QString TableSchema::formatValue(const QVariant &value, ColumnType type) const {
    switch (type) {
    case ColumnType::String:
    case ColumnType::Currency:
        return QString("'%1'").arg(value.toString().replace("'", "''"));
    case ColumnType::Date:
        return QString("'%1'").arg(value.toDate().toString("yyyy-MM-dd"));
    case ColumnType::Int:
    case ColumnType::Float:
        return value.toString();
    }
    return "NULL";
}

/**
 * @brief Determine if column is an enumerted column
 *
 * @param col Column to be checked
 * @returns True if enumerated column, otherwise false
 */
bool TableSchema::isEnumConstraint(const ColumnDefinition &col) const {
    return std::dynamic_pointer_cast<EnumConstraint>(col.constraint) != nullptr;
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

        if (cond.op == FilterOperator::IsNull || cond.op == FilterOperator::IsNotNull) {
            clauses << QString("%1 %2").arg(col.name, opStr);
        } else if (cond.op == FilterOperator::In && cond.value.canConvert<QVariantList>()) {
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
 * @brief Convert operator to related SQL syntax
 *
 * @param op Operator value
 * @return String value associated with operator
 */
QString TableSchema::operatorToSql(FilterOperator op) const {
    switch (op) {
    case FilterOperator::Equals:                return "=";
    case FilterOperator::NotEquals:             return "!=";
    case FilterOperator::LessThan:              return "<";
    case FilterOperator::GreaterThan:           return ">";
    case FilterOperator::LessThanOrEqual:       return "<=";
    case FilterOperator::GreaterThanOrEqual:    return ">=";
    case FilterOperator::Like:                  return "ILIKE";
    case FilterOperator::In:                    return "IN";
    case FilterOperator::IsNull:                return "IS NULL";
    case FilterOperator::IsNotNull:             return "IS NOT NULL";
    }
    return "";
}
