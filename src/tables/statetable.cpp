#include "statetable.h"

/**
 * @brief State table schema constructor
 *
 * @param tableName Name of table for schema
 * @param parent Reference to parent class.
 */
StateTable::StateTable(QObject *parent) : TableSchema ("States", parent) {
    //         column,              title,                  data type,              sql type,           PKey,   Incr    Null,   default
    addColumn({"id",                tr("id"),               COLUMNTYPE::STRING,     "UUID",             true,   false,  false,  "gen_random_uuid()"});
    addColumn({"object",            tr("Object"),           COLUMNTYPE::STRING,     "TEXT",             false,  false,  false,  ""});
    addColumn({"property_name",     tr("Property\nName"),   COLUMNTYPE::STRING,     "TEXT",             false,  false,  false,  ""});
    addColumn({"property_value",    tr("Property\nValue"),  COLUMNTYPE::STRING,     "TEXT",             false,  false,  false,  ""});
}
