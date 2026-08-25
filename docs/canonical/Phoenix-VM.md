# Phoenix VM Semantics — Canonical Specification v0.1

## 1. VM Identity
The Phoenix VM is the structural executor of ABI-compliant transforms.

It does not interpret instructions.
It does not execute code.
It does not manage control flow.

The VM applies unary transforms to windows on Tape.

---

## 2. VM Object Model

### 2.1 Tape
A Tape is a structural bit sequence.

### 2.2 Window
A Window is a projection:

W_k = Tape[k : k + window_size]

### 2.3 Transform
A Transform is a unary function:

Φ_k = P(W_k)

### 2.4 Reintegration
Reintegration produces a new Tape:

T' = merge(T, k, Φ_k)

---

## 3. VM Semantics

### 3.1 Select
select(T, k) → W_k

### 3.2 Apply
apply(P, W_k) → Φ_k

### 3.3 Merge
merge(T, k, Φ_k) → T'

These three operations define the entire VM.

There are no other operations.

---

## 4. VM Purity Rules

The VM must:

- be deterministic
- be stateless
- be structural
- have no side effects
- have no semantic dependencies

The VM is a structural executor, not a semantic machine.

---

## 5. VM Execution Model

### 5.1 Unary Execution
Each transform is unary:

P(W_k)

### 5.2 No Control Flow
The VM does not:

- branch
- loop
- jump
- interpret instructions

### 5.3 No Shared State
Transforms cannot modify:

- global state
- external state
- other windows
- other tapes

### 5.4 No Ordering Guarantees
Execution order is not semantically meaningful.

---

## 6. VM Example (Structural Form)

T = [bit sequence]

k = 16
W_k = select(T, k)

P = xor_shift
Φ_k = apply(P, W_k)

T' = merge(T, k, Φ_k)
