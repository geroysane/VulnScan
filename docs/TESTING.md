# Testing Guide

## Overview

VulnScan has comprehensive test coverage with 231+ unit tests and 32 integration tests covering all core functionality.

## Test Structure

Tests are organized using Qt Test framework (QTest) with the following categories:

- **Database Tests**: TestDatabaseManager, TestScanRepository, TestVulnerabilityDatabase
- **Configuration Tests**: TestConfiguration
- **Network Scanner Tests**: TestPortScanner, TestServiceDetector, TestDnsResolver, TestNetworkInterface, TestScanEngine
- **Vulnerability Assessment Tests**: TestVulnerabilityMatcher, TestVulnerabilityScanner, TestSslTlsChecker, TestServiceVulnChecker
- **Report Generation Tests**: TestReportGenerator
- **Integration Tests**: IntegrationTests (24 tests), IntegrationTestsVuln (8 tests)

## Running Tests

### Build Tests

```bash
cd build
cmake --build . --target vulnscan_tests -j12
```

### Important Note About Test Execution

**Known Issue**: Running all tests together may hang or timeout due to async operations (QTimer, QThread Pool, network operations) waiting for events when QCoreApplication is present but event loop is not running.

**Recommended Approach**: Use CTest to run individual test classes or run tests in smaller groups.

### Using CTest (Recommended)

CTest is the recommended way to run tests as it handles timeouts and individual test execution:

```bash
cd build
ctest --output-on-failure
```

Run with timeout:
```bash
ctest --timeout 60 --output-on-failure
```

Run specific test:
```bash
ctest -R VulnScanTests --verbose
```

### Running Individual Test Classes

To avoid hanging, you can run individual test classes by creating small test executables or using Qt Test's filtering.

### Integration Tests

Integration tests are disabled by default. Enable them with `--integration` flag:

```bash
./build/tests/vulnscan_tests.exe --integration
```

**Note**: Integration tests may timeout as they perform actual network operations.

## Test Coverage

### FASE 1 - Foundation (COMPLETE)
- ✅ TestDatabaseManager (12+ tests)
- ✅ TestConfiguration (8+ tests)

### FASE 2 - Network Scanner (COMPLETE)
- ✅ TestPortScanner (24 tests)
- ✅ TestServiceDetector (18 tests)
- ✅ TestDnsResolver (20 tests)
- ✅ TestNetworkInterface (19 tests)
- ✅ TestScanEngine (22 tests)
- ✅ TestScanRepository (20 tests)
- ✅ IntegrationTests (24 integration tests, optional)

### FASE 3 - Vulnerability Assessment (COMPLETE)
- ✅ TestVulnerabilityDatabase (26 tests)
- ✅ TestVulnerabilityMatcher (26 tests)
- ✅ TestVulnerabilityScanner (13 tests)
- ✅ TestSslTlsChecker (27 tests)
- ✅ TestServiceVulnChecker (22 tests)
- ✅ TestReportGenerator (16 tests)
- ✅ IntegrationTestsVuln (8 integration tests)

**Total**: 231+ unit tests + 32 integration tests

## Known Test Limitations

1. **Network Tests May Timeout**: Tests that require actual network connections (DNS resolution, port scanning remote hosts, SSL/TLS handshakes) may timeout or fail if network is unavailable.

2. **Async Operation Tests**: Tests involving QTimer, QThreadPool, or QEventLoop may hang when running all tests together. This is due to Qt's event system waiting for events that don't get processed.

3. **Database Tests**: Some tests use in-memory SQLite databases (":memory:") for speed and isolation.

## Workarounds

### For Development
- Run tests individually during development
- Use CTest with timeout for CI/CD
- Focus on specific test classes relevant to your changes

### For CI/CD
```bash
# Run with timeout
cd build
ctest --timeout 120 --output-on-failure

# Or run in parallel with shorter timeout per test
ctest -j4 --timeout 60 --output-on-failure
```

## Future Improvements

- [ ] Split tests into separate executables by category
- [ ] Implement test fixtures that properly clean up async operations
- [ ] Add mock objects for network operations to avoid actual network dependencies
- [ ] Configure individual test timeouts in CMakeLists.txt

## Test Best Practices

When writing new tests:

1. **Clean Up Resources**: Ensure QTimer, QThread, and network objects are properly deleted
2. **Use Scoped Blocks**: Wrap test objects in `{}` to ensure proper destruction
3. **Avoid Event Loop**: Don't use `QEventLoop` or `app.exec()` in tests
4. **Mock Network**: Use mock objects instead of real network operations where possible
5. **Test in Isolation**: Verify your test works both individually and with other tests

## Debugging Test Failures

If a specific test hangs:

1. Identify the test class causing the hang
2. Check for QTimer or QThread objects that aren't deleted
3. Verify all signal/slot connections are properly disconnected
4. Ensure async operations have proper cleanup in destructors
5. Run the test class in isolation to verify it's not an interaction issue

## Example: Creating a Minimal Test

```cpp
#include <QtTest>

class MyTest : public QObject {
    Q_OBJECT

private slots:
    void testSomething() {
        QVERIFY(true);
    }
};

int main(int argc, char *argv[]) {
    MyTest test;
    return QTest::qExec(&test, argc, argv);
}

#include "MyTest.moc"
```

Note: This creates a minimal test without QCoreApplication which avoids event loop issues.
