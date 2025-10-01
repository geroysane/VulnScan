#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QCoreApplication>

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

DatabaseManager::DatabaseManager(QObject* parent)
    : QObject(parent)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
}

DatabaseManager::~DatabaseManager() {
    if (m_database.isOpen()) {
        m_database.close();
    }
}

bool DatabaseManager::initialize(const QString& dbPath) {
    m_dbPath = dbPath;
    m_database.setDatabaseName(dbPath);

    if (!m_database.open()) {
        qCritical() << "Failed to open database:" << m_database.lastError().text();
        return false;
    }

    qDebug() << "Database opened successfully:" << dbPath;

    // Execute schema to create tables
    if (!executeSchema()) {
        qCritical() << "Failed to create database schema";
        return false;
    }

    qDebug() << "Database schema created successfully";
    return true;
}

QSqlDatabase& DatabaseManager::getDatabase() {
    return m_database;
}

bool DatabaseManager::isOpen() const {
    return m_database.isOpen();
}

void DatabaseManager::close() {
    if (m_database.isOpen()) {
        m_database.close();
        qDebug() << "Database closed";
    }
}

bool DatabaseManager::executeSchema() {
    // NOTE: Schema is defined in code for reliability.
    // The canonical version is maintained in resources/database/schema.sql
    // Keep both in sync when modifying the database structure.

    // Define schema directly in code as fallback
    QStringList schemaStatements = {
        "CREATE TABLE IF NOT EXISTS scans ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    target_host TEXT NOT NULL,"
        "    scan_start_time DATETIME NOT NULL,"
        "    scan_end_time DATETIME,"
        "    status TEXT NOT NULL,"
        "    created_at DATETIME DEFAULT CURRENT_TIMESTAMP"
        ")",

        "CREATE TABLE IF NOT EXISTS ports ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    scan_id INTEGER NOT NULL,"
        "    port INTEGER NOT NULL,"
        "    protocol TEXT NOT NULL,"
        "    state TEXT NOT NULL,"
        "    service TEXT,"
        "    version TEXT,"
        "    FOREIGN KEY (scan_id) REFERENCES scans(id) ON DELETE CASCADE"
        ")",

        "CREATE TABLE IF NOT EXISTS vulnerabilities ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    cve_id TEXT UNIQUE NOT NULL,"
        "    description TEXT,"
        "    severity TEXT NOT NULL,"
        "    cvss_score REAL,"
        "    published_date DATE,"
        "    modified_date DATE"
        ")",

        "CREATE TABLE IF NOT EXISTS scan_vulnerabilities ("
        "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    scan_id INTEGER NOT NULL,"
        "    vulnerability_id INTEGER NOT NULL,"
        "    affected_service TEXT,"
        "    detected_at DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "    FOREIGN KEY (scan_id) REFERENCES scans(id) ON DELETE CASCADE,"
        "    FOREIGN KEY (vulnerability_id) REFERENCES vulnerabilities(id) ON DELETE CASCADE"
        ")",

        "CREATE INDEX IF NOT EXISTS idx_scans_target ON scans(target_host)",
        "CREATE INDEX IF NOT EXISTS idx_scans_status ON scans(status)",
        "CREATE INDEX IF NOT EXISTS idx_ports_scan ON ports(scan_id)",
        "CREATE INDEX IF NOT EXISTS idx_ports_state ON ports(state)",
        "CREATE INDEX IF NOT EXISTS idx_vulns_cve ON vulnerabilities(cve_id)",
        "CREATE INDEX IF NOT EXISTS idx_vulns_severity ON vulnerabilities(severity)",
        "CREATE INDEX IF NOT EXISTS idx_scan_vulns_scan ON scan_vulnerabilities(scan_id)",
        "CREATE INDEX IF NOT EXISTS idx_scan_vulns_vuln ON scan_vulnerabilities(vulnerability_id)"
    };

    QSqlQuery query(m_database);
    for (const QString& statement : schemaStatements) {
        if (!query.exec(statement)) {
            qCritical() << "Failed to execute SQL statement:"
                       << query.lastError().text();
            qCritical() << "Statement was:" << statement;
            return false;
        }
    }

    qDebug() << "Database schema created successfully (embedded)";
    return true;
}
