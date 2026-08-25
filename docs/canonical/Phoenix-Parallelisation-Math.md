# Phoenix Parallelisation Math — Canonical Specification v0.1

## 1. Parallel Window Independence

Two windows W_i and W_j are parallelisable iff:

```
i ≠ j
and
window_size(P_i) = window_size(P_j)
and
no overlap(W_i, W_j)
```

This defines structural independence.

---

## 2. Parallel Transform Independence

Two transforms P_i and P_j are parallelisable iff:

```
P_i and P_j are unary
and
P_i and P_j have no shared state
and
P_i and P_j are pure
```

Phoenix parallelism is **structural**, not semantic.

---

## 3. Parallel Reintegration

Parallel reintegration is valid iff:

```
merge(T, i, Φ_i) and merge(T, j, Φ_j)
do not modify overlapping regions
```

This ensures deterministic reintegration.

---

## 4. Parallel Execution Set

A parallel execution set S is defined as:

```
S = { (k, P_k) | windows independent and transforms independent }
```

Phoenix executes S in parallel.

---

## 5. Parallel Cycle

Parallel Phoenix execution is:

```
for all (k, P_k) in S:
    W_k = select(T, k)
    Φ_k = apply(P_k, W_k)

for all (k, Φ_k) in S:
    T = merge(T, k, Φ_k)
```

This is deterministic parallelism.

---

## 6. Deterministic Guarantee

Parallel Phoenix execution is deterministic iff:

```
∀ i,j ∈ S:
    no overlap(W_i, W_j)
    no overlap(Φ_i, Φ_j)
```

This is the core mathematical guarantee.

---

## 7. Next Clock
Phoenix Distribution Math will be defined in Clock 10.

