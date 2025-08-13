#include "databasetables.h"
#include "tables/vendortable.h"
#include "tables/statetable.h"

/**
 * @brief Data registry constructor
 *
 * This creates an object for each table in the database
 *
 * @param db Database object where tables are located
 * @param parent Reference to parent class.
 */
DatabaseTables::DatabaseTables(QObject *parent) : QObject{parent} {
    VendorTable *vendor = new VendorTable(this);
    m_tables.insert(vendor->tableName(), vendor);
    StateTable *state = new StateTable(this);
    m_tables.insert(state->tableName(), state);
}

TableSchema* DatabaseTables::fetch(const QString &tableName) const {
    auto it = m_tables.find(tableName);
    return (it != m_tables.end()) ? it.value() : nullptr;
}
