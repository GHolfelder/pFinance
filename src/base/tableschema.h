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

enum class ColumnLabels {                           // Column labels option
    False,                                          // Column labels are not used
    Label,                                          // Use "_label" for any enumereated columns
    Expression                                      // Use an expression to retrieve the enumerated label and a "_label"
};

enum class ReferentialAction {                      // Referential integrity policies
    NoAction,
    Cascade,
    SetNull,
    SetDefault,
    Restrict
};

struct FilterCondition {                            // Filter condition structure
    QString columnName;
    FilterOperator op;
    QVariant value;                                 // Optional, depending on op
};

struct ColumnDefinition {                           // Column definition structure
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

struct ForeignKey {                                 // Foreign key structure
    QString localColumn;                            // Column name in table e.g. "vendor_id"
    QString referencedTable;                        // Related table e.g. "vendors"
    QString referencedColumn;                       // Related table id e.g. "id"
    QString alias;                                  // Optional alias for joined table
    ReferentialAction onDelete = ReferentialAction::NoAction;
    ReferentialAction onUpdate = ReferentialAction::NoAction;
};

class TableSchema : public QObject
{
    Q_OBJECT
public:
    explicit TableSchema(const QString &tableName, QObject *parent = nullptr);

    void addColumn(const ColumnDefinition &column);
    void addForeignKey(const ForeignKey &fk);
    QString tableName() const;
    const QList<ColumnDefinition> &columns() const;

    QVariantMap initialize();

    // Table properties
    int columnCount() const;
    QStringList columnNames(bool includePrimary = true, ColumnLabels labelOption = ColumnLabels::False) const;
    QStringList columnPlaceholders(bool includePrimary = true) const;
    QStringList columnTitles(bool includePrimary = true) const;
    QStringList columnTypes(bool includePrimary = true) const;
    QVariantMap columnValues(const QString &columnName) const;
    QString defaultSort() const;
    QString primaryKey(bool placeholder = false) const;
    QString toName(const QString placeholder) const;

    // Validation methods
    bool isColumnListValid(QStringList &nameList, bool useLabels=false) const;
    bool isColumnValid(QString &namebool, bool useLabels=false) const;

    // Generate Sql
    QString countSql() const;
    QString createColumnConstraintSql() const;
    QString createForeignKeySql() const;
    QString createTableSql() const;
    QString deleteSql() const;
    QString insertSql(const QVariantMap &data) const;
    QString selectSql(bool useLabels = false) const;
    QString selectSql(const QList<FilterCondition> &filters, bool useLabels = false) const;
    QString selectSql(const QList<FilterCondition> &filters, const QString &sortColumn, const Qt::SortOrder sortOrder, bool useLabels = false) const;
    QString updateSql(const QVariantMap &data) const;
    QString updateInsertSql(const QVariantMap &data, const QStringList matchColumns) const;

private:
    // Constraints
    std::shared_ptr<EnumConstraint> enumConstraint(const QString &columnName) const;

    // Utlity methods
    QString constraintClause(const QString policy, const ReferentialAction constraint) const;
    QString enumClause(const QString &columnName, const EnumConstraint &constraint) const;
    QString formatValue(const QVariant &value, ColumnType type) const;
    bool isEnumConstraint(const ColumnDefinition &col) const;
    QString operatorToSql(FilterOperator op) const;
    QString whereClause(const QList<FilterCondition> &conditions) const;

    QString m_tableName;                            // Table name
    QList<ColumnDefinition> m_columns;              // Column properties
    QList<ForeignKey> m_foreignKeys;                // Foreign keys
};

#endif // TABLESCHEMA_H
