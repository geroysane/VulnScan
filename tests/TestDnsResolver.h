#ifndef TESTDNSRESOLVER_H
#define TESTDNSRESOLVER_H

#include <QObject>
#include <QtTest>
#include "core/scanner/DnsResolver.h"

/**
 * @class TestDnsResolver
 * @brief Unit tests for DnsResolver class
 */
class TestDnsResolver : public QObject {
    Q_OBJECT

private slots:
    // Setup and cleanup
    void init();
    void cleanup();

    // Constructor tests
    void testConstructor();

    // Timeout tests
    void testSetTimeout();
    void testGetTimeout();

    // Validation tests
    void testValidHostname();
    void testInvalidHostname();
    void testValidIpAddress();
    void testInvalidIpAddress();

    // Synchronous resolution tests
    void testResolveHost_ValidIp();
    void testResolveHost_Localhost();
    void testResolveHost_InvalidHost();
    void testResolveHost_Timeout();

    // Reverse lookup tests
    void testReverseResolve_Loopback();
    void testReverseResolve_InvalidIp();

    // Asynchronous tests
    void testResolveHostAsync_ValidIp();
    void testResolveHostAsync_InvalidHost();
    void testReverseResolveAsync();

    // Signal tests
    void testSignals_HostResolved();
    void testSignals_ResolutionFailed();

    // Cancel tests
    void testCancelLookup();

private:
    DnsResolver *m_resolver;
};

#endif // TESTDNSRESOLVER_H
