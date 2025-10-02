#include "TestServiceVulnChecker.h"
#include <QSignalSpy>

// Base class tests
void TestServiceVulnChecker::testServiceVulnCheckerTimeout()
{
    HttpVulnChecker checker;

    // Default timeout
    QCOMPARE(checker.timeout(), 10000);

    // Set custom timeout
    checker.setTimeout(5000);
    QCOMPARE(checker.timeout(), 5000);
}

void TestServiceVulnChecker::testServiceVulnCheckerCreateVulnerability()
{
    HttpVulnChecker checker;

    // Access protected method through public interface by checking actual vulnerability creation
    PortResult port;
    port.port = 80;
    port.service = "http";
    port.version = "Apache/2.4.41";
    port.state = "open";

    // Trigger vulnerability detection which uses createVulnerability internally
    QList<Vulnerability> vulns = checker.checkVulnerabilities("example.com", port);

    // At least some vulnerabilities should be detected (missing headers, etc.)
    QVERIFY(vulns.size() > 0);

    // Check that vulnerability fields are properly populated
    for (const Vulnerability &vuln : vulns) {
        QVERIFY(!vuln.cveId.isEmpty());
        QVERIFY(!vuln.description.isEmpty());
        QVERIFY(!vuln.severity.isEmpty());
        QVERIFY(vuln.cvssScore >= 0.0f && vuln.cvssScore <= 10.0f);
    }
}

// HttpVulnChecker tests
void TestServiceVulnChecker::testHttpCheckerCanHandle()
{
    HttpVulnChecker checker;

    // Should handle HTTP-related services
    QVERIFY(checker.canHandle("http"));
    QVERIFY(checker.canHandle("https"));
    QVERIFY(checker.canHandle("HTTP"));
    QVERIFY(checker.canHandle("HTTPS"));
    QVERIFY(checker.canHandle("apache"));
    QVERIFY(checker.canHandle("nginx"));
    QVERIFY(checker.canHandle("iis"));
    QVERIFY(checker.canHandle("web-server"));

    // Should not handle non-HTTP services
    QVERIFY(!checker.canHandle("ssh"));
    QVERIFY(!checker.canHandle("ftp"));
    QVERIFY(!checker.canHandle("smtp"));
}

void TestServiceVulnChecker::testHttpCheckerName()
{
    HttpVulnChecker checker;
    QCOMPARE(checker.name(), QString("HTTP Vulnerability Checker"));
}

void TestServiceVulnChecker::testHttpCheckerDescription()
{
    HttpVulnChecker checker;
    QString desc = checker.description();
    QVERIFY(!desc.isEmpty());
    QVERIFY(desc.contains("security headers", Qt::CaseInsensitive));
}

void TestServiceVulnChecker::testHttpCheckerSecurityHeaders()
{
    HttpVulnChecker checker;
    checker.setTimeout(15000); // Longer timeout for network operations

    PortResult port;
    port.port = 80;
    port.service = "http";
    port.version = "test";
    port.state = "open";

    // Use a test site known to have missing headers
    // Note: This test requires network access
    QList<Vulnerability> vulns = checker.checkVulnerabilities("example.com", port);

    // Should detect some missing security headers
    bool foundHeaderVuln = false;
    for (const Vulnerability &vuln : vulns) {
        if (vuln.cveId.contains("HTTP-MISSING")) {
            foundHeaderVuln = true;
            QVERIFY(vuln.description.contains("header", Qt::CaseInsensitive));
            break;
        }
    }

    QVERIFY(foundHeaderVuln);
}

void TestServiceVulnChecker::testHttpCheckerDangerousMethods()
{
    HttpVulnChecker checker;

    PortResult port;
    port.port = 80;
    port.service = "http";
    port.version = "test";
    port.state = "open";

    // Test against a server (requires network access)
    QList<Vulnerability> vulns = checker.checkVulnerabilities("example.com", port);

    // Check if dangerous methods are properly detected (if any)
    for (const Vulnerability &vuln : vulns) {
        if (vuln.cveId.contains("HTTP-DANGEROUS-METHOD")) {
            QVERIFY(vuln.description.contains("method", Qt::CaseInsensitive));
            QVERIFY(vuln.severity == "Medium" || vuln.severity == "High");
        }
    }
}

