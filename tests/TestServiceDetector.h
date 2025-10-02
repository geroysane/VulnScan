#ifndef TESTSERVICEDETECTOR_H
#define TESTSERVICEDETECTOR_H

#include <QObject>
#include <QtTest>
#include "core/scanner/ServiceDetector.h"

/**
 * @class TestServiceDetector
 * @brief Unit tests for ServiceDetector class
 */
class TestServiceDetector : public QObject {
    Q_OBJECT

private slots:
    // Setup and cleanup
    void init();
    void cleanup();

    // Constructor tests
    void testConstructor();

    // Port mapping tests
    void testGetServiceByPort_TCP();
    void testGetServiceByPort_UDP();
    void testGetServiceByPort_Unknown();

    // Timeout tests
    void testSetTimeout();
    void testGetTimeout();

    // Banner parsing tests
    void testParseBanner_HTTP();
    void testParseBanner_SSH();
    void testParseBanner_FTP();
    void testParseBanner_SMTP();
    void testParseBanner_POP3();
    void testParseBanner_IMAP();
    void testParseBanner_Empty();

    // Service detection tests (integration - requires network)
    void testDetectService_InvalidHost();
    void testDetectService_Timeout();

    // Signal tests
    void testSignals_DetectionStarted();
    void testSignals_DetectionFailed();

private:
    ServiceDetector *m_detector;
};

#endif // TESTSERVICEDETECTOR_H
