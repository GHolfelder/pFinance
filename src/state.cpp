#include "state.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QRandomGenerator>
#include <QUuid>

/**
 * @brief State save and restore constructor
 *
 * Note: All property updates are silent
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

/**
 * @brief Restore sort oprder property
 *
 * @param propertyName Name of property to be restored
 * @param defaultValue Default value for property if not yet set
 * @returns Property value or default value
 */
Qt::SortOrder State::restoreSortOrder(const QString propertyName, const Qt::SortOrder defaultValue) {
    const QString propertyValue = restoreValue(propertyName);
    if (propertyValue.isEmpty()) return defaultValue;
    return static_cast<Qt::SortOrder>(propertyValue.toInt());
}

/**
 * @brief Restore string property
 *
 * @param propertyName Name of property to be restored
 * @param defaultValue Default value for property if not yet set
 * @returns Property value or default value
 */
QString State::restoreString(const QString propertyName, const QString defaultValue) {
    const QString propertyValue = restoreValue(propertyName);
    if (propertyValue.isEmpty()) return defaultValue;
    return propertyValue;
}

/**
 * @brief Restore string list property
 *
 * @param propertyName Name of property to be restored
 * @param defaultValue Default value for property if not yet set
 * @returns Property value or default value
 */
QStringList State::restoreStringList(const QString propertyName, const QStringList defaultValue) {
    const QString propertyValue = restoreValue(propertyName);
    if (propertyValue.isEmpty()) return defaultValue;
    return propertyValue.split(',');
}

/**
 * @brief Save sort order property
 *
 * @param propertyName Name of property to be saved
 * @param propertyValue Value of property to be saved
 * @returns True if successful, otherwise false
 */
bool State::save(const QString propertyName, const Qt::SortOrder propertyValue) {
    return saveValue(propertyName, QString::number(static_cast<int>(propertyValue)));
}

/**
 * @brief Save string property
 *
 * @param propertyName Name of property to be saved
 * @param propertyValue Value of property to be saved
 * @returns True if successful, otherwise false
 */
bool State::save(const QString propertyName, const QString propertyValue) {
    return saveValue(propertyName, propertyValue);
}

/**
 * @brief Save string list property
 *
 * @param propertyName Name of property to be saved
 * @param propertyValue Value of property to be saved
 * @returns True if successful, otherwise false
 */
bool State::save(const QString propertyName, const QStringList propertyValue) {
    return saveValue(propertyName, propertyValue.join(","));
}

/**
 * @brief Retrieve property value from database
 *
 * @param propertyName Name of property
 * @returns Property value or empty string
 */
QString State::restoreValue(const QString propertyName) {
    QSqlQuery query(m_db);

    // Variables needed for getting value from database
    const QString sql = m_table->selectSql({
        { "sta_object",         FilterOperator::Equals, m_object},
        { "sta_property_name",  FilterOperator::Equals, propertyName}
    });

    // Retrieve record from database
    query.prepare(sql);
    if (!query.exec()) {
        fail("restore failed: " + query.lastError().text());
        return "";
    } else if (!query.next()) {
        return "";
    }
    return query.value("sta_property_value").toString();
}

/**
 * @brief Save property property value
 *
 * @param propertyName Name of property to be saved
 * @param propertyValue Value of property to be saved
 * @returns True if successful, otherwise false
 */
bool State::saveValue(const QString propertyName, const QString propertyValue) {
    QSqlQuery query(m_db);
    QVariantMap data;
    const QStringList matchOn = { "sta_object", "sta_property_name" };
    const QStringList placeholders = m_table->columnPlaceholders(true);

    // Prepare values to be updated / inserted
    data["sta_id"]              = QUuid::createUuid();
    data["sta_object"]          = m_object;
    data["sta_property_name"]   = propertyName;
    data["sta_property_value"]  = propertyValue;

    // Prepare query to do update / insert
    const QString sql = m_table->updateInsertSql(data, matchOn);
    query.prepare(sql);
    for (const QString& placeholder : placeholders) {
        const QString column = m_table->toAlias(placeholder);
        if (data.contains(column))
            query.bindValue(placeholder, data[column]);
    }

    // Update / insert record in database
    if (!query.exec())
        return fail("state update failed: " + query.lastError().text());

    return success("updated:", propertyName);
}
