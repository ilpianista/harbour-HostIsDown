/*
  Copyright (C) 2014 Andrea Scarpino <me@andreascarpino.it>
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
#include <QSqlQuery>

const static QString DB_NAME = "hostisdown";
const static QString TABLE_HOSTS = "hosts";

DBManager::DBManager(QObject *parent) :
    QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    const QString dbPath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    db.setDatabaseName(dbPath + QDir::separator() + DB_NAME + ".sql");

    const QDir dir;
    if (!dir.exists(dbPath))
    {
        if (!dir.mkpath(dbPath))
        {
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

void DBManager::init()
{
    QSqlQuery createTable(db);
    if (!createTable.exec("CREATE TABLE IF NOT EXISTS " + TABLE_HOSTS + "(host TEXT, status SMALLINT, timestamp DATETIME DEFAULT CURRENT_TIMESTAMP);"))
    {
        qCritical("Cannot create table!");
    }
}
