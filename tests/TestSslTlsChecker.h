#ifndef TESTSSLTLSCHECKER_H
#define TESTSSLTLSCHECKER_H

#include <QObject>
#include <QtTest>

/**
 * @class TestSslTlsChecker
 * @brief Unit tests for SslTlsChecker SSL/TLS security analysis
 *
 * Tests SSL/TLS protocol detection, cipher suite analysis,
 * and certificate validation functionality.
 */
class TestSslTlsChecker : public QObject
{
    Q_OBJECT

private slots:
    // Test fixture setup/teardown
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    // Basic functionality tests
    void testCheckerInitialization();
    void testCheckHostInvalidHost();
    void testCheckHostTimeout();
    void testIsCheckingState();
    void testCancelCheck();

    // SSL/TLS handshake tests
    void testSslHandshakeSuccess();
    void testSslHandshakeFailure();
    void testSslErrorHandling();

    // Protocol analysis tests
    void testProtocolDetection();
    void testDeprecatedProtocolDetection();

    // Cipher suite tests
    void testCipherSuiteDetection();
    void testWeakCipherDetection();
    void testStrongCipherAcceptance();

    // Certificate validation tests
    void testCertificateValidation();
    void testExpiredCertificateDetection();
    void testCertificateExpiringSoonDetection();
    void testSelfSignedCertificateDetection();
    void testCertificateChainValidation();

    // Signal tests
    void testFinishedSignal();
    void testErrorSignal();
    void testVulnerabilityDetectedSignal();

    // Result tests
    void testGetLastResult();
    void testResultWithVulnerabilities();
    void testResultWithoutVulnerabilities();

private:
    // Helper methods
    bool waitForSignal(QObject* sender, const char* signal, int timeout = 15000);
};

#endif // TESTSSLTLSCHECKER_H
