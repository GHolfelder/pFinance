#include "DatabaseManager.h"
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
    QString error;
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
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(host);
    db.setPort(port);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPassword(password);

    // Open connection
    if (!db.open()) {
        error = "Connection failed: " + db.lastError().text();
        emit initializationFailed(error);
        qCritical() << error;
        return false;
    }
    return true;
}

/**
 * @brief Initialize database schema
 *
 * The database is initialized as follows:
 * 1. Add pgcrypto extension to database. This provides a suite of cryptographic
 *    functions that enhance the security of data stored in the database.
 * 2. Initialize all the datyabase tables
 *
 * @return True if successful, otherwise false
 */
bool DatabaseManager::initializeSchema() {
    QSqlQuery query;
    QString error;

    // Enable extension
    if (!query.exec("CREATE EXTENSION IF NOT EXISTS pgcrypto")) {
        error = "Extension init failed: " + query.lastError().text();
        emit initializationFailed(error);
        qCritical() << error;
        return false;
    }

    // Vendors table
    if (!initializeVendor()) {
        return false;
    }

    // Finish up
    error = "Database schema initialized.";
    emit initializationSuccess(error);
    qDebug() << error;
    return true;
}

/**
 * @brief Initialize vendor table
 *
 * @return True if successful, otherwise false
 */
bool DatabaseManager::initializeVendor()
{
    QSqlQuery query;
    QString error;

    const QString vendorTable = R"(
        CREATE TABLE IF NOT EXISTS vendors (
            id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
            name TEXT NOT NULL,
            address1 TEXT,
            address2 TEXT,
            city TEXT,
            state TEXT,
            postal_code TEXT,
            phone TEXT,
            unpaid_balance NUMERIC(12, 2) DEFAULT 0.00
        )
    )";

    if (!query.exec(vendorTable)) {
        error = "Vendor table init failed: " + query.lastError().text();
        emit initializationFailed(error);
        qCritical() << error;
        return false;
    }

    return true;
}

/**
 * @brief Get the internal QSqlDatabase object stored in the private member db.
 *
 * @return QSqlDatabase object
 */
QSqlDatabase DatabaseManager::database() const {
    return db;
}
