# FASE 1: Foundation & Core Architecture ⭐

**Priorità**: MASSIMA
**Obiettivo**: Struttura base funzionante e compilabile

## 🎯 Obiettivi della Fase

Creare l'architettura base del progetto con:
- Struttura directory completa
- Sistema di build funzionante (qmake)
- Core interfaces e data models
- Database schema di base
- Sistema di logging e configurazione
- Unit tests per componenti fondamentali

## 📂 Struttura Directory da Creare

```
vulnscan/
├── README.md
├── LICENSE (MIT)
├── CONTRIBUTING.md
├── CODE_OF_CONDUCT.md
├── CHANGELOG.md
├── SECURITY.md
├── .gitignore
├── .clang-format
├── vulnscan.pro (root project)
├── common.pri (shared config)
│
├── src/
│   ├── vulnscan.pro (SUBDIRS)
│   ├── core/
│   │   └── core.pro
│   ├── gui/
│   │   └── gui.pro
│   ├── cli/
│   │   └── cli.pro
│   └── common/
│
├── tests/
│   └── tests.pro
│
├── resources/
│   ├── database/
│   │   ├── schema.sql
│   │   └── vulnerabilities_seed.sql
│   ├── config/
│   │   └── default_config.json
│   └── resources.qrc
│
├── docs/
│   ├── Doxyfile
│   └── architecture.md
│
├── translations/
│   ├── vulnscan_en.ts
│   ├── vulnscan_it.ts
│   ├── vulnscan_fr.ts
│   └── vulnscan_de.ts
│
└── scripts/
    ├── build.sh
    ├── build.ps1
    ├── setup_deps.sh
    └── setup_deps.ps1
```

## 🏗️ Core Interfaces da Implementare

### 1. IScanner.h
```cpp
/**
 * @interface IScanner
 * @brief Base interface for all scanner implementations
 */
class IScanner {
public:
    virtual ~IScanner() = default;
    virtual void startScan() = 0;
    virtual void stopScan() = 0;
    virtual bool isScanning() const = 0;
};
```

### 2. IAIProvider.h
```cpp
/**
 * @interface IAIProvider
 * @brief Interface for AI analysis providers
 */
class IAIProvider {
public:
    virtual ~IAIProvider() = default;
    virtual AIAnalysis analyze(const QList<Vulnerability>& vulns) = 0;
    virtual QString getName() const = 0;
};
```

### 3. IRepository.h
```cpp
/**
 * @interface IRepository
 * @brief Generic repository pattern interface
 */
template<typename T>
class IRepository {
public:
    virtual ~IRepository() = default;
    virtual void save(const T& entity) = 0;
    virtual std::optional<T> findById(int id) = 0;
    virtual QList<T> findAll() = 0;
    virtual void remove(int id) = 0;
};
```

### 4. IVulnerabilityChecker.h
```cpp
/**
 * @interface IVulnerabilityChecker
 * @brief Interface for vulnerability checking implementations
 */
class IVulnerabilityChecker {
public:
    virtual ~IVulnerabilityChecker() = default;
    virtual QList<Vulnerability> check(const ServiceInfo& service) = 0;
};
```

### 5. IReportGenerator.h
```cpp
/**
 * @interface IReportGenerator
 * @brief Interface for report generation
 */
class IReportGenerator {
public:
    virtual ~IReportGenerator() = default;
    virtual bool generate(const ReportData& data, const QString& outputPath) = 0;
    virtual QString getFormatName() const = 0;
};
```

## 📊 Data Models

### ScanResult.h
```cpp
/**
 * @struct ScanResult
 * @brief Contains the results of a network scan
 */
struct ScanResult {
    int id;
    QString targetHost;
    QDateTime scanStartTime;
    QDateTime scanEndTime;
    QList<PortResult> ports;
    QString status;

    Q_GADGET
};
```

### Vulnerability.h
```cpp
/**
 * @struct Vulnerability
 * @brief Represents a detected vulnerability
 */
struct Vulnerability {
    QString cveId;
    QString description;
    QString severity;  // Critical, High, Medium, Low
    float cvssScore;
    QString affectedService;
    QString recommendedAction;

    Q_GADGET
};
```

### PortResult.h
```cpp
/**
 * @struct PortResult
 * @brief Result of a single port scan
 */
struct PortResult {
    int port;
    QString protocol;  // TCP, UDP
    QString state;     // open, closed, filtered
    QString service;
    QString version;

    Q_GADGET
};
```

### ServiceInfo.h
```cpp
/**
 * @struct ServiceInfo
 * @brief Information about a detected service
 */
struct ServiceInfo {
    QString name;
    QString version;
    int port;
    QString protocol;
    QString banner;

    Q_GADGET
};
```

## 🗄️ Database Schema

