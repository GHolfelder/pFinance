#include "vendortable.h"

/**
 * @brief Vendor table schema constructor
 *
 * @param tableName Name of table for schema
 * @param parent Reference to parent class.
 */
VendorTable::VendorTable(QObject *parent) : TableSchema ("Vendors", "ven", parent) {
    //         column,              title,                  data type,              sql type,           PKey,   Incr    Null,   default
    addColumn({"id",                tr("id"),               ColumnType::String,     "UUID",             true,   false,  false,  "gen_random_uuid()"});
    addColumn({"category_id",       tr("category_id"),      ColumnType::String,     "UUID",             false,  false,  false,  ""});
    addColumn({"name",              tr("Name"),             ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"address1",          tr("Address 1"),        ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"address2",          tr("Address 2"),        ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"city",              tr("City"),             ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"state",             tr("State"),            ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"postal_code",       tr("Post code"),        ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"phone",             tr("Phone\nnumber"),    ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    // Foreign keys
    addForeignKey({"category_id",   "categories",       "id",   "cat",      ReferentialAction::Restrict,    ReferentialAction::Cascade});
}
