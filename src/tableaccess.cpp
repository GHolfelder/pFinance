#include "tableaccess.h"
#include <qsqlquery.h>
#include <QSqlError>
#include <QRandomGenerator>
#include <QUuid>
#include <QDebug>

/**
 * @brief Table access constructor
 *
 * This method sets the name of the object to tablename + TableAccess for easier
 * diagnosis of any errors encountered.
 *
 * @param db Database object where tables are located
 * @param table Table schema to be used for all access
 * @param parent Reference to parent class.
 */
TableAccess::TableAccess(QSqlDatabase db, TableSchema *table, QObject *parent) : QObject(parent), TableMixin<TableAccess>(db, table) {
    setObjectName(m_table->tableName() + "TableAccess");
}

/**
 * @brief Count number of records in table
 *
 * @returns Number of records in table, -1 if an error occurred
 */
int TableAccess::count() {
    QSqlQuery query(m_db);

    if (!query.exec(m_table->countSql())) {
        fail("count(*) failed: " + query.lastError().text());
        return -1;
    }
    if (query.next()) {
        success("count(*) successful", query.value(0).toString());
        return query.value(0).toInt();
    }
    fail("count(*) failed to retrieve count");
    return -1;
}

/**
 * @brief Add new row to database
 *
 * @param data Variant map containing fields and their associated value
 * @returns True if successful, otherwise false
 */
bool TableAccess::add(const QVariantMap &data) {
    QStringList const placeholders = m_table->columnPlaceholders();
    QString const pKey = m_table->primaryKey(true);
    QSqlQuery query(m_db);
    QString guid = QUuid::createUuid().toString(QUuid::WithoutBraces);

    // Prepare insert
    query.prepare(m_table->insertSql(data));

    // Bind column values.
    for (const QString &placeholder : placeholders) {
        const QString name = m_table->toName(placeholder);
        if (placeholder == pKey)
            query.bindValue(placeholder, guid);
        else if (data.contains(name))
            query.bindValue(placeholder, data[name]);
    }

    // Add row to table
    if (!query.exec())
        return fail("add failed: " + query.lastError().text());

    return success("added ID:", guid);
}

/**
 * @brief Retrieve row from database
 *
 * The caller is responsible for clearing the variant map if desired. When an empty
 * string is passed as the id, a default record will be returned.
 *
 * @param id Id of row to be retrieved from table.
 * @param result Variant map where fields and their associated value will be returned
 * @return Variant map containing retrieved data
 */
QVariantMap TableAccess::get(const QString &id) {
    // Initialize empty record when no id is provided
    if (id == "") {
        success("get ID:", id);
        return m_table->initialize();
    }

    // Variables needed for getting value from database
    QString const sql = m_table->selectSql();
    QString const pKey = m_table->primaryKey();
    QStringList const columns = m_table->columnNames();
    QSqlQuery query(m_db);
    QVariantMap result;

    // Retrieve record from database
    query.prepare(sql);
    if (pKey != "")
        query.bindValue(m_table->primaryKey(true), id);
    if (!query.exec()) {
        fail("get failed: " + query.lastError().text());
        return result;
    } else if (!query.next()) {
        fail("id not found: " + id);
        return result;
    }

    // Save results into variant map
    for (const QString& name : columns) {
        result[name] = query.value(name);
    }

    success("get ID:", id);
    return result;
}

/**
 * @brief Update row in database
 *
 * @param id Id of row to be updated in table.
 * @param data Variant map containing fields and their associated value
 * @returns True if successful, otherwise false
 */
bool TableAccess::update(const QString &id, const QVariantMap &data) {
    QString const sql = m_table->updateSql(data);
    QString const pKey = m_table->primaryKey(true);
    QStringList const placeholders = m_table->columnPlaceholders();
    QSqlQuery query(m_db);

    // Prepare query to do update
    query.prepare(sql);
    for (const QString& placeholder : placeholders) {
        const QString column = m_table->toName(placeholder);

        if (placeholder == pKey)
            query.bindValue(placeholder, id);
        else if (data.contains(column))
            query.bindValue(placeholder, data[column]);
    }

    // Update record in database
    if (!query.exec())
        return fail("update failed: " + query.lastError().text());

    return success("updated ID:", id);
}

/**
 * @brief Remove row from database
 *
 * @param id Id of row to be removed from table.
 * @returns True if successful, otherwise false
 */
bool TableAccess::remove(const QString &id) {
    QString const sql = m_table->deleteSql();
    QString const pKey = m_table->primaryKey(true);
    QSqlQuery query(m_db);

    // Prepare query to do delete
    query.prepare(sql);
    query.bindValue(pKey, id);

    // Delete record in database
    if (!query.exec())
        return fail("delete failed: " + query.lastError().text());

    return success("deleted ID:", id);
}
