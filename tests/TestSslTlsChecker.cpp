#include "TestSslTlsChecker.h"
#include "../src/core/vulnerability/SslTlsChecker.h"
#include <QSignalSpy>
#include <QEventLoop>
#include <QTimer>

void TestSslTlsChecker::initTestCase()
{
    // Test case initialization
    qRegisterMetaType<SslTlsResult>("SslTlsResult");
    qRegisterMetaType<Vulnerability>("Vulnerability");
}

void TestSslTlsChecker::cleanupTestCase()
{
    // Test case cleanup
}

void TestSslTlsChecker::init()
{
    // Per-test initialization
}

void TestSslTlsChecker::cleanup()
{
    // Per-test cleanup
}

void TestSslTlsChecker::testCheckerInitialization()
{
    SslTlsChecker checker;
    QVERIFY(!checker.isChecking());

    SslTlsResult result = checker.getLastResult();
    QVERIFY(result.host.isEmpty());
    QCOMPARE(result.port, 0);
    QVERIFY(!result.sslSupported);
}

void TestSslTlsChecker::testCheckHostInvalidHost()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    checker.checkHost("invalid.nonexistent.host.example", 443, 3000);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 5000));
    QCOMPARE(finishedSpy.count(), 1);

    SslTlsResult result = checker.getLastResult();
    QVERIFY(!result.sslSupported);
}

void TestSslTlsChecker::testCheckHostTimeout()
{
    SslTlsChecker checker;
    QSignalSpy errorSpy(&checker, &SslTlsChecker::error);
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    // Use a non-routable IP address (RFC 5737)
    checker.checkHost("192.0.2.1", 443, 2000);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 3000));
    QVERIFY(errorSpy.count() >= 1);
}

void TestSslTlsChecker::testIsCheckingState()
{
    SslTlsChecker checker;
    QVERIFY(!checker.isChecking());

    checker.checkHost("www.google.com", 443, 10000);
    // Note: State might change immediately or after event loop

    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);
    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    QVERIFY(!checker.isChecking());
}

void TestSslTlsChecker::testCancelCheck()
{
    SslTlsChecker checker;

    checker.checkHost("www.google.com", 443, 30000);
    checker.cancel();

    QVERIFY(!checker.isChecking());
}

void TestSslTlsChecker::testSslHandshakeSuccess()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    // Test with a known good SSL/TLS host
    checker.checkHost("www.google.com", 443);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));
    QCOMPARE(finishedSpy.count(), 1);

    SslTlsResult result = checker.getLastResult();
    QVERIFY(result.sslSupported);
    QCOMPARE(result.host, QString("www.google.com"));
    QCOMPARE(result.port, 443);
}

void TestSslTlsChecker::testSslHandshakeFailure()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    // Test with a non-SSL port
    checker.checkHost("www.google.com", 80, 3000);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 5000));

    SslTlsResult result = checker.getLastResult();
    QVERIFY(!result.sslSupported);
}

void TestSslTlsChecker::testSslErrorHandling()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    // Test with self-signed certificate host (badssl.com)
    checker.checkHost("self-signed.badssl.com", 443, 10000);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    // Should complete handshake but report certificate errors
    QVERIFY(result.sslSupported);
    QVERIFY(!result.certificateError.isEmpty());
}

void TestSslTlsChecker::testProtocolDetection()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    checker.checkHost("www.google.com", 443);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    QVERIFY(!result.protocol.isEmpty());
    QVERIFY(result.protocol.contains("TLS") || result.protocol.contains("SSL"));
}

void TestSslTlsChecker::testDeprecatedProtocolDetection()
{
    // Note: Modern servers should not support deprecated protocols
    // This test verifies that deprecated protocols are flagged if found
    SslTlsChecker checker;
    QSignalSpy vulnSpy(&checker, &SslTlsChecker::vulnerabilityDetected);
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    // Test with a known TLS 1.0 only host (if available)
    // For now, verify the detection logic exists
    checker.checkHost("tls-v1-0.badssl.com", 1010, 10000);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    if (result.protocol.contains("TLS 1.0") || result.protocol.contains("TLS 1.1")) {
        // Should have detected deprecated protocol vulnerability
        QVERIFY(result.vulnerabilities.size() > 0);
    }
}

void TestSslTlsChecker::testCipherSuiteDetection()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    checker.checkHost("www.google.com", 443);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    QVERIFY(result.ciphers.size() > 0);
}

void TestSslTlsChecker::testWeakCipherDetection()
{
    // Note: Modern servers should not use weak ciphers
    // This test verifies the detection logic
    SslTlsChecker checker;
    QSignalSpy vulnSpy(&checker, &SslTlsChecker::vulnerabilityDetected);
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    // Test with a known weak cipher host (if available)
    checker.checkHost("rc4.badssl.com", 443, 10000);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    if (result.sslSupported && result.ciphers.size() > 0) {
        // Check if weak cipher vulnerability was detected
        bool foundWeakCipher = false;
        for (const Vulnerability& vuln : result.vulnerabilities) {
            if (vuln.description.contains("Weak Cipher")) {
                foundWeakCipher = true;
                break;
            }
        }
        // Weak cipher detection should work if weak cipher is used
        if (result.ciphers[0].name().contains("RC4")) {
            QVERIFY(foundWeakCipher);
        }
    }
}

