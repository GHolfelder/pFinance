#include "state.h"

State::State(QSqlDatabase db, DatabaseTables *tables, QObject *parent) : TableAccess(db, tables, "States", parent){

}
