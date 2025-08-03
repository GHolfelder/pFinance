#ifndef TABLESCHEMA_H
#define TABLESCHEMA_H

#include <QObject>
#include <QVariantMap>

enum class COLUMNTYPE {
    STRING,
    INT,
    DATE,
    CURRENCY,
    FLOAT
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

    QStringList columnNames(bool includePrimary = true) const;
    QStringList columnPlaceholders(bool includePrimary = true) const;
    QStringList columnTitles(bool includePrimary = true) const;
    QString primaryKey(bool placeholder = false) const;
    QString toName(const QString placeholder) const;

    QString countSql() const;
    QString createTableSql() const;
    QString deleteSql() const;
    QString insertSql(const QVariantMap &data) const;
    QString selectSql() const;
    QString updateSql(const QVariantMap &data) const;

private:
    QString m_tableName;                            // Table name
    QList<ColumnDefinition> m_columns;              // Column properties
};

#endif // TABLESCHEMA_H
