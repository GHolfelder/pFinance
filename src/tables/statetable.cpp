#include "statetable.h"

/**
 * @brief State table schema constructor
 *
 * @param tableName Name of table for schema
 * @param parent Reference to parent class.
 */
StateTable::StateTable(QObject *parent) : TableSchema ("States", parent) {
    //         column,              title,                  data type,              sql type,           PKey,   Incr    Null,   default
    addColumn({"id",                tr("id"),               ColumnType::String,     "UUID",             true,   false,  false,  "gen_random_uuid()"});
    addColumn({"object",            tr("Object"),           ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"property_name",     tr("Property\nName"),   ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"property_value",    tr("Property\nValue"),  ColumnType::String,     "TEXT",             false,  false,  false,  ""});
}
