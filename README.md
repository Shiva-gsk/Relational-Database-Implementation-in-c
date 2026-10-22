# NITCbase — System Overview

NITCbase is an educational Relational Database Management System (RDBMS) implemented in C++. It demonstrates how database queries are processed, records are managed, and data is stored and retrieved through a layered architecture.

## System Architecture

NITCbase consists of eight layers, each responsible for a specific part of database processing.

| Layer | Responsibility |
|---|---|
| Frontend Interface | Accepts user commands and displays query results. |
| Schema Layer | Manages table definitions and schema modifications. |
| Algebra Layer | Executes relational operations such as selection, projection, and joins. |
| Cache Layer | Maintains relation and attribute metadata in memory. |
| Block Access Layer | Handles record-level operations on database blocks. |
| B+ Tree Layer | Supports indexed searching and index management. |
| Buffer Layer | Manages in-memory disk blocks to reduce disk access. |
| Physical Layer | Performs low-level block reads and writes. |

## How It Works

1. **Query Input:** The user submits a database command through the frontend.
2. **Query Processing:** The command is routed to the appropriate schema or relational algebra operations.
3. **Record Access:** The Block Access Layer locates and manipulates records using the underlying storage interfaces.
4. **Indexing:** The B+ Tree Layer supports efficient searches when an applicable index is available.
5. **Buffer Management:** The Buffer Layer caches disk blocks in memory and manages their replacement.
6. **Disk Operations:** The Physical Layer reads and writes blocks on the simulated XFS disk.
7. **Result Delivery:** The processed results are returned to the frontend for display.

## Core Functionality

- **Schema Management:** Create, delete, and modify database tables.
- **Relational Operations:** Insert, select, project, and join records.
- **Index Management:** Create and remove indexes using B+ tree structures.
- **Metadata Caching:** Maintain database schema information in memory.
- **Storage Management:** Access disk blocks through a buffer management layer.
- **Query Interface:** Execute supported database commands and display results.

## Implementation Scope

NITCbase provides the underlying disk interface and frontend user interface, while several database layers and programming interfaces require implementation according to the project specifications.

The project demonstrates the interaction between query processing, relational operations, indexing, buffer management, and physical storage in a modular database system.

**Documentation:** https://nitcbase.github.io/
