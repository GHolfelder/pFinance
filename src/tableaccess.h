#ifndef TABLEACCESS_H
#define TABLEACCESS_H

#include <QObject>
#include <QSqlDatabase>
#include <QVariantMap>
#include "base/tableschema.h"

class TableAccess : public QObject
{
    Q_OBJECT
public:
    explicit TableAccess(QSqlDatabase db, TableSchema *table, QObject *parent = nullptr);

    Q_INVOKABLE int count();
    Q_INVOKABLE bool add(const QVariantMap &data);
    Q_INVOKABLE QVariantMap get(const QString &id);
    Q_INVOKABLE bool update(const QString &id, const QVariantMap &data);
    Q_INVOKABLE bool remove(const QString &id);

    QString error() const;

signals:
    void operationSuccess(const QString &message);
    void operationFailed(const QString &error);

protected:
    bool fail(QString error);
    bool success(QString message, QString id);

    QSqlDatabase m_db;                              // Database object
    TableSchema *m_table;                             // Table schema
    QString m_error;                                // Text of last error encountered
};

#endif // TABLEACCESS_H
