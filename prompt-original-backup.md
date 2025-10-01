# Prompt per Generazione Progetto VulnScan

Voglio che tu generi un progetto completo per un **Network Vulnerability Scanner** open source chiamato **VulnScan**. Il progetto deve essere professionale, pronto per GitHub, e seguire le best practices di sviluppo software.

## 🛡️ **IMPORTANTE: SCOPO DIFENSIVO E LEGALE**

**VulnScan è un tool di SICUREZZA DIFENSIVA per professionisti IT, security researchers e amministratori di sistema.**

### Scopo Legittimo:
- ✅ **Vulnerability Assessment** di sistemi PROPRI o AUTORIZZATI
- ✅ **Security Auditing** per compliance e hardening
- ✅ **Network Discovery** per inventory e documentazione
- ✅ **Configuration Analysis** per identificare misconfigurations
- ✅ **Educational purposes** per apprendimento cybersecurity
- ✅ **Penetration Testing AUTORIZZATO** da clienti/datori di lavoro
- ✅ **Cybersecurity Outreach** e divulgazione scientifica
- ✅ **Academic Research** su AI applications in security

### Cosa NON fa (nessuna funzionalità offensiva):
- ❌ **Exploit execution** o sfruttamento attivo di vulnerabilità
- ❌ **Payload delivery** o code injection
- ❌ **Credential harvesting** o password cracking
- ❌ **DoS attacks** o disruption di servizi
- ❌ **Unauthorized access** o privilege escalation
- ❌ **Data exfiltration** o lateral movement

### Responsabilità dell'Utente:
**L'utente è COMPLETAMENTE responsabile dell'uso legale e autorizzato del tool.**
- Ottenere **autorizzazione scritta** prima di qualsiasi scan
- Rispettare **leggi locali** e **termini di servizio**
- Usare solo su **reti proprie** o **esplicitamente autorizzate**
- **NON utilizzare** per attività illegali o non autorizzate

## 📋 Requisiti Generali

