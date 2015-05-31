#include "hostssqlmodel.h"

const static char* COLUMN_NAMES[] = {
    "host",
    "status",
    "timestamp",
    NULL
};
const static QString SQL_SELECT = "SELECT * FROM hosts ORDER BY timestamp DESC;";

HostsSqlModel::HostsSqlModel(QObject *parent):
    QSqlQueryModel(parent)
{
    int idx = 0;
    while (COLUMN_NAMES[idx]) {
        m_roleNames[Qt::UserRole + idx + 1] = COLUMN_NAMES[idx];
        idx++;
    }

    refresh();
}

void HostsSqlModel::refresh()
{
    setQuery(SQL_SELECT);
}

QVariant HostsSqlModel::data(const QModelIndex &index, int role) const
{
    QVariant value;
    if (role < Qt::UserRole) {
        value = QSqlQueryModel::data(index, role);
    } else {
        const int columnIdx = role - Qt::UserRole - 1;
        const QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
    }

    return value;
}

QHash<int, QByteArray> HostsSqlModel::roleNames() const
{
    return m_roleNames;
}
