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

#ifndef HOSTSMANAGER_H
#define HOSTSMANAGER_H

#include <QObject>

#include "hostssqlmodel.h"

class DBManager;
class QProcess;
class QSettings;
class QTimer;

class HostsManager : public QObject
{
    Q_OBJECT
public:
    explicit HostsManager(QObject *parent = 0);
    virtual ~HostsManager();

    Q_INVOKABLE void clearHistory();
    Q_INVOKABLE void forget(const QString &host);
    Q_INVOKABLE void openSSH(const QString &host);
    Q_INVOKABLE void ping(const QString &host, const bool ipv6);
    Q_INVOKABLE void pingAll();
    Q_INVOKABLE void pingLast();
    Q_INVOKABLE quint16 getPoller() const;
    Q_INVOKABLE void setPoller(const quint16 minutes);

    HostsSqlModel* recentHosts();

Q_SIGNALS:
    void pingResult(const QString &host, const int exitCode);

private:
    void slotResult(const QString &host, const int exitCode, const bool ipv6);

    DBManager *m_db;
    HostsSqlModel* m_model;
    QSettings* m_settings;
    QTimer* m_timer;
    quint16 m_poller;
    QList<QProcess *> m_sshProcesses;

};

#endif // HOSTSMANAGER_H
