#include "accessbase.h"

/**
 * @brief Table access base constructor
 *
 * @param db Database object where connection has been made
 * @param parent Reference to parent class.
 */
AccessBase::AccessBase(QSqlDatabase db, QObject *parent) : QObject(parent), m_db(db) {
    setObjectName("AccessBase");
}

/**
 * @brief Get text of last error generated
 *
 * @return QSqlDatabase object
 */
QString AccessBase::error() const {
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
 * @return false
 */
bool AccessBase::fail(QString error) {
    m_error = error;
    qDebug() << error;
    emit operationFailed(m_error);
    return false;
}

/**
 * @brief Successgful operation return
 *
 * This clears the current error message and logs
 * the informational message.
 *
 * A true is always returned.
 *
 * @param error Message to be emitted with successful operation
 * @return true
 */
bool AccessBase::success(QString message) {
    m_error = "";
    qInfo() << message;
    emit operationSuccess(message);
    return true;
}
