# Phoenix ABI — Canonical Specification v0.1

## 1. ABI Identity
The Phoenix ABI defines the structural contract between the following ABR subsystems:

- Core
- VM
- Plugins
- Registry
- Interface Layer

The ABI is purely structural and mathematical. It does not define lexical semantics or execution models.

---

## 2. ABI Object Model

### 2.1 Tape
A Tape is a structural sequence of bits treated as a mathematical object.

### 2.2 Window
A Window is a projection on Tape:

W_k = Tape[k : k + n]

### 2.3 Transform
A Transform is a unary function:

Φ = P(W)

### 2.4 Plugin
A Plugin is a packaged Transform that conforms to the Phoenix ABI.

---

## 3. ABI Function Signatures (Structural)

### 3.1 Window Selector
select(T, k) → W_k

### 3.2 Plugin Transform
apply(P, W_k) → Φ_k

### 3.3 Reintegration
merge(T, k, Φ_k) → T'

These are the only structural operations permitted by the Phoenix ABI.

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

This ensures ABR remains a structural machine, not a semantic program.

---

## 5. ABI Parallelisation Rule

Parallelisation is defined mathematically:

If ∀ k ∈ S, W_k are independent,

then { P(W_k) } for k ∈ S

is parallelisable.

No threads, processes, or async constructs are implied. Parallelism is a property of independence.

---

## 6. ABI Distribution Rule

Distribution is defined as:

T_i are independent tapes

with identical ABI rules.

No RPC, MPI, or channels are implied. Distribution is structural, not procedural.

---

## 7. ABI Plugin Contract (Preview)

A Phoenix plugin must define:

- name
- version
- window size
- transform function
- purity declaration
- Phoenix compliance flag

The full plugin contract will be defined in Phoenix Rebirth — Clock 2.

