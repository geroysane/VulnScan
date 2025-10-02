#ifndef TESTNETWORKINTERFACE_H
#define TESTNETWORKINTERFACE_H

#include <QObject>
#include <QtTest>
#include "core/scanner/NetworkInterface.h"

/**
 * @class TestNetworkInterface
 * @brief Unit tests for NetworkInterface class
 */
class TestNetworkInterface : public QObject {
    Q_OBJECT

private slots:
    // Setup and cleanup
    void init();
    void cleanup();

    // Constructor tests
    void testConstructor();

    // Interface enumeration tests
    void testGetInterfaces();
    void testGetInterface();
    void testGetInterfaceAddresses();
    void testGetLocalIpAddress();

    // CIDR expansion tests
    void testExpandCIDR_Valid();
    void testExpandCIDR_Small();
    void testExpandCIDR_Large();
    void testExpandCIDR_Invalid();

    // Range expansion tests
    void testExpandRange_Valid();
    void testExpandRange_Single();
    void testExpandRange_Invalid();
    void testExpandRange_Reversed();

    // Host discovery tests
    void testIsHostAlive_Loopback();
    void testIsHostAlive_InvalidHost();
    void testIsHostAlive_Timeout();

    // Discovery tests (integration)
    void testDiscoverHosts_Empty();
    void testDiscoverHosts_SingleIP();
    void testDiscoverHosts_CIDR();

    // Signal tests
    void testSignals_Discovery();

private:
    NetworkInterface *m_netInterface;
};

#endif // TESTNETWORKINTERFACE_H