### schema.sql
```sql
-- Scans table
CREATE TABLE IF NOT EXISTS scans (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    target_host TEXT NOT NULL,
    scan_start_time DATETIME NOT NULL,
    scan_end_time DATETIME,
    status TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- Ports table
CREATE TABLE IF NOT EXISTS ports (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    scan_id INTEGER NOT NULL,
    port INTEGER NOT NULL,
    protocol TEXT NOT NULL,
    state TEXT NOT NULL,
    service TEXT,
    version TEXT,
    FOREIGN KEY (scan_id) REFERENCES scans(id) ON DELETE CASCADE
);

-- Vulnerabilities table
CREATE TABLE IF NOT EXISTS vulnerabilities (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    cve_id TEXT UNIQUE NOT NULL,
    description TEXT,
    severity TEXT NOT NULL,
    cvss_score REAL,
    published_date DATE,
    modified_date DATE
);

-- Scan Vulnerabilities (junction table)
CREATE TABLE IF NOT EXISTS scan_vulnerabilities (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    scan_id INTEGER NOT NULL,
    vulnerability_id INTEGER NOT NULL,
    affected_service TEXT,
    detected_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (scan_id) REFERENCES scans(id) ON DELETE CASCADE,
    FOREIGN KEY (vulnerability_id) REFERENCES vulnerabilities(id) ON DELETE CASCADE
);

-- Indexes
CREATE INDEX IF NOT EXISTS idx_scans_target ON scans(target_host);
CREATE INDEX IF NOT EXISTS idx_ports_scan ON ports(scan_id);
CREATE INDEX IF NOT EXISTS idx_vulns_cve ON vulnerabilities(cve_id);
```

## ⚙️ Core Components

### 1. DatabaseManager.h/cpp
```cpp
/**
 * @class DatabaseManager
 * @brief Manages SQLite database connection and initialization
 */
class DatabaseManager : public QObject {
    Q_OBJECT

public:
    static DatabaseManager& instance();

    bool initialize(const QString& dbPath);
    QSqlDatabase& getDatabase();
    bool isOpen() const;

private:
    explicit DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager() override;

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_database;
};
```

### 2. Configuration.h/cpp
```cpp
/**
 * @class Configuration
 * @brief Application configuration manager
 */
class Configuration : public QObject {
    Q_OBJECT

public:
    static Configuration& instance();

    bool load(const QString& configPath);
    bool save(const QString& configPath);

    QVariant get(const QString& key, const QVariant& defaultValue = QVariant()) const;
    void set(const QString& key, const QVariant& value);

private:
    explicit Configuration(QObject* parent = nullptr);
    ~Configuration() override;

    QVariantMap m_settings;
};
```

### 3. Logger.h/cpp
```cpp
/**
 * @class Logger
 * @brief Application-wide logging system
 */
class Logger {
public:
    enum class Level {
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    static void initialize(const QString& logFilePath);
    static void log(Level level, const QString& message);
    static void setLogLevel(Level level);

private:
    static void messageHandler(QtMsgType type,
                              const QMessageLogContext& context,
                              const QString& msg);
};
```

## 📝 File qmake da Creare

### vulnscan.pro (root)
```qmake
TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    src/core \
    src/gui \
    src/cli \
    tests

# Dependencies
src/gui.depends = src/core
src/cli.depends = src/core
tests.depends = src/core
```

### common.pri
```qmake
QT += core network sql
CONFIG += c++17 warn_on

# Compiler flags
gcc|clang {
    QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic
}

msvc {
    QMAKE_CXXFLAGS += /W4 /std:c++17
}

# Output directories
CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/build/debug
    OBJECTS_DIR = $$PWD/build/debug/.obj
    MOC_DIR = $$PWD/build/debug/.moc
} else {
    DESTDIR = $$PWD/build/release
    OBJECTS_DIR = $$PWD/build/release/.obj
    MOC_DIR = $$PWD/build/release/.moc
}

INCLUDEPATH += $$PWD/src
```

### src/core/core.pro
```qmake
TEMPLATE = lib
CONFIG += staticlib
TARGET = vulnscan_core

include(../../common.pri)

QT -= gui widgets

HEADERS += \
    scanner/IScanner.h \
    scanner/ScanResult.h \
    ai/IAIProvider.h \
    database/IRepository.h \
    database/DatabaseManager.h \
    config/Configuration.h \
    common/Logger.h

SOURCES += \
    database/DatabaseManager.cpp \
    config/Configuration.cpp \
    common/Logger.cpp
```

## 🧪 Unit Tests Iniziali

### test_database_manager.cpp
```cpp
class TestDatabaseManager : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testInitialization();
    void testConnection();
    void testSchemaCreation();
};
```

### test_configuration.cpp
```cpp
class TestConfiguration : public QObject {
    Q_OBJECT

private slots:
    void testLoadConfig();
    void testSaveConfig();
    void testGetSet();
};
```

## ✅ Deliverable della Fase 1

- [x] Struttura directory completa
- [x] File qmake (.pro) funzionanti
- [x] common.pri con configurazioni condivise
- [x] Core interfaces (IScanner, IAIProvider, IRepository, IVulnerabilityChecker, IReportGenerator)
- [x] Basic data models (ScanResult, Vulnerability, PortResult, ServiceInfo)
- [x] Database schema (schema.sql)
- [x] DatabaseManager implementation
- [x] Configuration system
- [x] Logger system
- [x] Unit tests per componenti base
- [x] Progetto che compila con qmake
- [x] README.md, LICENSE, CONTRIBUTING.md
- [x] .gitignore, .clang-format
- [x] CHANGELOG.md iniziale

## 🔄 Next Steps

Una volta completata la FASE 1, procedi con:
- **FASE 2**: Network Scanner Core (implementazione scanner con parallelizzazione)

## 📚 Riferimenti

- [Qt Documentation](https://doc.qt.io/)
- [qmake Manual](https://doc.qt.io/qt-6/qmake-manual.html)
- [C++17 Standard](https://en.cppreference.com/w/cpp/17)
- [SQLite Documentation](https://www.sqlite.org/docs.html)