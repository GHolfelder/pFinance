#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DatabaseManager : public QObject {
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = nullptr);

    bool connect();
    bool initializeSchema();

    QSqlDatabase database() const;

signals:
    void initializationSuccess(const QString &message);
    void initializationFailed(const QString &error);

private:
    bool initializeVendor();

    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
