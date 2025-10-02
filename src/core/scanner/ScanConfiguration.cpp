#include "ScanConfiguration.h"

ScanConfiguration::ScanConfiguration()
    : startPort(1)
    , endPort(1000)
    , scanType(PortScan)
    , timeout(2000)
    , maxThreads(100)
    , enableServiceDetection(false)
    , enableDnsResolution(false)
    , enableHostDiscovery(false)
{
}

bool ScanConfiguration::isValid() const
{
    // Check if we have at least one target
    if (target.isEmpty() && targets.isEmpty()) {
        return false;
    }

    // Check port range validity
    if (specificPorts.isEmpty()) {
        if (startPort < 1 || startPort > 65535) {
            return false;
        }
        if (endPort < 1 || endPort > 65535) {
            return false;
        }
        if (startPort > endPort) {
            return false;
        }
    }

    // Check timeout
    if (timeout <= 0) {
        return false;
    }

    // Check thread count
    if (maxThreads <= 0) {
        return false;
    }

    return true;
}

int ScanConfiguration::getPortCount() const
{
    if (!specificPorts.isEmpty()) {
        return specificPorts.size();
    }
    return (endPort - startPort + 1);
}

QList<int> ScanConfiguration::getPortsToScan() const
{
    if (!specificPorts.isEmpty()) {
        return specificPorts;
    }

    QList<int> ports;
    for (int port = startPort; port <= endPort; ++port) {
        ports.append(port);
    }
    return ports;
}

ScanConfiguration ScanConfiguration::quickScan(const QString &target)
{
    ScanConfiguration config;
    config.target = target;
    config.startPort = 1;
    config.endPort = 1000;
    config.scanType = PortScan;
    config.timeout = 1000;
    config.maxThreads = 200;
    config.enableServiceDetection = false;
    config.enableDnsResolution = false;
    config.enableHostDiscovery = false;
    return config;
}

ScanConfiguration ScanConfiguration::fullScan(const QString &target)
{
    ScanConfiguration config;
    config.target = target;
    config.startPort = 1;
    config.endPort = 65535;
    config.scanType = FullScan;
    config.timeout = 2000;
    config.maxThreads = 100;
    config.enableServiceDetection = true;
    config.enableDnsResolution = true;
    config.enableHostDiscovery = false;
    return config;
}

ScanConfiguration ScanConfiguration::customScan(const QString &target, int startPort, int endPort)
{
    ScanConfiguration config;
    config.target = target;
    config.startPort = startPort;
    config.endPort = endPort;
    config.scanType = ServiceDetection;
    config.timeout = 2000;
    config.maxThreads = 100;
    config.enableServiceDetection = true;
    config.enableDnsResolution = false;
    config.enableHostDiscovery = false;
    return config;
}
