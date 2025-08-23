#include "state.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>
#include <QUuid>

/**
 * @brief State save and restore constructor
 *
 * @param db Database object where tables are located
 * @param table Table schema to be used for all access
 * @param object Name of object whose properties are to be saved
 * @param parent Reference to parent class.
 */
State::State(QSqlDatabase db, DatabaseTables *tables, QString object, QObject *parent) : TableAccess(db, tables, "States", parent) {
    setObjectName(object + "State");
    m_object = object;
    m_quiet = true;
}

QString State::restore(const QString propertyName, const QString defaultValue) {
    QSqlQuery query(m_db);

    // Variables needed for getting value from database
    const QString sql = m_table->selectSql({
        { "object",         FILTEROPERATOR::EQUALS, m_object},
        { "property_name",  FILTEROPERATOR::EQUALS, propertyName}
    });

    // Retrieve record from database
    query.prepare(sql);
    if (!query.exec()) {
        fail("restore failed: " + query.lastError().text());
        return defaultValue;
    } else if (!query.next()) {
        return defaultValue;
    }
    return query.value("property_value").toString();
}

bool State::save(const QString propertyName, const QString propertyValue) {
    QSqlQuery query(m_db);
    QVariantMap data;
    const QStringList matchOn = { "object", "property_name" };
    const QStringList placeholders = m_table->columnPlaceholders();

    // Prepare values to be updated / inserted
    data["id"]              = QUuid::createUuid();
    data["object"]          = m_object;
    data["property_name"]   = propertyName;
    data["property_value"]  = propertyValue;

    // Prepare query to do update / insert
    const QString sql = m_table->updateInsertSql(data, matchOn);
    query.prepare(sql);
    for (const QString& placeholder : placeholders) {
        const QString column = m_table->toName(placeholder);
        if (data.contains(column))
            query.bindValue(placeholder, data[column]);
    }

    // Update / insert record in database
    if (!query.exec())
        return fail("state update failed: " + query.lastError().text());

    return success("updated:", propertyName);
}
