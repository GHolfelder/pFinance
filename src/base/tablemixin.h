#ifndef TABLEMIXIN_H
#define TABLEMIXIN_H

#include <QSqlDatabase>
#include "base/tableschema.h"

template <typename Derived>
class TableMixin {
public:
    TableMixin(QSqlDatabase db, TableSchema *table) : m_db(db), m_table(table) {}

    void setQuiet(bool quiet = true) { m_quiet = quiet; }
    QString error() const { return m_error; }

protected:
    bool fail(const QString &error) {
        m_error = m_table->tableName() + " " + error;
        qDebug() << m_error;
        if (!m_quiet) static_cast<Derived*>(this)->emitFailed(error);
        return false;
    }

    bool success(const QString &message, const QString &id) {
        m_error = "";
        qInfo() << m_table->tableName() + " " + message + " " + id;
        if (!m_quiet) static_cast<Derived*>(this)->emitSuccess(message, id);
        return true;
    }

    QSqlDatabase m_db;                              // Database object where tables are located
    TableSchema *m_table;                           // Table being managed
    QString m_error;                                // Last error encountered
    bool m_quiet;                                   // True if signals are to be disabled
};

#endif // TABLEMIXIN_H
