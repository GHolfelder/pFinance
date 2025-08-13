#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject {
    Q_OBJECT
    QSqlDatabase m_db;                              // Database object
    QString m_error;                                // Last error encountered

public:
    explicit DatabaseManager(QObject *parent = nullptr);

    bool connect();
    bool initializeSchema();

    QSqlDatabase database() const;
    QString error() const;

signals:
    void operationSuccess(const QString &message);
    void operationFailed(const QString &error);

private:
    bool initializeVendor();

    bool fail(QString error);
    bool success(QString message);
};

#endif // DATABASEMANAGER_H
