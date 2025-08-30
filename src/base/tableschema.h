#ifndef TABLESCHEMA_H
#define TABLESCHEMA_H

#include <QObject>
#include <QVariantMap>
#include "columnconstraint.h"

enum class FilterOperator {                         // Where filter operators
    Equals,
    NotEquals,
    LessThan,
    GreaterThan,
    LessThanOrEqual,
    GreaterThanOrEqual,
    Like,
    In,
    IsNull,
    IsNotNull
};

enum class ColumnType {                             // Logical column data types
    String,
    Int,
    Date,
    Currency,
    Float
};

struct FilterCondition {                            // Filter condition structure
    QString columnName;
    FilterOperator op;
    QVariant value;                                 // Optional, depending on op
};

struct ColumnDefinition {
    QString name;                                   // Internal name
    QString title;                                  // Display title
    ColumnType type;                                // Logical data type (e.g. ColumnType:STRING, ColumnType:INT)
    QString sqlType;                                // PostgreSQL type (e.g. "UUID", "VARCHAR(100)", "INTEGER")
    bool isPrimaryKey = false;                      // True if this is the primary key
    bool isAutoIncrement = false;                   // True if this is an auto increment value
    bool isNullable = true;                         // True if column is nullable
    QString defaultValue;                           // Optional default value for column
    std::function<QString(QVariant)> render;        // Optional formatter for query or display
    std::shared_ptr<ColumnConstraint> constraint;   // Optional constraint
};

class TableSchema : public QObject
{
    Q_OBJECT
public:
    explicit TableSchema(const QString &tableName, QObject *parent = nullptr);

    void addColumn(const ColumnDefinition &column);
    QString tableName() const;
    const QList<ColumnDefinition> &columns() const;

    QVariantMap initialize();

    // Table properties
    int columnCount() const;
    QStringList columnNames(bool includePrimary = true) const;
    QStringList columnPlaceholders(bool includePrimary = true) const;
    QStringList columnTitles(bool includePrimary = true) const;
    QStringList columnTypes(bool includePrimary = true) const;
    QVariantMap columnValues(const QString &columnName) const;
    QString defaultSort() const;
    QString primaryKey(bool placeholder = false) const;
    QString toName(const QString placeholder) const;

    // Generate Sql
    QString countSql() const;
    QString createColumnConstraintSql() const;
    QString createTableSql() const;
    QString deleteSql() const;
    QString insertSql(const QVariantMap &data) const;
    QString selectSql() const;
    QString selectSql(const QList<FilterCondition> &filters) const;
    QString selectSql(const QList<FilterCondition> &filters, const QString &sortColumn, const Qt::SortOrder sortOrder) const;
    QString updateSql(const QVariantMap &data) const;
    QString updateInsertSql(const QVariantMap &data, const QStringList matchColumns) const;


private:
    // Constraints
    std::shared_ptr<EnumConstraint> enumConstraint(const QString &columnName) const;

    // Utlity methods
    QString formatValue(const QVariant &value, ColumnType type) const;
    QString operatorToSql(FilterOperator op) const;
    QString whereClause(const QList<FilterCondition> &conditions) const;

    QString m_tableName;                            // Table name
    QList<ColumnDefinition> m_columns;              // Column properties
};

#endif // TABLESCHEMA_H
