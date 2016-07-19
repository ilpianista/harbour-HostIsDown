/*
  The MIT License (MIT)

  Copyright (c) 2015-2016 Andrea Scarpino <me@andreascarpino.it>

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

#include <QCoreApplication>
#include <QDebug>
#include <QEventLoop>
#include <QProcess>
#include <QSettings>
#include <QSqlRecord>
#include <QTimer>

#include "dbmanager.h"
#include "pingaction.h"

HostsManager::HostsManager(QObject *parent) :
    QObject(parent)
  , m_db(new DBManager(this))
  , m_model(new HostsSqlModel(this))
  , m_timer(0)
{
    m_settings = new QSettings(QCoreApplication::applicationName(), QCoreApplication::applicationName(), this);
    setPoller(m_settings->value("Poller", 0).toUInt());

    if (m_timer) {
        connect(m_timer, &QTimer::timeout, this, &HostsManager::pingAll);
    }
}

HostsManager::~HostsManager()
{
    delete m_model;
    delete m_db;
    delete m_settings;
    delete m_timer;

    Q_FOREACH (QProcess *p, m_sshProcesses) {
        p->kill();
        delete p;
    }
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

void HostsManager::openSSH(const QString &host)
{
    const QProcessEnvironment env;
    const QString terminal = env.value(QLatin1Literal("TERMINAL"), QLatin1Literal("fingerterm"));

    QProcess *ssh = new QProcess();
    ssh->start(QStringLiteral("%1 -e \"ssh %2\"").arg(terminal).arg(host));

    m_sshProcesses.append(ssh);
}

void HostsManager::pingAll()
{
    QVariantMap pairs;
    for (int i = m_model->rowCount() - 1; i >= 0; i--) {
        const QSqlRecord host = m_model->record(i);
        pairs.insert(host.value("host").toString(), host.value("ipv6"));
    }

    Q_FOREACH (const QString host, pairs.keys()) {
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

void HostsManager::setPoller(const quint16 minutes)
{
    m_poller = minutes;
    m_settings->setValue("Poller", minutes);

    if (minutes > 0) {
        if (!m_timer) {
            m_timer = new QTimer();
        } else {
            m_timer->stop();
        }

        qDebug() << "Starting a new timer every" << minutes << "minutes";
        m_timer->start(minutes * 60 * 1000);
    } else {
        if (m_timer) {
            qDebug() << "Stopping the timer";
            m_timer->stop();
        }
    }
}

quint16 HostsManager::getPoller() const
{
    return m_poller;
}

HostsSqlModel *HostsManager::recentHosts()
{
    return m_model;
}

void HostsManager::slotResult(const QString &host, const int exitCode, const bool ipv6)
{
    m_db->insert(host, exitCode, ipv6);
    m_model->refresh();

    Q_EMIT pingResult(host, exitCode);
}
