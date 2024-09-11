# 3hrs challenge | Minimal Database Engine in Netwide Assembly (NASM)

## Project Overview

The goal is to build a minimal database engine in NASM (Netwide Assembly) within **3 hours**. This database engine will support basic CRUD (Create, Read, Update, Delete) operations and allow interaction via a command-line interface (CLI).

### Features
- **Create**:   Add new records with a key-value pair.
- **Read**:     Retrieve a record by its key.
- **Update**:   Modify the value of an existing record.
- **Delete**:   Remove a record by its key.
- **Search**:   Efficiently retrieve records using a simple search algorithm.

### Query Examples
- CREATE key value
- READ key
- UPDATE key new_value
- DELETE key

### Constraints
- The database will operate entirely in memory.
- Use a simple key-value data structure to store records.
- Implement basic error handling (e.g., for missing keys).
- All operations will be performed via a command-line interface (CLI).

---

## Results

I was able to create a minimal database engine in 3 hours, respecting all the features.
- **Create**:   Add new records with a key-value pair.
- **Read**:     Retrieve a record by its key.
- **Update**:   Modify the value of an existing record.
- **Delete**:   Remove a record by its key.
- **Search**:   Efficiently retrieve records using a simple search algorithm.

Here are the features that could have been achieved in more time:
- writing error messages **(DONE after 3hrs)**
- clean up the read/update/delete functions as they share similar processes
- check duplicate keys
- manage edge cases in the query structure **(DONE after 3hrs)**
