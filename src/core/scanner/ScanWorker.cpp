#include "ScanWorker.h"
#include <QTcpSocket>
#include <QEventLoop>
#include <QTimer>

ScanWorker::ScanWorker(const QString &target,
                       int port,
                       int timeout,
                       QAtomicInt *cancelFlag)
    : QObject(nullptr)
    , m_target(target)
    , m_port(port)
    , m_timeout(timeout)
    , m_cancelFlag(cancelFlag)
{
    setAutoDelete(true); // Worker will be deleted by thread pool
}

ScanWorker::~ScanWorker()
{
}

void ScanWorker::run()
{
    // Check if scan has been cancelled before starting
    if (m_cancelFlag && m_cancelFlag->loadAcquire() != 0) {
        return;
    }

    PortResult result = scanPort();

    // Only emit if not cancelled
    if (!m_cancelFlag || m_cancelFlag->loadAcquire() == 0) {
        emit scanCompleted(result);
    }
}

PortResult ScanWorker::scanPort()
{
    PortResult result;
    result.port = m_port;
    result.protocol = "TCP";
    result.state = "closed";
    result.service = "";
    result.version = "";

    QTcpSocket socket;

    // Check cancellation before connecting
    if (m_cancelFlag && m_cancelFlag->loadAcquire() != 0) {
        result.state = "cancelled";
        return result;
    }

    // Attempt connection
    socket.connectToHost(m_target, static_cast<quint16>(m_port));

    // Wait for connection with timeout
    bool connected = socket.waitForConnected(m_timeout);

    if (connected) {
        result.state = "open";
        socket.disconnectFromHost();
        if (socket.state() != QAbstractSocket::UnconnectedState) {
            socket.waitForDisconnected(1000);
        }
    } else {
        // Determine state from error
        result.state = getStateFromError(socket.errorString());
    }

    return result;
}

QString ScanWorker::getStateFromError(const QString &errorString) const
{
    // Analyze socket error to determine port state

    // Connection refused = port is closed
    if (errorString.contains("Connection refused", Qt::CaseInsensitive)) {
        return "closed";
    }

    // Timeout or network unreachable = filtered
    if (errorString.contains("timed out", Qt::CaseInsensitive) ||
        errorString.contains("timeout", Qt::CaseInsensitive) ||
        errorString.contains("unreachable", Qt::CaseInsensitive) ||
        errorString.contains("no route", Qt::CaseInsensitive)) {
        return "filtered";
    }

    // Host not found
    if (errorString.contains("not found", Qt::CaseInsensitive) ||
        errorString.contains("could not find", Qt::CaseInsensitive)) {
        return "unknown-host";
    }

    // Default: closed
    return "closed";
}
