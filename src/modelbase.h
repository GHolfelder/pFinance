#ifndef MODELBASE_H
#define MODELBASE_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QAbstractTableModel>

class ModelBase : public QAbstractTableModel {
    Q_OBJECT
    Qt::SortOrder m_sortOrder;                      // Current sort order (Ascending / Descending)
    QString m_sortColumn;                           // Current sort column

    // Properties to be made available to the interface
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder NOTIFY sortOrderChanged)
    Q_PROPERTY(QString sortColumn READ sortColumn NOTIFY sortColumnChanged)

public:
    explicit ModelBase(QSqlDatabase db, const QStringList &columnNames, const QStringList &columnTitles, QObject *parent = nullptr);
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    Qt::SortOrder sortOrder();
    QString sortColumn();

signals:
    void sortOrderChanged();
    void sortColumnChanged();

protected:
    void setSort(QString sortColumn);

    QSqlDatabase m_db;                              // Database object
    QVector<QVector<QString>> m_data;               // Vector of vectors of data to be displayed
    QStringList m_columnNames;                      // List of column names held by class
    QStringList m_columnTitles;                     // List of column titles held by class
};

#endif // MODELBASE_H
