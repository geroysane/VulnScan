-- VulnScan Database Schema
-- SQLite 3 Database Schema for vulnerability scanning application
-- Version: 1.0

-- ============================================================================
-- Scans Table
-- ============================================================================
CREATE TABLE IF NOT EXISTS scans (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    target_host TEXT NOT NULL,
    scan_start_time DATETIME NOT NULL,
    scan_end_time DATETIME,
    status TEXT NOT NULL,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP
);

-- ============================================================================
-- Ports Table
-- ============================================================================
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

-- ============================================================================
-- Vulnerabilities Table
-- ============================================================================
CREATE TABLE IF NOT EXISTS vulnerabilities (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    cve_id TEXT UNIQUE NOT NULL,
    description TEXT,
    severity TEXT NOT NULL,
    cvss_score REAL,
    published_date DATE,
    modified_date DATE
);

-- ============================================================================
-- Scan Vulnerabilities Junction Table
-- ============================================================================
CREATE TABLE IF NOT EXISTS scan_vulnerabilities (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    scan_id INTEGER NOT NULL,
    vulnerability_id INTEGER NOT NULL,
    affected_service TEXT,
    detected_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (scan_id) REFERENCES scans(id) ON DELETE CASCADE,
    FOREIGN KEY (vulnerability_id) REFERENCES vulnerabilities(id) ON DELETE CASCADE
);

-- ============================================================================
-- Indexes for Performance
-- ============================================================================
CREATE INDEX IF NOT EXISTS idx_scans_target ON scans(target_host);
CREATE INDEX IF NOT EXISTS idx_scans_status ON scans(status);
CREATE INDEX IF NOT EXISTS idx_ports_scan ON ports(scan_id);
CREATE INDEX IF NOT EXISTS idx_ports_state ON ports(state);
CREATE INDEX IF NOT EXISTS idx_vulns_cve ON vulnerabilities(cve_id);
CREATE INDEX IF NOT EXISTS idx_vulns_severity ON vulnerabilities(severity);
CREATE INDEX IF NOT EXISTS idx_scan_vulns_scan ON scan_vulnerabilities(scan_id);
CREATE INDEX IF NOT EXISTS idx_scan_vulns_vuln ON scan_vulnerabilities(vulnerability_id);
