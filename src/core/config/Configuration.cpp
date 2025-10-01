#include "Configuration.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

Configuration& Configuration::instance() {
    static Configuration instance;
    return instance;
}

Configuration::Configuration(QObject* parent)
    : QObject(parent)
{
    loadDefaults();
}

Configuration::~Configuration() {
}

bool Configuration::load(const QString& configPath) {
    QFile file(configPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open configuration file:" << configPath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qCritical() << "Invalid JSON in configuration file";
        return false;
    }

    m_settings = doc.object().toVariantMap();
    m_configPath = configPath;

    qDebug() << "Configuration loaded successfully from:" << configPath;
    return true;
}

bool Configuration::save(const QString& configPath) {
    QJsonObject jsonObj = QJsonObject::fromVariantMap(m_settings);
    QJsonDocument doc(jsonObj);

    QFile file(configPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qCritical() << "Failed to open configuration file for writing:" << configPath;
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    m_configPath = configPath;
    qDebug() << "Configuration saved successfully to:" << configPath;
    return true;
}

QVariant Configuration::get(const QString& key, const QVariant& defaultValue) const {
    QVariant value = getNestedValue(m_settings, key);
    return value.isValid() ? value : defaultValue;
}

void Configuration::set(const QString& key, const QVariant& value) {
    setNestedValue(m_settings, key, value);
    emit configurationChanged(key);
}

bool Configuration::contains(const QString& key) const {
    return getNestedValue(m_settings, key).isValid();
}

void Configuration::loadDefaults() {
    m_settings.clear();

    // Network settings
    QVariantMap network;
    network["timeout"] = 5000;
    network["maxRetries"] = 3;
    network["threadPoolSize"] = 10;
    m_settings["network"] = network;

    // Scan settings
    QVariantMap scan;
    scan["portRange"] = "1-1000";
    scan["scanDelay"] = 0;
    scan["enableServiceDetection"] = true;
    scan["enableVersionDetection"] = true;
    m_settings["scan"] = scan;

    // AI settings
    QVariantMap ai;
    ai["enabled"] = false;
    ai["provider"] = "openai";
    ai["apiKey"] = "";
    ai["model"] = "gpt-4";
    m_settings["ai"] = ai;

    // Database settings
    QVariantMap database;
    database["path"] = "./vulnscan.db";
    m_settings["database"] = database;

    // Report settings
    QVariantMap report;
    report["format"] = "pdf";
    report["outputPath"] = "./reports";
    report["companyName"] = "";
    m_settings["report"] = report;

    // Logging settings
    QVariantMap logging;
    logging["level"] = "info";
    logging["filePath"] = "./vulnscan.log";
    logging["enableConsole"] = true;
    m_settings["logging"] = logging;

    qDebug() << "Default configuration loaded";
}

QVariant Configuration::getNestedValue(const QVariantMap& map, const QString& key) const {
    QStringList keys = key.split('.');
    QVariant current = map;

    for (const QString& k : keys) {
        if (current.canConvert<QVariantMap>()) {
            QVariantMap currentMap = current.toMap();
            if (currentMap.contains(k)) {
                current = currentMap[k];
            } else {
                return QVariant();
            }
        } else {
            return QVariant();
        }
    }

    return current;
}

void Configuration::setNestedValue(QVariantMap& map, const QString& key, const QVariant& value) {
    QStringList keys = key.split('.');
    QVariantMap* currentMap = &map;

    for (int i = 0; i < keys.size() - 1; ++i) {
        const QString& k = keys[i];
        if (!currentMap->contains(k) || !(*currentMap)[k].canConvert<QVariantMap>()) {
            (*currentMap)[k] = QVariantMap();
        }
        currentMap = reinterpret_cast<QVariantMap*>(&(*currentMap)[k]);
    }

    (*currentMap)[keys.last()] = value;
}
