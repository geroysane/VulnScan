#include "ScanEngine.h"

ScanEngine::ScanEngine(QObject *parent)
    : QObject(parent)
    , m_isScanning(false)
    , m_totalPorts(0)
    , m_scannedPorts(0)
    , m_servicesDetected(0)
{
    // Initialize scanner components
    m_portScanner = QSharedPointer<PortScanner>::create(this);
    m_serviceDetector = QSharedPointer<ServiceDetector>::create(this);
    m_dnsResolver = QSharedPointer<DnsResolver>::create(this);
    m_networkInterface = QSharedPointer<NetworkInterface>::create(this);

    // Connect port scanner signals
    connect(m_portScanner.data(), &PortScanner::scanProgress,
            this, &ScanEngine::handlePortScanProgress);
    connect(m_portScanner.data(), &PortScanner::portFound,
            this, &ScanEngine::handlePortFound);
    connect(m_portScanner.data(),
            static_cast<void (PortScanner::*)(bool)>(&PortScanner::scanCompleted),
            this, [this]() {
                QVector<PortResult> results = m_portScanner->getResults();
                handlePortScanCompleted(QList<PortResult>(results.begin(), results.end()));
            });

    // Connect service detector signals
    connect(m_serviceDetector.data(), &ServiceDetector::detectionCompleted,
            this, &ScanEngine::handleServiceDetected);

    // Connect DNS resolver signals
    connect(m_dnsResolver.data(), &DnsResolver::hostResolved,
            this, &ScanEngine::handleDnsResolved);
}

ScanEngine::~ScanEngine()
{
    if (m_isScanning) {
        cancelScan();
    }
}

bool ScanEngine::startScan(const ScanConfiguration &config)
{
    if (m_isScanning) {
        return false;
    }

    if (!config.isValid()) {
        emit scanFailed("Invalid scan configuration");
        return false;
    }

    // Initialize scan
    m_config = config;
    m_isScanning = true;
    m_scannedPorts = 0;
    m_servicesDetected = 0;
    m_totalPorts = config.getPortCount();

    // Initialize result
    m_result = ScanResult();
    m_result.targetHost = config.target;
    m_result.scanStartTime = QDateTime::currentDateTime();
    m_result.status = "running";

    emit scanStarted(config.target);
    emit scanProgress(0, "Starting scan...");

    // Start scan workflow
    if (m_config.enableDnsResolution && !m_config.target.isEmpty()) {
        performDnsResolution();
    } else {
        performPortScan();
    }

    return true;
}

void ScanEngine::cancelScan()
{
    if (!m_isScanning) {
        return;
    }

    // Cancel port scanner
    m_portScanner->stopScan();

    m_isScanning = false;
    m_result.status = "cancelled";
    m_result.scanEndTime = QDateTime::currentDateTime();

    emit scanFailed("Scan cancelled by user");
}

bool ScanEngine::isScanning() const
{
    return m_isScanning;
}

ScanResult ScanEngine::getCurrentResult() const
{
    return m_result;
}

ScanConfiguration ScanEngine::getConfiguration() const
{
    return m_config;
}

void ScanEngine::performDnsResolution()
{
    emit scanProgress(5, "Resolving DNS...");

    // Perform synchronous DNS resolution
    QList<QHostAddress> addresses = m_dnsResolver->resolveHost(m_config.target, 5000);

    if (!addresses.isEmpty()) {
        // Use first resolved address
        m_config.target = addresses.first().toString();
    }

    // Continue with port scan
    performPortScan();
}

void ScanEngine::performPortScan()
{
    emit scanProgress(10, "Starting port scan...");

    // Configure port scanner
    m_portScanner->setTarget(m_config.target);
    m_portScanner->setTimeout(m_config.timeout);
    m_portScanner->setThreadCount(m_config.maxThreads);

    // Get ports to scan
    QList<int> ports = m_config.getPortsToScan();
    QVector<int> portsVector = QVector<int>(ports.begin(), ports.end());

    // Set ports and start scan
    m_portScanner->setPorts(portsVector);
    m_portScanner->startScan();
}

void ScanEngine::performServiceDetection()
{
    if (!m_config.enableServiceDetection) {
        finalizeScan();
        return;
    }

    emit scanProgress(80, "Detecting services...");

    // Detect services on open ports
    for (const PortResult &portResult : m_result.ports) {
        if (portResult.state == "open") {
            ServiceInfo serviceInfo = m_serviceDetector->detectService(
                m_config.target,
                portResult.port,
                m_config.timeout
            );

            if (!serviceInfo.name.isEmpty()) {
                m_servicesDetected++;
                emit serviceDetected(serviceInfo);
            }
        }
    }

    finalizeScan();
}

void ScanEngine::finalizeScan()
{
    m_isScanning = false;
    m_result.scanEndTime = QDateTime::currentDateTime();
    m_result.status = "completed";

    emit scanProgress(100, "Scan completed");
    emit scanCompleted(m_result);
}

int ScanEngine::calculateProgress() const
{
    if (m_totalPorts == 0) {
        return 0;
    }

    // Port scanning is 10-80% of progress
    int portProgress = 10 + (m_scannedPorts * 70 / m_totalPorts);

    // Service detection is 80-100%
    if (!m_config.enableServiceDetection) {
        return portProgress > 80 ? 100 : portProgress;
    }

    return portProgress;
}

void ScanEngine::handlePortScanProgress(int current, int total)
{
    m_scannedPorts = current;
    m_totalPorts = total;

    int progress = calculateProgress();
    QString message = QString("Scanning port %1 of %2...").arg(current).arg(total);

    emit scanProgress(progress, message);
}

void ScanEngine::handlePortFound(const PortResult &result)
{
    m_result.ports.append(result);
    emit portDiscovered(result);
}

void ScanEngine::handlePortScanCompleted(const QList<PortResult> &results)
{
    m_result.ports = results;

    emit scanProgress(75, QString("Port scan completed. Found %1 open ports").arg(
        std::count_if(results.begin(), results.end(),
                     [](const PortResult &r) { return r.state == "open"; })
    ));

    // Continue with service detection if enabled
    if (m_config.enableServiceDetection) {
        performServiceDetection();
    } else {
        finalizeScan();
    }
}

void ScanEngine::handleServiceDetected(const ServiceInfo &serviceInfo)
{
    // Update corresponding port result with service info
    for (PortResult &portResult : m_result.ports) {
        if (portResult.port == serviceInfo.port) {
            portResult.service = serviceInfo.name;
            portResult.version = serviceInfo.version;
            break;
        }
    }

    emit serviceDetected(serviceInfo);
}

void ScanEngine::handleDnsResolved(int lookupId, const QString &hostname, const QList<QHostAddress> &addresses)
{
    Q_UNUSED(lookupId);
    Q_UNUSED(hostname);

    if (!addresses.isEmpty()) {
        m_config.target = addresses.first().toString();
    }

    performPortScan();
}
