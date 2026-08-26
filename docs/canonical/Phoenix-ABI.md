# Phoenix ABI — Canonical Specification v1.0  
**Phoenix Rebirth — Clock 1**  
**Date:** 2026‑08‑25  
**Status:** Canonical, Immutable

---

## 1. ABI Identity
The Phoenix ABI defines the structural contract between the following ABR subsystems:

- Core  
- VM  
- Plugins  
- Registry  
- Interface Layer  

The ABI is purely structural and mathematical.  
It does not define lexical semantics or execution models.

---

## 2. ABI Object Model

### 2.1 Tape
A **Tape** is a structural sequence of bits treated as a mathematical object.

### 2.2 Window
A **Window** is a projection on Tape:



\[
W_k = \text{Tape}[k : k + n]
\]



### 2.3 Transform
A **Transform** is a unary function:



\[
\Phi = P(W)
\]



### 2.4 Plugin
A **Plugin** is a packaged Transform that conforms to the Phoenix ABI.

---

## 3. ABI Function Signatures (Structural)

### 3.1 Window Selector


\[
\text{select}(T, k) \rightarrow W_k
\]



### 3.2 Plugin Transform


\[
\text{apply}(P, W_k) \rightarrow \Phi_k
\]



### 3.3 Reintegration


\[
\text{merge}(T, k, \Phi_k) \rightarrow T'
\]



These are the **only** structural operations permitted by the Phoenix ABI.

---

## 4. ABI Purity Rules

Phoenix ABI enforces:

- Unary transforms only  
- No shared state  
- No lexical control flow  
- No grouping constructs  
- No semantic dependencies  
- No external side effects  
- No mutable global state  

This ensures ABR remains a **structural machine**, not a semantic program.

---

## 5. ABI Parallelisation Rule

Parallelisation is defined mathematically:

If:



\[
\forall k \in S,\; W_k \text{ are independent}
\]



Then:



\[
\{ P(W_k) \}_{k \in S}
\]



is **parallelisable**.

No threads, processes, or async constructs are implied.  
Parallelism is a property of **independence**, not execution models.

---

## 6. ABI Distribution Rule

Distribution is defined as:



\[
T_i \text{ are independent tapes}
\]



with identical ABI rules.

No RPC, MPI, or channels are implied.  
Distribution is **structural**, not procedural.

---

## 7. ABI Plugin Contract (Preview)

A Phoenix plugin must define:

- name  
- version  
- window size  
- transform function  
- purity declaration  
- Phoenix compliance flag  

The full plugin contract is defined in:

**Phoenix Rebirth — Clock 2: Plugin Contract**

---

## Canonical Footer
This document is part of the **Phoenix Rebirth Canonical Sequence (Clocks 1–11)**.  
It is immutable and historically preserved.

---

## Navigation
**Next → Clock 2: Phoenix Plugin Contract**  
**Index → Phoenix Rebirth Canonical Sequence (docs/canonical/INDEX.md)**