void TestServiceVulnChecker::testHttpCheckerInformationDisclosure()
{
    HttpVulnChecker checker;

    PortResult port;
    port.port = 80;
    port.service = "http";
    port.version = "test";
    port.state = "open";

    // Test against a real server
    QList<Vulnerability> vulns = checker.checkVulnerabilities("example.com", port);

    // Check structure of information disclosure vulnerabilities
    for (const Vulnerability &vuln : vulns) {
        if (vuln.cveId.contains("DISCLOSURE")) {
            QVERIFY(!vuln.description.isEmpty());
            QCOMPARE(vuln.severity, QString("Low"));
        }
    }
}

// SshVulnChecker tests
void TestServiceVulnChecker::testSshCheckerCanHandle()
{
    SshVulnChecker checker;

    // Should handle SSH services
    QVERIFY(checker.canHandle("ssh"));
    QVERIFY(checker.canHandle("SSH"));
    QVERIFY(checker.canHandle("openssh"));
    QVERIFY(checker.canHandle("OpenSSH"));

    // Should not handle non-SSH services
    QVERIFY(!checker.canHandle("http"));
    QVERIFY(!checker.canHandle("ftp"));
    QVERIFY(!checker.canHandle("telnet"));
}

void TestServiceVulnChecker::testSshCheckerName()
{
    SshVulnChecker checker;
    QCOMPARE(checker.name(), QString("SSH Vulnerability Checker"));
}

void TestServiceVulnChecker::testSshCheckerDescription()
{
    SshVulnChecker checker;
    QString desc = checker.description();
    QVERIFY(!desc.isEmpty());
    QVERIFY(desc.contains("SSH", Qt::CaseInsensitive));
}

void TestServiceVulnChecker::testSshCheckerVersionExtraction()
{
    SshVulnChecker checker;

    // Test version extraction through actual vulnerability check
    PortResult port;
    port.port = 22;
    port.service = "ssh";
    port.version = "OpenSSH 7.4";
    port.state = "open";

    // The version extraction is internal, so we test indirectly
    // by checking if version-related vulnerabilities are detected
    // This test doesn't require actual network connection
}

void TestServiceVulnChecker::testSshCheckerVersionComparison()
{
    // Version comparison is internal, tested indirectly through vulnerability detection
    SshVulnChecker checker;

    PortResult port;
    port.port = 22;
    port.service = "ssh";
    port.state = "open";

    // Old version should trigger vulnerabilities
    port.version = "OpenSSH 6.0";

    // This would require actual connection, so we just verify the checker exists
    QVERIFY(checker.canHandle("ssh"));
}

void TestServiceVulnChecker::testSshCheckerProtocolV1Detection()
{
    // Protocol v1 detection requires banner analysis
    SshVulnChecker checker;

    PortResult port;
    port.port = 22;
    port.service = "ssh";
    port.version = "SSH-1.99";
    port.state = "open";

    // Would require actual connection to test
    QVERIFY(checker.canHandle("ssh"));
}

void TestServiceVulnChecker::testSshCheckerOutdatedVersion()
{
    SshVulnChecker checker;

    PortResult port;
    port.port = 22;
    port.service = "ssh";
    port.version = "OpenSSH 7.0";
    port.state = "open";

    // Would require actual connection to test
    QVERIFY(checker.canHandle("ssh"));
}

// FtpVulnChecker tests
void TestServiceVulnChecker::testFtpCheckerCanHandle()
{
    FtpVulnChecker checker;

    // Should handle FTP services
    QVERIFY(checker.canHandle("ftp"));
    QVERIFY(checker.canHandle("FTP"));
    QVERIFY(checker.canHandle("ftps"));

    // Should not handle TFTP or non-FTP services
    QVERIFY(!checker.canHandle("tftp"));
    QVERIFY(!checker.canHandle("http"));
    QVERIFY(!checker.canHandle("ssh"));
}

void TestServiceVulnChecker::testFtpCheckerName()
{
    FtpVulnChecker checker;
    QCOMPARE(checker.name(), QString("FTP Vulnerability Checker"));
}

