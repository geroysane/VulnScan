#ifndef SCANREPOSITORY_H
#define SCANREPOSITORY_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDateTime>
#include "IRepository.h"
#include "../scanner/ScanResult.h"
#include "../scanner/PortResult.h"

/**
 * @class ScanRepository
 * @brief Repository for persisting and retrieving scan results
 *
 * This class provides CRUD operations for scan results in the database.
 * Implements the IRepository interface for scan data management.
 */
class ScanRepository : public QObject, public IRepository<ScanResult> {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent QObject
     */
    explicit ScanRepository(QObject *parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ScanRepository() override;

    // IRepository interface implementation
    void save(const ScanResult &result) override;
    std::optional<ScanResult> findById(int id) override;
    QList<ScanResult> findAll() override;
    void remove(int id) override;

    /**
     * @brief Update an existing scan
     * @param result Scan result to update
     * @return true if successful
     */
    bool update(const ScanResult &result);

    /**
     * @brief Find scans by target host
     * @param targetHost Target hostname or IP
     * @return List of scan results for the target
     */
    QList<ScanResult> findByTarget(const QString &targetHost);

    /**
     * @brief Find scans by status
     * @param status Scan status (running, completed, failed, cancelled)
     * @return List of scan results with the given status
     */
    QList<ScanResult> findByStatus(const QString &status);

    /**
     * @brief Find recent scans
     * @param limit Maximum number of scans to return
     * @return List of most recent scan results
     */
    QList<ScanResult> findRecent(int limit = 10);

    /**
     * @brief Find scans within a date range
     * @param startDate Start of date range
     * @param endDate End of date range
     * @return List of scan results in the date range
     */
    QList<ScanResult> findByDateRange(const QDateTime &startDate, const QDateTime &endDate);

    /**
     * @brief Get total number of scans
     * @return Total scan count
     */
    int count();

    /**
     * @brief Delete all scans older than specified days
     * @param days Number of days
     * @return Number of scans deleted
     */
    int deleteOlderThan(int days);

private:
    /**
     * @brief Save port results for a scan
     * @param scanId Scan ID
     * @param ports List of port results
     * @return true if successful
     */
    bool savePorts(int scanId, const QList<PortResult> &ports);

    /**
     * @brief Load port results for a scan
     * @param scanId Scan ID
     * @return List of port results
     */
    QList<PortResult> loadPorts(int scanId);

    /**
     * @brief Delete port results for a scan
     * @param scanId Scan ID
     * @return true if successful
     */
    bool deletePorts(int scanId);

    /**
     * @brief Map database row to ScanResult
     * @return ScanResult from current query row
     */
    ScanResult mapToScanResult();
};

#endif // SCANREPOSITORY_H
