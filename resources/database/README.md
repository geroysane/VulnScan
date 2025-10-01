# Database Schema

## schema.sql

This file contains the canonical database schema for VulnScan.

**IMPORTANT**: The schema is currently embedded in `src/core/database/DatabaseManager.cpp` for reliability and portability. If you modify the database structure:

1. Update `schema.sql` first (this is the source of truth)
2. Update the embedded schema in `DatabaseManager::executeSchema()`
3. Keep both versions synchronized

## Why keep both versions?

- **schema.sql**: Human-readable documentation and external tool compatibility
- **Embedded in code**: Reliability, no file loading issues, always available

## Future improvements

- Implement Qt resource system (`.qrc`) to embed schema.sql
- Add database migration system for version upgrades
- Generate C++ code from schema.sql automatically
