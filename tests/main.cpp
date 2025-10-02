#include <QtTest>
#include "TestDatabaseManager.h"
#include "TestConfiguration.h"
#include "TestPortScanner.h"
#include "TestServiceDetector.h"
#include "TestDnsResolver.h"
#include "TestNetworkInterface.h"
#include "TestScanEngine.h"

/**
 * @brief Main entry point for unit tests
 *
 * This function runs all registered test classes.
 */
int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    int status = 0;

    // Run DatabaseManager tests
    {
        TestDatabaseManager test;
        status |= QTest::qExec(&test, argc, argv);
    }

    // Run Configuration tests
    {
        TestConfiguration test;
        status |= QTest::qExec(&test, argc, argv);
    }

    // Run PortScanner tests
    {
        TestPortScanner test;
        status |= QTest::qExec(&test, argc, argv);
    }

    // Run ServiceDetector tests
    {
        TestServiceDetector test;
        status |= QTest::qExec(&test, argc, argv);
    }

    // Run DnsResolver tests
    {
        TestDnsResolver test;
        status |= QTest::qExec(&test, argc, argv);
    }

    // Run NetworkInterface tests
    {
        TestNetworkInterface test;
        status |= QTest::qExec(&test, argc, argv);
    }

    // Run ScanEngine tests
    {
        TestScanEngine test;
        status |= QTest::qExec(&test, argc, argv);
    }

    return status;
}
