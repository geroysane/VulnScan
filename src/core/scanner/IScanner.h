#ifndef ISCANNER_H
#define ISCANNER_H

#include <QObject>

/**
 * @interface IScanner
 * @brief Base interface for all scanner implementations
 *
 * This interface defines the contract for network scanning operations.
 * All concrete scanner implementations must inherit from this interface.
 */
class IScanner {
public:
    virtual ~IScanner() = default;

    /**
     * @brief Starts the scanning operation
     */
    virtual void startScan() = 0;

    /**
     * @brief Stops the current scanning operation
     */
    virtual void stopScan() = 0;

    /**
     * @brief Checks if a scan is currently in progress
     * @return true if scanning, false otherwise
     */
    virtual bool isScanning() const = 0;
};

#endif // ISCANNER_H
