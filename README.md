# VulnScan - Network Vulnerability Scanner

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C++-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Qt](https://img.shields.io/badge/Qt-6.9-green.svg)](https://www.qt.io/)
[![Build System](https://img.shields.io/badge/Build-CMake%20%7C%20qmake-orange.svg)](https://cmake.org/)

**VulnScan** is a professional, open-source network vulnerability scanner designed for defensive security, security auditing, and educational purposes. It provides both GUI and CLI interfaces for comprehensive network security assessments.

## 🛡️ Defensive Security Tool

**VulnScan is a DEFENSIVE SECURITY tool for IT professionals, security researchers, and system administrators.**

### ✅ Legitimate Use Cases:
- **Vulnerability Assessment** of owned or authorized systems
- **Security Auditing** for compliance and hardening
- **Network Discovery** for inventory and documentation
- **Configuration Analysis** to identify misconfigurations
- **Educational purposes** for cybersecurity learning
- **Authorized Penetration Testing** from clients/employers
- **Cybersecurity Outreach** and scientific dissemination
- **Academic Research** on AI applications in security

### ❌ What VulnScan Does NOT Do:
- **No exploit execution** or active vulnerability exploitation
- **No payload delivery** or code injection
- **No credential harvesting** or password cracking
- **No DoS attacks** or service disruption
- **No unauthorized access** or privilege escalation
- **No data exfiltration** or lateral movement

### ⚖️ User Responsibility:
**Users are FULLY responsible for legal and authorized use of this tool.**
- Obtain **written authorization** before any scan
- Comply with **local laws** and **terms of service**
- Use only on **owned networks** or **explicitly authorized** systems
- **DO NOT use** for illegal or unauthorized activities

## ✨ Features

### Current (Phase 1 - Completed ✅)
- ✅ Modular architecture with clean interfaces
- ✅ SQLite database for scan results persistence
- ✅ Configuration system with JSON support
- ✅ Multi-level logging system
- ✅ Unit testing framework
- ✅ Cross-platform support (Windows, Linux, macOS)
- ✅ Both GUI (Qt) and CLI interfaces

### Planned (Phase 2-9)
- 🚧 Network scanner with port detection
- 🚧 Service fingerprinting and banner grabbing
- 🚧 Vulnerability database integration
- 🚧 AI-powered analysis and recommendations
- 🚧 Comprehensive reporting (PDF, HTML, JSON)
- 🚧 Real-time scan monitoring
- 🚧 Multi-threaded scanning for performance
- 🚧 Educational mode with learning resources

## 🚀 Quick Start

### Prerequisites

- **Qt 6.9+** (with Qt Widgets)
- **CMake 3.16+** or **qmake**
- **C++17 compatible compiler**
  - GCC 7+ / Clang 5+ / MSVC 2017+
- **SQLite 3**

### Building with CMake (Recommended)

```bash
# Clone the repository
git clone https://github.com/paolosereno/VulnScan.git
cd VulnScan

# Create build directory
mkdir build && cd build

# Configure
cmake -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/6.9.1/mingw_64" ..

# Build
cmake --build . -j4

# Run tests
ctest --output-on-failure
```

### Building with qmake

```bash
# Clone the repository
git clone https://github.com/paolosereno/VulnScan.git
cd VulnScan

# Generate Makefile
qmake vulnscan.pro

# Build
make
```

### Running the Application

```bash
# GUI Application
./build/src/gui/vulnscan_gui

# CLI Application
./build/src/cli/vulnscan_cli --help

# Run Unit Tests
./build/tests/vulnscan_tests
```

## 📂 Project Structure

```
VulnScan/
├── src/
│   ├── core/          # Core library (scanner, AI, database, config)
│   ├── gui/           # Qt GUI application
│   └── cli/           # Command-line interface
├── tests/             # Unit tests
├── docs/              # Documentation
│   ├── fasi/          # Development phases documentation
│   └── requirements.md
├── resources/         # Resources (icons, database schema, etc.)
├── CMakeLists.txt     # CMake build configuration
└── vulnscan.pro       # qmake build configuration
```

## 🧪 Testing

The project includes comprehensive unit tests:

```bash
# Using CMake
cd build
ctest --output-on-failure

# Or run directly
./build/tests/vulnscan_tests
```

**Current Test Coverage:**
- ✅ DatabaseManager tests (100% passing)
- ✅ Configuration system tests (100% passing)

## 📖 Documentation

- **[Development Phases](docs/fasi/README.md)** - Overview of all development phases
- **[Technical Requirements](docs/requirements.md)** - Detailed technical specifications
- **[Project State](project-state.md)** - Current development status
- **[Contributing Guide](CONTRIBUTING.md)** - How to contribute to the project

## 🗺️ Development Roadmap

The project is divided into 9 incremental phases:

1. ✅ **Phase 1**: Foundation & Core Architecture (COMPLETED)
2. 🚧 **Phase 2**: Network Scanner Core
3. 🔜 **Phase 3**: Vulnerability Assessment
4. 🔜 **Phase 4**: AI Integration
5. 🔜 **Phase 5**: GUI Application
6. 🔜 **Phase 6**: Advanced Features
7. 🔜 **Phase 7**: DevOps & Distribution
8. 🔜 **Phase 8**: Documentation & Community
9. 🔜 **Phase 9**: Educational Enhancement

See [docs/fasi/README.md](docs/fasi/README.md) for detailed phase descriptions.

## 🤝 Contributing

Contributions are welcome! Please read our [Contributing Guide](CONTRIBUTING.md) for details on:
- Code of conduct
- Development workflow
- Coding standards
- How to submit pull requests

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## ⚠️ Legal Disclaimer

**IMPORTANT**: This tool is provided for legal and authorized security testing only. Users must:

- Have explicit written authorization before scanning any network or system
- Comply with all applicable local, state, and federal laws
- Not use this tool for unauthorized access or malicious activities
- Accept full responsibility for their actions

The developers assume NO liability for misuse of this tool. Unauthorized network scanning may be illegal in your jurisdiction.

## 👨‍💻 Authors

- **Paolo Sereno** - [paolosereno](https://github.com/paolosereno)

## 🙏 Acknowledgments

- Built with [Qt Framework](https://www.qt.io/)
- Developed with [Claude Code](https://claude.com/claude-code)
- Inspired by professional security tools like Nmap, OpenVAS, and Nessus

## 📧 Contact

For questions, suggestions, or security concerns, please open an issue on GitHub.

---

**Made with ❤️ for the cybersecurity community**
