# VulnScan - Requisiti Tecnici

## 🛠️ Tecnologie

- **Linguaggio**: C++17
- **Framework GUI**: Qt 6.9
- **Build System**: qmake
- **IDE Target**: Qt Creator
- **Database**: SQLite 3
- **Licenza**: MIT
- **i18n**: Qt Linguist (en, it, fr, de)
- **Docs**: Doxygen

## 🏗️ Architettura

- **Stand-alone** (no cloud)
- **Cross-platform**: Linux, Windows, macOS
- **Dual mode**: GUI (Qt Widgets) + CLI
- **Modulare** con Single Responsibility Principle (SRP)
- **Multi-threaded** thread-safe design

## 📐 Coding Standards

### Header Guards
```cpp
#pragma once  // SEMPRE usare questo
```

### Include Order
```cpp
// 1. Header corrispondente
#include "PortScanner.h"

// 2. Qt headers (alfabetico)
#include <QObject>
#include <QString>

// 3. Standard library (alfabetico)
#include <memory>
#include <vector>

// 4. Project headers (alfabetico)
#include "ScanResult.h"
```

### Naming Conventions
- **Classes**: `PascalCase` (es: `PortScanner`)
- **Methods**: `camelCase` (es: `scanNetwork`)
- **Variables**: `camelCase` (es: `portNumber`)
- **Constants**: `UPPER_SNAKE_CASE` (es: `MAX_PORTS`)
- **Private members**: `m_` prefix (es: `m_database`)
- **Interfaces**: `I` prefix (es: `IScanner`)

### Smart Pointers
```cpp
// ✅ Ownership esclusivo
std::unique_ptr<PortScanner> scanner = std::make_unique<PortScanner>();

// ✅ Ownership condiviso
std::shared_ptr<VulnerabilityDatabase> db = std::make_shared<VulnerabilityDatabase>();

// ✅ Qt parent-child
auto* scanner = new PortScanner(this);  // Qt gestisce lifetime
```

### Const Correctness
```cpp
// ✅ Const ovunque possibile
int getTotalScans() const;
void addResult(const ScanResult& result);
const QList<ScanResult>& getResults() const;
```

### RAII Pattern
```cpp
// ✅ Mutex con RAII
void addResult(const ScanResult& result) {
    QMutexLocker locker(&m_mutex);  // Auto-lock/unlock
    m_results.append(result);
}
```

## 🎨 Design Patterns

### Strategy Pattern (AI Providers)
```cpp
class IAIProvider {
public:
    virtual AIAnalysis analyze(const QList<Vulnerability>& vulns) = 0;
    virtual QString getName() const = 0;
};
```

### Repository Pattern (Database)
```cpp
template<typename T>
class IRepository {
public:
    virtual void save(const T& entity) = 0;
    virtual std::optional<T> findById(int id) = 0;
    virtual QList<T> findAll() = 0;
};
```

### Observer Pattern (Progress)
```cpp
class IScanObserver {
public:
    virtual void onScanStarted() = 0;
    virtual void onProgressUpdate(int percentage) = 0;
    virtual void onScanCompleted() = 0;
};
```

## 🧵 Multi-Threading

### Thread Safety
- **QMutex** per risorse condivise
- **std::atomic** per counters/flags
- **Qt::QueuedConnection** per signal cross-thread
- **Thread pool** con `QThreadPool`

```cpp
class PortScanner {
private:
    QThreadPool* m_threadPool;
    QMutex m_resultsMutex;
    std::atomic<int> m_completedScans;
    std::atomic<bool> m_cancelled;
};
```

### Worker Pattern
```cpp
class ScanWorker : public QRunnable {
public:
    void run() override {
        try {
            ScanResult result = performScan();
            // Notifica thread-safe con callback
        } catch (...) {
            // SEMPRE catturare in thread workers!
        }
    }
};
```

## 📝 Doxygen Documentation

```cpp
/**
 * @file PortScanner.h
 * @brief Port scanning functionality
 * @author VulnScan Team
 * @date 2025
 */

/**
 * @class PortScanner
 * @brief Implements parallel TCP/UDP port scanning
 *
 * Example:
 * @code
 * PortScanner scanner;
 * scanner.setTimeout(5000);
 * auto results = scanner.scanPorts("192.168.1.1", PortRange(1, 1024));
 * @endcode
 */
class PortScanner {
public:
    /**
     * @brief Scans ports on target host
     * @param host Target IP or hostname
     * @param range Port range to scan
     * @return List of scan results
     * @throws NetworkException if host unreachable
     */
    QList<PortResult> scanPorts(const QString& host, const PortRange& range);

private:
    int m_timeout; ///< Timeout in milliseconds
};
```

## 🌐 Linguaggio

### Codice e Commenti: INGLESE
- Tutti i commenti
- Documentazione Doxygen
- Nomi di variabili/funzioni
- Commit messages (preferibile)

### UI Strings: MULTILINGUA
```cpp
// ✅ Traducibile con tr()
QString msg = tr("Scanning port %1 of %2...").arg(current).arg(total);

// ❌ Non tradurre log messages
qCDebug(log) << "Connection timeout for host:" << host;
```

## ⚠️ Error Handling

### Thread Workers
```cpp
void ScanWorker::run() {
    try {
        // work here
    } catch (const std::exception& e) {
        // handle error
    } catch (...) {
        // OBBLIGATORIO: catch tutto
    }
}
```

### Qt APIs
```cpp
// ✅ Check error state (Qt non usa exceptions)
if (!file.open(QIODevice::ReadOnly)) {
    qCritical() << "Failed:" << file.errorString();
    return false;
}
```

## 📦 Qt Modules

### Core (tutti)
```cpp
QT += core network sql concurrent
```

### GUI only
```cpp
QT += widgets
```

### Common Headers
```cpp
#include <QObject>
#include <QString>
#include <QList>
#include <QTcpSocket>
#include <QSqlDatabase>
#include <QThreadPool>
#include <QMutex>
```

## 🔧 qmake Structure

### Root vulnscan.pro
```qmake
TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = \
    src/core \
    src/gui \
    src/cli \
    tests

src/gui.depends = src/core
src/cli.depends = src/core
tests.depends = src/core
```

### common.pri
```qmake
QT += core network sql
CONFIG += c++17 warn_on

# Output directories
CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/build/debug
} else {
    DESTDIR = $$PWD/build/release
}

INCLUDEPATH += $$PWD/src
```

## 🧪 Testing

- **Framework**: Qt Test
- **Coverage**: Minimo 60%
- **Unit tests** per ogni classe core
- **Integration tests** per workflows
- **Thread safety tests**

```cpp
class PortScannerTest : public QObject {
    Q_OBJECT

private slots:
    void testScanOpenPort();
    void testScanClosedPort();
    void testTimeout();
    void testParallelScanning();
};
```

## 🔒 Security

- No hardcoded credentials
- Secure storage (QSettings encrypted)
- Input validation ovunque
- SQL injection prevention (prepared statements)
- XSS prevention in HTML reports
- Rate limiting per scan

## 📚 Riferimenti Completi

Per approfondimenti, consulta il file originale:
- [prompt-original-backup.md](../prompt-original-backup.md) - Documentazione completa e dettagliata