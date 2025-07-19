#ifndef ACCESSBASE_H
#define ACCESSBASE_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>

class AccessBase : public QObject {
    Q_OBJECT

public:
    explicit AccessBase(QSqlDatabase db, QObject *parent = nullptr);
    QString error() const;

signals:
    void operationSuccess(const QString &message);
    void operationFailed(const QString &error);

protected:
    bool fail(QString error);
    bool success(QString message);

    QSqlDatabase m_db;              // Database object
    QString m_error;                // Last error encountered
};

#endif // ACCESSBASE_H
