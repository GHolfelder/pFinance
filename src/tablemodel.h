#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QObject>
#include <QSqlDatabase>
#include <QAbstractTableModel>
#include <QtQml/qqmlregistration.h>
#include "base/tablemixin.h"

enum TableRoles {                                   // Used to setup custom roles if the built-in roles are inadequate
    CellDataRole = Qt::UserRole + 1,
    CellNameRole,
    RowRole,
    ColumnRole,
    IdRole
};

class TableModel : public QAbstractTableModel, public TableMixin<TableModel>
{
    Q_OBJECT
    QML_ELEMENT                                     // This makes the class available for use/instantiation on the QML side.

    // Properties to be made available to the interface
    Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder NOTIFY sortOrderChanged)
    Q_PROPERTY(QString sortColumn READ sortColumn NOTIFY sortColumnChanged)
    Q_PROPERTY(QStringList columnNames READ columnNames CONSTANT)
    Q_PROPERTY(QStringList columnTitles READ columnTitles CONSTANT)
    Q_PROPERTY(QStringList visibleColumns READ visibleColumns WRITE setVisibleColumns NOTIFY visibleColumnsChanged)

public:
    explicit TableModel(QSqlDatabase db, TableSchema *table, QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    Qt::SortOrder sortOrder();
    QString sortColumn();
    QStringList columnNames() const;
    QStringList columnTitles() const;
    QStringList visibleColumns() const;

    Q_INVOKABLE QString defaultSort();
    Q_INVOKABLE void setVisibleColumns(const QStringList &columns);
    Q_INVOKABLE int sortBy(const QString sortColumn, const QString &id);
    Q_INVOKABLE int refresh(const QString &id);

    // Expose signal emitters for the mixin
    void emitSuccess(const QString &message, const QString &id) {
        emit operationSuccess(message, id);
    }
    void emitFailed(const QString &error) {
        emit operationFailed(error);
    }

signals:
    void sortOrderChanged();
    void sortColumnChanged();
    void visibleColumnsChanged();
    void operationSuccess(const QString &message, const QString &id);
    void operationFailed(const QString &error);

protected:
    int columnToIndex(const int column) const;

    Qt::SortOrder m_sortOrder;                      // Current sort order (Ascending / Descending)
    QString m_sortColumn;                           // Current sort column
    QStringList m_visibleColumns;                   // List of visible column names
    QVector<QVector<QString>> m_data;               // Vector of vectors of data to be displayed
};

#endif // TABLEMODEL_H
