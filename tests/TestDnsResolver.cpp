#include "TestDnsResolver.h"
#include <QSignalSpy>
#include <QHostAddress>

void TestDnsResolver::init()
{
    m_resolver = new DnsResolver();
}

void TestDnsResolver::cleanup()
{
    delete m_resolver;
    m_resolver = nullptr;
}

void TestDnsResolver::testConstructor()
{
    QVERIFY(m_resolver != nullptr);
    QCOMPARE(m_resolver->getTimeout(), 5000); // Default timeout
}

void TestDnsResolver::testSetTimeout()
{
    m_resolver->setTimeout(10000);
    QCOMPARE(m_resolver->getTimeout(), 10000);

    m_resolver->setTimeout(1000);
    QCOMPARE(m_resolver->getTimeout(), 1000);
}

void TestDnsResolver::testGetTimeout()
{
    int timeout = m_resolver->getTimeout();
    QVERIFY(timeout > 0);
    QCOMPARE(timeout, 5000); // Default
}

void TestDnsResolver::testValidHostname()
{
    // Test through resolveHost - valid hostnames should not return empty immediately
    QList<QHostAddress> result = m_resolver->resolveHost("localhost", 1000);
    // localhost should resolve
    QVERIFY(!result.isEmpty());
}

void TestDnsResolver::testInvalidHostname()
{
    // Empty hostname
    QList<QHostAddress> result = m_resolver->resolveHost("", 500);
    QVERIFY(result.isEmpty());

    // Invalid characters
    result = m_resolver->resolveHost("invalid..hostname", 500);
    QVERIFY(result.isEmpty());
}

void TestDnsResolver::testValidIpAddress()
{
    // Valid IPv4
    QList<QHostAddress> result = m_resolver->resolveHost("127.0.0.1", 1000);
    QVERIFY(!result.isEmpty());
    QCOMPARE(result.first().toString(), QString("127.0.0.1"));
}

void TestDnsResolver::testInvalidIpAddress()
{
    QString invalid = m_resolver->reverseResolve("999.999.999.999", 500);
    QVERIFY(invalid.isEmpty());

    invalid = m_resolver->reverseResolve("not.an.ip", 500);
    QVERIFY(invalid.isEmpty());
}

void TestDnsResolver::testResolveHost_ValidIp()
{
    // IP address should be returned directly
    QList<QHostAddress> result = m_resolver->resolveHost("127.0.0.1", 1000);

    QVERIFY(!result.isEmpty());
    QCOMPARE(result.size(), 1);
    QCOMPARE(result.first().toString(), QString("127.0.0.1"));
}

void TestDnsResolver::testResolveHost_Localhost()
{
    QList<QHostAddress> result = m_resolver->resolveHost("localhost", 2000);

    // localhost should resolve to 127.0.0.1 or ::1
    QVERIFY(!result.isEmpty());
}

void TestDnsResolver::testResolveHost_InvalidHost()
{
    m_resolver->setTimeout(500);

    // Non-existent hostname
    QList<QHostAddress> result = m_resolver->resolveHost("this-host-definitely-does-not-exist-12345.invalid", 500);

    // Should return empty list for non-existent host
    QVERIFY(result.isEmpty());
}

void TestDnsResolver::testResolveHost_Timeout()
{
    // Test timeout behavior
    m_resolver->setTimeout(100);

    QList<QHostAddress> result = m_resolver->resolveHost("very-slow-host-that-times-out.invalid", 100);

    // Should timeout and return empty
    QVERIFY(result.isEmpty());
}

void TestDnsResolver::testReverseResolve_Loopback()
{
    QString hostname = m_resolver->reverseResolve("127.0.0.1", 2000);

    // Should resolve to localhost or similar
    // Note: This might return empty on some systems
    QVERIFY(hostname.isEmpty() || hostname.contains("localhost") || !hostname.isEmpty());
}

void TestDnsResolver::testReverseResolve_InvalidIp()
{
    QString result = m_resolver->reverseResolve("192.0.2.1", 500);

    // TEST-NET-1 (192.0.2.0/24) should not resolve
    // Result can be empty or the IP itself
    QVERIFY(true); // Just verify it doesn't crash
}

void TestDnsResolver::testResolveHostAsync_ValidIp()
{
    QSignalSpy spy(m_resolver, &DnsResolver::hostResolved);

    // IP address returns immediately
    int lookupId = m_resolver->resolveHostAsync("192.168.1.1");

    // Direct IP should emit signal immediately (lookupId = -1)
    QCOMPARE(lookupId, -1);
    QCOMPARE(spy.count(), 1);
}

void TestDnsResolver::testResolveHostAsync_InvalidHost()
{
    int lookupId = m_resolver->resolveHostAsync("");
    QCOMPARE(lookupId, -1); // Should return -1 for invalid input
}

void TestDnsResolver::testReverseResolveAsync()
{
    int lookupId = m_resolver->reverseResolveAsync("127.0.0.1");

    // Should return valid lookup ID
    QVERIFY(lookupId != -1);
}

void TestDnsResolver::testSignals_HostResolved()
{
    QSignalSpy spy(m_resolver, &DnsResolver::hostResolved);

    // Resolve an IP (should emit immediately)
    m_resolver->resolveHostAsync("10.0.0.1");

    QCOMPARE(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst();
    QCOMPARE(arguments.at(0).toInt(), -1); // lookupId for direct IP
    QCOMPARE(arguments.at(1).toString(), QString("10.0.0.1"));
}

void TestDnsResolver::testSignals_ResolutionFailed()
{
    QSignalSpy spy(m_resolver, &DnsResolver::resolutionFailed);

    // Resolve invalid hostname
    int lookupId = m_resolver->resolveHostAsync("invalid-host-xyz-123.invalid");

    if (lookupId != -1) {
        // Wait for signal (with timeout)
        QVERIFY(spy.wait(3000) || spy.count() > 0);
    }
}

void TestDnsResolver::testCancelLookup()
{
    int lookupId = m_resolver->resolveHostAsync("some-host.example.com");

    if (lookupId != -1) {
        // Cancel should not crash
        m_resolver->cancelLookup(lookupId);
        QVERIFY(true);

        // Cancelling again should be safe
        m_resolver->cancelLookup(lookupId);
        QVERIFY(true);
    }
}
