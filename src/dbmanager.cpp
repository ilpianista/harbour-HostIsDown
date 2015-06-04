/*
  The MIT License (MIT)

  Copyright (c) 2015 Andrea Scarpino <me@andreascarpino.it>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include "dbmanager.h"

#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QSqlQuery>

const static QString DB_NAME = "hostisdown";

const static QString CLEAR_HISTORY = "DELETE FROM hosts;";
const static QString CREATE_HOSTS_TABLE = "CREATE TABLE IF NOT EXISTS hosts(host TEXT PRIMARY KEY, status SMALLINT, timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);";
const static QString DELETE_HOST = "DELETE FROM hosts WHERE host=\"%1\";";
const static QString INSERT_INTO_HOSTS = "INSERT INTO hosts(host, status) VALUES(\"%1\", %2);";

DBManager::DBManager(QObject *parent) :
    QObject(parent),
    m_model(0)
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    const QString dbPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    db.setDatabaseName(dbPath + QDir::separator() + DB_NAME + ".sql");

    const QDir dir;
    if (!dir.exists(dbPath)) {
        if (!dir.mkpath(dbPath)) {
            qCritical("Cannot create data folder!");
        }
    }

    if (!db.open()) {
        qCritical("Unable to open database!");
    } else {
        init();
    }
}

DBManager::~DBManager()
{
    delete m_model;
    db.close();
}

void DBManager::clearHistory()
{
    QSqlQuery clearHistory(db);
    if (!clearHistory.exec(CLEAR_HISTORY)) {
        qCritical("Cannot clear history");
    }
    if (m_model) {
        m_model->refresh();
    }
}

const QVariant DBManager::lastHost()
{
    return m_model->data(m_model->index(0, 0));
}

void DBManager::init()
{
    QSqlQuery createTable(db);
    if (!createTable.exec(CREATE_HOSTS_TABLE)) {
        qCritical("Cannot create table!");
    }
}

HostsSqlModel* DBManager::recentHosts()
{
    m_model = new HostsSqlModel(this);

    return m_model;
}

void DBManager::insert(const QString &host, const int status)
{
    QSqlQuery query(db);
    if (!query.exec(DELETE_HOST.arg(host))) {
        qCritical("Cannot delete data!");
    }
    if (!query.exec(INSERT_INTO_HOSTS.arg(host).arg(status))) {
        qCritical("Cannot save data!");
    } else {
        m_model->refresh();
    }
}
