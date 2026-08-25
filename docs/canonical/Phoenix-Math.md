# Phoenix Canonical Math — Structural Specification v0.1

## 1. Tape Definition
A Tape T is a finite sequence of bits:

T = { t_0, t_1, ..., t_n }

Tape is a mathematical object, not a data structure.

---

## 2. Window Projection

A Window W_k is defined as:

W_k = T[k : k + n]

Where:

- k is the starting index
- n is the window size declared by the plugin

Window projection is a pure structural operation.

---

## 3. Unary Transform Algebra

A Phoenix transform is a unary function:

Φ_k = P(W_k)

Where:

- P is a plugin transform
- W_k is a window
- Φ_k is the transformed window

Unary transforms form the algebraic core of Phoenix.

---

## 4. Reintegration Operator

Reintegration is defined as:

T' = merge(T, k, Φ_k)

Where:

- T is the original Tape
- Φ_k is the transformed window
- T' is the updated Tape

merge is a structural operator, not a semantic one.

---

## 5. Independence Condition

Two windows W_i and W_j are independent if:

[i : i + n] ∩ [j : j + n] = ∅

Independence is the basis for parallelisation.

---

## 6. Parallelisation Condition

If:

∀ k ∈ S, W_k are independent

Then:

{ P(W_k) } for k ∈ S

is parallelisable.

Parallelisation is a mathematical property, not an execution model.

---

## 7. Distribution Condition

If:

T_1, T_2, ..., T_m

are independent tapes,

then Phoenix operations may be distributed across them.

Distribution is structural, not procedural.

---

## 8. Structural Machine Identity

Phoenix is defined as:

M = { T, W, P, Φ, merge }

Where:

- T is Tape
- W is Window
- P is Plugin
- Φ is Transform
- merge is Reintegration

This quintuple defines the Phoenix structural machine.

---

## 9. Next Clock

Phoenix Structural Machine Language will be defined in Clock 8.

