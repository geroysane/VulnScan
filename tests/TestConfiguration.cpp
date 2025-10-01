#include "TestConfiguration.h"
#include "core/config/Configuration.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

void TestConfiguration::initTestCase() {
    qDebug() << "Starting Configuration tests";
    m_testConfigPath = "test_config.json";
}

void TestConfiguration::cleanupTestCase() {
    // Clean up test configuration file
    QFile::remove(m_testConfigPath);
    qDebug() << "Configuration tests completed";
}

void TestConfiguration::init() {
    // Remove test config before each test
    if (QFile::exists(m_testConfigPath)) {
        QFile::remove(m_testConfigPath);
    }

    // Reload defaults
    Configuration::instance().loadDefaults();
}

void TestConfiguration::cleanup() {
    // Clean up after each test
    if (QFile::exists(m_testConfigPath)) {
        QFile::remove(m_testConfigPath);
    }
}

void TestConfiguration::testDefaults() {
    Configuration& config = Configuration::instance();
    config.loadDefaults();

    // Test default values
    QVERIFY(config.get("network.timeout").toInt() == 5000);
    QVERIFY(config.get("scan.portRange").toString() == "1-1000");
    QVERIFY(config.get("ai.enabled").toBool() == false);
}

void TestConfiguration::testGetSet() {
    Configuration& config = Configuration::instance();

    // Set a value
    config.set("test.value", 12345);

    // Get the value back
    int value = config.get("test.value").toInt();
    QVERIFY(value == 12345);

    // Test with default value
    int defaultValue = config.get("nonexistent.key", 999).toInt();
    QVERIFY(defaultValue == 999);
}

void TestConfiguration::testNestedKeys() {
    Configuration& config = Configuration::instance();

    // Set nested value
    config.set("level1.level2.level3", "deep value");

    // Get nested value
    QString value = config.get("level1.level2.level3").toString();
    QVERIFY(value == "deep value");

    // Verify parent exists
    QVERIFY(config.contains("level1.level2"));
}

void TestConfiguration::testLoadSave() {
    Configuration& config = Configuration::instance();

    // Set some values
    config.set("test.string", "hello");
    config.set("test.number", 42);
    config.set("test.boolean", true);

    // Save to file
    bool saveResult = config.save(m_testConfigPath);
    QVERIFY(saveResult == true);
    QVERIFY(QFile::exists(m_testConfigPath) == true);

    // Reload defaults to clear current config
    config.loadDefaults();

    // Load from file
    bool loadResult = config.load(m_testConfigPath);
    QVERIFY(loadResult == true);

    // Verify values
    QVERIFY(config.get("test.string").toString() == "hello");
    QVERIFY(config.get("test.number").toInt() == 42);
    QVERIFY(config.get("test.boolean").toBool() == true);
}

void TestConfiguration::testContains() {
    Configuration& config = Configuration::instance();
    config.loadDefaults();

    // Test existing keys
    QVERIFY(config.contains("network.timeout") == true);
    QVERIFY(config.contains("scan.portRange") == true);

    // Test non-existing keys
    QVERIFY(config.contains("nonexistent.key") == false);
}

void TestConfiguration::testInvalidJson() {
    Configuration& config = Configuration::instance();

    // Create invalid JSON file
    QFile file(m_testConfigPath);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write("{ invalid json content }");
    file.close();

    // Try to load invalid JSON
    bool result = config.load(m_testConfigPath);
    QVERIFY(result == false);
}
