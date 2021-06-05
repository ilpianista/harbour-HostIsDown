/*
  The MIT License (MIT)

  Copyright (c) 2014-2021 Andrea Scarpino <andrea@scarpino.dev>

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

#include "pingaction.h"

#include <QDebug>
#include <QUrl>

PingAction::PingAction(QObject *parent) :
    QObject(parent),
    m_process(new QProcess(this)), m_host(QString()), m_ipv6(false)
{
    connect(m_process, (void (QProcess::*)(int))&QProcess::finished, this, &PingAction::pingResult);
}

PingAction::~PingAction()
{
    if (m_process->state() == QProcess::Running) {
        m_process->kill();
    }

    delete m_process;
}

void PingAction::ping(const QString &host, const bool ipv6)
{
    const QUrl url(host);
    if (url.isValid()) {
        m_host = host;
        m_ipv6 = ipv6;

        QStringList parameters;
        parameters << QStringLiteral("-c");
        parameters << QStringLiteral("1");
        parameters << host;
        if (ipv6) {
            qDebug() << "Pinging" << host << "using IPv6";
            m_process->start(QStringLiteral("/bin/ping6"), parameters);
        } else {
            qDebug() << "Pinging" << host << "using IPv4";
            m_process->start(QStringLiteral("/bin/ping"), parameters);
        }
    } else {
        qDebug() << "Not a valid URL:" << host;
    }
}

void PingAction::pingResult(const int exitCode)
{
    Q_EMIT result(m_host, exitCode, m_ipv6);
}
