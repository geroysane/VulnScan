#ifndef PORTSCANNER_H
#define PORTSCANNER_H

#include "IScanner.h"
#include "PortResult.h"
#include <QObject>
#include <QVector>
#include <QString>
#include <QAtomicInt>
#include <QMutex>
#include <QThreadPool>

/**
 * @class PortScanner
 * @brief Multi-threaded TCP port scanner implementation
 *
 * This class implements parallel port scanning using a thread pool.
 * It provides progress tracking, cancellation support, and configurable
 * timeout and thread count.
 *
 * Features:
 * - Multi-threaded scanning with QThreadPool
 * - TCP connect() scan
 * - Configurable timeout and thread count
 * - Progress tracking via Qt signals
 * - Cancellation support
 * - Thread-safe result collection
 *
 * Usage example:
 * @code
 * PortScanner scanner;
 * scanner.setTarget("192.168.1.1");
 * scanner.setPortRange(1, 1000);
 * scanner.setThreadCount(100);
 * scanner.setTimeout(2000);
 * connect(&scanner, &PortScanner::scanProgress, this, &MyClass::onProgress);
 * connect(&scanner, &PortScanner::scanCompleted, this, &MyClass::onCompleted);
 * scanner.startScan();
 * @endcode
 */
class PortScanner : public QObject, public IScanner {
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent QObject
     */
    explicit PortScanner(QObject *parent = nullptr);

    /**
     * @brief Destructor - waits for all threads to complete
     */
    ~PortScanner() override;

    /**
     * @brief Set the target host to scan
     * @param target IP address or hostname
     */
    void setTarget(const QString &target);

    /**
     * @brief Set the port range to scan
     * @param startPort First port in range (inclusive)
     * @param endPort Last port in range (inclusive)
     */
    void setPortRange(int startPort, int endPort);

    /**
     * @brief Set individual ports to scan
     * @param ports Vector of port numbers
     */
    void setPorts(const QVector<int> &ports);

    /**
     * @brief Set connection timeout per port
     * @param milliseconds Timeout in milliseconds (default: 2000ms)
     */
    void setTimeout(int milliseconds);

    /**
     * @brief Set number of concurrent threads
     * @param count Thread count (default: 100)
     */
    void setThreadCount(int count);

    /**
     * @brief Get current scan results
     * @return Vector of PortResult for all scanned ports
     */
    QVector<PortResult> getResults() const;

    /**
     * @brief Get only open ports from results
     * @return Vector of PortResult for open ports only
     */
    QVector<PortResult> getOpenPorts() const;

    // IScanner interface implementation
    void startScan() override;
    void stopScan() override;
    bool isScanning() const override;

signals:
    /**
     * @brief Emitted when scan progress updates
     * @param current Number of ports scanned
     * @param total Total number of ports to scan
     */
    void scanProgress(int current, int total);

    /**
     * @brief Emitted when a port is found open
     * @param result PortResult for the open port
     */
    void portFound(const PortResult &result);

    /**
     * @brief Emitted when scan completes (successfully or cancelled)
     * @param cancelled true if scan was cancelled
     */
    void scanCompleted(bool cancelled);

    /**
     * @brief Emitted when an error occurs
     * @param error Error message
     */
    void scanError(const QString &error);

private slots:
    /**
     * @brief Internal slot called when a port scan completes
     * @param result Result of the port scan
     */
    void onPortScanned(const PortResult &result);

private:
    QString m_target;                    ///< Target host
    QVector<int> m_ports;                ///< Ports to scan
    int m_timeout;                       ///< Connection timeout (ms)
    int m_threadCount;                   ///< Number of threads
    QThreadPool *m_threadPool;           ///< Thread pool for workers

    mutable QMutex m_mutex;              ///< Mutex for thread-safe access
    QVector<PortResult> m_results;       ///< Scan results
    QAtomicInt m_scannedCount;           ///< Number of ports scanned
    QAtomicInt m_totalPorts;             ///< Total ports to scan
    QAtomicInt m_isRunning;              ///< Scanning flag (atomic)
    QAtomicInt m_isCancelled;            ///< Cancellation flag (atomic)

    /**
     * @brief Validates scan configuration
     * @return Error message if invalid, empty string if valid
     */
    QString validateConfiguration() const;

    /**
     * @brief Resets internal state for new scan
     */
    void reset();
};

#endif // PORTSCANNER_H
