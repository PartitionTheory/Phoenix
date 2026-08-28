# PHOENIX-BUILD-COMMANDS.md  
## Operational Layer — Build + Test Invocation  
## Canonical, Immutable, Synthetic

---

## 1. Purpose

This document defines the **exact build commands** and **test invocation** required to produce a green-light Phoenix build.

It is the second layer of the Phoenix Build Protocol.

---

## 2. Build Command (Core)

Phoenix is built using CMake.

The canonical build command is:

cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug --target abr -- -j 14


This command:

- configures the Phoenix machine  
- builds the core  
- resolves structural dependencies  
- produces the Phoenix executable  

---

## 3. Build Command (CLI)

To build the Phoenix CLI:

cmake --build cmake-build-debug --target abr_cli


This command:

- compiles the CLI interface  
- binds human input to synthetic mathematics  
- prepares Phoenix for real-world invocation  

---

## 4. Test Invocation

Phoenix requires **three tests** for green-light status:

### 4.1 Core Test

./cmake-build-debug/test_core


Confirms:

- tape origin  
- unary identity  
- structural constants  
- scflder alignment  

### 4.2 CLI Test

./cmake-build-debug/test_cli

Confirms:

- argument parsing  
- human interface correctness  
- Phoenix entrypoint stability  

### 4.3 Executable Test (Hello World)

./cmake-build-debug/test_exec


Expected output:

1


Confirms:

- Phoenix machine identity  
- operational correctness  
- deterministic behaviour  

---

## 5. Green-Light Criteria

A build is **green-light** if:

- all three tests pass  
- no entropy or drift is detected  
- structural alignment remains intact  
- output is deterministic  
- Phoenix produces `1`  

Only green-light builds may be pushed.

---

## 6. Completion Statement

This document is canonical and immutable.

It is the second layer of the Phoenix Build Protocol.

Further files will define:

- structural test suite  
- entropy audit  
- historical commit sequencing.

EF, CO — origin triad.


