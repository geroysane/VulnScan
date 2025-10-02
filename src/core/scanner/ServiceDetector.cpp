#include "ServiceDetector.h"
#include <QTcpSocket>
#include <QRegularExpression>
#include <QEventLoop>
#include <QTimer>

ServiceDetector::ServiceDetector(QObject *parent)
    : QObject(parent)
    , m_timeout(3000)
{
    initializePortMappings();
}

ServiceDetector::~ServiceDetector()
{
}

void ServiceDetector::initializePortMappings()
{
    // Common TCP services
    m_tcpPortMap[20] = "ftp-data";
    m_tcpPortMap[21] = "ftp";
    m_tcpPortMap[22] = "ssh";
    m_tcpPortMap[23] = "telnet";
    m_tcpPortMap[25] = "smtp";
    m_tcpPortMap[53] = "dns";
    m_tcpPortMap[80] = "http";
    m_tcpPortMap[110] = "pop3";
    m_tcpPortMap[143] = "imap";
    m_tcpPortMap[443] = "https";
    m_tcpPortMap[445] = "smb";
    m_tcpPortMap[3306] = "mysql";
    m_tcpPortMap[3389] = "rdp";
    m_tcpPortMap[5432] = "postgresql";
    m_tcpPortMap[5900] = "vnc";
    m_tcpPortMap[6379] = "redis";
    m_tcpPortMap[8080] = "http-proxy";
    m_tcpPortMap[8443] = "https-alt";
    m_tcpPortMap[27017] = "mongodb";

    // Common UDP services
    m_udpPortMap[53] = "dns";
    m_udpPortMap[67] = "dhcp-server";
    m_udpPortMap[68] = "dhcp-client";
    m_udpPortMap[69] = "tftp";
    m_udpPortMap[123] = "ntp";
    m_udpPortMap[161] = "snmp";
    m_udpPortMap[162] = "snmp-trap";
    m_udpPortMap[500] = "ipsec";
    m_udpPortMap[514] = "syslog";
}

ServiceInfo ServiceDetector::detectService(const QString &host, int port, int timeout)
{
    emit detectionStarted(host, port);

    ServiceInfo info;
    info.port = port;
    info.protocol = "TCP";
    info.name = getServiceByPort(port, "TCP");

    QTcpSocket socket;
    socket.connectToHost(host, static_cast<quint16>(port));

    if (!socket.waitForConnected(timeout)) {
        emit detectionFailed(host, port, socket.errorString());
        return info;
    }

    // Try to grab banner
    info.banner = grabBanner(&socket, port);

    // Parse banner to extract version and refine service name
    if (!info.banner.isEmpty()) {
        ServiceInfo parsed = parseBanner(info.banner, port);
        if (!parsed.name.isEmpty()) {
            info.name = parsed.name;
        }
        info.version = parsed.version;
    }

    socket.disconnectFromHost();
    if (socket.state() != QAbstractSocket::UnconnectedState) {
        socket.waitForDisconnected(1000);
    }

    emit detectionCompleted(info);
    return info;
}

QString ServiceDetector::grabBanner(QTcpSocket *socket, int port)
{
    if (!socket || socket->state() != QAbstractSocket::ConnectedState) {
        return QString();
    }

    // Send service-specific probe if needed
    QByteArray probe = getServiceProbe(port);
    if (!probe.isEmpty()) {
        socket->write(probe);
        socket->flush();
    }

    // Wait for response
    if (!socket->waitForReadyRead(m_timeout)) {
        return QString();
    }

    // Read banner (up to 4KB)
    QByteArray banner = socket->read(4096);
    return QString::fromLatin1(banner).trimmed();
}

QByteArray ServiceDetector::getServiceProbe(int port)
{
    switch (port) {
    case 80:
    case 8080:
    case 8443:
        // HTTP GET request
        return "GET / HTTP/1.0\r\n\r\n";
    case 443:
        // HTTPS - no probe, SSL handshake would be needed
        return QByteArray();
    case 25:
        // SMTP - wait for banner
        return QByteArray();
    case 110:
        // POP3 - wait for banner
        return QByteArray();
    case 143:
        // IMAP - wait for banner
        return QByteArray();
    case 21:
        // FTP - wait for banner
        return QByteArray();
    case 22:
        // SSH - wait for banner
        return QByteArray();
    default:
        // Generic probe - just wait for data
        return QByteArray();
    }
}

