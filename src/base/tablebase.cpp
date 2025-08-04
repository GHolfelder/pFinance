#include "tablebase.h"

/**
 * @brief Table base constructor
 *
 * This method constructs the base class for all database operations classes.
 * This class provides common properties, success and failure signalling.
 *
 * @param db Database object where tables are located
 * @param table Table schema to be used for all access
 * @param parent Reference to parent class.
 */

TableBase::TableBase(QSqlDatabase db, TableSchema *table, QObject *parent) : QObject{parent} {
    m_db = db;
    m_table = table;
    setObjectName("TableBase");
}

/**
 * @brief Get text of last error generated
 *
 * @returns Error text
 */
QString TableBase::error() const {
    return m_error;
}

/**
 * @brief Failed operation return.
 *
 * This saves any error that has occurred, logs the error
 * for debug purposes and emits an operation failed signal.
 *
 * A false is always returned.
 *
 * @param error Message indicating error that has occurred
 * @returns false
 */
bool TableBase::fail(QString error) {
    m_error = m_table->tableName() + " " + error;
    qDebug() << m_error;
    emit operationFailed(m_error);
    return false;
}

/**
 * @brief Successful operation return
 *
 * This clears the current error text and logs
 * an informational message.
 *
 * A true is always returned.
 *
 * @param message Message to be logged with successful operation
 * @param id Id upon which operation was performed
 * @returns true
 */
bool TableBase::success(QString message, QString id) {
    m_error = "";
    qInfo() << m_table->tableName() + " " + message + " " + id;
    emit operationSuccess(id);
    return true;
}
