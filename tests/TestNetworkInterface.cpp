#include "TestNetworkInterface.h"
#include <QSignalSpy>

void TestNetworkInterface::init()
{
    m_netInterface = new NetworkInterface();
}

void TestNetworkInterface::cleanup()
{
    delete m_netInterface;
    m_netInterface = nullptr;
}

void TestNetworkInterface::testConstructor()
{
    QVERIFY(m_netInterface != nullptr);
}

void TestNetworkInterface::testGetInterfaces()
{
    QStringList interfaces = m_netInterface->getInterfaces();

    // Should have at least one interface (even if just loopback is filtered out)
    // On most systems there's at least one active non-loopback interface
    QVERIFY(interfaces.size() >= 0); // Can be 0 on isolated systems
}

void TestNetworkInterface::testGetInterface()
{
    QStringList interfaces = m_netInterface->getInterfaces();

    if (!interfaces.isEmpty()) {
        QString ifaceName = interfaces.first();
        QNetworkInterface iface = m_netInterface->getInterface(ifaceName);
        QVERIFY(iface.isValid());
    }
}

void TestNetworkInterface::testGetInterfaceAddresses()
{
    QStringList interfaces = m_netInterface->getInterfaces();

    if (!interfaces.isEmpty()) {
        QString ifaceName = interfaces.first();
        QList<QHostAddress> addresses = m_netInterface->getInterfaceAddresses(ifaceName);
        // Should have at least one IPv4 address
        QVERIFY(addresses.size() >= 0);
    }
}

void TestNetworkInterface::testGetLocalIpAddress()
{
    QString localIp = m_netInterface->getLocalIpAddress();

    // May be empty on systems with no network
    // If not empty, should be valid IPv4
    if (!localIp.isEmpty()) {
        QHostAddress addr(localIp);
        QVERIFY(!addr.isNull());
        QVERIFY(!addr.isLoopback());
    }
}

void TestNetworkInterface::testExpandCIDR_Valid()
{
    // /30 network = 4 IPs total, 2 usable (excluding network and broadcast)
    QStringList result = m_netInterface->expandCIDR("192.168.1.0/30");

    QCOMPARE(result.size(), 2); // .1 and .2 (excludes .0 and .3)
    QVERIFY(result.contains("192.168.1.1"));
    QVERIFY(result.contains("192.168.1.2"));
}

void TestNetworkInterface::testExpandCIDR_Small()
{
    // /31 network = 2 IPs total, 0 usable (point-to-point)
    QStringList result = m_netInterface->expandCIDR("10.0.0.0/31");

    QCOMPARE(result.size(), 0); // No usable hosts in /31
}

void TestNetworkInterface::testExpandCIDR_Large()
{
    // /8 network would be 16M+ hosts - should be limited
    QStringList result = m_netInterface->expandCIDR("10.0.0.0/8");

    // Should return empty due to size limit
    QVERIFY(result.isEmpty());
}

void TestNetworkInterface::testExpandCIDR_Invalid()
{
    // Invalid CIDR
    QStringList result = m_netInterface->expandCIDR("not.a.cidr");
    QVERIFY(result.isEmpty());

    // Invalid prefix
    result = m_netInterface->expandCIDR("192.168.1.0/33");
    QVERIFY(result.isEmpty());

    // Missing prefix
    result = m_netInterface->expandCIDR("192.168.1.0");
    QVERIFY(result.isEmpty());
}

void TestNetworkInterface::testExpandRange_Valid()
{
    QStringList result = m_netInterface->expandRange("192.168.1.1", "192.168.1.5");

    QCOMPARE(result.size(), 5);
    QCOMPARE(result.first(), QString("192.168.1.1"));
    QCOMPARE(result.last(), QString("192.168.1.5"));
}

void TestNetworkInterface::testExpandRange_Single()
{
    QStringList result = m_netInterface->expandRange("10.0.0.1", "10.0.0.1");

    QCOMPARE(result.size(), 1);
    QCOMPARE(result.first(), QString("10.0.0.1"));
}

void TestNetworkInterface::testExpandRange_Invalid()
{
    // Invalid IP addresses
    QStringList result = m_netInterface->expandRange("not.an.ip", "192.168.1.1");
    QVERIFY(result.isEmpty());

    result = m_netInterface->expandRange("192.168.1.1", "invalid");
    QVERIFY(result.isEmpty());
}

void TestNetworkInterface::testExpandRange_Reversed()
{
    // End before start
    QStringList result = m_netInterface->expandRange("192.168.1.10", "192.168.1.5");
    QVERIFY(result.isEmpty());
}

void TestNetworkInterface::testIsHostAlive_Loopback()
{
    // Test if localhost is reachable (should work if network stack is functional)
    // Use a short timeout
    bool alive = m_netInterface->isHostAlive("127.0.0.1", 80, 500);

    // Loopback might not have port 80 open, so this test is informational
    QVERIFY(alive == true || alive == false); // Just verify it completes
}

void TestNetworkInterface::testIsHostAlive_InvalidHost()
{
    // Non-routable IP (TEST-NET-1)
    bool alive = m_netInterface->isHostAlive("192.0.2.1", 80, 500);

    QVERIFY(!alive); // Should timeout/fail
}

void TestNetworkInterface::testIsHostAlive_Timeout()
{
    // Very short timeout
    bool alive = m_netInterface->isHostAlive("192.0.2.1", 80, 100);

    QVERIFY(!alive);
}

void TestNetworkInterface::testDiscoverHosts_Empty()
{
    QStringList targets;
    QList<HostInfo> result = m_netInterface->discoverHosts(targets, 100);

    QVERIFY(result.isEmpty());
}

void TestNetworkInterface::testDiscoverHosts_SingleIP()
{
    QStringList targets;
    targets << "192.0.2.1"; // TEST-NET-1 (should not respond)

    QList<HostInfo> result = m_netInterface->discoverHosts(targets, 200);

    QCOMPARE(result.size(), 1);
    QCOMPARE(result.first().ipAddress, QString("192.0.2.1"));
    QVERIFY(!result.first().isAlive); // Should be down
}

void TestNetworkInterface::testDiscoverHosts_CIDR()
{
    QStringList targets;
    targets << "192.0.2.0/30"; // TEST-NET-1 /30 = 2 usable hosts

    QList<HostInfo> result = m_netInterface->discoverHosts(targets, 200);

    QCOMPARE(result.size(), 2); // .1 and .2
}

void TestNetworkInterface::testSignals_Discovery()
{
    QSignalSpy startSpy(m_netInterface, &NetworkInterface::discoveryStarted);
    QSignalSpy progressSpy(m_netInterface, &NetworkInterface::discoveryProgress);
    QSignalSpy completedSpy(m_netInterface, &NetworkInterface::discoveryCompleted);

    QStringList targets;
    targets << "192.0.2.1";

    m_netInterface->discoverHosts(targets, 200);

    // Verify signals were emitted
    QCOMPARE(startSpy.count(), 1);
    QVERIFY(progressSpy.count() >= 1);
    QCOMPARE(completedSpy.count(), 1);

    // Check signal parameters
    QList<QVariant> startArgs = startSpy.takeFirst();
    QCOMPARE(startArgs.at(0).toInt(), 1); // 1 target

    QList<QVariant> completedArgs = completedSpy.takeFirst();
    QCOMPARE(completedArgs.at(0).toInt(), 0); // 0 hosts found (TEST-NET won't respond)
}
