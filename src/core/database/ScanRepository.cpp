#include "ScanRepository.h"
#include "DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

ScanRepository::ScanRepository(QObject *parent)
    : QObject(parent)
{
}

ScanRepository::~ScanRepository()
{
}

void ScanRepository::save(const ScanResult &result)
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO scans (target_host, scan_start_time, scan_end_time, status) "
                  "VALUES (:target_host, :scan_start_time, :scan_end_time, :status)");

    query.bindValue(":target_host", result.targetHost);
    query.bindValue(":scan_start_time", result.scanStartTime);
    query.bindValue(":scan_end_time", result.scanEndTime);
    query.bindValue(":status", result.status);

    if (!query.exec()) {
        qWarning() << "Failed to save scan:" << query.lastError().text();
        return;
    }

    // Get the inserted scan ID
    int scanId = query.lastInsertId().toInt();

    // Save port results
    if (!result.ports.isEmpty()) {
        if (!savePorts(scanId, result.ports)) {
            qWarning() << "Failed to save ports for scan ID:" << scanId;
        }
    }
}

std::optional<ScanResult> ScanRepository::findById(int id)
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return std::nullopt;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, target_host, scan_start_time, scan_end_time, status "
                  "FROM scans WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to find scan:" << query.lastError().text();
        return std::nullopt;
    }

    if (query.next()) {
        ScanResult result;
        result.id = query.value(0).toInt();
        result.targetHost = query.value(1).toString();
        result.scanStartTime = query.value(2).toDateTime();
        result.scanEndTime = query.value(3).toDateTime();
        result.status = query.value(4).toString();

        // Load associated ports
        result.ports = loadPorts(result.id);

        return result;
    }

    return std::nullopt;
}

QList<ScanResult> ScanRepository::findAll()
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();
    QList<ScanResult> results;

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return results;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, target_host, scan_start_time, scan_end_time, status "
                  "FROM scans ORDER BY scan_start_time DESC");

    if (!query.exec()) {
        qWarning() << "Failed to fetch all scans:" << query.lastError().text();
        return results;
    }

    while (query.next()) {
        ScanResult result;
        result.id = query.value(0).toInt();
        result.targetHost = query.value(1).toString();
        result.scanStartTime = query.value(2).toDateTime();
        result.scanEndTime = query.value(3).toDateTime();
        result.status = query.value(4).toString();

        // Load associated ports
        result.ports = loadPorts(result.id);

        results.append(result);
    }

    return results;
}

bool ScanRepository::update(const ScanResult &result)
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE scans SET target_host = :target_host, "
                  "scan_start_time = :scan_start_time, "
                  "scan_end_time = :scan_end_time, "
                  "status = :status "
                  "WHERE id = :id");

    query.bindValue(":target_host", result.targetHost);
    query.bindValue(":scan_start_time", result.scanStartTime);
    query.bindValue(":scan_end_time", result.scanEndTime);
    query.bindValue(":status", result.status);
    query.bindValue(":id", result.id);

    if (!query.exec()) {
        qWarning() << "Failed to update scan:" << query.lastError().text();
        return false;
    }

    // Update ports (delete old, insert new)
    deletePorts(result.id);
    if (!result.ports.isEmpty()) {
        savePorts(result.id, result.ports);
    }

    return true;
}

void ScanRepository::remove(int id)
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return;
    }

    // Delete ports first (foreign key constraint)
    deletePorts(id);

    QSqlQuery query(db);
    query.prepare("DELETE FROM scans WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Failed to delete scan:" << query.lastError().text();
    }
}

QList<ScanResult> ScanRepository::findByTarget(const QString &targetHost)
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();
    QList<ScanResult> results;

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return results;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, target_host, scan_start_time, scan_end_time, status "
                  "FROM scans WHERE target_host = :target_host "
                  "ORDER BY scan_start_time DESC");
    query.bindValue(":target_host", targetHost);

    if (!query.exec()) {
        qWarning() << "Failed to find scans by target:" << query.lastError().text();
        return results;
    }

    while (query.next()) {
        ScanResult result;
        result.id = query.value(0).toInt();
        result.targetHost = query.value(1).toString();
        result.scanStartTime = query.value(2).toDateTime();
        result.scanEndTime = query.value(3).toDateTime();
        result.status = query.value(4).toString();
        result.ports = loadPorts(result.id);
        results.append(result);
    }

    return results;
}

QList<ScanResult> ScanRepository::findByStatus(const QString &status)
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();
    QList<ScanResult> results;

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return results;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, target_host, scan_start_time, scan_end_time, status "
                  "FROM scans WHERE status = :status "
                  "ORDER BY scan_start_time DESC");
    query.bindValue(":status", status);

    if (!query.exec()) {
        qWarning() << "Failed to find scans by status:" << query.lastError().text();
        return results;
    }

    while (query.next()) {
        ScanResult result;
        result.id = query.value(0).toInt();
        result.targetHost = query.value(1).toString();
        result.scanStartTime = query.value(2).toDateTime();
        result.scanEndTime = query.value(3).toDateTime();
        result.status = query.value(4).toString();
        result.ports = loadPorts(result.id);
        results.append(result);
    }

    return results;
}

