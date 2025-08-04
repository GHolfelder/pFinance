#ifndef TABLEACCESS_H
#define TABLEACCESS_H

#include <QObject>
#include <QSqlDatabase>
#include <QVariantMap>
#include <QtQml/qqmlregistration.h>
#include "base/tablemixin.h"

class TableAccess : public QObject, public TableMixin<TableAccess>
{
    Q_OBJECT
    QML_ELEMENT                                     // This makes the class available for use/instantiation on the QML side.

public:
    explicit TableAccess(QSqlDatabase db, TableSchema *table, QObject *parent = nullptr);

    Q_INVOKABLE int count();
    Q_INVOKABLE bool add(const QVariantMap &data);
    Q_INVOKABLE QVariantMap get(const QString &id);
    Q_INVOKABLE bool update(const QString &id, const QVariantMap &data);
    Q_INVOKABLE bool remove(const QString &id);

    // Expose signal emitters for the mixin
    void emitSuccess(const QString &message, const QString &id) {
        emit operationSuccess(message, id);
    }
    void emitFailed(const QString &error) {
        emit operationFailed(error);
    }

signals:
    void operationSuccess(const QString &message, const QString &id);
    void operationFailed(const QString &error);

};

#endif // TABLEACCESS_H
