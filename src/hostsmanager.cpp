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

#include "hostsmanager.h"

#include <QDebug>
#include <QEventLoop>

#include "pingaction.h"

HostsManager::HostsManager(QObject *parent) :
    QObject(parent),
    db(0)
{
    db = new DBManager;
}

HostsManager::~HostsManager()
{
    delete db;
}

void HostsManager::clearHistory()
{
    db->clearHistory();
}

void HostsManager::ping(const QString &host, const bool ipv6)
{
    PingAction ping;
    ping.ping(host, ipv6);

    QEventLoop loop;
    connect(&ping, SIGNAL(result(QString,int)), this, SIGNAL(pingResult(QString,int)));
    connect(&ping, SIGNAL(result(QString,int)), db, SLOT(insert(QString, int)));
    connect(&ping, SIGNAL(result(QString,int)), &loop, SLOT(quit()));

    // Wait for PingAction to finish
    loop.exec();
}

void HostsManager::pingLast()
{
    const QVariant last = db->lastHost();
    ping(last.toString(), false);
}

HostsSqlModel *HostsManager::recentHosts()
{
    return db->recentHosts();
}
