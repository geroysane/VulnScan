# VulnScan - Network Vulnerability Scanner

> **Defensive Security Tool** per vulnerability assessment autorizzato

## 📖 Documentazione Principale

Il progetto VulnScan è stato riorganizzato per facilitare lo sviluppo incrementale. La documentazione è stata suddivisa in file modulari:

### 🚀 Inizia da Qui

**[prompt-main.md](./prompt-main.md)** - Overview completo del progetto con:
- Scopo e obiettivi del tool
- Requisiti generali
- Tecnologie utilizzate
- Disclaimer legale
- Note finali

### 📋 Requisiti Tecnici

**[docs/requirements.md](./docs/requirements.md)** - Specifiche tecniche dettagliate:
- Coding standards C++17
- Design patterns obbligatori
- Qt best practices
- Multi-threading requirements
- Error handling patterns
- Doxygen documentation standards

### 🏗️ Sviluppo a Fasi

La documentazione è organizzata in 9 fasi incrementali. Inizia dalla FASE 1:

| Fase | Documento | Descrizione |
|------|-----------|-------------|
| ⭐ **FASE 1** | [fase1-foundation.md](./docs/fasi/fase1-foundation.md) | **Foundation & Core Architecture** - INIZIA DA QUI |
| 🔍 FASE 2 | [fase2-network-scanner.md](./docs/fasi/fase2-network-scanner.md) | Network Scanner Core |
| 🛡️ FASE 3 | [fase3-vulnerability.md](./docs/fasi/fase3-vulnerability.md) | Vulnerability Assessment |
| 🤖 FASE 4 | [fase4-ai-integration.md](./docs/fasi/fase4-ai-integration.md) | AI Integration |
| 🖥️ FASE 5 | [fase5-gui.md](./docs/fasi/fase5-gui.md) | GUI Application |
| 🚀 FASE 6 | [fase6-advanced.md](./docs/fasi/fase6-advanced.md) | Advanced Features |
| 📦 FASE 7 | [fase7-devops.md](./docs/fasi/fase7-devops.md) | DevOps & Distribution |
| 📚 FASE 8 | [fase8-documentation.md](./docs/fasi/fase8-documentation.md) | Documentation & Community |
| 🎓 FASE 9 | [fase9-educational.md](./docs/fasi/fase9-educational.md) | Educational Enhancement |

Per una panoramica completa delle fasi: **[docs/fasi/README.md](./docs/fasi/README.md)**

## 🎯 Primo Passo

**Inizia dalla FASE 1** - Foundation & Core Architecture:

```bash
# Leggi la FASE 1 per iniziare
cat docs/fasi/fase1-foundation.md
```

La FASE 1 include:
- ✅ Struttura directory completa
- ✅ File qmake (.pro) per build system
- ✅ Core interfaces (IScanner, IAIProvider, IRepository, ecc.)
- ✅ Basic data models (ScanResult, Vulnerability, ecc.)
- ✅ Database schema (SQLite)
- ✅ Configuration e logging system
- ✅ Unit tests iniziali

**Deliverable**: Progetto che compila con architettura definita

## 📂 Struttura Documentazione

```
vulnscan/
├── prompt.md                    ← SEI QUI
├── prompt-main.md              ← Overview generale
├── prompt-original-backup.md   ← Backup file originale
│
└── docs/
    ├── requirements.md          ← Requisiti tecnici dettagliati
    │
    └── fasi/
        ├── README.md            ← Overview fasi di sviluppo
        ├── fase1-foundation.md  ← ⭐ INIZIA QUI
        ├── fase2-network-scanner.md
        ├── fase3-vulnerability.md
        ├── fase4-ai-integration.md
        ├── fase5-gui.md
        ├── fase6-advanced.md
        ├── fase7-devops.md
        ├── fase8-documentation.md
        └── fase9-educational.md
```

## 🔒 IMPORTANTE: Uso Legale

⚠️ **VulnScan è un tool di SICUREZZA DIFENSIVA per uso AUTORIZZATO.**

- ✅ Usa SOLO su reti di tua proprietà o con autorizzazione scritta
- ✅ Rispetta leggi locali e termini di servizio
- ❌ NON usare per attività illegali o non autorizzate

Leggi il disclaimer completo in [prompt-main.md](./prompt-main.md#security--legal)

## 💡 Vantaggi della Riorganizzazione

- ✅ **Più leggibile**: File più piccoli e focalizzati
- ✅ **Manutenibile**: Più facile aggiornare singole sezioni
- ✅ **Incrementale**: Sviluppo fase per fase
- ✅ **Navigabile**: Link diretti tra documenti
- ✅ **Versionabile**: Git-friendly con modifiche isolate

## 🚀 Quick Links

- [Panoramica Progetto](./prompt-main.md)
- [Requisiti Tecnici](./docs/requirements.md)
- [⭐ FASE 1 - Inizia Qui](./docs/fasi/fase1-foundation.md)
- [Tutte le Fasi](./docs/fasi/README.md)
- [File Originale (Backup)](./prompt-original-backup.md)

---

**Inizia lo sviluppo dalla FASE 1 e procedi in modo incrementale!**