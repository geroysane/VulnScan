#include "DnsResolver.h"
#include <QEventLoop>
#include <QTimer>
#include <QRegularExpression>

DnsResolver::DnsResolver(QObject *parent)
    : QObject(parent)
    , m_timeout(5000)
{
}

DnsResolver::~DnsResolver()
{
}

QList<QHostAddress> DnsResolver::resolveHost(const QString &hostname, int timeout)
{
    if (!isValidHostname(hostname)) {
        return QList<QHostAddress>();
    }

    // Check if it's already an IP address
    QHostAddress addr(hostname);
    if (!addr.isNull()) {
        return QList<QHostAddress>() << addr;
    }

    // Perform synchronous lookup with timeout
    QEventLoop loop;
    QTimer timer;
    QList<QHostAddress> result;
    bool completed = false;

    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    int lookupId = QHostInfo::lookupHost(hostname, [&](const QHostInfo &info) {
        completed = true;
        if (info.error() == QHostInfo::NoError) {
            result = info.addresses();
        }
        loop.quit();
    });

    timer.start(timeout);
    loop.exec();

    if (!completed) {
        QHostInfo::abortHostLookup(lookupId);
    }

    return result;
}

QString DnsResolver::reverseResolve(const QString &address, int timeout)
{
    if (!isValidIpAddress(address)) {
        return QString();
    }

    QHostAddress addr(address);
    if (addr.isNull()) {
        return QString();
    }

    // Perform synchronous reverse lookup with timeout
    QEventLoop loop;
    QTimer timer;
    QString result;
    bool completed = false;

    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    int lookupId = QHostInfo::lookupHost(address, [&](const QHostInfo &info) {
        completed = true;
        if (info.error() == QHostInfo::NoError && !info.hostName().isEmpty()) {
            result = info.hostName();
        }
        loop.quit();
    });

    timer.start(timeout);
    loop.exec();

    if (!completed) {
        QHostInfo::abortHostLookup(lookupId);
    }

    return result;
}

int DnsResolver::resolveHostAsync(const QString &hostname)
{
    if (!isValidHostname(hostname)) {
        return -1;
    }

    // Check if it's already an IP address
    QHostAddress addr(hostname);
    if (!addr.isNull()) {
        QList<QHostAddress> addresses;
        addresses << addr;
        emit hostResolved(-1, hostname, addresses);
        return -1;
    }

    int lookupId = QHostInfo::lookupHost(hostname, this, SLOT(handleLookupResult(QHostInfo)));

    if (lookupId != -1) {
        m_pendingLookups[lookupId] = hostname;
        m_isReverseLookup[lookupId] = false;
    }

    return lookupId;
}

int DnsResolver::reverseResolveAsync(const QString &address)
{
    if (!isValidIpAddress(address)) {
        return -1;
    }

    int lookupId = QHostInfo::lookupHost(address, this, SLOT(handleLookupResult(QHostInfo)));

    if (lookupId != -1) {
        m_pendingLookups[lookupId] = address;
        m_isReverseLookup[lookupId] = true;
    }

    return lookupId;
}

void DnsResolver::handleLookupResult(const QHostInfo &hostInfo)
{
    int lookupId = hostInfo.lookupId();

    if (!m_pendingLookups.contains(lookupId)) {
        return;
    }

    QString originalQuery = m_pendingLookups[lookupId];
    bool isReverse = m_isReverseLookup[lookupId];

    // Remove from pending
    m_pendingLookups.remove(lookupId);
    m_isReverseLookup.remove(lookupId);

    // Check for errors
    if (hostInfo.error() != QHostInfo::NoError) {
        emit resolutionFailed(lookupId, hostInfo.errorString());
        return;
    }

    // Emit appropriate signal
    if (isReverse) {
        QString hostname = hostInfo.hostName();
        emit reverseResolved(lookupId, originalQuery, hostname);
    } else {
        QList<QHostAddress> addresses = hostInfo.addresses();
        emit hostResolved(lookupId, originalQuery, addresses);
    }
}

void DnsResolver::cancelLookup(int lookupId)
{
    if (m_pendingLookups.contains(lookupId)) {
        QHostInfo::abortHostLookup(lookupId);
        m_pendingLookups.remove(lookupId);
        m_isReverseLookup.remove(lookupId);
    }
}

bool DnsResolver::isValidHostname(const QString &hostname) const
{
    if (hostname.isEmpty() || hostname.length() > 253) {
        return false;
    }

    // Check if it's a valid IP address
    QHostAddress addr(hostname);
    if (!addr.isNull()) {
        return true;
    }

    // Validate hostname format
    QRegularExpression hostnameRegex(
        R"(^(?:[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?\.)*[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?$)"
    );

    return hostnameRegex.match(hostname).hasMatch();
}

bool DnsResolver::isValidIpAddress(const QString &address) const
{
    QHostAddress addr(address);
    return !addr.isNull();
}

void DnsResolver::setTimeout(int timeout)
{
    m_timeout = timeout;
}

int DnsResolver::getTimeout() const
{
    return m_timeout;
}
