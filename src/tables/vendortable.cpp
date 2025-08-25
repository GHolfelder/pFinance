#include "vendortable.h"

/**
 * @brief Vendor table schema constructor
 *
 * @param tableName Name of table for schema
 * @param parent Reference to parent class.
 */
VendorTable::VendorTable(QObject *parent) : TableSchema ("Vendors", parent) {
    //         column,              title,                  data type,              sql type,           PKey,   Incr    Null,   default
    addColumn({"id",                tr("id"),               ColumnType::String,     "UUID",             true,   false,  false,  "gen_random_uuid()"});
    addColumn({"name",              tr("Name"),             ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"address1",          tr("Address 1"),        ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"address2",          tr("Address 2"),        ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"city",              tr("City"),             ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"state",             tr("State"),            ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"postal_code",       tr("Post code"),        ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"phone",             tr("Phone\nnumber"),    ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"unpaid_balance",    tr("Unpaid\nbalance"),  ColumnType::Currency,   "DECIMAL(12,2)",    false,  false,  false,  "0"});
}
