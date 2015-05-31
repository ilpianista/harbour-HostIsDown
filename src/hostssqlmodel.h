#ifndef HOSTSSQLMODEL_H
#define HOSTSSQLMODEL_H

#include <QSqlQueryModel>

class HostsSqlModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit HostsSqlModel(QObject *parent = 0);

    void refresh();

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QHash<int,QByteArray> roleNames() const;

private:
    QHash<int, QByteArray> m_roleNames;

};

#endif // HOSTSSQLMODEL_H