### Tecnologie
- **Linguaggio**: C++17
- **Framework GUI**: Qt 6.9
- **Build System**: qmake (Qt's native build system)
- **IDE Target**: Qt Creator
- **Database**: SQLite 3
- **Licenza**: MIT
- **Internazionalizzazione**: Qt Linguist (supporto multilingua)
- **Documentazione API**: Doxygen

### Architettura
- Applicazione **stand-alone** (non servizio cloud)
- **Cross-platform**: Linux, Windows, macOS
- Due modalità: **GUI** (Qt Widgets) e **CLI**
- Codice **modulare** e **testabile**
- Rispettare rigorosamente il **Single Responsibility Principle (SRP)**
- **Architettura multi-threaded** per performance ottimali
- **Thread-safe** design per accesso concorrente a risorse condivise

## 🏗️ Struttura del Progetto

Genera la seguente struttura completa:

```
vulnscan/
├── README.md (completo con badges, screenshots placeholders, esempi)
├── LICENSE (MIT)
├── CONTRIBUTING.md
├── CODE_OF_CONDUCT.md
├── CHANGELOG.md
├── SECURITY.md
├── .gitignore (C++/Qt/qmake)
├── .clang-format
├── vulnscan.pro (root project file)
├── common.pri (shared configurations)
│
├── .github/
│   ├── workflows/
│   │   ├── build.yml (CI per Linux/Windows/macOS)
│   │   ├── test.yml (run tests)
│   │   ├── release.yml (automated releases con binary)
│   │   └── codeql.yml (security analysis)
│   ├── ISSUE_TEMPLATE/
│   │   ├── bug_report.md
│   │   ├── feature_request.md
│   │   └── security_vulnerability.md
│   ├── PULL_REQUEST_TEMPLATE.md
│   └── dependabot.yml
│
├── docs/
│   ├── installation.md
│   ├── user_guide.md
│   ├── developer_guide.md
│   ├── architecture.md
│   ├── api_reference.md
│   ├── contributing_guide.md
│   ├── Doxyfile (configurazione Doxygen)
│   └── doxygen/ (output generato da Doxygen)
│
├── translations/
│   ├── vulnscan_en.ts (inglese - lingua base)
│   ├── vulnscan_it.ts (italiano)
│   ├── vulnscan_fr.ts (francese)
│   ├── vulnscan_de.ts (tedesco)
│   └── translations.pri (configurazione Qt Linguist)
│
├── src/
│   ├── vulnscan.pro (subdirs project)
│   │
│   ├── core/ (Business Logic - NO Qt Widgets)
│   │   ├── core.pro
│   │   │
│   │   ├── scanner/
│   │   │   ├── IScanner.h (interface)
│   │   │   ├── PortScanner.h/cpp (SRP: solo port scanning)
│   │   │   ├── ServiceDetector.h/cpp (SRP: solo service detection)
│   │   │   ├── ScanEngine.h/cpp (orchestrator)
│   │   │   ├── ScanWorker.h/cpp (SRP: thread worker for parallel scans)
│   │   │   └── ScanResult.h (data model)
│   │   │
│   │   ├── network/
│   │   │   ├── NetworkInterface.h/cpp (SRP: network discovery)
│   │   │   ├── PacketHandler.h/cpp (SRP: packet operations)
│   │   │   ├── DnsResolver.h/cpp (SRP: DNS queries)
│   │   │   └── NetworkUtils.h/cpp (utility functions)
│   │   │
│   │   ├── vulnerability/
│   │   │   ├── IVulnerabilityChecker.h (interface)
│   │   │   ├── VulnerabilityDatabase.h/cpp (CVE storage/query)
│   │   │   ├── VulnerabilityMatcher.h/cpp (SRP: matching logic)
│   │   │   ├── VulnerabilityScanner.h/cpp (SRP: parallel vulnerability analysis)
│   │   │   ├── VulnerabilityWorker.h/cpp (SRP: thread worker for vuln checks)
│   │   │   ├── SslTlsChecker.h/cpp (SRP: SSL/TLS checks)
│   │   │   ├── ServiceVulnChecker.h/cpp (SRP: service vulns)
│   │   │   └── Vulnerability.h (data model)
│   │   │
│   │   ├── ai/
│   │   │   ├── IAIProvider.h (interface per multiple AI)
│   │   │   ├── ClaudeProvider.h/cpp (SRP: Claude API)
│   │   │   ├── OpenAIProvider.h/cpp (SRP: OpenAI API)
│   │   │   ├── GeminiProvider.h/cpp (SRP: Google Gemini)
│   │   │   ├── GenericProvider.h/cpp (SRP: generic/custom providers)
│   │   │   ├── LocalLLMProvider.h/cpp (SRP: local models)
│   │   │   ├── AIAnalyzer.h/cpp (SRP: analysis orchestration)
│   │   │   ├── AIWorker.h/cpp (SRP: async AI analysis in threads)
│   │   │   ├── AIConfig.h/cpp (configuration)
│   │   │   └── PromptBuilder.h/cpp (SRP: prompt construction)
│   │   │
│   │   ├── database/
│   │   │   ├── IRepository.h (interface)
│   │   │   ├── DatabaseManager.h/cpp (SRP: connection management)
│   │   │   ├── ScanRepository.h/cpp (SRP: scan CRUD)
│   │   │   ├── VulnRepository.h/cpp (SRP: vulnerability CRUD)
│   │   │   └── DatabaseSchema.h (schema definitions)
│   │   │
│   │   ├── report/
│   │   │   ├── IReportGenerator.h (interface)
│   │   │   ├── HtmlReportGenerator.h/cpp (SRP: HTML reports)
│   │   │   ├── PdfReportGenerator.h/cpp (SRP: PDF reports)
│   │   │   ├── JsonReportGenerator.h/cpp (SRP: JSON export)
│   │   │   ├── CsvReportGenerator.h/cpp (SRP: CSV export)
│   │   │   └── ReportData.h (data model)
│   │   │
│   │   └── config/
│   │       ├── Configuration.h/cpp (app configuration)
│   │       ├── ConfigLoader.h/cpp (SRP: load config)
│   │       └── ConfigValidator.h/cpp (SRP: validate config)
│   │
│   ├── gui/ (Presentation Layer)
│   │   ├── gui.pro
│   │   ├── main.cpp
│   │   │
│   │   ├── mainwindow/
│   │   │   ├── MainWindow.h/cpp/ui
│   │   │   └── MainWindowController.h/cpp
│   │   │
│   │   ├── scan/
│   │   │   ├── ScanView.h/cpp/ui (SRP: scan UI)
│   │   │   ├── ScanController.h/cpp (SRP: scan logic)
│   │   │   └── ScanProgressWidget.h/cpp/ui
│   │   │
│   │   ├── results/
│   │   │   ├── ResultsView.h/cpp/ui (SRP: display results)
│   │   │   ├── ResultsController.h/cpp
│   │   │   ├── VulnerabilityTableModel.h/cpp
│   │   │   └── NetworkTreeModel.h/cpp
│   │   │
│   │   ├── settings/
│   │   │   ├── SettingsDialog.h/cpp/ui (SRP: settings UI)
│   │   │   ├── SettingsController.h/cpp
│   │   │   ├── AIConfigWidget.h/cpp/ui
│   │   │   └── CustomProviderDialog.h/cpp/ui (SRP: add custom AI providers)
│   │   │
│   │   ├── report/
│   │   │   ├── ReportDialog.h/cpp/ui
│   │   │   └── ReportPreviewWidget.h/cpp
│   │   │
│   │   └── widgets/
│   │       ├── NetworkDiagram.h/cpp (custom widget)
│   │       └── VulnerabilitySeverityWidget.h/cpp
│   │
│   ├── cli/ (Command Line Interface)
│   │   ├── cli.pro
│   │   ├── main.cpp
│   │   ├── CliApplication.h/cpp (SRP: CLI orchestration)
│   │   ├── CommandParser.h/cpp (SRP: argument parsing)
│   │   ├── CommandExecutor.h/cpp (SRP: command execution)
│   │   └── OutputFormatter.h/cpp (SRP: terminal output)
│   │
│   └── common/
│       ├── Logger.h/cpp (SRP: logging)
│       ├── Utils.h/cpp (utility functions)
│       └── Constants.h (application constants)
│
├── tests/
│   ├── tests.pro (test suite)
│   ├── unit/
│   │   ├── scanner/
│   │   │   ├── test_port_scanner.cpp
│   │   │   └── test_service_detector.cpp
│   │   ├── ai/
│   │   │   ├── test_claude_provider.cpp
│   │   │   └── test_prompt_builder.cpp
│   │   └── database/
│   │       └── test_scan_repository.cpp
│   ├── integration/
│   │   └── test_scan_workflow.cpp
│   └── test_utils.h/cpp
│
├── resources/
│   ├── database/
│   │   ├── schema.sql
│   │   └── vulnerabilities_seed.sql
│   ├── icons/
│   │   └── vulnscan.png (placeholder)
│   ├── ui/
│   │   └── styles.qss
│   ├── config/
│   │   └── default_config.json
│   └── resources.qrc
│
├── scripts/
│   ├── setup_deps.sh (Linux/macOS dependency installer)
│   ├── setup_deps.ps1 (Windows PowerShell)
│   ├── build.sh (qmake + make wrapper)
│   ├── build.ps1
│   ├── install.sh
│   ├── run_tests.sh
│   ├── update_translations.sh (lupdate per aggiornare .ts files)
│   └── generate_docs.sh (Doxygen documentation generation)
│
├── third_party/
│   └── README.md (list external dependencies)
│
├── packaging/
│   ├── linux/
│   │   ├── vulnscan.desktop
│   │   └── appimage.yml
│   ├── windows/
│   │   ├── installer.nsi
│   │   └── vulnscan.ico
│   └── macos/
│       ├── Info.plist
│       └── create_dmg.sh
│
├── conanfile.txt (dependency management - optional)
├── vcpkg.json (Windows dependencies - optional)
└── Dockerfile (optional containerization)
```

## 🎯 qmake Project Structure

### Root vulnscan.pro (SUBDIRS template)
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

# Build configuration
CONFIG(debug, debug|release) {
    message(Building in DEBUG mode)
} else {
    message(Building in RELEASE mode)
}
```

### common.pri (Shared configuration)
```qmake
# Common settings for all subprojects
QT += core network sql
CONFIG += c++17

# Compiler flags
CONFIG += c++17 warn_on

gcc|clang {
    QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic -Wno-unused-parameter
    QMAKE_CXXFLAGS += -fno-omit-frame-pointer  # Better debugging

    # Additional warnings
    QMAKE_CXXFLAGS += -Wshadow -Wnon-virtual-dtor -Wold-style-cast
    QMAKE_CXXFLAGS += -Wcast-align -Woverloaded-virtual

    # Release optimizations
    CONFIG(release, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}

msvc {
    QMAKE_CXXFLAGS += /W4 /std:c++17
    QMAKE_CXXFLAGS += /permissive-  # Strict conformance

    # Release optimizations
    CONFIG(release, debug|release) {
        QMAKE_CXXFLAGS += /O2 /GL  # Global optimization
    }
}

# Output directories
CONFIG(debug, debug|release) {
    DESTDIR = $PWD/build/debug
    OBJECTS_DIR = $PWD/build/debug/.obj
    MOC_DIR = $PWD/build/debug/.moc
    RCC_DIR = $PWD/build/debug/.rcc
    UI_DIR = $PWD/build/debug/.ui
} else {
    DESTDIR = $PWD/build/release
    OBJECTS_DIR = $PWD/build/release/.obj
    MOC_DIR = $PWD/build/release/.moc
    RCC_DIR = $PWD/build/release/.rcc
    UI_DIR = $PWD/build/release/.ui
}

# Include paths
INCLUDEPATH += $PWD/src

# Platform-specific
win32 {
    DEFINES += WIN32
}
unix:!macx {
    DEFINES += LINUX
}
macx {
    DEFINES += MACOS
}
```

### src/core/core.pro
```qmake
TEMPLATE = lib
CONFIG += staticlib
TARGET = vulnscan_core

include(../../common.pri)

QT += network sql
QT -= gui widgets

HEADERS += \
    scanner/IScanner.h \
    scanner/PortScanner.h \
    scanner/ServiceDetector.h \
    scanner/ScanEngine.h \
    scanner/ScanWorker.h \
    scanner/ScanResult.h \
    network/NetworkInterface.h \
    network/PacketHandler.h \
    network/DnsResolver.h \
    network/NetworkUtils.h \
    vulnerability/IVulnerabilityChecker.h \
    vulnerability/VulnerabilityDatabase.h \
    vulnerability/VulnerabilityMatcher.h \
    vulnerability/VulnerabilityScanner.h \
    vulnerability/VulnerabilityWorker.h \
    vulnerability/SslTlsChecker.h \
    vulnerability/ServiceVulnChecker.h \
    vulnerability/Vulnerability.h \
    ai/IAIProvider.h \
    ai/ClaudeProvider.h \
    ai/OpenAIProvider.h \
    ai/GeminiProvider.h \
    ai/GenericProvider.h \
    ai/LocalLLMProvider.h \
    ai/AIAnalyzer.h \
    ai/AIWorker.h \
    ai/AIConfig.h \
    ai/PromptBuilder.h \
    database/IRepository.h \
    database/DatabaseManager.h \
    database/ScanRepository.h \
    database/VulnRepository.h \
    database/DatabaseSchema.h \
    report/IReportGenerator.h \
    report/HtmlReportGenerator.h \
    report/PdfReportGenerator.h \
    report/JsonReportGenerator.h \
    report/CsvReportGenerator.h \
    report/ReportData.h \
    config/Configuration.h \
    config/ConfigLoader.h \
    config/ConfigValidator.h

SOURCES += \
    scanner/PortScanner.cpp \
    scanner/ServiceDetector.cpp \
    scanner/ScanEngine.cpp \
    scanner/ScanWorker.cpp \
    network/NetworkInterface.cpp \
    network/PacketHandler.cpp \
    network/DnsResolver.cpp \
    network/NetworkUtils.cpp \
    vulnerability/VulnerabilityDatabase.cpp \
    vulnerability/VulnerabilityMatcher.cpp \
    vulnerability/VulnerabilityScanner.cpp \
    vulnerability/VulnerabilityWorker.cpp \
    vulnerability/SslTlsChecker.cpp \
    vulnerability/ServiceVulnChecker.cpp \
    ai/ClaudeProvider.cpp \
    ai/OpenAIProvider.cpp \
    ai/GeminiProvider.cpp \
    ai/GenericProvider.cpp \
    ai/LocalLLMProvider.cpp \
    ai/AIAnalyzer.cpp \
    ai/AIWorker.cpp \
    ai/AIConfig.cpp \
    ai/PromptBuilder.cpp \
    database/DatabaseManager.cpp \
    database/ScanRepository.cpp \
    database/VulnRepository.cpp \
    report/HtmlReportGenerator.cpp \
    report/PdfReportGenerator.cpp \
    report/JsonReportGenerator.cpp \
    report/CsvReportGenerator.cpp \
    config/Configuration.cpp \
    config/ConfigLoader.cpp \
    config/ConfigValidator.cpp

# Optional AI support
DEFINES += ENABLE_AI

# Export library headers
INSTALL_HEADERS.files = $HEADERS
INSTALL_HEADERS.path = $DESTDIR/include
INSTALLS += INSTALL_HEADERS
```

### src/gui/gui.pro
```qmake
TEMPLATE = app
TARGET = vulnscan

include(../../common.pri)

QT += widgets

LIBS += -L$DESTDIR -lvulnscan_core

HEADERS += \
    mainwindow/MainWindow.h \
    mainwindow/MainWindowController.h \
    scan/ScanView.h \
    scan/ScanController.h \
    scan/ScanProgressWidget.h \
    results/ResultsView.h \
    results/ResultsController.h \
    results/VulnerabilityTableModel.h \
    results/NetworkTreeModel.h \
    settings/SettingsDialog.h \
    settings/SettingsController.h \
    settings/AIConfigWidget.h \
    settings/CustomProviderDialog.h \
    report/ReportDialog.h \
    report/ReportPreviewWidget.h \
    widgets/NetworkDiagram.h \
    widgets/VulnerabilitySeverityWidget.h

SOURCES += \
    main.cpp \
    mainwindow/MainWindow.cpp \
    mainwindow/MainWindowController.cpp \
    scan/ScanView.cpp \
    scan/ScanController.cpp \
    scan/ScanProgressWidget.cpp \
    results/ResultsView.cpp \
    results/ResultsController.cpp \
    results/VulnerabilityTableModel.cpp \
    results/NetworkTreeModel.cpp \
    settings/SettingsDialog.cpp \
    settings/SettingsController.cpp \
    settings/AIConfigWidget.cpp \
    settings/CustomProviderDialog.cpp \
    report/ReportDialog.cpp \
    report/ReportPreviewWidget.cpp \
    widgets/NetworkDiagram.cpp \
    widgets/VulnerabilitySeverityWidget.cpp

FORMS += \
    mainwindow/MainWindow.ui \
    scan/ScanView.ui \
    scan/ScanProgressWidget.ui \
    results/ResultsView.ui \
    settings/SettingsDialog.ui \
    settings/AIConfigWidget.ui \
    settings/CustomProviderDialog.ui \
    report/ReportDialog.ui

RESOURCES += \
    ../../resources/resources.qrc

TRANSLATIONS += \
    ../../translations/vulnscan_en.ts \
    ../../translations/vulnscan_it.ts \
    ../../translations/vulnscan_fr.ts \
    ../../translations/vulnscan_de.ts

# Platform-specific installation
unix:!macx {
    target.path = /usr/local/bin
    INSTALLS += target
    
    desktop.files = ../../packaging/linux/vulnscan.desktop
    desktop.path = /usr/share/applications
    INSTALLS += desktop
    
    icon.files = ../../resources/icons/vulnscan.png
    icon.path = /usr/share/icons/hicolor/256x256/apps
    INSTALLS += icon
}

macx {
    ICON = ../../resources/icons/vulnscan.icns
    QMAKE_INFO_PLIST = ../../packaging/macos/Info.plist
}

win32 {
    RC_ICONS = ../../packaging/windows/vulnscan.ico
    VERSION = 1.0.0
}
```

### src/cli/cli.pro
```qmake
TEMPLATE = app
TARGET = vulnscan-cli
CONFIG += console
CONFIG -= app_bundle

include(../../common.pri)

QT -= gui widgets

LIBS += -L$DESTDIR -lvulnscan_core

HEADERS += \
    CliApplication.h \
    CommandParser.h \
    CommandExecutor.h \
    OutputFormatter.h

SOURCES += \
    main.cpp \
    CliApplication.cpp \
    CommandParser.cpp \
    CommandExecutor.cpp \
    OutputFormatter.cpp

# Installation
unix {
    target.path = /usr/local/bin
    INSTALLS += target
}
```

### tests/tests.pro
```qmake
TEMPLATE = app
TARGET = vulnscan_tests
CONFIG += testcase

include(../common.pri)

QT += testlib
QT -= gui widgets

LIBS += -L$DESTDIR -lvulnscan_core

HEADERS += \
    test_utils.h

SOURCES += \
    unit/scanner/test_port_scanner.cpp \
    unit/scanner/test_service_detector.cpp \
    unit/ai/test_claude_provider.cpp \
    unit/ai/test_prompt_builder.cpp \
    unit/database/test_scan_repository.cpp \
    integration/test_scan_workflow.cpp \
    test_utils.cpp
```

## 🎯 Requisiti Specifici per il Codice

### Principi SOLID (focus su SRP)
Ogni classe deve avere **UNA SOLA RESPONSABILITÀ**:

```cpp
// ❌ SBAGLIATO - Viola SRP
class Scanner {
    void scanPorts();           // responsabilità 1
    void detectServices();      // responsabilità 2
    void checkVulnerabilities(); // responsabilità 3
    void generateReport();      // responsabilità 4
    void saveToDatabase();      // responsabilità 5
};

// ✅ CORRETTO - Rispetta SRP
class PortScanner {
    QList<PortResult> scanPorts(const QString& host, const PortRange& range);
};

class ServiceDetector {
    ServiceInfo detectService(const QString& host, int port);
};

class VulnerabilityChecker {
    QList<Vulnerability> checkVulnerabilities(const ServiceInfo& service);
};

class ReportGenerator {
    Report generateReport(const ScanResults& results);
};

class ScanRepository {
    void saveScan(const ScanResults& results);
};
```

### Design Patterns Richiesti

1. **Strategy Pattern** per AI Providers
```cpp
/**
 * @interface IAIProvider
 * @brief Interface for AI analysis providers
 */
class IAIProvider {
public:
    virtual ~IAIProvider() = default;

    /**
     * @brief Analyzes vulnerabilities using AI
     * @param vulns List of detected vulnerabilities
     * @return AI analysis with recommendations
     */
    virtual AIAnalysis analyze(const QList<Vulnerability>& vulns) = 0;

    /**
     * @brief Gets the provider name
     * @return Provider display name (e.g., "Claude", "OpenAI", "OpenRouter")
     */
    virtual QString getName() const = 0;
};

class ClaudeProvider : public IAIProvider { /* ... */ };
class OpenAIProvider : public IAIProvider { /* ... */ };
class GeminiProvider : public IAIProvider { /* ... */ };

/**
 * @class GenericProvider
 * @brief Generic AI provider for custom endpoints (OpenRouter, Groq, Together AI, etc.)
 *
 * Supports any OpenAI-compatible API endpoint by configuring URL and API key.
 * This enables integration with aggregator services like OpenRouter or custom deployments.
 *
 * @note This provider supports custom HTTP headers for services requiring additional metadata
 */
class GenericProvider : public IAIProvider {
public:
    /**
     * @brief Constructs a generic provider with custom endpoint
     * @param name Provider display name (e.g., "OpenRouter", "Groq", "Custom")
     * @param endpoint API endpoint URL
     * @param apiKey Authentication API key
     * @param model Model identifier to use
     * @param apiFormat API format type (default: "openai")
     */
    GenericProvider(const QString& name,
                   const QString& endpoint,
                   const QString& apiKey,
                   const QString& model,
                   const QString& apiFormat = "openai");

    /**
     * @brief Adds a custom HTTP header to all requests
     * @param key Header name
     * @param value Header value
     */
    void addCustomHeader(const QString& key, const QString& value);
};
```

2. **Factory Pattern** per Report Generators
```cpp
class ReportGeneratorFactory {
public:
    static std::unique_ptr<IReportGenerator> create(ReportFormat format);
};
```

3. **Repository Pattern** per Database
```cpp
class IRepository<T> {
public:
    virtual void save(const T& entity) = 0;
    virtual std::optional<T> findById(int id) = 0;
    virtual QList<T> findAll() = 0;
};
```

4. **Observer Pattern** per Scan Progress
```cpp
class IScanObserver {
public:
    virtual void onScanStarted() = 0;
    virtual void onProgressUpdate(int percentage) = 0;
    virtual void onScanCompleted() = 0;
};
```

### Naming Conventions
- **Classes**: `PascalCase` (es: `PortScanner`)
- **Methods**: `camelCase` (es: `scanNetwork`)
- **Variables**: `camelCase` (es: `portNumber`)
- **Constants**: `UPPER_SNAKE_CASE` (es: `MAX_PORTS`)
- **Private members**: `m_` prefix (es: `m_database`)
- **Interfaces**: `I` prefix (es: `IScanner`)

## 💬 Comment and Documentation Language Policy

**TUTTI i commenti e la documentazione nel codice sorgente devono essere in INGLESE.**

### 1. Doxygen Documentation (OBBLIGATORIO in inglese)

```cpp
/**
 * @file PortScanner.h
 * @brief Port scanning functionality for network vulnerability assessment
 * @author VulnScan Development Team
 * @date 2025
 * @copyright MIT License
 */

/**
 * @class PortScanner
 * @brief Implements TCP/UDP port scanning with configurable timeout
 *
 * This class provides defensive port scanning capabilities for authorized
 * vulnerability assessment. It respects rate limiting and provides detailed
 * logging of all operations.
 *
 * @note This class should only be used on networks you own or have explicit
 *       written permission to test.
 */
class PortScanner : public QObject {
    // ...
};
```

### 2. Inline Comments (OBBLIGATORIO in inglese)

```cpp
// ✅ CORRETTO - Commenti in inglese
void PortScanner::scanPorts(const QString& host, const PortRange& range) {
    // Validate input range
    if (range.start < 1 || range.end > 65535) {
        throw std::invalid_argument("Invalid port range");
    }

    // Initialize thread pool with CPU core count
    m_threadPool->setMaxThreadCount(QThread::idealThreadCount());

    // Create worker for each port in range
    for (int port = range.start; port <= range.end; ++port) {
        auto* worker = new ScanWorker(host, port, m_timeout);
        m_threadPool->start(worker);
    }
}
```

```cpp
// ❌ EVITARE - Commenti in altre lingue
void PortScanner::scanPorts(const QString& host, const PortRange& range) {
    // Valida il range di input
    if (range.start < 1 || range.end > 65535) {
        throw std::invalid_argument("Invalid port range");
    }

    // Inizializza thread pool con numero di core CPU
    m_threadPool->setMaxThreadCount(QThread::idealThreadCount());
}
```

### 3. TODO/FIXME/NOTE/HACK Comments (OBBLIGATORIO in inglese)

```cpp
// ✅ CORRETTO
void VulnerabilityDatabase::loadCVEData() {
    // TODO: Implement incremental updates instead of full reload
    // FIXME: Memory leak when loading large CVE datasets (issue #123)
    // NOTE: This operation can take 5-10 seconds on slow systems
    // HACK: Temporary workaround for Qt 6.8 SQLite bug (remove after 6.9)

    loadAllCVEs();
}
```

### 4. Code Section Comments (in inglese)

```cpp
void ScanEngine::performFullScan(const QString& target) {
    // ====================================================================
    // Phase 1: Network Discovery
    // ====================================================================
    QList<QHostAddress> hosts = discoverHosts(target);

    // ====================================================================
    // Phase 2: Port Scanning
    // ====================================================================
    QList<PortResult> openPorts = scanAllPorts(hosts);

    // ====================================================================
    // Phase 3: Service Detection
    // ====================================================================
    QList<ServiceInfo> services = detectServices(openPorts);

    // ====================================================================
    // Phase 4: Vulnerability Analysis
    // ====================================================================
    QList<Vulnerability> vulnerabilities = analyzeVulnerabilities(services);
}
```

### 5. Complex Algorithm Explanation (in inglese)

```cpp
/**
 * @brief Implements exponential backoff for rate limiting
 *
 * Algorithm:
 * 1. Start with base delay (100ms)
 * 2. On each retry, double the delay (exponential backoff)
 * 3. Add random jitter to prevent thundering herd
 * 4. Cap maximum delay at 30 seconds
 *
 * Formula: delay = min(base * 2^retry + random(0, 100ms), max_delay)
 */
int AIProvider::calculateBackoffDelay(int retryCount) {
    const int BASE_DELAY_MS = 100;
    const int MAX_DELAY_MS = 30000;
    const int JITTER_MS = 100;

    // Calculate exponential backoff
    int delay = BASE_DELAY_MS * std::pow(2, retryCount);

    // Add random jitter to prevent synchronized retries
    int jitter = QRandomGenerator::global()->bounded(JITTER_MS);
    delay += jitter;

    // Cap at maximum delay
    return std::min(delay, MAX_DELAY_MS);
}
```

### 6. Tricky Code Explanation (in inglese)

```cpp
void NetworkInterface::parseSubnetMask(const QString& mask) {
    // Convert subnet mask to CIDR notation
    // Example: 255.255.255.0 -> /24

    QHostAddress addr(mask);
    quint32 ipv4 = addr.toIPv4Address();

    // Count consecutive 1 bits from left (network bits)
    // Uses bit manipulation: __builtin_popcount counts set bits
    int cidr = __builtin_popcount(ipv4);

    // Verify mask is contiguous (no gaps in 1 bits)
    // Valid: 11111111111111110000000000000000 (/16)
    // Invalid: 11111111000011110000000000000000 (gaps)
    quint32 contiguousMask = ~((1u << (32 - cidr)) - 1);
    if (ipv4 != contiguousMask) {
        throw std::invalid_argument("Non-contiguous subnet mask");
    }

    m_cidr = cidr;
}
```

### 7. WHY Comments (spiega il "perché", non il "cosa")

```cpp
// ✅ CORRETTO - Spiega il "perché"
void AIAnalyzer::analyzeVulnerabilities(const QList<Vulnerability>& vulns) {
    // Batch vulnerabilities in groups of 10 to avoid hitting API token limits
    // OpenAI GPT-4 has ~8k context window, each vuln ~200 tokens
    const int BATCH_SIZE = 10;

    // Process synchronously despite slower performance because:
    // 1. Rate limits: Most AI providers limit to 3-5 req/sec
    // 2. Cost optimization: Batching reduces total API calls
    // 3. Better context: AI sees related vulnerabilities together
    for (int i = 0; i < vulns.size(); i += BATCH_SIZE) {
        QList<Vulnerability> batch = vulns.mid(i, BATCH_SIZE);
        processVulnerabilityBatch(batch);
    }
}
```

```cpp
// ❌ EVITARE - Commenta l'ovvio (ridondante)
void PortScanner::setTimeout(int ms) {
    // Set timeout to ms
    m_timeout = ms;  // Il codice è già chiaro!
}
```

### 8. Temporary/Debug Comments (in inglese)

```cpp
void DatabaseManager::executeQuery(const QString& sql) {
    // qDebug() << "SQL:" << sql;  // Uncomment for debugging
    // qDebug() << "Execution time:" << timer.elapsed() << "ms";

    QSqlQuery query(m_database);
    query.exec(sql);
}
```

### 9. License Headers (in inglese)

```cpp
/**
 * @file VulnerabilityScanner.cpp
 * @brief Parallel vulnerability analysis implementation
 *
 * Copyright (c) 2025 VulnScan Development Team
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction...
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND...
 */
```

### 10. User-Facing Strings (MULTILINGUA tramite tr())

```cpp
// ✅ CORRETTO - Stringhe UI traducibili
QString PortScanner::getStatusMessage() const {
    if (m_isScanning) {
        return tr("Scanning port %1 of %2...").arg(m_currentPort).arg(m_totalPorts);
    }
    return tr("Scan completed. %n open port(s) found.", "", m_openPorts.size());
}

// Error messages for users (translatable)
QMessageBox::critical(
    this,
    tr("Scan Error"),
    tr("Failed to connect to target host: %1").arg(errorString)
);

// Log messages (English, not translated)
qCWarning(scannerLog) << "Connection timeout for host:" << host
                      << "port:" << port;
```

### Rationale (Perché inglese?)

1. **Collaboration**: Il codice può essere letto da sviluppatori internazionali
2. **Consistency**: Tutta la documentazione Qt e C++ è in inglese
3. **Tooling**: IDE, linters, e AI tools funzionano meglio con inglese
4. **Best Practice**: Standard nell'industria open source
5. **Future-proof**: Facilita contributi esterni e fork del progetto

### Exceptions (Eccezioni)

**Le uniche cose che possono essere in altre lingue:**

1. ✅ **User-facing strings** (tramite `tr()` per traduzioni)
2. ✅ **README.md e docs/** (possono avere versioni multilingua)
3. ✅ **Commit messages** (preferibilmente inglese, ma accettabile italiano)
4. ✅ **Issue/PR descriptions** su GitHub (preferibilmente inglese)

**Tutto il resto (codice, commenti, Doxygen) DEVE essere in inglese.**

## 📐 C++ Coding Standards

### Header Guards
**SEMPRE usare `#pragma once`** invece di include guards tradizionali:

```cpp
// ✅ CORRETTO
#pragma once

#include <QObject>

class PortScanner : public QObject {
    // ...
};
```

```cpp
// ❌ EVITARE (verbose e error-prone)
#ifndef PORTSCANNER_H
#define PORTSCANNER_H
// ...
#endif
```

### Include Order
**Ordine rigoroso** per tutti i file `.cpp`:

```cpp
// 1. Header corrispondente (se .cpp)
#include "PortScanner.h"

// 2. Qt headers (ordine alfabetico)
#include <QMutex>
#include <QThread>
#include <QThreadPool>

// 3. Standard library headers (ordine alfabetico)
#include <atomic>
#include <memory>
#include <vector>

// 4. Project headers (ordine alfabetico)
#include "ScanResult.h"
#include "ScanWorker.h"
```

**Nei file `.h`:**
```cpp
#pragma once

// 1. Qt headers necessari
#include <QObject>
#include <QString>

// 2. Standard library headers (solo se necessari nel .h)
#include <memory>

// 3. Forward declarations (prefer over includes)
class QThreadPool;
class ScanResult;

class PortScanner : public QObject {
    // ...
};
```

### Forward Declarations
**Usare forward declarations quando possibile** per ridurre dipendenze:

```cpp
// ✅ CORRETTO - Nel .h file
#pragma once

#include <QObject>

// Forward declarations
class QThreadPool;
class QMutex;
class ScanResult;
class ServiceInfo;

class PortScanner : public QObject {
    Q_OBJECT

public:
    explicit PortScanner(QObject* parent = nullptr);

    // Usa puntatori/reference a classi forward-declared
    void setScanResults(ScanResult* results);

private:
    QThreadPool* m_threadPool;  // OK: pointer
    // ScanResult m_result;     // ❌ ERRORE: serve include completo
};
```

```cpp
// Nel .cpp file - Include completi
#include "PortScanner.h"

#include <QMutex>
#include <QThreadPool>

#include "ScanResult.h"
#include "ServiceInfo.h"
```

### RAII Patterns (Obbligatori)

**Sempre usare RAII** per resource management:

```cpp
// ✅ CORRETTO - Mutex locking con RAII
void PortScanner::addResult(const ScanResult& result) {
    QMutexLocker locker(&m_mutex);  // Auto-lock
    m_results.append(result);
    // Auto-unlock quando locker esce dallo scope
}

// ❌ EVITARE - Manual lock/unlock (error-prone)
void PortScanner::addResult(const ScanResult& result) {
    m_mutex.lock();
    m_results.append(result);
    m_mutex.unlock();  // Dimenticato in caso di exception!
}
```

```cpp
// ✅ CORRETTO - File handling con RAII
bool ConfigLoader::loadConfig(const QString& path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    // file.close() chiamato automaticamente dal distruttore

    QTextStream stream(&file);
    // ...
    return true;
}
```

```cpp
// ✅ CORRETTO - Database transaction con RAII
class DatabaseTransaction {
public:
    explicit DatabaseTransaction(QSqlDatabase& db) : m_db(db), m_committed(false) {
        m_db.transaction();
    }

    ~DatabaseTransaction() {
        if (!m_committed) {
            m_db.rollback();  // Auto-rollback se non committed
        }
    }

    void commit() {
        m_db.commit();
        m_committed = true;
    }

private:
    QSqlDatabase& m_db;
    bool m_committed;
};

// Uso:
bool ScanRepository::saveScan(const ScanResult& result) {
    DatabaseTransaction transaction(m_database);

    // ... operazioni database ...

    transaction.commit();  // Explicit commit
    return true;
    // Auto-rollback se exception prima di commit
}
```

### Smart Pointer Usage Rules

**Ownership chiara con smart pointers:**

```cpp
// ✅ Ownership esclusivo
std::unique_ptr<PortScanner> scanner = std::make_unique<PortScanner>();

// ✅ Ownership condiviso
std::shared_ptr<VulnerabilityDatabase> db = std::make_shared<VulnerabilityDatabase>();

// ✅ Non-owning reference
std::weak_ptr<IAIProvider> providerRef = m_provider;

// ❌ EVITARE raw pointers per ownership
// PortScanner* scanner = new PortScanner();  // Chi lo dealloca?
```

**Qt parent-child ownership:**
```cpp
// ✅ CORRETTO - Qt gestisce lifetime via parent
auto* scanner = new PortScanner(this);  // 'this' è il parent
// Non serve delete, Qt lo fa automaticamente

// ✅ CORRETTO - Combinazione con smart pointers
class ScanEngine : public QObject {
public:
    ScanEngine(QObject* parent = nullptr)
        : QObject(parent),
          m_database(std::make_shared<VulnerabilityDatabase>()) {

        // Qt-managed object
        m_scanner = new PortScanner(this);

        // Shared ownership
        m_analyzer = std::make_shared<AIAnalyzer>();
    }

private:
    PortScanner* m_scanner;  // Owned by Qt parent
    std::shared_ptr<VulnerabilityDatabase> m_database;  // Shared
    std::shared_ptr<AIAnalyzer> m_analyzer;  // Shared
};
```

### Const Correctness

**Const ovunque possibile:**

```cpp
class PortScanner : public QObject {
public:
    // ✅ Const method (non modifica stato)
    int getTotalScans() const { return m_totalScans; }

    // ✅ Const parameter (pass by const reference)
    void addResult(const ScanResult& result);

    // ✅ Const return (se appropriato)
    const QList<ScanResult>& getResults() const { return m_results; }

    // ✅ Const pointer parameter
    void setDatabase(const VulnerabilityDatabase* db);

private:
    int m_totalScans;
    QList<ScanResult> m_results;
};
```

### nullptr vs NULL vs 0

```cpp
// ✅ SEMPRE usare nullptr
QObject* obj = nullptr;

if (obj != nullptr) {
    // ...
}

// ❌ EVITARE NULL o 0
// QObject* obj = NULL;  // C-style
// QObject* obj = 0;     // Ambiguous
```

### Auto Keyword Usage

```cpp
// ✅ CORRETTO - Auto per iteratori complessi
for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
    // ...
}

// ✅ CORRETTO - Auto con range-based for
for (const auto& result : results) {
    // ...
}

// ✅ CORRETTO - Auto per tipi verbosi
auto scanner = std::make_unique<PortScanner>();
auto future = QtConcurrent::run([]() { /* ... */ });

// ⚠️ ATTENZIONE - Non oscurare il tipo quando è importante
auto value = getSomeValue();  // Che tipo è? Non chiaro
int value = getSomeValue();   // Meglio: tipo esplicito
```

### Explicit Constructors

```cpp
// ✅ SEMPRE usare explicit per single-argument constructors
class PortScanner : public QObject {
public:
    explicit PortScanner(QObject* parent = nullptr);
    explicit PortScanner(int timeout);

    // OK: multi-argument, non serve explicit
    PortScanner(const QString& host, int port);
};

// Previene conversioni implicite:
void processScanner(const PortScanner& scanner);

// ❌ Senza explicit, questo compilerebbe:
// processScanner(5);  // Converte int a PortScanner!

// ✅ Con explicit, errore di compilazione
```

### Delete Special Members When Appropriate

```cpp
// ✅ Disabilita copy per classi non-copiabili
class PortScanner : public QObject {
    Q_OBJECT

public:
    explicit PortScanner(QObject* parent = nullptr);

    // Delete copy operations
    PortScanner(const PortScanner&) = delete;
    PortScanner& operator=(const PortScanner&) = delete;

    // Default move operations (optional)
    PortScanner(PortScanner&&) = default;
    PortScanner& operator=(PortScanner&&) = default;
};
```

### Doxygen Documentation Standards
Ogni file di codice deve includere commenti Doxygen per generare documentazione automatica:

```cpp
/**
 * @file PortScanner.h
 * @brief Port scanning functionality for network vulnerability assessment
 * @author VulnScan Development Team
 * @date 2025
 * @copyright MIT License
 */

/**
 * @class PortScanner
 * @brief Implements TCP/UDP port scanning with configurable timeout and parallelism
 *
 * This class provides defensive port scanning capabilities for authorized
 * vulnerability assessment. It respects rate limiting and provides detailed
 * logging of all operations.
 *
 * @note This class should only be used on networks you own or have explicit
 *       written permission to test.
 *
 * Example usage:
 * @code
 * PortScanner scanner;
 * scanner.setTimeout(5000);
 * QList<PortResult> results = scanner.scanPorts("192.168.1.1", PortRange(1, 1024));
 * @endcode
 */
class PortScanner {
public:
    /**
     * @brief Constructs a new PortScanner with default settings
     */
    PortScanner();

    /**
     * @brief Scans a range of ports on the specified host
     * @param host Target IP address or hostname
     * @param range Port range to scan (e.g., 1-65535)
     * @return List of port scan results with status and service information
     * @throws NetworkException if host is unreachable
     * @throws ScanException if scanning fails
     */
    QList<PortResult> scanPorts(const QString& host, const PortRange& range);

    /**
     * @brief Sets the connection timeout for port scanning
     * @param milliseconds Timeout value in milliseconds (default: 3000)
     */
    void setTimeout(int milliseconds);

private:
    int m_timeout; ///< Connection timeout in milliseconds
    QThreadPool* m_threadPool; ///< Thread pool for parallel scanning
};
```

**Requisiti Doxygen:**
- Usa `@brief` per descrizioni brevi
- Usa `@param` per parametri con descrizione
- Usa `@return` per valori di ritorno
- Usa `@throws` per eccezioni
- Usa `@note`, `@warning`, `@todo` dove appropriato
- Usa `@code` e `@endcode` per esempi
- Commenta membri privati con `///< descrizione`
- Includi `@file`, `@author`, `@date`, `@copyright` in ogni file

### Error Handling
```cpp
// Usa eccezioni custom
class ScanException : public std::runtime_error {
public:
    explicit ScanException(const QString& message);
};

class NetworkException : public ScanException { /* ... */ };
class DatabaseException : public ScanException { /* ... */ };
```

### Dependency Injection
```cpp
// ✅ Constructor injection
class ScanEngine {
public:
    ScanEngine(
        std::shared_ptr<IScanner> scanner,
        std::shared_ptr<IVulnerabilityChecker> vulnChecker,
        std::shared_ptr<IRepository<ScanResult>> repository
    );
};
```

## 🔄 Multi-Threading & Parallelization

### Threading Strategy
VulnScan implementa una **architettura multi-threaded** per massimizzare le performance:

#### 1. **Port Scanning Parallelization**
```cpp
/**
 * @class PortScanner
 * @brief Thread-safe parallel port scanner using QThreadPool
 */
class PortScanner : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Scans ports in parallel using thread pool
     * @param host Target host
     * @param portRange Range of ports to scan
     * @param maxThreads Maximum concurrent threads (default: CPU cores)
     * @return List of scan results
     */
    QList<PortResult> scanPorts(const QString& host,
                                const PortRange& portRange,
                                int maxThreads = QThread::idealThreadCount());

signals:
    /**
     * @brief Emitted for each completed port scan
     * @param result Single port scan result
     */
    void portScanned(const PortResult& result);

    /**
     * @brief Emitted to report scan progress
     * @param current Current port number
     * @param total Total ports to scan
     */
    void progressUpdate(int current, int total);

private:
    QThreadPool* m_threadPool; ///< Thread pool for parallel scanning
    QMutex m_resultsMutex; ///< Protects shared results list
    std::atomic<int> m_completedScans; ///< Thread-safe scan counter
};

/**
 * @class ScanWorker
 * @brief Worker class for parallel port scanning tasks
 */
class ScanWorker : public QRunnable {
public:
    /**
     * @brief Constructs a scan worker for a single port
     * @param host Target host
     * @param port Port to scan
     * @param timeout Connection timeout in milliseconds
     */
    ScanWorker(const QString& host, int port, int timeout);

    void run() override;

signals:
    void scanCompleted(const PortResult& result);
};
```

#### 2. **Vulnerability Analysis Parallelization**
```cpp
/**
 * @class VulnerabilityScanner
 * @brief Parallel vulnerability analysis engine
 *
 * Analyzes multiple services concurrently for vulnerabilities.
 * Thread-safe design with mutex protection for shared resources.
 */
class VulnerabilityScanner : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Analyzes services in parallel for vulnerabilities
     * @param services List of detected services
     * @param maxThreads Maximum concurrent threads
     * @return List of detected vulnerabilities
     */
    QList<Vulnerability> analyzeServices(const QList<ServiceInfo>& services,
                                        int maxThreads = QThread::idealThreadCount());

signals:
    /**
     * @brief Emitted when a service analysis completes
     */
    void serviceAnalyzed(const ServiceInfo& service,
                        const QList<Vulnerability>& vulns);

    void progressUpdate(int current, int total);

private:
    QThreadPool* m_threadPool; ///< Thread pool for parallel checks
    QMutex m_databaseMutex; ///< Protects CVE database access
    QMutex m_resultsMutex; ///< Protects vulnerability results
    std::atomic<bool> m_cancelled; ///< Cancellation flag

    /**
     * @brief Thread-safe CVE database query
     */
    QList<CVE> queryCVEThreadSafe(const ServiceInfo& service);
};

/**
 * @class VulnerabilityWorker
 * @brief Worker for parallel vulnerability checks
 */
class VulnerabilityWorker : public QRunnable {
public:
    VulnerabilityWorker(const ServiceInfo& service,
                       VulnerabilityDatabase* database,
                       QMutex* dbMutex);

    void run() override;

signals:
    void analysisCompleted(const QList<Vulnerability>& vulns);
};
```

#### 3. **AI Analysis Parallelization**
```cpp
/**
 * @class AIAnalyzer
 * @brief Parallel AI analysis with multiple providers
 *
 * Supports concurrent calls to multiple AI providers for faster analysis
 * and fallback capabilities.
 */
class AIAnalyzer : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Analyzes vulnerabilities using AI (async)
     * @param vulns Vulnerabilities to analyze
     * @param providers List of AI providers to use
     * @param parallel If true, queries all providers concurrently
     * @return Future containing AI analysis results
     */
    QFuture<AIAnalysisResult> analyzeAsync(
        const QList<Vulnerability>& vulns,
        const QList<IAIProvider*>& providers,
        bool parallel = true
    );

    /**
     * @brief Analyzes with automatic fallback
     *
     * Tries primary provider first, falls back to secondary if timeout/error
     */
    QFuture<AIAnalysisResult> analyzeWithFallback(
        const QList<Vulnerability>& vulns,
        IAIProvider* primary,
        IAIProvider* fallback,
        int timeoutMs = 30000
    );

signals:
    void analysisStarted(const QString& providerName);
    void analysisCompleted(const AIAnalysisResult& result);
    void analysisFailed(const QString& providerName, const QString& error);

private:
    QThreadPool* m_threadPool; ///< Dedicated thread pool for AI calls
    QMutex m_rateLimitMutex; ///< Protects rate limiting state

    /**
     * @brief Executes AI call in separate thread with timeout
     */
    void executeAICallAsync(IAIProvider* provider,
                           const QList<Vulnerability>& vulns);
};

/**
 * @class AIWorker
 * @brief Worker for asynchronous AI API calls
 */
class AIWorker : public QRunnable {
public:
    AIWorker(IAIProvider* provider,
            const QList<Vulnerability>& vulns,
            int timeoutMs);

    void run() override;

signals:
    void completed(const AIAnalysisResult& result);
    void failed(const QString& error);

private:
    std::atomic<bool> m_cancelled; ///< Cancellation support
    QTimer* m_timeoutTimer; ///< Timeout enforcement
};
```

#### 4. **Thread Safety Requirements**

**Tutte le classi multi-threaded devono:**

1. **Usare QMutex** per proteggere risorse condivise:
```cpp
QMutexLocker locker(&m_resultsMutex); // RAII lock
m_results.append(newResult);
```

2. **Usare std::atomic** per contatori e flags:
```cpp
std::atomic<int> m_progress{0};
std::atomic<bool> m_cancelled{false};
```

3. **Emettere signal thread-safe** con `Qt::QueuedConnection`:
```cpp
emit progressUpdate(current, total); // Auto-queued from worker threads
```

4. **Database access thread-safe**:
```cpp
// Usa connection pooling o mutex per SQLite
QMutexLocker locker(&m_databaseMutex);
QSqlQuery query(m_database);
```

5. **Cancellation support**:
```cpp
void cancelScan() {
    m_cancelled.store(true, std::memory_order_release);
    m_threadPool->clear(); // Clear pending tasks
}
```

6. **Progress tracking thread-safe**:
```cpp
int progress = m_completedScans.fetch_add(1, std::memory_order_relaxed);
emit progressUpdate(progress + 1, m_totalScans);
```

### Performance Considerations

- **Thread pool size**: Default = `QThread::idealThreadCount()` (CPU cores)
- **Port scanning**: Batch di 100-1000 porte per thread
- **Vulnerability checks**: 1 servizio per thread
- **AI calls**: Max 3-5 concurrent requests (rate limiting API)
- **Memory**: Limit result accumulation, stream to database
- **Timeout**: Implement per-thread timeout con `QTimer` o `QDeadlineTimer`

### Testing Multi-Threading

```cpp
class ThreadSafetyTest : public QObject {
    Q_OBJECT

private slots:
    void testConcurrentPortScanning();
    void testConcurrentVulnerabilityAnalysis();
    void testThreadSafeDatabaseAccess();
    void testCancellationSupport();
    void testProgressTracking();
    void testMemoryLeaks(); // Con valgrind/sanitizers
};
```

## 🎯 Qt-Specific Requirements

### Q_OBJECT Macro Usage

**SEMPRE includere Q_OBJECT** in classi che usano signals/slots:

```cpp
// ✅ CORRETTO
class PortScanner : public QObject {
    Q_OBJECT  // ⚠️ OBBLIGATORIO per signals/slots

public:
    explicit PortScanner(QObject* parent = nullptr);

signals:
    void scanCompleted();

public slots:
    void startScan();
};
```

**Q_OBJECT richiede:**
- La classe deve ereditare da `QObject` (direttamente o indirettamente)
- Il file `.h` deve essere processato dal **MOC** (Meta-Object Compiler)
- Il file deve essere listato in `HEADERS` nel `.pro` file

### QRunnable Limitations

**IMPORTANTE: QRunnable NON supporta signals/slots** (non eredita da QObject):

```cpp
// ❌ ERRORE - QRunnable non può avere signals
class ScanWorker : public QRunnable {
    Q_OBJECT  // ❌ ERRORE: QRunnable non eredita da QObject

signals:  // ❌ NON FUNZIONA
    void completed();
};
```

**Soluzioni alternative:**

**Opzione 1: QObject + moveToThread**
```cpp
// ✅ CORRETTO - Usa QObject per signals
class ScanWorker : public QObject {
    Q_OBJECT

public:
    explicit ScanWorker(QObject* parent = nullptr);

    void doWork();  // Chiamato nel thread

signals:
    void workCompleted(const ScanResult& result);
    void workFailed(const QString& error);
};

// Uso:
auto* worker = new ScanWorker();
auto* thread = new QThread();
worker->moveToThread(thread);

connect(thread, &QThread::started, worker, &ScanWorker::doWork);
connect(worker, &ScanWorker::workCompleted, this, &PortScanner::handleResult);
connect(worker, &ScanWorker::workFailed, thread, &QThread::quit);
connect(thread, &QThread::finished, worker, &QObject::deleteLater);
connect(thread, &QThread::finished, thread, &QObject::deleteLater);

thread->start();
```

**Opzione 2: QRunnable con callback**
```cpp
// ✅ CORRETTO - QRunnable con std::function callback
class ScanWorker : public QRunnable {
public:
    using CompletionCallback = std::function<void(const ScanResult&)>;
    using ErrorCallback = std::function<void(const QString&)>;

    ScanWorker(const QString& host, int port,
              CompletionCallback onComplete,
              ErrorCallback onError)
        : m_host(host),
          m_port(port),
          m_onComplete(onComplete),
          m_onError(onError) {
        setAutoDelete(true);  // QThreadPool dealloca automaticamente
    }

    void run() override {
        try {
            ScanResult result = performScan(m_host, m_port);
            if (m_onComplete) {
                m_onComplete(result);  // Callback thread-safe
            }
        } catch (const std::exception& e) {
            if (m_onError) {
                m_onError(QString::fromStdString(e.what()));
            }
        }
    }

private:
    QString m_host;
    int m_port;
    CompletionCallback m_onComplete;
    ErrorCallback m_onError;

    ScanResult performScan(const QString& host, int port);
};

// Uso con QThreadPool:
auto* worker = new ScanWorker(
    "192.168.1.1",
    80,
    [this](const ScanResult& result) {
        // Callback eseguito nel worker thread
        // Usa QMetaObject::invokeMethod per thread-safety se necessario
        QMetaObject::invokeMethod(this, [this, result]() {
            handleResult(result);  // Eseguito nel main thread
        }, Qt::QueuedConnection);
    },
    [this](const QString& error) {
        QMetaObject::invokeMethod(this, [this, error]() {
            handleError(error);
        }, Qt::QueuedConnection);
    }
);

QThreadPool::globalInstance()->start(worker);
```

**Opzione 3: QtConcurrent (più semplice)**
```cpp
// ✅ CORRETTO - QtConcurrent::run con QFuture
QFuture<ScanResult> future = QtConcurrent::run([host, port]() {
    return performScan(host, port);
});

// Monitor con QFutureWatcher
auto* watcher = new QFutureWatcher<ScanResult>(this);
connect(watcher, &QFutureWatcher<ScanResult>::finished, this, [this, watcher]() {
    ScanResult result = watcher->result();
    handleResult(result);
    watcher->deleteLater();
});
watcher->setFuture(future);
```

### Signal/Slot Connection Types

**Specifica il tipo di connessione appropriato:**

```cpp
// Qt::AutoConnection (default)
// - Same thread: direct call
// - Different threads: queued
connect(sender, &Sender::signal, receiver, &Receiver::slot);

// Qt::DirectConnection
// - Always direct call (anche cross-thread, PERICOLOSO!)
connect(sender, &Sender::signal, receiver, &Receiver::slot, Qt::DirectConnection);

// Qt::QueuedConnection
// - Always queued via event loop (thread-safe)
connect(sender, &Sender::signal, receiver, &Receiver::slot, Qt::QueuedConnection);

// Qt::BlockingQueuedConnection
// - Queued + wait for completion (solo cross-thread)
connect(sender, &Sender::signal, receiver, &Receiver::slot, Qt::BlockingQueuedConnection);
```

**Best practices:**

```cpp
// ✅ Worker thread -> Main thread: Usa Qt::QueuedConnection
connect(workerThread, &WorkerThread::resultReady,
        this, &MainWindow::handleResult,
        Qt::QueuedConnection);  // Esplicito per chiarezza

// ✅ Same thread: AutoConnection è OK
connect(button, &QPushButton::clicked,
        this, &MainWindow::onButtonClicked);

// ❌ EVITARE DirectConnection cross-thread
// connect(workerThread, &WorkerThread::resultReady,
//         this, &MainWindow::handleResult,
//         Qt::DirectConnection);  // RACE CONDITIONS!
```

### QObject Parent-Child Ownership

**Qt gestisce automaticamente la memoria per parent-child relationship:**

```cpp
// ✅ CORRETTO - Parent gestisce i children
class MainWindow : public QWidget {
public:
    MainWindow() {
        // 'this' è il parent, Qt dealloca automaticamente
        m_scanner = new PortScanner(this);
        m_database = new VulnerabilityDatabase(this);
        m_analyzer = new AIAnalyzer(this);

        // Widgets
        auto* layout = new QVBoxLayout(this);  // Parent = this
        auto* button = new QPushButton("Scan", this);
        layout->addWidget(button);
    }

    // ❌ Non serve distruttore, Qt gestisce tutto
    // ~MainWindow() {
    //     delete m_scanner;  // NO! Double-delete
    // }

private:
    PortScanner* m_scanner;
    VulnerabilityDatabase* m_database;
    AIAnalyzer* m_analyzer;
};
```

**Eccezioni:**
```cpp
// ⚠️ Oggetti senza parent NON sono deallocati automaticamente
auto* scanner = new PortScanner();  // ❌ Memory leak!

// ✅ Opzioni:
// 1. Fornire parent
auto* scanner = new PortScanner(this);

// 2. Usare smart pointer
auto scanner = std::make_unique<PortScanner>();

// 3. Usare deleteLater() per delete thread-safe
scanner->deleteLater();  // Qt dealloca nel prossimo event loop
```

### Qt Containers vs STL Containers

**Preferire Qt containers con Qt APIs:**

```cpp
// ✅ CORRETTO - Qt containers
QList<ScanResult> results;
QVector<int> ports;
QMap<QString, int> hostPorts;
QHash<QString, ServiceInfo> services;  // Più veloce di QMap

// ✅ OK - STL containers (quando serve compatibilità STL)
std::vector<ScanResult> results;
std::unordered_map<std::string, int> map;

// Conversione Qt <-> STL:
QList<int> qlist = {1, 2, 3};
std::vector<int> vec = qlist.toStdVector();
QList<int> qlist2 = QList<int>::fromStdVector(vec);
```

**Differenze chiave:**

| Feature | Qt Containers | STL Containers |
|---------|--------------|----------------|
| **Copy-on-write** | ✅ Implicit sharing | ❌ Deep copy |
| **Qt foreach** | ✅ Supportato | ❌ No |
| **Range-for** | ✅ Supportato | ✅ Supportato |
| **Qt signals** | ✅ Nativamente | ⚠️ Richiede conversione |
| **Performance** | Good | Excellent |

### MOC (Meta-Object Compiler) Requirements

**MOC processa automaticamente i file listati in HEADERS:**

```qmake
# .pro file
HEADERS += \
    scanner/PortScanner.h \    # MOC genera moc_PortScanner.cpp
    network/NetworkInterface.h  # MOC genera moc_NetworkInterface.cpp

SOURCES += \
    scanner/PortScanner.cpp \
    network/NetworkInterface.cpp
```

**MOC genera codice per:**
- Signals e slots
- Properties (`Q_PROPERTY`)
- Invokable methods (`Q_INVOKABLE`)
- Meta-type system (`QMetaObject`)

**Errori comuni MOC:**

```cpp
// ❌ ERRORE - Q_OBJECT in .cpp file
// PortScanner.cpp
class PortScannerPrivate : public QObject {
    Q_OBJECT  // ❌ MOC non processa .cpp files
signals:
    void completed();
};

// ✅ SOLUZIONE - Sposta in .h o usa separate header
// PortScannerPrivate.h
class PortScannerPrivate : public QObject {
    Q_OBJECT
signals:
    void completed();
};
```

```cpp
// ❌ ERRORE - Dimenticato Q_OBJECT
class PortScanner : public QObject {
    // Q_OBJECT  ← MANCANTE!

signals:  // ❌ Errore di compilazione
    void scanCompleted();
};
```

## 📚 Common Qt Headers per Module

### Core Module (tutti i subprojects)
```cpp
#include <QObject>        // Base per signals/slots
#include <QString>        // String handling
#include <QList>          // Lista dinamica
#include <QVector>        // Vettore ottimizzato
#include <QMap>           // Mappa ordinata
#include <QHash>          // Hash table
#include <QVariant>       // Tipo variant
#include <QByteArray>     // Array di byte
#include <QFile>          // File I/O
#include <QDir>           // Directory operations
#include <QDateTime>      // Date/time
#include <QTimer>         // Timer/delayed execution
#include <QDebug>         // Debug output
```

### Network Module
```cpp
#include <QTcpSocket>      // TCP client
#include <QTcpServer>      // TCP server
#include <QUdpSocket>      // UDP socket
#include <QHostAddress>    // IP address
#include <QHostInfo>       // DNS lookup
#include <QNetworkAccessManager>  // HTTP/HTTPS client
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslSocket>      // SSL/TLS
#include <QSslConfiguration>
#include <QSslCertificate>
```

### SQL Module (Database)
```cpp
#include <QSqlDatabase>    // Database connection
#include <QSqlQuery>       // SQL query execution
#include <QSqlError>       // Error handling
#include <QSqlRecord>      // Query result record
#include <QSqlDriver>      // Driver info
```

### Concurrent Module (Threading)
```cpp
#include <QThread>         // Thread management
#include <QThreadPool>     // Thread pool
#include <QRunnable>       // Thread pool task
#include <QMutex>          // Mutex lock
#include <QMutexLocker>    // RAII mutex lock
#include <QReadWriteLock>  // Reader-writer lock
#include <QSemaphore>      // Semaphore
#include <QWaitCondition>  // Condition variable
#include <QtConcurrent>    // High-level concurrency
#include <QFuture>         // Future/promise
#include <QFutureWatcher>  // Future monitoring
#include <atomic>          // std::atomic (C++11)
```

### Widgets Module (GUI only)
```cpp
#include <QWidget>         // Base widget
#include <QMainWindow>     // Main window
#include <QDialog>         // Dialog window
#include <QPushButton>     // Button
#include <QLabel>          // Text label
#include <QLineEdit>       // Text input
#include <QTextEdit>       // Multi-line text
#include <QTableView>      // Table view
#include <QTreeView>       // Tree view
#include <QListView>       // List view
#include <QTableWidget>    // Simple table
#include <QTreeWidget>     // Simple tree
#include <QListWidget>     // Simple list
#include <QLayout>         // Layout base
#include <QVBoxLayout>     // Vertical layout
#include <QHBoxLayout>     // Horizontal layout
#include <QGridLayout>     // Grid layout
#include <QMessageBox>     // Message dialog
#include <QFileDialog>     // File picker
#include <QProgressBar>    // Progress bar
#include <QStatusBar>      // Status bar
#include <QMenuBar>        // Menu bar
#include <QToolBar>        // Tool bar
#include <QAction>         // Menu/toolbar action
```

### Standard Library Headers (Common)
```cpp
#include <memory>          // std::unique_ptr, std::shared_ptr
#include <functional>      // std::function
#include <algorithm>       // std::sort, std::find, etc.
#include <string>          // std::string
#include <vector>          // std::vector
#include <map>             // std::map
#include <unordered_map>   // std::unordered_map
#include <set>             // std::set
#include <exception>       // std::exception
#include <stdexcept>       // std::runtime_error, etc.
#include <atomic>          // std::atomic
#include <chrono>          // std::chrono (timing)
#include <optional>        // std::optional (C++17)
#include <variant>         // std::variant (C++17)
```

## ⚠️ Error Handling Patterns

### Exception Safety in Threads

**SEMPRE catturare eccezioni nei thread:**

```cpp
// ✅ CORRETTO - Worker con exception handling
class ScanWorker : public QRunnable {
public:
    void run() override {
        try {
            // Codice che può lanciare eccezioni
            ScanResult result = performScan();

            // Notifica successo (thread-safe)
            QMetaObject::invokeMethod(m_receiver, [this, result]() {
                m_receiver->handleSuccess(result);
            }, Qt::QueuedConnection);

        } catch (const NetworkException& e) {
            // Gestisci eccezione specifica
            QMetaObject::invokeMethod(m_receiver, [this, e]() {
                m_receiver->handleNetworkError(e.what());
            }, Qt::QueuedConnection);

        } catch (const std::exception& e) {
            // Gestisci eccezione generica
            QMetaObject::invokeMethod(m_receiver, [this, e]() {
                m_receiver->handleError(e.what());
            }, Qt::QueuedConnection);

        } catch (...) {
            // ⚠️ OBBLIGATORIO - Cattura tutto
            QMetaObject::invokeMethod(m_receiver, [this]() {
                m_receiver->handleError("Unknown error occurred");
            }, Qt::QueuedConnection);
        }
    }

private:
    QObject* m_receiver;
};
```

**❌ MAI lasciare propagare eccezioni fuori da thread:**
```cpp
// ❌ ERRORE - Eccezione uncaught termina il programma
void ScanWorker::run() {
    performScan();  // Se lancia eccezione → CRASH!
}
```

### RAII for Exception Safety

```cpp
// ✅ CORRETTO - Tutti i lock rilasciati anche con exception
void VulnerabilityScanner::analyzeService(const ServiceInfo& service) {
    QMutexLocker dbLock(&m_databaseMutex);  // Auto-unlock on exception
    QMutexLocker resultsLock(&m_resultsMutex);

    // Se query lancia exception, entrambi i lock sono rilasciati automaticamente
    QList<CVE> cves = m_database->queryCVE(service);

    resultsLock.unlock();  // Rilascia early se possibile
    processResults(cves);
}
```

### Exception Specifications (C++17)

```cpp
// ✅ CORRETTO - noexcept per funzioni che non lanciano mai
class PortScanner {
public:
    // noexcept garantisce no exceptions
    int getPort() const noexcept { return m_port; }

    // Destructors sono implicitamente noexcept
    ~PortScanner();  // noexcept implicito

    // Move operations dovrebbero essere noexcept
    PortScanner(PortScanner&& other) noexcept;
    PortScanner& operator=(PortScanner&& other) noexcept;
};
```

### Error Return vs Exceptions

**Usa exceptions per errori eccezionali, return values per errori attesi:**

```cpp
// ✅ CORRETTO - Return value per errori attesi
std::optional<ScanResult> PortScanner::scanPort(int port) {
    if (port < 1 || port > 65535) {
        return std::nullopt;  // Invalid input
    }

    try {
        return performScan(port);
    } catch (const TimeoutException&) {
        return std::nullopt;  // Timeout è atteso
    }
}

// ✅ CORRETTO - Exception per errori eccezionali
QList<CVE> VulnerabilityDatabase::queryCVE(const QString& cveId) {
    if (!m_database.isOpen()) {
        throw DatabaseException("Database not open");  // Errore grave
    }

    // ...
}
```

### Qt Error Handling (No Exceptions)

**Molte Qt APIs non usano exceptions:**

```cpp
// ✅ CORRETTO - Controlla error state
QFile file("config.json");
if (!file.open(QIODevice::ReadOnly)) {
    qCritical() << "Failed to open file:" << file.errorString();
    return false;
}

QSqlQuery query(m_database);
if (!query.exec("SELECT * FROM scans")) {
    qCritical() << "Query failed:" << query.lastError().text();
    return false;
}

QTcpSocket socket;
socket.connectToHost("192.168.1.1", 80);
if (!socket.waitForConnected(3000)) {
    qWarning() << "Connection failed:" << socket.errorString();
    return std::nullopt;
}
```

### Logging Best Practices

```cpp
// Qt Logging Categories
Q_LOGGING_CATEGORY(scannerLog, "vulnscan.scanner")
Q_LOGGING_CATEGORY(networkLog, "vulnscan.network")
Q_LOGGING_CATEGORY(databaseLog, "vulnscan.database")

// Uso:
qCDebug(scannerLog) << "Starting scan on host:" << host;
qCInfo(networkLog) << "Connected to" << address;
qCWarning(databaseLog) << "Slow query detected:" << query;
qCCritical(scannerLog) << "Critical error:" << error;

// In production, disabilita debug:
// QLoggingCategory::setFilterRules("vulnscan.*.debug=false");
```

## 🤖 AI Integration - Multiple Providers

### Configuration File (config.json)
```json
{
  "ai": {
    "enabled": true,
    "default_provider": "openrouter",
    "providers": {
      "claude": {
        "type": "claude",
        "api_key": "",
        "model": "claude-sonnet-4-20250514",
        "endpoint": "https://api.anthropic.com/v1/messages"
      },
      "openai": {
        "type": "openai",
        "api_key": "",
        "model": "gpt-4-turbo",
        "endpoint": "https://api.openai.com/v1/chat/completions"
      },
      "gemini": {
        "type": "gemini",
        "api_key": "",
        "model": "gemini-pro",
        "endpoint": "https://generativelanguage.googleapis.com/v1beta"
      },
      "openrouter": {
        "type": "generic",
        "name": "OpenRouter",
        "api_key": "",
        "endpoint": "https://openrouter.ai/api/v1/chat/completions",
        "model": "anthropic/claude-3.5-sonnet",
        "api_format": "openai",
        "headers": {
          "HTTP-Referer": "https://github.com/vulnscan/vulnscan",
          "X-Title": "VulnScan"
        }
      },
      "groq": {
        "type": "generic",
        "name": "Groq",
        "enabled": false,
        "api_key": "",
        "endpoint": "https://api.groq.com/openai/v1/chat/completions",
        "model": "llama-3.1-70b-versatile",
        "api_format": "openai"
      },
      "custom": {
        "type": "generic",
        "name": "Custom Provider",
        "enabled": false,
        "api_key": "",
        "endpoint": "https://your-custom-endpoint.com/v1/chat/completions",
        "model": "your-model-name",
        "api_format": "openai",
        "headers": {}
      },
      "local": {
        "type": "generic",
        "name": "Local Ollama",
        "enabled": false,
        "endpoint": "http://localhost:11434/v1/chat/completions",
        "model": "llama2",
        "api_format": "openai",
        "api_key": "not-needed"
      }
    }
  }
}
```

### AI Provider Implementation Requirements
Ogni provider deve:
1. Implementare `IAIProvider` interface
2. Gestire autenticazione API (con supporto custom headers per `GenericProvider`)
3. Costruire prompt appropriati
4. Parsare risposte JSON
5. Gestire rate limiting
6. Implementare retry logic
7. Logging di tutte le chiamate

### GenericProvider Features
Il `GenericProvider` deve supportare:
- **Custom endpoints**: Qualsiasi URL compatibile OpenAI API
- **Custom headers**: Per servizi come OpenRouter che richiedono `HTTP-Referer`, `X-Title`, ecc.
- **Multiple API formats**: OpenAI-compatible, Anthropic-compatible (future)
- **Dynamic configuration**: Aggiunta/rimozione provider senza ricompilazione
- **Provider presets**: OpenRouter, Groq, Together AI, Ollama preconfigurati

**Use cases principali:**
- **OpenRouter**: Accesso a 200+ modelli con singola API key e routing automatico
- **Groq**: Inferenza ultra-veloce per modelli open source
- **Together AI**: Modelli open source con pricing competitivo
- **Ollama/LM Studio**: LLM locali senza costi API
- **Custom deployments**: vLLM, TGI, o deployment proprietari

## 📝 File di Configurazione GitHub

### .github/workflows/build.yml
- Build matrix per Linux (Ubuntu/Fedora), Windows, macOS
- Install Qt via aqtinstall or system packages
- Run `qmake` and `make`/`nmake`
- Cache Qt installation
- Run clang-format check
- Run unit tests (./vulnscan_tests)
- Upload artifacts (binaries)

### .github/workflows/release.yml
- Trigger su tags `v*.*.*`
- Build per tutte le piattaforme con qmake
- Deploy Qt dependencies (windeployqt, macdeployqt, linuxdeploy)
- Genera AppImage (Linux), NSIS installer (Windows), DMG (macOS)
- Calcola SHA256 checksums
- Crea GitHub Release con changelog automatico
- Upload binary assets

### .github/workflows/codeql.yml
- Security scanning
- CodeQL analysis per C++

### .github/ISSUE_TEMPLATE/
- bug_report.md con template strutturato
- feature_request.md
- security_vulnerability.md (private reporting)

## 📚 Documentation Requirements

### README.md deve includere:
- Banner/logo (placeholder)
- Badges (build status, license, version, stars)
- Screenshot/GIF demo (placeholder)
- Quick start (3-4 comandi)
- Feature list con emoji
- Architecture diagram (ASCII art o Mermaid)
- Installation per ogni OS
- Usage examples (GUI e CLI)
- Comparison table con altri scanner
- Contributing link
- License info
- Star history placeholder
- Contact info

### CONTRIBUTING.md deve includere:
- Code of Conduct reference
- Development setup steps
- Coding standards (con esempi)
- Commit message convention (Conventional Commits)
- Branch naming
- PR process
- Testing requirements

### docs/architecture.md deve includere:
- Layer architecture diagram
- Component descriptions
- Class diagrams (Mermaid)
- Sequence diagrams per main workflows
- Design decisions rationale

## 🧪 Testing Requirements

- **Coverage minima**: 60%
- Unit tests per ogni classe core
- Integration tests per workflows
- Mock objects per external dependencies
- Qt Test framework
- Test fixtures e helpers

```cpp
class PortScannerTest : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();    // setup
    void cleanupTestCase(); // teardown
    void testScanOpenPort();
    void testScanClosedPort();
    void testScanInvalidHost();
    void testScanTimeout();
};
```

## 🔧 CMake Structure

### Root CMakeLists.txt features:
- Project version from git tags
- Options: BUILD_GUI, BUILD_CLI, BUILD_TESTS, ENABLE_AI
- Find Qt6 packages
- Subdirectories
- CPack configuration per piattaforma
- Installation rules
- Summary output

### Per ogni subdirectory:
- Separate CMakeLists.txt
- Library targets dove appropriato
- Link dependencies esplicite

## 📦 Additional Files

### .clang-format
Configura stile Qt con modifiche

### .gitignore
Ignora:
- Build directories (build/, debug/, release/)
- Qt Creator files (*.pro.user, *.autosave)
- Qt generated files (moc_*, ui_*, qrc_*, *.o, *.obj)
- Qt Linguist compiled translations (*.qm)
- Makefiles (Makefile, Makefile.*)
- Binary outputs (vulnscan, vulnscan.exe, *.dll, *.so, *.dylib)
- IDE files (.vscode, .idea)
- Test coverage files
- Doxygen output (docs/doxygen/html/, docs/doxygen/latex/)

### SECURITY.md
- Responsible disclosure policy
- Contact email
- PGP key (optional)
- Supported versions

### CHANGELOG.md
- Formato Keep a Changelog
- Sezioni: Added, Changed, Fixed, Removed

### CODE_OF_CONDUCT.md
- Contributor Covenant 2.1

## 🎨 UI/UX Guidelines

### Qt GUI deve avere:
- Dark mode e light mode
- Toolbar con icone standard Qt
- Status bar con progress
- Dockable widgets
- Keyboard shortcuts
- Context menus
- Tooltips informativi
- Settings dialog organizzato a tabs
- **Custom AI Provider Dialog** con:
  - ☑️ Enable/Disable provider
  - 📝 Provider Name field
  - 🔗 Endpoint URL field (con validation)
  - 🔑 API Key field (password masked)
  - 🤖 Model identifier field
  - 📋 API Format selector (OpenAI-compatible, Anthropic-compatible)
  - ➕ Custom Headers table (key-value pairs)
  - 🧪 "Test Connection" button
  - 💾 Save/Cancel buttons
  - 📖 Preset selector (OpenRouter, Groq, Together AI, Ollama, Custom)

### CLI deve supportare:
```bash
vulnscan-cli scan --target <IP/range> [options]
vulnscan-cli report --scan-id <ID> --format <fmt>
vulnscan-cli list-scans
vulnscan-cli update-db
vulnscan-cli config --set ai.provider=claude
vulnscan-cli --help
vulnscan-cli --version
```

## ⚠️ Security & Legal

### DISCLAIMER OBBLIGATORIO
Includi in README e all'avvio dell'applicazione:
```
⚠️  LEGAL DISCLAIMER ⚠️

VulnScan is a DEFENSIVE SECURITY TOOL for authorized vulnerability assessment only.

AUTHORIZED USE ONLY:
• Use only on networks you OWN or have EXPLICIT WRITTEN PERMISSION to test
• Obtain proper authorization before any scanning activity
• Comply with all local laws and regulations
• Respect terms of service of tested systems

PROHIBITED USES:
• Unauthorized network scanning or penetration testing
• Any form of cyber attack or malicious activity
• Violation of computer fraud and abuse laws
• Testing systems without explicit permission

By using this tool, you acknowledge:
• You are responsible for ensuring legal and authorized use
• Unauthorized scanning may be ILLEGAL in your jurisdiction
• The developers assume NO LIABILITY for misuse of this software
• This tool is for EDUCATIONAL and LEGITIMATE SECURITY PURPOSES only

If you do not have explicit authorization, DO NOT USE this tool.
```

### Secure Coding
- No hardcoded credentials
- Secure credential storage (QSettings with encryption)
- Input validation ovunque
- SQL injection prevention (prepared statements)
- XSS prevention nei report HTML

## 🚀 **SVILUPPO A FASI**

**IMPORTANTE: Lo sviluppo di VulnScan deve essere suddiviso in fasi incrementali per garantire qualità, testing e feedback progressivo.**

### **FASE 1: Foundation & Core Architecture** ⭐ (PRIORITÀ MASSIMA)
**Obiettivo**: Struttura base funzionante e compilabile
- **Struttura directory completa** con tutti i moduli
- **File qmake (.pro)** per root e subprojects
- **common.pri** con configurazioni condivise
- **Core interfaces** (IScanner, IAIProvider, IRepository, ecc.)
- **Basic data models** (ScanResult, Vulnerability, ecc.)
- **Database schema** e DatabaseManager base
- **Logger** e sistema di configuration
- **Unit tests** per componenti base
- **Build system** funzionante (qmake + scripts)

**Deliverable**: Progetto che compila e ha architettura definita

### **FASE 2: Network Scanner Core** 🔍
**Obiettivo**: Funzionalità di scanning base
- **PortScanner** implementation (TCP/UDP) con **parallelizzazione**
- **ScanWorker** per thread-safe port scanning
- **ServiceDetector** per identificazione servizi
- **NetworkInterface** per discovery
- **DnsResolver** per hostname resolution
- **ScanEngine** orchestrator con thread pool management
- **Progress tracking** thread-safe
- **Cancellation support** per long-running scans
- **Basic CLI** per testing manuale
- **Integration tests** per scanner workflow
- **Thread safety tests** per concorrenza
- **Database storage** dei scan results

**Deliverable**: Scanner parallelo funzionante via CLI

### **FASE 3: Vulnerability Assessment** 🛡️
**Obiettivo**: Identificazione vulnerabilità
- **VulnerabilityDatabase** con CVE data (thread-safe)
- **VulnerabilityScanner** per analisi parallela
- **VulnerabilityWorker** per thread-safe vuln checks
- **VulnerabilityMatcher** per correlation
- **SslTlsChecker** per audit SSL/TLS
- **ServiceVulnChecker** per vulnerabilità note
- **Thread-safe database access** con mutex/connection pooling
- **Parallel service analysis** per performance
- **Basic reporting** (JSON/CSV export)
- **CVE database seeding** scripts

**Deliverable**: Scanner con parallel vulnerability detection

### **FASE 4: AI Integration** 🤖
**Obiettivo**: Analisi intelligente con AI
- **AI Provider interfaces** (Claude, OpenAI, Gemini)
- **GenericProvider** per custom endpoints (OpenRouter, Groq, Together AI, Ollama)
- **AIWorker** per chiamate API asincrone in thread separati
- **PromptBuilder** per context injection
- **AIAnalyzer** per vulnerability analysis con parallelizzazione
- **Concurrent AI calls** a provider multipli
- **Automatic fallback** tra provider (timeout/error handling)
- **AI Configuration** management con supporto custom headers
- **Provider presets** per servizi comuni
- **Rate limiting** thread-safe per API
- **Smart recommendations** engine
- **AI-powered risk scoring**
- **Connection testing** per custom providers
- **Timeout management** per slow providers

**Deliverable**: Scanner con parallel AI-enhanced analysis e supporto provider flessibile

### **FASE 5: GUI Application** 🖥️
**Obiettivo**: Interfaccia grafica utente
- **MainWindow** con navigation
- **ScanView** per configurazione scan
- **ResultsView** con tables/trees
- **SettingsDialog** per configuration
- **AIConfigWidget** con gestione provider
- **CustomProviderDialog** per aggiungere provider personalizzati
- **ReportDialog** per export
- **Progress tracking** widgets
- **Dark/Light themes**
- **Educational Dashboard** integrato
- **Welcome Wizard** per nuovi utenti
- **Interactive tooltips** e help system

**Deliverable**: GUI completa, user-friendly ed educativa

### **FASE 6: Advanced Features** 🚀
**Obiettivo**: Funzionalità avanzate
- **Multiple report formats** (HTML, PDF)
- **Advanced filtering** e search
- **Scan scheduling** e automation
- **Custom vulnerability rules**
- **Network topology visualization**
- **Historical scan comparison**

**Deliverable**: Tool professionale completo

### **FASE 7: DevOps & Distribution** 📦
**Obiettivo**: Deployment e distribution
- **GitHub Actions** per CI/CD
- **Cross-platform builds** (Linux/Windows/macOS)
- **Automated testing** pipeline
- **Packaging scripts** (AppImage, NSIS, DMG)
- **Release automation**
- **Translation pipeline** (lupdate/lrelease automation)
- **Doxygen documentation** generation in CI
- **Documentation website** con API docs

**Deliverable**: Progetto production-ready

### **FASE 8: Documentation & Community** 📚
**Obiettivo**: Documentazione complata e community
- **User guides** e tutorials (multilingua)
- **Developer documentation**
- **API reference** completo (generato da Doxygen)
- **Translation guides** per contributors
- **Video tutorials** e demos
- **Contributing guidelines**
- **Community management** tools
- **Educational content** library (multilingua)
- **Workshop materials** e curricula
- **Certification program** setup
- **Research collaboration** framework

**Deliverable**: Progetto open source maturo con forte focus educativo e internazionale

### **FASE 9: Educational Enhancement** 🎓 (NUOVA FASE DEDICATA)
**Obiettivo**: Massimizzare valore educativo e divulgativo
- **Demo Environment** automatizzato
- **AI Transparency** dashboard completo
- **Certification System** funzionante
- **LMS Integration** modules
- **Content Creation** tools
- **Outreach Metrics** tracking
- **Multi-language** support
- **Accessibility** compliance

**Deliverable**: Platform educativo completo per cybersecurity outreach

## 🎯 **Approccio per Ogni Fase:**

1. **Planning**: Definire scope e requirements specifici
2. **Development**: Implementazione incrementale con TDD
3. **Testing**: Unit/Integration tests per ogni componente
4. **Review**: Code review e quality assurance
5. **Documentation**: Documenta mentre sviluppi
6. **Demo**: Crea esempi e use cases
7. **Feedback**: Raccolta feedback e iterazioni

## 📋 **Deliverables per Fase:**

Ogni fase deve produrre:
- **Codice funzionante e testato**
- **File .pro** aggiornati se necessario
- **Tests** per nuove funzionalità
- **Documentation** aggiornata
- **Demo/Examples** per la fase
- **CHANGELOG** aggiornato
- **Tag Git** per milestone

## ⚡ **Vantaggi Sviluppo a Fasi:**

- **Feedback progressivo** su architettura
- **Testing continuo** di ogni componente
- **Rilasci incrementali** per early adopters
- **Riduzione rischi** di design errors
- **Motivazione** tramite milestone raggiunti
- **Possibilità di pivot** se necessario

## 💡 Note Finali

- Il codice deve essere **production-ready**
- Usa **qmake** come build system nativo Qt
- File **.pro** devono essere ottimizzati per Qt Creator
- Usa **SUBDIRS** template per struttura modulare
- **common.pri** per evitare duplicazione configurazioni
- **Commenti e documentazione Doxygen** in inglese
- **Stringhe UI** traducibili con Qt Linguist (en, it, fr, de)
- Documentation generale può essere in **italiano o inglese**
- Preferisci **smart pointers** a raw pointers
- Usa **const correctness**
- **Evita duplicazione** di codice
- Ogni file header deve avere **header guard** (`#pragma once`)
- Segui **Qt Best Practices** e **Qt Coding Conventions**
- Configura correttamente **DESTDIR**, **OBJECTS_DIR**, **MOC_DIR**, **UI_DIR**, **RCC_DIR**
- **Tutti i file .h e .cpp** devono includere commenti Doxygen completi
- **Genera documentazione** con Doxygen regolarmente durante sviluppo

## 🔒 **ETHICAL DEVELOPMENT GUIDELINES**

Durante lo sviluppo, assicurati che:
- **Nessuna funzionalità offensiva** sia implementata (exploit, brute force, DoS)
- Tutti i **disclaimer legali** siano presenti nell'interfaccia utente
- I **rate limiting** siano implementati per evitare scan aggressivi
- Le **best practices** di responsible disclosure siano documentate
- Il tool includa **warning** per l'uso autorizzato prima di ogni scan
- La documentazione enfatizzi l'**uso etico** e la **compliance legale**

Inizia generando la struttura e i file .pro, poi procedi con il resto. Per ogni file generato, assicurati che rispetti SRP, sia ben documentato, e contenga solo funzionalità difensive.

## 🎓 **SCOPO EDUCATIVO E DIVULGATIVO**

**VulnScan è progettato specificamente per supportare attività di divulgazione della cybersecurity e intelligenza artificiale.**

### **Target Educativo:**
- **Studenti universitari** in corsi di cybersecurity e informatica
- **Professionisti IT** che vogliono apprendere AI-enhanced security
- **Management** che deve comprendere ROI dell'AI nella sicurezza
- **General public** interessato alla cybersecurity awareness
- **Academic researchers** su AI applications in infosec

### **Caratteristiche Divulgative da Implementare:**

#### **1. Educational Dashboard**
```cpp
// Core educational features da implementare:
class EducationalDashboard {
public:
    // Step-by-step explanation of scan process
    void showScanSteps(const ScanProgress& progress);

    // Vulnerability explanations in plain language
    void explainVulnerability(const Vulnerability& vuln);

    // AI reasoning transparency
    void showAIAnalysisReasoning(const AIAnalysis& analysis);

    // Learning progress tracking
    void trackLearningProgress(const User& user);
};
```

#### **2. Interactive Learning Mode**
- **Guided tutorials** integrati nell'applicazione
- **Tooltips esplicativi** su ogni funzionalità
- **"Why is this dangerous?"** per ogni vulnerability
- **Remediation steps** con difficulty levels
- **Progress badges** per motivation

#### **3. AI Transparency Features**
```cpp
class AIExplainer {
public:
    // Mostra il ragionamento dell'AI step-by-step
    QString explainAnalysis(const AIAnalysis& analysis);

    // Confidence scores con spiegazioni
    void showConfidenceReasoning(float score);

    // Confronto tra diversi AI providers
    void compareAIResponses(const QList<AIResponse>& responses);

    // Prompt engineering examples
    void showPromptExamples(const VulnerabilityType& type);
};
```

#### **4. Demo Environment Builder**
```cpp
class DemoEnvironment {
public:
    // Crea test networks vulnerabili (container-based)
    void createVulnerableTestNetwork(const ScenarioType& scenario);

    // Safe sandbox per testing
    void setupSafeSandbox();

    // Pre-configured scenarios
    void loadEducationalScenarios();

    // Reset to clean state
    void resetDemoEnvironment();
};
```

### **Contenuti Divulgativi Integrati:**

#### **Welcome Wizard Educativo**
- **"What is a vulnerability scanner?"** intro
- **"How AI enhances cybersecurity"** explanation
- **"Your first authorized scan"** guided tutorial
- **"Understanding scan results"** walkthrough

#### **Knowledge Base Integrata**
- **CVE explanations** in plain language
- **Attack vectors** descriptions
- **Mitigation strategies** library
- **AI concepts** simplified
- **Legal considerations** guidance

#### **Certification Path**
```cpp
enum class CertificationLevel {
    Beginner,      // Basic network security concepts
    Intermediate,  // Vulnerability assessment skills
    Advanced,      // AI-powered security analysis
    Expert         // Custom rules and automation
};

class CertificationManager {
public:
    void trackProgress(const User& user, const CompletedTask& task);
    void generateCertificate(const User& user, CertificationLevel level);
    void provideNextSteps(const User& user);
};
```

### **Workshop/Conference Features:**

#### **Presentation Mode**
- **Live demo** capabilities con audience-friendly UI
- **Step-by-step** scan execution con pause points
- **AI analysis** real-time con explanations
- **Q&A integration** per audience engagement

#### **Educator Tools**
```cpp
class EducatorDashboard {
public:
    // Create custom learning scenarios
    void createScenario(const LearningObjective& objective);

    // Track student progress
    void trackStudentProgress(const QList<Student>& students);

    // Generate reports for assessment
    Report generateProgressReport(const Student& student);

    // Provide hints and guidance
    void provideHint(const Student& student, const Challenge& challenge);
};
```

### **Community & Collaboration Features:**

#### **Knowledge Sharing**
- **Community-contributed** vulnerability signatures
- **Shared scenarios** library
- **Best practices** wiki integration
- **Peer learning** forums integration

#### **Research Support**
```cpp
class ResearchTools {
public:
    // Export data for academic research
    void exportResearchData(const ScanResults& results, const Format& format);

    // Anonymize data for sharing
    void anonymizeResults(ScanResults& results);

    // Statistical analysis tools
    void generateStatistics(const QList<ScanResults>& historicalData);

    // Research paper citation generator
    void generateCitation(const CitationStyle& style);
};
```

### **Content Creation Support:**

#### **Automated Content Generation**
- **Blog post** templates basati sui scan results
- **Infographic** generation per vulnerabilities found
- **Video script** templates per demonstrations
- **Social media** content suggestions

#### **Documentation Templates**
- **Workshop** curricula templates
- **Lab exercises** based on real scenarios
- **Assessment** questionnaires generation
- **Instructor guides** with teaching notes

### **Metrics & Analytics per Divulgazione:**

```cpp
class OutreachMetrics {
public:
    // Learning engagement metrics
    void trackEngagementMetrics(const User& user);

    // Content effectiveness analysis
    void analyzeContentEffectiveness(const ContentType& content);

    // Community growth tracking
    void trackCommunityGrowth();

    // Educational impact measurement
    void measureEducationalImpact(const Program& program);
};
```

### **Integration con Piattaforme Educational:**

- **LMS integration** (Moodle, Canvas, Blackboard)
- **GitHub Classroom** compatibility
- **Jupyter Notebooks** per data analysis
- **Docker containers** per consistent environments
- **Cloud deployment** per remote learning

### **Accessibility & Inclusivity:**

- **Multi-language** support per global outreach (inglese, italiano, francese, tedesco)
- **Qt Linguist** integration per traduzioni
- **Dynamic language switching** senza restart
- **Right-to-Left (RTL)** support preparato per future lingue
- **Screen reader** compatibility
- **High contrast** themes
- **Keyboard navigation** completa
- **Mobile responsive** per diverse piattaforme

### **Internazionalizzazione Implementation:**

```cpp
/**
 * @class LanguageManager
 * @brief Manages application translations and language switching
 */
class LanguageManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Available languages in the application
     */
    enum class Language {
        English,  ///< English (default)
        Italian,  ///< Italiano
        French,   ///< Français
        German    ///< Deutsch
    };

    /**
     * @brief Loads and applies the specified language
     * @param lang Target language to switch to
     * @return true if language was loaded successfully
     */
    bool loadLanguage(Language lang);

    /**
     * @brief Gets the current application language
     * @return Current active language
     */
    Language currentLanguage() const;

    /**
     * @brief Gets list of available language codes
     * @return List of ISO 639-1 language codes (en, it, fr, de)
     */
    QStringList availableLanguages() const;

signals:
    /**
     * @brief Emitted when language has been changed
     * @param newLang The newly active language
     */
    void languageChanged(Language newLang);

private:
    QTranslator* m_translator; ///< Qt translator instance
    Language m_currentLanguage; ///< Currently active language
};
```

**Requisiti Internazionalizzazione:**
- Usa `tr()` per tutte le stringhe visibili all'utente
- Usa `QT_TR_NOOP()` per stringhe statiche
- Fornisci contesto con `tr("text", "context")`
- Usa `%n` per plurali corretti
- Testa con `lupdate` e `lrelease`
- Documenta stringhe che richiedono contesto specifico