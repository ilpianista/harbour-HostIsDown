/*
  Copyright (C) 2015 Andrea Scarpino <me@andreascarpino.it>
  All rights reserved.

  You may use this file under the terms of BSD license as follows:

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Jolla Ltd nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

void DBManager::insert(const QString &host, int status)
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
