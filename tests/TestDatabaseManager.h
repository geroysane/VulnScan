#ifndef TESTDATABASEMANAGER_H
#define TESTDATABASEMANAGER_H

#include <QtTest>
#include <QObject>

/**
 * @class TestDatabaseManager
 * @brief Unit tests for DatabaseManager class
 */
class TestDatabaseManager : public QObject {
    Q_OBJECT

private slots:
    /**
     * @brief Called before the first test function
     */
    void initTestCase();

    /**
     * @brief Called after the last test function
     */
    void cleanupTestCase();

    /**
     * @brief Called before each test function
     */
    void init();

    /**
     * @brief Called after each test function
     */
    void cleanup();

    // Test functions
    void testInitialization();
    void testConnection();
    void testSchemaCreation();
    void testInvalidPath();

private:
    QString m_testDbPath;
};

#endif // TESTDATABASEMANAGER_H