void TestSslTlsChecker::testStrongCipherAcceptance()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    checker.checkHost("www.google.com", 443);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    // Google should use strong ciphers
    if (result.sslSupported && result.ciphers.size() > 0) {
        QVERIFY(result.ciphers[0].usedBits() >= 128);
    }
}

void TestSslTlsChecker::testCertificateValidation()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    checker.checkHost("www.google.com", 443);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    QVERIFY(!result.certificate.isNull());
    QVERIFY(result.certificateChain.size() > 0);
}

void TestSslTlsChecker::testExpiredCertificateDetection()
{
    SslTlsChecker checker;
    QSignalSpy vulnSpy(&checker, &SslTlsChecker::vulnerabilityDetected);
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    // Test with expired certificate host
    checker.checkHost("expired.badssl.com", 443, 10000);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    if (result.sslSupported) {
        // Should detect expired certificate vulnerability
        bool foundExpired = false;
        for (const Vulnerability& vuln : result.vulnerabilities) {
            if (vuln.description.contains("Expired Certificate")) {
                foundExpired = true;
                QCOMPARE(vuln.severity, QString("CRITICAL"));
                break;
            }
        }
        QVERIFY(foundExpired);
    }
}

void TestSslTlsChecker::testCertificateExpiringSoonDetection()
{
    // This test would require a certificate expiring within 30 days
    // For now, verify the logic exists by checking a valid certificate
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    checker.checkHost("www.google.com", 443);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    // Google's certificate should not be expiring soon
    bool foundExpiringSoon = false;
    for (const Vulnerability& vuln : result.vulnerabilities) {
        if (vuln.description.contains("Expiring Soon")) {
            foundExpiringSoon = true;
        }
    }
    QVERIFY(!foundExpiringSoon);
}

void TestSslTlsChecker::testSelfSignedCertificateDetection()
{
    SslTlsChecker checker;
    QSignalSpy vulnSpy(&checker, &SslTlsChecker::vulnerabilityDetected);
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    // Test with self-signed certificate host
    checker.checkHost("self-signed.badssl.com", 443, 10000);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    if (result.sslSupported) {
        // Should detect self-signed certificate vulnerability
        bool foundSelfSigned = false;
        for (const Vulnerability& vuln : result.vulnerabilities) {
            if (vuln.description.contains("Self-Signed Certificate")) {
                foundSelfSigned = true;
                QCOMPARE(vuln.severity, QString("MEDIUM"));
                break;
            }
        }
        QVERIFY(foundSelfSigned);
    }
}

void TestSslTlsChecker::testCertificateChainValidation()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    checker.checkHost("www.google.com", 443);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    // Google should have a valid certificate chain
    QVERIFY(result.certificateChain.size() > 1);
}

void TestSslTlsChecker::testFinishedSignal()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    checker.checkHost("www.google.com", 443);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));
    QCOMPARE(finishedSpy.count(), 1);

    // Check signal parameters
    QList<QVariant> arguments = finishedSpy.takeFirst();
    SslTlsResult result = arguments.at(0).value<SslTlsResult>();
    QCOMPARE(result.host, QString("www.google.com"));
    QCOMPARE(result.port, 443);
}

void TestSslTlsChecker::testErrorSignal()
{
    SslTlsChecker checker;
    QSignalSpy errorSpy(&checker, &SslTlsChecker::error);

    // Trigger timeout error
    checker.checkHost("192.0.2.1", 443, 1000);

    QVERIFY(waitForSignal(&checker, SIGNAL(error(QString)), 3000));
    QVERIFY(errorSpy.count() >= 1);
}

void TestSslTlsChecker::testVulnerabilityDetectedSignal()
{
    SslTlsChecker checker;
    QSignalSpy vulnSpy(&checker, &SslTlsChecker::vulnerabilityDetected);
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    // Test with expired certificate
    checker.checkHost("expired.badssl.com", 443, 10000);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    if (vulnSpy.count() > 0) {
        QList<QVariant> arguments = vulnSpy.takeFirst();
        Vulnerability vuln = arguments.at(0).value<Vulnerability>();
        QVERIFY(!vuln.description.isEmpty());
        QVERIFY(!vuln.severity.isEmpty());
    }
}

void TestSslTlsChecker::testGetLastResult()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    checker.checkHost("www.google.com", 443);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    QCOMPARE(result.host, QString("www.google.com"));
    QCOMPARE(result.port, 443);
    QVERIFY(result.sslSupported);
}

void TestSslTlsChecker::testResultWithVulnerabilities()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    // Test with expired certificate
    checker.checkHost("expired.badssl.com", 443, 10000);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    if (result.sslSupported) {
        QVERIFY(result.vulnerabilities.size() > 0);
    }
}

void TestSslTlsChecker::testResultWithoutVulnerabilities()
{
    SslTlsChecker checker;
    QSignalSpy finishedSpy(&checker, &SslTlsChecker::finished);

    // Test with a secure host
    checker.checkHost("www.google.com", 443);

    QVERIFY(waitForSignal(&checker, SIGNAL(finished(SslTlsResult)), 15000));

    SslTlsResult result = checker.getLastResult();
    // Google should have minimal or no vulnerabilities
    // (might have some based on detection criteria, but should be secure overall)
    QVERIFY(result.sslSupported);
}

bool TestSslTlsChecker::waitForSignal(QObject* sender, const char* signal, int timeout)
{
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);

    connect(sender, signal, &loop, SLOT(quit()));
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    timer.start(timeout);
    loop.exec();

    return timer.isActive();
}
