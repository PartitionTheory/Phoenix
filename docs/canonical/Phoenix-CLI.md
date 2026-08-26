# Phoenix CLI Shape — Canonical Specification v1.0  
**Phoenix Rebirth — Clock 6**  
**Date:** 2026‑08‑26  
**Status:** Canonical, Immutable

---

## 1. CLI Identity
The Phoenix CLI is the human interface to the structural Phoenix engine.

It does **not** interpret code.  
It does **not** execute instructions.  
It exposes **structural operations** only.

The CLI is a thin wrapper over Core, VM, Registry, and Tape.

---

## 2. CLI Principles

The CLI must be:

- deterministic  
- structural  
- minimal  
- declarative  
- ABI‑compliant  

These principles ensure that the CLI reflects the purity of the Phoenix engine.

---

## 3. CLI Commands

### 3.1 `phoenix load <file>`
Loads a Tape from a file.

### 3.2 `phoenix plugins`
Lists all registered plugins.

### 3.3 `phoenix apply <plugin> <k>`
Applies a plugin transform at position **k**.

Equivalent to:

1. \( W_k = \text{select}(T, k) \)  
2. \( \Phi_k = \text{apply}(P, W_k) \)  
3. \( T' = \text{merge}(T, k, \Phi_k) \)

### 3.4 `phoenix dump`
Outputs the current Tape.

### 3.5 `phoenix info`
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

The CLI is a **deterministic structural interface**, not a semantic shell.

---

## 5. CLI Execution Example (Structural Form)

```
phoenix load tape.bin
phoenix plugins
phoenix apply xor_shift 16
phoenix dump
```

This corresponds to:

```
W_16 = select(Tape, 16)
Φ_16 = apply(xor_shift, W_16)
Tape' = merge(Tape, 16, Φ_16)
Tape = Tape'
```

---

## Canonical Footer
This document is part of the **Phoenix Rebirth Canonical Sequence (Clocks 1–11)**.  
It is immutable and historically preserved.

---

## Navigation
**Previous → Clock 5: Phoenix Core Structure**  
**Next → Clock 7: Phoenix Canonical Math Documents**  
**Index → Phoenix Rebirth Canonical Sequence (docs/canonical/INDEX.md)**