ServiceInfo ServiceDetector::parseBanner(const QString &banner, int port)
{
    ServiceInfo info;
    info.port = port;

    if (banner.isEmpty()) {
        return info;
    }

    // HTTP detection
    QRegularExpression httpRegex(R"(HTTP/(\d+\.\d+)\s+(\d+))");
    QRegularExpressionMatch httpMatch = httpRegex.match(banner);
    if (httpMatch.hasMatch()) {
        info.name = "http";
        info.version = "HTTP/" + httpMatch.captured(1);

        // Try to detect server
        QRegularExpression serverRegex(R"(Server:\s*([^\r\n]+))");
        QRegularExpressionMatch serverMatch = serverRegex.match(banner);
        if (serverMatch.hasMatch()) {
            info.version += " (" + serverMatch.captured(1).trimmed() + ")";
        }
        return info;
    }

    // SSH detection
    if (banner.startsWith("SSH-")) {
        info.name = "ssh";
        QRegularExpression sshRegex(R"(SSH-(\d+\.\d+)-(.+))");
        QRegularExpressionMatch sshMatch = sshRegex.match(banner);
        if (sshMatch.hasMatch()) {
            info.version = "SSH " + sshMatch.captured(1) + " (" + sshMatch.captured(2).trimmed() + ")";
        }
        return info;
    }

    // FTP detection
    if (banner.startsWith("220")) {
        info.name = "ftp";
        QRegularExpression ftpRegex(R"(220[- ](.+))");
        QRegularExpressionMatch ftpMatch = ftpRegex.match(banner);
        if (ftpMatch.hasMatch()) {
            info.version = ftpMatch.captured(1).trimmed();
        }
        return info;
    }

    // SMTP detection
    if (banner.startsWith("220") && banner.contains("SMTP")) {
        info.name = "smtp";
        QRegularExpression smtpRegex(R"(220[- ](.+))");
        QRegularExpressionMatch smtpMatch = smtpRegex.match(banner);
        if (smtpMatch.hasMatch()) {
            info.version = smtpMatch.captured(1).trimmed();
        }
        return info;
    }

    // POP3 detection
    if (banner.startsWith("+OK")) {
        info.name = "pop3";
        QRegularExpression popRegex(R"(\+OK (.+))");
        QRegularExpressionMatch popMatch = popRegex.match(banner);
        if (popMatch.hasMatch()) {
            info.version = popMatch.captured(1).trimmed();
        }
        return info;
    }

    // IMAP detection
    if (banner.contains("IMAP")) {
        info.name = "imap";
        QRegularExpression imapRegex(R"(\* OK (.+))");
        QRegularExpressionMatch imapMatch = imapRegex.match(banner);
        if (imapMatch.hasMatch()) {
            info.version = imapMatch.captured(1).trimmed();
        }
        return info;
    }

    // MySQL detection
    if (banner.contains("mysql") || port == 3306) {
        info.name = "mysql";
        // MySQL banner is binary, try to extract version
        QRegularExpression mysqlRegex(R"((\d+\.\d+\.\d+))");
        QRegularExpressionMatch mysqlMatch = mysqlRegex.match(banner);
        if (mysqlMatch.hasMatch()) {
            info.version = mysqlMatch.captured(1);
        }
        return info;
    }

    // PostgreSQL detection
    if (port == 5432) {
        info.name = "postgresql";
        return info;
    }

    // Redis detection
    if (port == 6379) {
        info.name = "redis";
        return info;
    }

    // MongoDB detection
    if (port == 27017) {
        info.name = "mongodb";
        return info;
    }

    return info;
}

QString ServiceDetector::getServiceByPort(int port, const QString &protocol) const
{
    if (protocol.toUpper() == "TCP") {
        return m_tcpPortMap.value(port, "unknown");
    } else if (protocol.toUpper() == "UDP") {
        return m_udpPortMap.value(port, "unknown");
    }
    return "unknown";
}

void ServiceDetector::setTimeout(int timeout)
{
    m_timeout = timeout;
}

int ServiceDetector::getTimeout() const
{
    return m_timeout;
}
