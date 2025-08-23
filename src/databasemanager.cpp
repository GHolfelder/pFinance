#include "databasemanager.h"
#include "base/tableschema.h"
#include <QSettings>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

/**
 * @brief Constructor
 *
 * @param parent Reference to parent class.
 */
DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent) {
    setObjectName("DatabaseManager");
}

/**
 * @brief Connect to database
 *
 * Retrieve database connection properties from "config.ini". Establish
 * a connection and return true if successful.
 *
 * @return True if successful, otherwise false
 */
bool DatabaseManager::connect() {
    QSettings settings("config.ini", QSettings::IniFormat);

    // Retrieve settings
    settings.beginGroup("Database");

    QString host     = settings.value("host").toString();
    int     port     = settings.value("port").toInt();
    QString dbname   = settings.value("dbname").toString();
    QString username = settings.value("username").toString();
    QString password = settings.value("password").toString();

    settings.endGroup();

    // Add a new database connection
    m_db = QSqlDatabase::addDatabase("QPSQL");
    m_db.setHostName(host);
    m_db.setPort(port);
    m_db.setDatabaseName(dbname);
    m_db.setUserName(username);
    m_db.setPassword(password);

    // Open connection
    if (!m_db.open())
        return fail("Connection failed: " + m_db.lastError().text());

    return success("Connection successful");
}

/**
 * @brief Initialize database schema
 *
 * The database is initialized as follows:
 * 1. Add pgcrypto extension to database. This provides a suite of cryptographic
 *    functions that enhance the security of data stored in the database.
 * 2. Initialize all the database tables
 *
 * @return True if successful, otherwise false
 */
bool DatabaseManager::initializeSchema(DatabaseTables *schemas) {
    QSqlQuery query;

    // Enable extension
    if (!query.exec("CREATE EXTENSION IF NOT EXISTS pgcrypto"))
        return fail("Extension init failed: " + query.lastError().text());

    // Initialize tables
    const QVector<TableSchema *> tables = schemas->getTableSchemasVector();
    for (const TableSchema *table : tables) {
        QSqlQuery query;
        const QString tableName(table->tableName());
        const QString sql(table->createTableSql());
        qInfo() << "Creating" << tableName;
        if (!query.exec(sql))
            return fail(tableName + " table create failed: " + query.lastError().text());

    }

    // Finish up
    return success("Database schema initialized.");
}

/**
 * @brief Get the internal QSqlDatabase object stored in the private member m_db.
 *
 * @return QSqlDatabase object
 */
QSqlDatabase DatabaseManager::database() const {
    return m_db;
}

/**
 * @brief Get text of last error generated
 *
 * @return QSqlDatabase object
 */
QString DatabaseManager::error() const {
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
bool DatabaseManager::fail(QString error) {
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
bool DatabaseManager::success(QString message) {
    m_error = "";
    qInfo() << message;
    emit operationSuccess(message);
    return true;
}

