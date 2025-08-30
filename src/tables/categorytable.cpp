#include "categorytable.h"

/**
 * @brief Category table schema constructor
 *
 * @param tableName Name of table for schema
 * @param parent Reference to parent class.
 */
CategoryTable::CategoryTable(QObject *parent) : TableSchema ("Categories", parent) {
    TypeConstraint.valueMap = {
        { 0, tr("Income") },
        { 1, tr("Expense") }
    };

    //         column,              title,                  data type,              sql type,           PKey,   Incr    Null,   default
    addColumn({"id",                tr("id"),               ColumnType::String,     "UUID",             true,   false,  false,  "gen_random_uuid()"});
    addColumn({"name",              tr("Category"),         ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"description",       tr("Description"),      ColumnType::String,     "TEXT",             false,  false,  false,  ""});
    addColumn({"type",              tr("Type"),             ColumnType::Int,        "SMALLINT",         false,  false,  false,  "", nullptr, std::make_shared<EnumConstraint>(TypeConstraint)});
}
