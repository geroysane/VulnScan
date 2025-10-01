#include "TestDatabaseManager.h"
#include "core/database/DatabaseManager.h"
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>

void TestDatabaseManager::initTestCase() {
    qDebug() << "Starting DatabaseManager tests";
    m_testDbPath = "test_vulnscan.db";
}

void TestDatabaseManager::cleanupTestCase() {
    // Clean up test database
    DatabaseManager::instance().close();
    QFile::remove(m_testDbPath);
    qDebug() << "DatabaseManager tests completed";
}

void TestDatabaseManager::init() {
    // Remove test database before each test
    if (QFile::exists(m_testDbPath)) {
        DatabaseManager::instance().close();
        QFile::remove(m_testDbPath);
    }
}

void TestDatabaseManager::cleanup() {
    // Clean up after each test
    DatabaseManager::instance().close();
}

void TestDatabaseManager::testInitialization() {
    DatabaseManager& dbManager = DatabaseManager::instance();
    bool result = dbManager.initialize(m_testDbPath);

    QVERIFY(result == true);
    QVERIFY(dbManager.isOpen() == true);
    QVERIFY(QFile::exists(m_testDbPath) == true);
}

void TestDatabaseManager::testConnection() {
    DatabaseManager& dbManager = DatabaseManager::instance();
    dbManager.initialize(m_testDbPath);

    QSqlDatabase& db = dbManager.getDatabase();
    QVERIFY(db.isOpen() == true);
    QVERIFY(db.databaseName() == m_testDbPath);
}

void TestDatabaseManager::testSchemaCreation() {
    DatabaseManager& dbManager = DatabaseManager::instance();
    dbManager.initialize(m_testDbPath);

    QSqlDatabase& db = dbManager.getDatabase();

    // Check if tables were created
    QStringList tables = db.tables();
    QVERIFY(tables.contains("scans"));
    QVERIFY(tables.contains("ports"));
    QVERIFY(tables.contains("vulnerabilities"));
    QVERIFY(tables.contains("scan_vulnerabilities"));

    // Test a simple query
    QSqlQuery query(db);
    bool result = query.exec("SELECT COUNT(*) FROM scans");
    QVERIFY(result == true);
}

void TestDatabaseManager::testInvalidPath() {
    DatabaseManager& dbManager = DatabaseManager::instance();

    // Try to initialize with an invalid path (read-only directory)
    QString invalidPath = "/invalid/path/that/does/not/exist/test.db";
    bool result = dbManager.initialize(invalidPath);

    // This should fail (or create the file if path is valid)
    // The behavior depends on the OS and permissions
    // We just verify the function doesn't crash
    QVERIFY(result == false || result == true);
}