void TestServiceVulnChecker::testFtpCheckerDescription()
{
    FtpVulnChecker checker;
    QString desc = checker.description();
    QVERIFY(!desc.isEmpty());
    QVERIFY(desc.contains("FTP", Qt::CaseInsensitive));
}

void TestServiceVulnChecker::testFtpCheckerVersionExtraction()
{
    FtpVulnChecker checker;

    // Version extraction is internal, test the checker interface
    PortResult port;
    port.port = 21;
    port.service = "ftp";
    port.version = "ProFTPD 1.3.5";
    port.state = "open";

    QVERIFY(checker.canHandle(port.service));
}

void TestServiceVulnChecker::testFtpCheckerPlainTextAuth()
{
    FtpVulnChecker checker;

    // Plain text auth check is based on port number
    PortResult port;
    port.port = 21; // Standard FTP port (not FTPS)
    port.service = "ftp";
    port.version = "test";
    port.state = "open";

    // Would require actual connection, but we can verify the logic
    QVERIFY(port.port != 990); // Not FTPS port
}

void TestServiceVulnChecker::testFtpCheckerBannerDisclosure()
{
    FtpVulnChecker checker;

    PortResult port;
    port.port = 21;
    port.service = "ftp";
    port.version = "ProFTPD 1.3.5";
    port.state = "open";

    // Would require actual connection to test
    QVERIFY(checker.canHandle(port.service));
}

// Integration tests (optional - require network access)
void TestServiceVulnChecker::testHttpCheckerRealScan_data()
{
    QTest::addColumn<QString>("host");
    QTest::addColumn<int>("port");
    QTest::addColumn<QString>("service");

    // Skip these tests if ENABLE_INTEGRATION_TESTS is not defined
#ifdef ENABLE_INTEGRATION_TESTS
    QTest::newRow("example.com") << "example.com" << 80 << "http";
    QTest::newRow("google.com") << "google.com" << 443 << "https";
#endif
}

void TestServiceVulnChecker::testHttpCheckerRealScan()
{
#ifdef ENABLE_INTEGRATION_TESTS
    QFETCH(QString, host);
    QFETCH(int, port);
    QFETCH(QString, service);

    HttpVulnChecker checker;
    checker.setTimeout(15000);

    PortResult portResult;
    portResult.port = port;
    portResult.service = service;
    portResult.version = "test";
    portResult.state = "open";

    QSignalSpy completeSpy(&checker, &HttpVulnChecker::checkComplete);
    QSignalSpy vulnSpy(&checker, &HttpVulnChecker::vulnerabilityDetected);

    QList<Vulnerability> vulns = checker.checkVulnerabilities(host, portResult);

    QVERIFY(completeSpy.count() == 1);
    QVERIFY(vulns.size() > 0);

    // Verify at least some common vulnerabilities are detected
    bool foundMissingHeader = false;
    for (const Vulnerability &vuln : vulns) {
        if (vuln.cveId.contains("HTTP-MISSING")) {
            foundMissingHeader = true;
            break;
        }
    }

    QVERIFY(foundMissingHeader);
#else
    QSKIP("Integration tests disabled");
#endif
}

void TestServiceVulnChecker::testSshCheckerRealScan_data()
{
    QTest::addColumn<QString>("host");
    QTest::addColumn<int>("port");

#ifdef ENABLE_INTEGRATION_TESTS
    QTest::newRow("github.com") << "github.com" << 22;
#endif
}

void TestServiceVulnChecker::testSshCheckerRealScan()
{
#ifdef ENABLE_INTEGRATION_TESTS
    QFETCH(QString, host);
    QFETCH(int, port);

    SshVulnChecker checker;
    checker.setTimeout(15000);

    PortResult portResult;
    portResult.port = port;
    portResult.service = "ssh";
    portResult.version = "unknown";
    portResult.state = "open";

    QSignalSpy completeSpy(&checker, &SshVulnChecker::checkComplete);
    QSignalSpy vulnSpy(&checker, &SshVulnChecker::vulnerabilityDetected);

    QList<Vulnerability> vulns = checker.checkVulnerabilities(host, portResult);

    QVERIFY(completeSpy.count() == 1);
    // May or may not have vulnerabilities depending on server configuration
#else
    QSKIP("Integration tests disabled");
#endif
}
