#ifndef TESTCONFIGURATION_H
#define TESTCONFIGURATION_H

#include <QtTest>
#include <QObject>

/**
 * @class TestConfiguration
 * @brief Unit tests for Configuration class
 */
class TestConfiguration : public QObject {
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
    void testDefaults();
    void testGetSet();
    void testNestedKeys();
    void testLoadSave();
    void testContains();
    void testInvalidJson();

private:
    QString m_testConfigPath;
};

#endif // TESTCONFIGURATION_H
