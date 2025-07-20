#ifndef VENDORMODEL_H
#define VENDORMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QAbstractTableModel>
#include <QtQml/qqmlregistration.h>

class VendorModel : public QAbstractTableModel {
    Q_OBJECT
    QML_ELEMENT                                     // This makes the class available for use/instantiation on the QML side.
    QVector<QVector<QString>> m_vendors;            // List of vendor rows
    Qt::SortOrder m_sortOrder;                      // Current sort order (Ascending / Descending)
    QString m_sortColumn;                           // Current sort column
    QSqlDatabase m_db;                              // Database object

    // Properties to be made available to the interface
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder NOTIFY sortOrderChanged)
    Q_PROPERTY(QString sortColumn READ sortColumn NOTIFY sortColumnChanged)

public:
    explicit VendorModel(QSqlDatabase db, QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::SortOrder sortOrder();
    QString sortColumn();

    enum TableRoles {                               // Used to setup custom roles if the built-in roles are inadequate
        CellDataRole = Qt::UserRole + 1,
        CellNameRole,
        RowRole,
        ColumnRole
    };

    Q_INVOKABLE void sortBy(const QString columnName);
    Q_INVOKABLE QVariant get_display_data(const QModelIndex& index);

signals:
    void sortOrderChanged();
    void sortColumnChanged();

private:
    static const QStringList COLUMN_TITLES;
    static const QStringList COLUMN_NAMES;
};
#endif // VENDORMODEL_H
