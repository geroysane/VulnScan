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

## 📂 Documentazione Dettagliata

### Requisiti Tecnici
Per i requisiti tecnici dettagliati, consulta:
- **[requirements.md](./docs/requirements.md)** - Requisiti tecnici completi, coding standards, design patterns

### Sviluppo a Fasi
Il progetto è suddiviso in 9 fasi incrementali. Vedi:
- **[docs/fasi/README.md](./docs/fasi/README.md)** - Overview di tutte le fasi
- **[docs/fasi/fase1-foundation.md](./docs/fasi/fase1-foundation.md)** - ⭐ **INIZIA DA QUI** - Foundation & Core Architecture
- **[docs/fasi/fase2-network-scanner.md](./docs/fasi/fase2-network-scanner.md)** - Network Scanner Core
- **[docs/fasi/fase3-vulnerability.md](./docs/fasi/fase3-vulnerability.md)** - Vulnerability Assessment
- **[docs/fasi/fase4-ai-integration.md](./docs/fasi/fase4-ai-integration.md)** - AI Integration
- **[docs/fasi/fase5-gui.md](./docs/fasi/fase5-gui.md)** - GUI Application
- **[docs/fasi/fase6-advanced.md](./docs/fasi/fase6-advanced.md)** - Advanced Features
- **[docs/fasi/fase7-devops.md](./docs/fasi/fase7-devops.md)** - DevOps & Distribution
- **[docs/fasi/fase8-documentation.md](./docs/fasi/fase8-documentation.md)** - Documentation & Community
- **[docs/fasi/fase9-educational.md](./docs/fasi/fase9-educational.md)** - Educational Enhancement

## 🚀 Come Iniziare

### Primo Passo: FASE 1
Il primo passo è **FASE 1: Foundation & Core Architecture** che include:
- Creazione struttura directory completa
- File qmake (.pro) per root e subprojects
- Core interfaces (IScanner, IAIProvider, IRepository, ecc.)
- Basic data models (ScanResult, Vulnerability, ecc.)
- Database schema e DatabaseManager base
- Logger e sistema di configuration
- Unit tests per componenti base
- Build system funzionante (qmake + scripts)

**Deliverable FASE 1**: Progetto che compila e ha architettura definita

Leggi [docs/fasi/fase1-foundation.md](./docs/fasi/fase1-foundation.md) per i dettagli completi.

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

---

**Inizia dalla FASE 1 e procedi in modo incrementale!**