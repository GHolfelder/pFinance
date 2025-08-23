#ifndef TABLESCHEMA_H
#define TABLESCHEMA_H

#include <QObject>
#include <QVariantMap>

enum class COLUMNTYPE {                             // Logical column data types
    STRING,
    INT,
    DATE,
    CURRENCY,
    FLOAT
};

enum class FILTEROPERATOR {                         // Where filter operators
    EQUALS,
    NOTEQUALS,
    LESSTHAN,
    GREATERTHAN,
    LESSTHANOREQUAL,
    GREATERTHANOREQUAL,
    LIKE,
    IN,
    ISNULL,
    ISNOTNULL
};

struct FilterCondition {                            // Filter condition structure
    QString columnName;
    FILTEROPERATOR op;
    QVariant value;                                 // Optional, depending on op
};

struct ColumnDefinition {
    QString name;                                   // Internal name
    QString title;                                  // Display title
    COLUMNTYPE type;                                // Logical data type (e.g. COLUMNTYPE:STRING, COLUMNTYPE:INT)
    QString sqlType;                                // PostgreSQL type (e.g. "UUID", "VARCHAR(100)", "INTEGER")
    bool isPrimaryKey = false;                      // True if this is the primary key
    bool isAutoIncrement = false;                   // True if this is an auto increment value
    bool isNullable = true;                         // True if column is nullable
    QString defaultValue;                           // Optional default value for column
    std::function<QString(QVariant)> render;        // Optional formatter for query or display
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

    int columnCount() const;
    QStringList columnNames(bool includePrimary = true) const;
    QStringList columnPlaceholders(bool includePrimary = true) const;
    QStringList columnTitles(bool includePrimary = true) const;
    QStringList columnTypes(bool includePrimary = true) const;
    QString defaultSort() const;
    QString primaryKey(bool placeholder = false) const;
    QString toName(const QString placeholder) const;

    QString countSql() const;
    QString createTableSql() const;
    QString deleteSql() const;
    QString insertSql(const QVariantMap &data) const;
    QString selectSql() const;
    QString selectSql(const QList<FilterCondition> &filters) const;
    QString selectSql(const QList<FilterCondition> &filters, const QString sortColumn, const Qt::SortOrder sortOrder) const;
    QString updateSql(const QVariantMap &data) const;
    QString updateInsertSql(const QVariantMap &data, const QStringList matchColumns) const;

private:
    QString formatValue(const QVariant &value, COLUMNTYPE type) const;
    QString operatorToSql(FILTEROPERATOR op) const;
    QString whereClause(const QList<FilterCondition> &conditions) const;

    QString m_tableName;                            // Table name
    QList<ColumnDefinition> m_columns;              // Column properties
};

#endif // TABLESCHEMA_H
