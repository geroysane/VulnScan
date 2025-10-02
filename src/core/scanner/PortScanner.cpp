#include "PortScanner.h"
#include "ScanWorker.h"
#include <QThreadPool>
#include <QMutexLocker>

PortScanner::PortScanner(QObject *parent)
    : QObject(parent)
    , m_timeout(2000)
    , m_threadCount(100)
    , m_threadPool(new QThreadPool(this))
    , m_scannedCount(0)
    , m_totalPorts(0)
    , m_isRunning(0)
    , m_isCancelled(0)
{
    m_threadPool->setMaxThreadCount(m_threadCount);
}

PortScanner::~PortScanner()
{
    if (isScanning()) {
        stopScan();
    }
    m_threadPool->waitForDone();
}

void PortScanner::setTarget(const QString &target)
{
    QMutexLocker locker(&m_mutex);
    m_target = target;
}

void PortScanner::setPortRange(int startPort, int endPort)
{
    QMutexLocker locker(&m_mutex);
    m_ports.clear();
    for (int port = startPort; port <= endPort; ++port) {
        m_ports.append(port);
    }
}

void PortScanner::setPorts(const QVector<int> &ports)
{
    QMutexLocker locker(&m_mutex);
    m_ports = ports;
}

void PortScanner::setTimeout(int milliseconds)
{
    m_timeout = milliseconds;
}

void PortScanner::setThreadCount(int count)
{
    m_threadCount = count;
    m_threadPool->setMaxThreadCount(count);
}

QVector<PortResult> PortScanner::getResults() const
{
    QMutexLocker locker(&m_mutex);
    return m_results;
}

QVector<PortResult> PortScanner::getOpenPorts() const
{
    QMutexLocker locker(&m_mutex);
    QVector<PortResult> openPorts;
    for (const auto &result : m_results) {
        if (result.state == "open") {
            openPorts.append(result);
        }
    }
    return openPorts;
}

void PortScanner::startScan()
{
    // Check if already scanning
    if (m_isRunning.loadAcquire() != 0) {
        emit scanError("Scan already in progress");
        return;
    }

    // Validate configuration
    QString error = validateConfiguration();
    if (!error.isEmpty()) {
        emit scanError(error);
        return;
    }

    // Reset state
    reset();

    // Mark as running
    m_isRunning.storeRelease(1);
    m_isCancelled.storeRelease(0);

    // Get total port count
    int totalPorts;
    {
        QMutexLocker locker(&m_mutex);
        totalPorts = m_ports.size();
    }
    m_totalPorts.storeRelease(totalPorts);

    // Create and submit workers
    QMutexLocker locker(&m_mutex);
    for (int port : m_ports) {
        // Create worker (no parent - managed by thread pool)
        ScanWorker *worker = new ScanWorker(m_target, port, m_timeout, &m_isCancelled);

        // Connect signal
        connect(worker, &ScanWorker::scanCompleted,
                this, &PortScanner::onPortScanned,
                Qt::QueuedConnection);

        // Submit to thread pool
        m_threadPool->start(worker);
    }
}

void PortScanner::stopScan()
{
    if (m_isRunning.loadAcquire() == 0) {
        return; // Not running
    }

    // Set cancellation flag
    m_isCancelled.storeRelease(1);

    // Wait for all workers to finish
    m_threadPool->waitForDone();

    // Mark as not running
    m_isRunning.storeRelease(0);

    // Emit completion with cancelled flag
    emit scanCompleted(true);
}

bool PortScanner::isScanning() const
{
    return m_isRunning.loadAcquire() != 0;
}

void PortScanner::onPortScanned(const PortResult &result)
{
    // Store result
    {
        QMutexLocker locker(&m_mutex);
        m_results.append(result);
    }

    // Emit if port is open
    if (result.state == "open") {
        emit portFound(result);
    }

    // Update progress
    int scanned = m_scannedCount.fetchAndAddRelease(1) + 1;
    int total = m_totalPorts.loadAcquire();

    emit scanProgress(scanned, total);

    // Check if scan is complete
    if (scanned >= total) {
        m_isRunning.storeRelease(0);
        bool cancelled = (m_isCancelled.loadAcquire() != 0);
        emit scanCompleted(cancelled);
    }
}

QString PortScanner::validateConfiguration() const
{
    QMutexLocker locker(&m_mutex);

    if (m_target.isEmpty()) {
        return "Target host not set";
    }

    if (m_ports.isEmpty()) {
        return "No ports specified for scanning";
    }

    if (m_timeout <= 0) {
        return "Invalid timeout value";
    }

    if (m_threadCount <= 0) {
        return "Invalid thread count";
    }

    return QString(); // Valid
}

void PortScanner::reset()
{
    QMutexLocker locker(&m_mutex);
    m_results.clear();
    m_scannedCount.storeRelease(0);
    m_totalPorts.storeRelease(0);
}
