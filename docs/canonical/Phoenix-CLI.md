# Phoenix CLI Shape — Canonical Specification v0.1

## 1. CLI Identity
The Phoenix CLI is the human interface to the structural Phoenix engine.

It does not interpret code.
It does not execute instructions.
It exposes structural operations.

---

## 2. CLI Principles

The CLI must be:

- deterministic
- structural
- minimal
- declarative
- ABI-compliant

The CLI is a thin wrapper over Core, VM, Registry, and Tape.

---

## 3. CLI Commands

### 3.1 phoenix load <file>
Loads a Tape from a file.

### 3.2 phoenix plugins
Lists all registered plugins.

### 3.3 phoenix apply <plugin> <k>
Applies a plugin transform at position k.

Equivalent to:

1. select(Tape, k)
2. apply(P, W_k)
3. merge(Tape, k, Φ_k)

### 3.4 phoenix dump
Outputs the current Tape.

### 3.5 phoenix info
Displays:

- Tape length
- Registered plugins
- VM status
- Core status

---

## 4. CLI Determinism

CLI commands must:

- produce identical output for identical input
- have no side effects outside Tape
- be purely structural

---

## 5. CLI Execution Example (Structural Form)

phoenix load tape.bin
phoenix plugins
phoenix apply xor_shift 16
phoenix dump


This corresponds to:


---

## 6. Next Clock

Phoenix Canonical Math Documents will be defined in Clock 7.

