#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QString>

/**
 * @class DatabaseManager
 * @brief Manages SQLite database connection and initialization
 *
 * This class implements the Singleton pattern for database management.
 * It handles database connection, initialization, and provides access
 * to the database instance throughout the application.
 */
class DatabaseManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Gets the singleton instance of DatabaseManager
     * @return Reference to the DatabaseManager instance
     */
    static DatabaseManager& instance();

    /**
     * @brief Initializes the database with given path
     * @param dbPath Path to the SQLite database file
     * @return true if initialization was successful, false otherwise
     */
    bool initialize(const QString& dbPath);

    /**
     * @brief Gets reference to the database connection
     * @return Reference to QSqlDatabase
     */
    QSqlDatabase& getDatabase();

    /**
     * @brief Checks if database connection is open
     * @return true if database is open, false otherwise
     */
    bool isOpen() const;

    /**
     * @brief Closes the database connection
     */
    void close();

private:
    /**
     * @brief Private constructor for Singleton pattern
     * @param parent Parent QObject
     */
    explicit DatabaseManager(QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~DatabaseManager() override;

    // Delete copy constructor and assignment operator
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    /**
     * @brief Executes the schema SQL file
     * @return true if schema was created successfully
     */
    bool executeSchema();

    QSqlDatabase m_database;    ///< SQLite database connection
    QString m_dbPath;           ///< Path to database file
};

#endif // DATABASEMANAGER_H
