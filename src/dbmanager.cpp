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
#include <QSqlRecord>

const static QString DB_NAME = QStringLiteral("hostisdown");
const static int DB_VERSION = 1;

const static QString ALTER_TABLE_IPV6 = QStringLiteral("ALTER TABLE hosts ADD ipv6 SMALLINT;");
const static QString CLEAR_HISTORY = QStringLiteral("DELETE FROM hosts;");
const static QString CREATE_HOSTS_TABLE = QStringLiteral("CREATE TABLE IF NOT EXISTS hosts(host TEXT PRIMARY KEY, status SMALLINT, timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, ipv6 SMALLINT);");
const static QString DELETE_HOST = QStringLiteral("DELETE FROM hosts WHERE host=\"%1\";");
const static QString INSERT_INTO_HOSTS = QStringLiteral("INSERT INTO hosts(host, status, ipv6) VALUES(\"%1\", %2, %3);");
const static QString READ_DB_VERSION = QStringLiteral("PRAGMA user_version;");
const static QString UPDATE_DB_VERSION = QStringLiteral("PRAGMA user_version=%1;");

DBManager::DBManager(QObject *parent) :
    QObject(parent)
{
    db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"));

    const QString dbPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    db.setDatabaseName(dbPath + QDir::separator() + DB_NAME + QStringLiteral(".sql"));

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
    db.close();
}

void DBManager::clearHistory()
{
    QSqlQuery clearHistory(db);
    if (!clearHistory.exec(CLEAR_HISTORY)) {
        qCritical("Cannot clear history");
    }
}

void DBManager::deleteHost(const QString &host)
{
    QSqlQuery query(db);
    if (!query.exec(DELETE_HOST.arg(host))) {
        qCritical("Cannot delete data!");
    }
}

void DBManager::init()
{
    QSqlQuery query(db);

    if (!query.exec(CREATE_HOSTS_TABLE)) {
        qCritical("Cannot create table!");
        return;
    }

    // This is wrong. The first version did not set any DB version and now,
    // at the first start, we don't know if the table has been altered yet.
    // Then, alter it anyway and hope for the best everything goes well.
    const int dbVersion = readDBVersion();
    if (dbVersion == 0) {
        if (!query.exec(ALTER_TABLE_IPV6)) {
            qCritical("Cannot alter table for IPv6!");
        }
    }

    if (dbVersion != DB_VERSION) {
        if (!query.exec(UPDATE_DB_VERSION.arg(DB_VERSION))) {
            qCritical("Cannot update DB version!");
        }
    }
}

int DBManager::readDBVersion() const {
    QSqlQuery query(db);
    if (!query.exec(READ_DB_VERSION)) {
        qCritical("Cannot determine DB version!");
        return 0;
    }

    query.next();

    return query.record().value(0).toInt();
}

void DBManager::insert(const QString &host, const int status, const bool ipv6)
{
    deleteHost(host);

    int useIpv6 = 0;
    if (ipv6) {
        useIpv6 = 1;
    }

    QSqlQuery query(db);
    if (!query.exec(INSERT_INTO_HOSTS.arg(host).arg(status).arg(useIpv6))) {
        qCritical("Cannot save data!");
    }
}
