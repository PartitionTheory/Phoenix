# Phoenix Structural Machine Language — Canonical Specification v1.0  
**Phoenix Rebirth — Clock 8**  
**Date:** 2026‑08‑26  
**Status:** Canonical, Immutable

---

## 1. SML Identity
Phoenix SML is a symbolic language describing structural operations on Tape.  
It is **not** an instruction set, programming language, or semantic language.  
SML expresses Phoenix operations in **minimal structural form**.

---

## 2. SML Symbols

T      — Tape  
W_k    — Window at position k  
P      — Plugin transform  
Φ_k    — Result of applying \( P(W_k) \)  
merge  — Reintegration operator  
k      — Position index  

These symbols define the complete vocabulary of SML.

---

## 3. SML Operations

### Select
```
SEL k
```
Selects window \( W_k \) from Tape \( T \).

### Apply
```
APPLY P
```
Applies plugin transform \( P \) to the current window \( W_k \).

### Merge
```
MERGE k
```
Reintegrates \( \Phi_k \) back into Tape \( T \) at position \( k \).

---

## 4. SML Execution Cycle

```
SEL k
APPLY P
MERGE k
```

Equivalent to:

1. \( W_k = \text{select}(T, k) \)  
2. \( \Phi_k = \text{apply}(P, W_k) \)  
3. \( T' = \text{merge}(T, k, \Phi_k) \)

This is the **entire** SML execution model.

---

## 5. SML Determinism

SML must be:

- deterministic  
- structural  
- unary  
- side‑effect free  

SML does **not** define:

- loops  
- branches  
- jumps  
- conditions  
- semantics  

SML is a **pure structural language**.

---

## 6. SML Example (Structural Form)

```
SEL 16
APPLY xor_shift
MERGE 16
```

Equivalent to:

```
W_16 = select(T, 16)
Φ_16 = apply(xor_shift, W_16)
T' = merge(T, 16, Φ_16)
T = T'
```

---

## Canonical Footer
This document is part of the **Phoenix Rebirth Canonical Sequence (Clocks 1–11)**.  
It is immutable and historically preserved.

---

## Navigation
**Previous → Clock 7: Phoenix Canonical Math**  
**Next → Clock 9: Phoenix Parallelisation Math**  
**Index → Phoenix Rebirth Canonical Sequence (docs/canonical/INDEX.md)**


