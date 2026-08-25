# Phoenix Structural Machine Language — Canonical Specification v0.1

## 1. SML Identity
Phoenix SML is a symbolic language describing structural operations on Tape.  
It is not an instruction set, programming language, or semantic language.  
SML expresses Phoenix operations in minimal symbolic form.

---

## 2. SML Symbols

T      — Tape  
W_k    — Window at position k  
P      — Plugin transform  
Φ_k    — Result of applying P to W_k  
merge  — Reintegration operator  
k      — Position index

---

## 3. SML Operations

### Select
```
SEL k
```
Selects window W_k from Tape T.

### Apply
```
APPLY P
```
Applies plugin transform P to the current window W_k.

### Merge
```
MERGE k
```
Reintegrates Φ_k back into Tape T at position k.

---

## 4. SML Execution Cycle

```
SEL k
APPLY P
MERGE k
```

Equivalent to:

1. W_k = select(T, k)  
2. Φ_k = apply(P, W_k)  
3. T' = merge(T, k, Φ_k)

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

## 7. Next Clock
Phoenix Parallelisation Math will be defined in Clock 9.

