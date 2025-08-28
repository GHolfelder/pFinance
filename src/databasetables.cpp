#include "databasetables.h"
#include "tables/categorytable.h"
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
    CategoryTable *category = new CategoryTable(this);
    m_tables.insert(category->tableName(), category);
    VendorTable *vendor = new VendorTable(this);
    m_tables.insert(vendor->tableName(), vendor);
    StateTable *state = new StateTable(this);
    m_tables.insert(state->tableName(), state);
}

/**
 * @brief Fetch requested table schema
 *
 * @param tableName Name of table to be fetched
 * @returns Nullptr or address of table schema object
 */
TableSchema* DatabaseTables::fetch(const QString &tableName) const {
    auto it = m_tables.find(tableName);
    return (it != m_tables.end()) ? it.value() : nullptr;
}

/**
 * @brief Return QVector of tables in the database
 *
 * @returns QVector of table schema pointers
 */
QVector<TableSchema *> DatabaseTables::getTableSchemasVector() const {
    QVector<TableSchema*> tableSchemas;

    tableSchemas.reserve(m_tables.size());

    for (auto it = m_tables.begin(); it != m_tables.end(); ++it) {
        tableSchemas.append(it.value());
    }

    return tableSchemas;
}
