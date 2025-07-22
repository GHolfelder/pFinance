#ifndef VENDORMODEL_H
#define VENDORMODEL_H

#include "modelbase.h"
#include <QtQml/qqmlregistration.h>

class VendorModel : public ModelBase {
    Q_OBJECT
    QML_ELEMENT                                     // This makes the class available for use/instantiation on the QML side.

public:
    explicit VendorModel(QSqlDatabase db, QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    enum TableRoles {                               // Used to setup custom roles if the built-in roles are inadequate
        CellDataRole = Qt::UserRole + 1,
        CellNameRole,
        RowRole,
        ColumnRole
    };

    Q_INVOKABLE void sortBy(const QString columnName);

private:
    static const QStringList COLUMN_TITLES;
    static const QStringList COLUMN_NAMES;
};
#endif // VENDORMODEL_H
