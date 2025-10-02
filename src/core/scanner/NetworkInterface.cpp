#include "NetworkInterface.h"
#include <QTcpSocket>
#include <QRegularExpression>
#include <QtEndian>
#include <QDateTime>

NetworkInterface::NetworkInterface(QObject *parent)
    : QObject(parent)
{
}

NetworkInterface::~NetworkInterface()
{
}

QStringList NetworkInterface::getInterfaces() const
{
    QStringList result;
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface &iface : interfaces) {
        if (iface.flags() & QNetworkInterface::IsUp &&
            !(iface.flags() & QNetworkInterface::IsLoopBack)) {
            result << iface.name();
        }
    }

    return result;
}

QNetworkInterface NetworkInterface::getInterface(const QString &interfaceName) const
{
    return QNetworkInterface::interfaceFromName(interfaceName);
}

QList<QHostAddress> NetworkInterface::getInterfaceAddresses(const QString &interfaceName) const
{
    QList<QHostAddress> result;
    QNetworkInterface iface = QNetworkInterface::interfaceFromName(interfaceName);

    if (!iface.isValid()) {
        return result;
    }

    QList<QNetworkAddressEntry> entries = iface.addressEntries();
    for (const QNetworkAddressEntry &entry : entries) {
        QHostAddress addr = entry.ip();
        // Only return IPv4 addresses
        if (addr.protocol() == QAbstractSocket::IPv4Protocol) {
            result << addr;
        }
    }

    return result;
}

QStringList NetworkInterface::expandCIDR(const QString &cidr) const
{
    QStringList result;
    QHostAddress network;
    int prefixLength;

    if (!parseCIDR(cidr, network, prefixLength)) {
        return result;
    }

    if (network.protocol() != QAbstractSocket::IPv4Protocol) {
        return result; // Only support IPv4 for now
    }

    quint32 networkInt = ipToUInt32(network);
    quint32 mask = 0xFFFFFFFF << (32 - prefixLength);
    networkInt = networkInt & mask;

    quint32 hostCount = (1U << (32 - prefixLength));

    // Limit to reasonable range size (max 65536 hosts)
    if (hostCount > 65536) {
        return result;
    }

    for (quint32 i = 1; i < hostCount - 1; ++i) {
        quint32 hostIp = networkInt + i;
        result << uInt32ToIp(hostIp).toString();
    }

    return result;
}

QStringList NetworkInterface::expandRange(const QString &startIp, const QString &endIp) const
{
    QStringList result;
    QHostAddress start(startIp);
    QHostAddress end(endIp);

    if (start.isNull() || end.isNull()) {
        return result;
    }

    if (start.protocol() != QAbstractSocket::IPv4Protocol ||
        end.protocol() != QAbstractSocket::IPv4Protocol) {
        return result;
    }

    quint32 startInt = ipToUInt32(start);
    quint32 endInt = ipToUInt32(end);

    if (startInt > endInt) {
        return result;
    }

    // Limit to reasonable range
    if (endInt - startInt > 65536) {
        return result;
    }

    for (quint32 i = startInt; i <= endInt; ++i) {
        result << uInt32ToIp(i).toString();
    }

    return result;
}

QList<HostInfo> NetworkInterface::discoverHosts(const QStringList &targets, int timeout)
{
    QList<HostInfo> result;
    QStringList allTargets;

    // Expand CIDR ranges
    for (const QString &target : targets) {
        if (target.contains('/')) {
            allTargets << expandCIDR(target);
        } else if (target.contains('-')) {
            // Range format: 192.168.1.1-192.168.1.254
            QStringList parts = target.split('-');
            if (parts.size() == 2) {
                allTargets << expandRange(parts[0].trimmed(), parts[1].trimmed());
            }
        } else {
            allTargets << target;
        }
    }

    emit discoveryStarted(allTargets.size());

    int current = 0;
    int hostsFound = 0;

    for (const QString &host : allTargets) {
        current++;
        emit discoveryProgress(current, allTargets.size());

        HostInfo info;
        info.ipAddress = host;

        QTcpSocket socket;
        qint64 startTime = QDateTime::currentMSecsSinceEpoch();

        socket.connectToHost(host, 80);
        bool connected = socket.waitForConnected(timeout);

        qint64 endTime = QDateTime::currentMSecsSinceEpoch();

        if (connected) {
            info.isAlive = true;
            info.responseTime = static_cast<int>(endTime - startTime);
            socket.disconnectFromHost();
            hostsFound++;
            emit hostDiscovered(info);
        } else {
            info.isAlive = false;
            info.responseTime = -1;
        }

        result << info;
    }

    emit discoveryCompleted(hostsFound);
    return result;
}

bool NetworkInterface::isHostAlive(const QString &host, int port, int timeout)
{
    QTcpSocket socket;
    socket.connectToHost(host, static_cast<quint16>(port));
    bool connected = socket.waitForConnected(timeout);

    if (connected) {
        socket.disconnectFromHost();
        if (socket.state() != QAbstractSocket::UnconnectedState) {
            socket.waitForDisconnected(1000);
        }
    }

    return connected;
}

QString NetworkInterface::getLocalIpAddress() const
{
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    for (const QNetworkInterface &iface : interfaces) {
        if (!(iface.flags() & QNetworkInterface::IsUp) ||
            (iface.flags() & QNetworkInterface::IsLoopBack)) {
            continue;
        }

        QList<QNetworkAddressEntry> entries = iface.addressEntries();
        for (const QNetworkAddressEntry &entry : entries) {
            QHostAddress addr = entry.ip();
            if (addr.protocol() == QAbstractSocket::IPv4Protocol &&
                !addr.isLoopback()) {
                return addr.toString();
            }
        }
    }

    return QString();
}

QString NetworkInterface::getGateway(const QString &interfaceName) const
{
    // Qt doesn't provide direct gateway access
    // This would require platform-specific code or parsing system commands
    // For now, return empty string - can be implemented later if needed
    Q_UNUSED(interfaceName);
    return QString();
}

bool NetworkInterface::parseCIDR(const QString &cidr, QHostAddress &network, int &prefixLength) const
{
    QRegularExpression cidrRegex(R"(^(\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3})/(\d{1,2})$)");
    QRegularExpressionMatch match = cidrRegex.match(cidr);

    if (!match.hasMatch()) {
        return false;
    }

    QString ipStr = match.captured(1);
    network = QHostAddress(ipStr);

    if (network.isNull()) {
        return false;
    }

    prefixLength = match.captured(2).toInt();

    if (prefixLength < 0 || prefixLength > 32) {
        return false;
    }

    return true;
}

QHostAddress NetworkInterface::getNetworkAddress(const QHostAddress &address, int prefixLength) const
{
    quint32 ip = ipToUInt32(address);
    quint32 mask = 0xFFFFFFFF << (32 - prefixLength);
    return uInt32ToIp(ip & mask);
}

QHostAddress NetworkInterface::getBroadcastAddress(const QHostAddress &network, int prefixLength) const
{
    quint32 networkInt = ipToUInt32(network);
    quint32 hostMask = (1U << (32 - prefixLength)) - 1;
    return uInt32ToIp(networkInt | hostMask);
}

quint32 NetworkInterface::ipToUInt32(const QHostAddress &address) const
{
    if (address.protocol() != QAbstractSocket::IPv4Protocol) {
        return 0;
    }
    return address.toIPv4Address();
}

QHostAddress NetworkInterface::uInt32ToIp(quint32 ip) const
{
    return QHostAddress(ip);
}
