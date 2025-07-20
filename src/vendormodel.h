#ifndef VENDORMODEL_H
#define VENDORMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QAbstractTableModel>
#include <QtQml/qqmlregistration.h>

class VendorModel : public QAbstractTableModel {
    Q_OBJECT
    QML_ELEMENT                             // This makes the class available for use/instantiation on the QML side.
    QVector<QVector<QString>> m_vendors;    // List of vendor rows

public:
    explicit VendorModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    void loadFromDatabase(QSqlDatabase db);

    enum TableRoles {                       // Used to setup custom roles if the built-in roles are inadequate
        CellDataRole = Qt::UserRole + 1,
        RowRole,
        ColumnRole
    };

    Q_INVOKABLE QVariant get_display_data(const QModelIndex& index);

private:
    static const QStringList COLUMN_TITLES;
    static const QStringList COLUMN_NAMES;
};
#endif // VENDORMODEL_H
