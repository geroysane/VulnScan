#include <QtTest>
#include "TestDatabaseManager.h"
#include "TestConfiguration.h"

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

    return status;
}
