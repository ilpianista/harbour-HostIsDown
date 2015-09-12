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
#include <QSqlRecord>

#include "pingaction.h"

HostsManager::HostsManager(QObject *parent) :
    QObject(parent),
    m_db(new DBManager(this)), m_model(new HostsSqlModel(this))
{
}

HostsManager::~HostsManager()
{
    delete m_model;
    delete m_db;
}

void HostsManager::clearHistory()
{
    m_db->clearHistory();
    m_model->refresh();
}

void HostsManager::forget(const QString &host)
{
    m_db->deleteHost(host);
    m_model->refresh();
}

void HostsManager::pingAll()
{
    QVariantMap pairs;
    for (int i = m_model->rowCount() - 1; i >= 0; i--) {
        const QSqlRecord host = m_model->record(i);
        pairs.insert(host.value("host").toString(), host.value("ipv6"));
    }

    Q_FOREACH (const QString host, pairs.keys()) {
        qDebug() << host << pairs.value(host).toBool();
        ping(host, pairs.value(host).toBool());
    }
}

void HostsManager::ping(const QString &host, const bool ipv6)
{
    PingAction ping;
    ping.ping(host, ipv6);

    QEventLoop loop;
    connect(&ping, &PingAction::result, this, &HostsManager::slotResult);
    connect(&ping, &PingAction::result, &loop, &QEventLoop::quit);

    // Wait for PingAction to finish
    loop.exec();
}

void HostsManager::pingLast()
{
    const QSqlRecord last = m_model->record(0);
    ping(last.value("host").toString(), last.value("ipv6").toBool());
}

HostsSqlModel *HostsManager::recentHosts()
{
    m_model = new HostsSqlModel(this);

    return m_model;
}

void HostsManager::slotResult(const QString &host, const int exitCode, const bool ipv6)
{
    m_db->insert(host, exitCode, ipv6);
    m_model->refresh();

    Q_EMIT pingResult(host, exitCode);
}