QList<ScanResult> ScanRepository::findRecent(int limit)
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();
    QList<ScanResult> results;

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return results;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, target_host, scan_start_time, scan_end_time, status "
                  "FROM scans ORDER BY scan_start_time DESC LIMIT :limit");
    query.bindValue(":limit", limit);

    if (!query.exec()) {
        qWarning() << "Failed to find recent scans:" << query.lastError().text();
        return results;
    }

    while (query.next()) {
        ScanResult result;
        result.id = query.value(0).toInt();
        result.targetHost = query.value(1).toString();
        result.scanStartTime = query.value(2).toDateTime();
        result.scanEndTime = query.value(3).toDateTime();
        result.status = query.value(4).toString();
        result.ports = loadPorts(result.id);
        results.append(result);
    }

    return results;
}

QList<ScanResult> ScanRepository::findByDateRange(const QDateTime &startDate, const QDateTime &endDate)
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();
    QList<ScanResult> results;

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return results;
    }

    QSqlQuery query(db);
    query.prepare("SELECT id, target_host, scan_start_time, scan_end_time, status "
                  "FROM scans WHERE scan_start_time BETWEEN :start_date AND :end_date "
                  "ORDER BY scan_start_time DESC");
    query.bindValue(":start_date", startDate);
    query.bindValue(":end_date", endDate);

    if (!query.exec()) {
        qWarning() << "Failed to find scans by date range:" << query.lastError().text();
        return results;
    }

    while (query.next()) {
        ScanResult result;
        result.id = query.value(0).toInt();
        result.targetHost = query.value(1).toString();
        result.scanStartTime = query.value(2).toDateTime();
        result.scanEndTime = query.value(3).toDateTime();
        result.status = query.value(4).toString();
        result.ports = loadPorts(result.id);
        results.append(result);
    }

    return results;
}

int ScanRepository::count()
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return 0;
    }

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM scans");

    if (!query.exec() || !query.next()) {
        qWarning() << "Failed to count scans:" << query.lastError().text();
        return 0;
    }

    return query.value(0).toInt();
}

int ScanRepository::deleteOlderThan(int days)
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();

    if (!db.isOpen()) {
        qWarning() << "Database is not open";
        return 0;
    }

    QDateTime cutoffDate = QDateTime::currentDateTime().addDays(-days);

    QSqlQuery query(db);
    query.prepare("DELETE FROM scans WHERE scan_start_time < :cutoff_date");
    query.bindValue(":cutoff_date", cutoffDate);

    if (!query.exec()) {
        qWarning() << "Failed to delete old scans:" << query.lastError().text();
        return 0;
    }

    return query.numRowsAffected();
}

bool ScanRepository::savePorts(int scanId, const QList<PortResult> &ports)
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();

    for (const PortResult &port : ports) {
        QSqlQuery query(db);
        query.prepare("INSERT INTO ports (scan_id, port, protocol, state, service, version) "
                      "VALUES (:scan_id, :port, :protocol, :state, :service, :version)");

        query.bindValue(":scan_id", scanId);
        query.bindValue(":port", port.port);
        query.bindValue(":protocol", port.protocol);
        query.bindValue(":state", port.state);
        query.bindValue(":service", port.service);
        query.bindValue(":version", port.version);

        if (!query.exec()) {
            qWarning() << "Failed to save port:" << query.lastError().text();
            return false;
        }
    }

    return true;
}

QList<PortResult> ScanRepository::loadPorts(int scanId)
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();
    QList<PortResult> ports;

    QSqlQuery query(db);
    query.prepare("SELECT port, protocol, state, service, version "
                  "FROM ports WHERE scan_id = :scan_id ORDER BY port");
    query.bindValue(":scan_id", scanId);

    if (!query.exec()) {
        qWarning() << "Failed to load ports:" << query.lastError().text();
        return ports;
    }

    while (query.next()) {
        PortResult port;
        port.port = query.value(0).toInt();
        port.protocol = query.value(1).toString();
        port.state = query.value(2).toString();
        port.service = query.value(3).toString();
        port.version = query.value(4).toString();
        ports.append(port);
    }

    return ports;
}

bool ScanRepository::deletePorts(int scanId)
{
    QSqlDatabase &db = DatabaseManager::instance().getDatabase();

    QSqlQuery query(db);
    query.prepare("DELETE FROM ports WHERE scan_id = :scan_id");
    query.bindValue(":scan_id", scanId);

    if (!query.exec()) {
        qWarning() << "Failed to delete ports:" << query.lastError().text();
        return false;
    }

    return true;
}
