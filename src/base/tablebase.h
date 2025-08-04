#ifndef TABLEBASE_H
#define TABLEBASE_H

#include <QObject>
#include <QSqlDatabase>
#include "base/tableschema.h"

class TableBase : public QObject
{
    Q_OBJECT
public:
    explicit TableBase(QSqlDatabase db, TableSchema *table, QObject *parent = nullptr);

    QString error() const;

signals:
    void operationSuccess(const QString &message);
    void operationFailed(const QString &error);

protected:
    bool fail(QString error);
    bool success(QString message, QString id);

    QSqlDatabase m_db;                              // Database object
    TableSchema *m_table;                           // Table schema
    QString m_error;                                // Text of last error encountered
};

#endif // TABLEBASE_H
