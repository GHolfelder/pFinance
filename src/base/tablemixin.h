#ifndef TABLEMIXIN_H
#define TABLEMIXIN_H

#include <QSqlDatabase>
#include "base/tableschema.h"

template <typename Derived>
class TableMixin {
public:
    TableMixin(QSqlDatabase db, TableSchema *table) : m_db(db), m_table(table) {}

    QString error() const { return m_error; }

protected:
    bool fail(const QString &error) {
        m_error = m_table->tableName() + " " + error;
        qDebug() << m_error;
        static_cast<Derived*>(this)->emitFailed(error);
        return false;
    }

    bool success(const QString &message, const QString &id) {
        m_error = "";
        qInfo() << m_table->tableName() + " " + message + " " + id;
        static_cast<Derived*>(this)->emitSuccess(message, id);
        return true;
    }

    QSqlDatabase m_db;                              // Database object where tables are located
    TableSchema *m_table;                           // Table being managed
    QString m_error;                                // Last error encountered
};

#endif // TABLEMIXIN_H
