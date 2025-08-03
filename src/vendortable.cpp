#include "vendortable.h"

/**
 * @brief Vendor table schema constructor
 *
 * @param tableName Name of table for schema
 * @param parent Reference to parent class.
 */
VendorTable::VendorTable(QObject *parent) : TableSchema ("Vendors", parent) {
    //         column,              title,                  data type,              sql type,           PKey,   Incr    Null,   default
    addColumn({"id",                tr("id"),               COLUMNTYPE::STRING,     "UUID",             true,   false,  false,  "gen_random_uuid()"});
    addColumn({"name",              tr("Name"),             COLUMNTYPE::STRING,     "TEXT",             false,  false,  false,  ""});
    addColumn({"address1",          tr("Address 1"),        COLUMNTYPE::STRING,     "TEXT",             false,  false,  false,  ""});
    addColumn({"address2",          tr("Address 2"),        COLUMNTYPE::STRING,     "TEXT",             false,  false,  false,  ""});
    addColumn({"city",              tr("City"),             COLUMNTYPE::STRING,     "TEXT",             false,  false,  false,  ""});
    addColumn({"state",             tr("State"),            COLUMNTYPE::STRING,     "TEXT",             false,  false,  false,  ""});
    addColumn({"postal_code",       tr("Post code"),        COLUMNTYPE::STRING,     "TEXT",             false,  false,  false,  ""});
    addColumn({"phone",             tr("Phone number"),     COLUMNTYPE::STRING,     "TEXT",             false,  false,  false,  ""});
    addColumn({"unpaid_balance",    tr("Unpaid\nBalance"),  COLUMNTYPE::CURRENCY,   "DECIMAL(12,2)",    false,  false,  false,  "0"});
}
