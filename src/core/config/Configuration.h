#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QObject>
#include <QVariant>
#include <QVariantMap>
#include <QString>

/**
 * @class Configuration
 * @brief Application configuration manager
 *
 * This class implements the Singleton pattern for configuration management.
 * It handles loading, saving, and accessing application settings stored
 * in JSON format.
 */
class Configuration : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Gets the singleton instance of Configuration
     * @return Reference to the Configuration instance
     */
    static Configuration& instance();

    /**
     * @brief Loads configuration from a JSON file
     * @param configPath Path to the configuration file
     * @return true if loading was successful, false otherwise
     */
    bool load(const QString& configPath);

    /**
     * @brief Saves current configuration to a JSON file
     * @param configPath Path where configuration will be saved
     * @return true if saving was successful, false otherwise
     */
    bool save(const QString& configPath);

    /**
     * @brief Gets a configuration value
     * @param key Configuration key (supports dot notation like "network.timeout")
     * @param defaultValue Default value if key doesn't exist
     * @return Configuration value or default
     */
    QVariant get(const QString& key, const QVariant& defaultValue = QVariant()) const;

    /**
     * @brief Sets a configuration value
     * @param key Configuration key (supports dot notation)
     * @param value Value to set
     */
    void set(const QString& key, const QVariant& value);

    /**
     * @brief Checks if a configuration key exists
     * @param key Configuration key to check
     * @return true if key exists, false otherwise
     */
    bool contains(const QString& key) const;

    /**
     * @brief Loads default configuration
     */
    void loadDefaults();

signals:
    /**
     * @brief Signal emitted when configuration changes
     * @param key The configuration key that changed
     */
    void configurationChanged(const QString& key);

private:
    /**
     * @brief Private constructor for Singleton pattern
     * @param parent Parent QObject
     */
    explicit Configuration(QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~Configuration() override;

    // Delete copy constructor and assignment operator
    Configuration(const Configuration&) = delete;
    Configuration& operator=(const Configuration&) = delete;

    /**
     * @brief Navigates nested QVariantMap using dot notation
     * @param map The map to navigate
     * @param key The key path (e.g., "network.timeout")
     * @return Value if found, invalid QVariant otherwise
     */
    QVariant getNestedValue(const QVariantMap& map, const QString& key) const;

    /**
     * @brief Sets value in nested QVariantMap using dot notation
     * @param map The map to modify
     * @param key The key path
     * @param value The value to set
     */
    void setNestedValue(QVariantMap& map, const QString& key, const QVariant& value);

    QVariantMap m_settings;  ///< Configuration settings map
    QString m_configPath;    ///< Current configuration file path
};

#endif // CONFIGURATION_H
