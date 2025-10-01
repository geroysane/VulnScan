# FASE 2: Network Scanner Core 🔍

**Obiettivo**: Funzionalità di scanning base con parallelizzazione

## 🎯 Obiettivi della Fase

- Implementare port scanning (TCP/UDP) parallelo
- Service detection e fingerprinting
- Network discovery capabilities
- DNS resolution
- Thread-safe scan orchestration
- Progress tracking e cancellation
- CLI funzionale per testing
- Database storage dei risultati

## 📦 Componenti da Implementare

### 1. PortScanner.h/cpp
**Responsabilità**: Port scanning con parallelizzazione
- Scan TCP/UDP ports
- Configurazione timeout
- Thread pool management
- Progress reporting

```cpp
class PortScanner : public QObject, public IScanner {
    Q_OBJECT

public:
    QList<PortResult> scanPorts(const QString& host,
                               const PortRange& range,
                               int maxThreads = QThread::idealThreadCount());

signals:
    void portScanned(const PortResult& result);
    void progressUpdate(int current, int total);

private:
    QThreadPool* m_threadPool;
    QMutex m_resultsMutex;
    std::atomic<int> m_completedScans;
};
```

### 2. ScanWorker.h/cpp
**Responsabilità**: Worker per scan parallelo di singola porta

```cpp
class ScanWorker : public QRunnable {
public:
    ScanWorker(const QString& host, int port, int timeout);
    void run() override;

private:
    QString m_host;
    int m_port;
    int m_timeout;
};
```

### 3. ServiceDetector.h/cpp
**Responsabilità**: Identificazione servizi da banner/versione

```cpp
class ServiceDetector : public QObject {
    Q_OBJECT

public:
    ServiceInfo detectService(const QString& host, int port);

private:
    QString fetchBanner(QTcpSocket& socket);
    QString identifyService(const QString& banner, int port);
};
```

### 4. NetworkInterface.h/cpp
**Responsabilità**: Network discovery e host enumeration

```cpp
class NetworkInterface : public QObject {
    Q_OBJECT

public:
    QList<QHostAddress> discoverHosts(const QString& network);
    QList<NetworkInterfaceInfo> getLocalInterfaces();
};
```

### 5. DnsResolver.h/cpp
**Responsabilità**: DNS lookups (hostname <-> IP)

```cpp
class DnsResolver : public QObject {
    Q_OBJECT

public:
    QString resolveHostname(const QString& ip);
    QHostAddress resolveIP(const QString& hostname);
};
```

### 6. ScanEngine.h/cpp
**Responsabilità**: Orchestrator principale degli scan

```cpp
class ScanEngine : public QObject {
    Q_OBJECT

public:
    void startScan(const ScanConfiguration& config);
    void cancelScan();

signals:
    void scanStarted();
    void scanProgress(int percentage);
    void scanCompleted(const ScanResult& result);
    void scanFailed(const QString& error);

private:
    std::unique_ptr<PortScanner> m_portScanner;
    std::unique_ptr<ServiceDetector> m_serviceDetector;
    std::unique_ptr<DnsResolver> m_dnsResolver;
    std::atomic<bool> m_cancelled;
};
```

### 7. ScanRepository.h/cpp
**Responsabilità**: Persistenza scan results nel database

```cpp
class ScanRepository : public IRepository<ScanResult> {
public:
    void save(const ScanResult& result) override;
    std::optional<ScanResult> findById(int id) override;
    QList<ScanResult> findAll() override;
    QList<ScanResult> findByHost(const QString& host);
};
```

## 🔧 CLI Implementation

### CliApplication.h/cpp
```cpp
class CliApplication : public QObject {
    Q_OBJECT

public:
    int run(const QStringList& args);

private:
    void printUsage();
    void handleScanCommand(const QStringList& args);
    void handleListCommand();
};
```

### CLI Usage
```bash
vulnscan-cli scan --target 192.168.1.1 --ports 1-1000
vulnscan-cli scan --target 192.168.1.0/24 --fast
vulnscan-cli list-scans
vulnscan-cli show-scan --id 42
```

## 🧵 Threading Strategy

### Port Scanning Parallelization
- **Thread Pool**: Usa `QThreadPool` con worker count = CPU cores
- **Batch Processing**: Divide port ranges in batch per worker
- **Thread Safety**: `QMutex` per shared results, `std::atomic` per counters
- **Cancellation**: `std::atomic<bool>` flag checked in workers

### Example Flow
```
ScanEngine
  └─> PortScanner (thread pool orchestrator)
      ├─> ScanWorker 1 (ports 1-100)
      ├─> ScanWorker 2 (ports 101-200)
      ├─> ScanWorker 3 (ports 201-300)
      └─> ...
```

## 🧪 Unit Tests

### test_port_scanner.cpp
- Test single port scan
- Test port range scan
- Test parallel scanning
- Test timeout handling
- Test cancellation

### test_service_detector.cpp
- Test banner grabbing
- Test service identification
- Test version parsing

### test_scan_repository.cpp
- Test save scan results
- Test query scans by host
- Test scan history

## ✅ Deliverable della Fase 2

- [x] PortScanner con parallelizzazione funzionante
- [x] ScanWorker thread-safe
- [x] ServiceDetector per identificazione servizi
- [x] NetworkInterface per discovery
- [x] DnsResolver per hostname resolution
- [x] ScanEngine orchestrator completo
- [x] ScanRepository per database storage
- [x] CLI funzionale per testing
- [x] Progress tracking thread-safe
- [x] Cancellation support
- [x] Unit tests per tutti i componenti
- [x] Integration test per scanner workflow
- [x] Thread safety tests

## 🔄 Next Steps

Procedi con **FASE 3**: Vulnerability Assessment (CVE database e vulnerability detection parallela)