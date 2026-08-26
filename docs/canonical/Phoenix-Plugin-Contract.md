# Phoenix Plugin Contract — Canonical Specification v1.0  
**Phoenix Rebirth — Clock 2**  
**Date:** 2026‑08‑25  
**Status:** Canonical, Immutable

---

## 1. Plugin Identity
A Phoenix plugin is a unary structural transform packaged as an ABI‑compliant unit.

Each plugin must define:

- `name` — unique identifier  
- `version` — semantic version (major.minor.patch)  
- `window_size` — number of bits consumed per transform  
- `transform` — unary function \( P(W) \)  
- `purity` — declaration of structural purity  
- `phoenix_compliance` — boolean flag  

---

## 2. Plugin Metadata Schema

### 2.1 Required Fields

| Field               | Type      | Description                                      |
|---------------------|-----------|--------------------------------------------------|
| name                | string    | Unique plugin identifier                         |
| version             | string    | Semantic version (e.g., "1.0.0")                 |
| window_size         | integer   | Size of window \( W_k \)                         |
| transform           | function  | Unary transform \( P(W_k) \)                     |
| purity              | enum      | { PURE, STRUCTURAL }                             |
| phoenix_compliance  | boolean   | Must be true                                     |

---

## 3. Transform Declaration

A Phoenix transform must satisfy:

### 3.1 Unary Rule


\[
P(W_k) \rightarrow \Phi_k
\]



### 3.2 Determinism Rule
For identical \( W_k \), \( P \) must produce identical \( \Phi_k \).

### 3.3 No Side Effects  
Transform must not modify:

- global state  
- external state  
- other windows  
- other tapes  

### 3.4 No Semantic Dependencies  
Transform must not rely on:

- lexical context  
- execution order  
- external metadata  

---

## 4. Window Size Rule

A plugin must declare a fixed window size:



\[
W_k = \text{Tape}[k : k + \text{window\_size}]
\]



Window size must be:

- positive  
- constant  
- ABI‑compliant  

---

## 5. Purity Declaration

Plugins must declare one of:

### PURE
Transform is mathematically pure:

- no randomness  
- no external input  
- no state  

### STRUCTURAL
Transform is structurally pure:

- depends only on \( W_k \)  
- no external side effects  

---

## 6. Phoenix Compliance Flag

A plugin must set:

```
phoenix_compliance = true
```

This asserts:

- ABI compliance  
- purity compliance  
- window compliance  
- transform compliance  

---

## 7. Plugin Example (Structural Form)

```
name: "xor_shift"
version: "1.0.0"
window_size: 8
transform: Φ = XOR(W)
purity: PURE
phoenix_compliance: true
```

---

## Canonical Footer
This document is part of the **Phoenix Rebirth Canonical Sequence (Clocks 1–11)**.  
It is immutable and historically preserved.

---

## Navigation
**Previous → Clock 1: Phoenix ABI**  
**Next → Clock 3: Phoenix Registry Architecture**  
**Index → Phoenix Rebirth Canonical Sequence (docs/canonical/INDEX.md)**


