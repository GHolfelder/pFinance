#include "databasetables.h"
#include "tables/vendortable.h"

/**
 * @brief Data registry constructor
 *
 * This creates an object for each table in the database
 *
 * @param db Database object where tables are located
 * @param parent Reference to parent class.
 */
DatabaseTables::DatabaseTables(QSqlDatabase db, QObject *parent) : QObject{parent} {
    m_vendortable = new VendorTable(parent);
}

/**
 * @brief Vendor access getter
 *
 * @return Pointer to vendor access object
 */
VendorTable *DatabaseTables::vendortable() const {
    return m_vendortable;
}
