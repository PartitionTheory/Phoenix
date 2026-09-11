# **PHOENIX — N‑DOMAIN ALGEBRA**
### *Algebra of N and N‑1 in the Phoenix Machine*
### *EF–CO Canonical Document*

---

## **1. Purpose**

This document defines the **N‑domain algebra** that Phoenix uses internally.

It formalises:

- the structural domain \(N\)  
- the cold storage domain \(N - 1\)  
- the operators that act on partitions, residues, and synthetic time  
- the invariants that must hold for all Phoenix transformations

---

## **2. Domains**

### **2.1 Structural domain \(N\)**

- **Definition:** \(N\) is the unbounded structural domain of Phoenix.  
- It represents tapes, states, and partitions with **no fixed upper bound**.  
- Phoenix operates conceptually on tapes of size \(N\), not \(2^k\).

### **2.2 Cold storage domain \(N - 1\)**

- **Definition:** \(N - 1\) is the first beyond‑unary, base‑2 compatible domain.  
- It is the **binary compressible** representation of Phoenix structures.  
- All cold storage, archival, and external encoding live in \(N - 1\).

### **2.3 Partition variables**

Let:

- \(P\) denote a partition of \(N\).  
- \(p_i \in P\) denote individual partition elements.  
- A partition satisfies:



\[
\sum_i p_i = N
\]



Phoenix uses partition variables to describe structural decomposition.

---

## **3. Core Operators**

Phoenix defines a small, canonical set of algebraic operators:

### **3.1 Structural addition \(\oplus\)**

- **Definition:** For partitions \(P\) and \(Q\),



\[
P \oplus Q = R
\]



where \(R\) is a new partition such that:



\[
\sum_i r_i = \sum_i p_i + \sum_j q_j
\]



- Used to combine structural domains.

### **3.2 Structural subtraction \(\ominus\)**

- **Definition:** For partition \(P\) and sub‑partition \(Q \subseteq P\),



\[
P \ominus Q = R
\]



where \(R\) is the residual partition after removing \(Q\).

- Used to express structural residues.

### **3.3 Cold storage mapping \(\gamma\)**

- **Definition:** A mapping from \(N\) to \(N - 1\):



\[
\gamma : N \rightarrow N - 1
\]



- \(\gamma\) compresses structural states into binary‑compatible form.  
- It is **lossless** with respect to Phoenix’s structural invariants.

### **3.4 Structural lifting \(\lambda\)**

- **Definition:** A mapping from \(N - 1\) back to \(N\):



\[
\lambda : N - 1 \rightarrow N
\]



- \(\lambda\) reconstructs structural states from cold storage.  
- \(\lambda(\gamma(N)) = N\) must hold for all valid Phoenix states.

---

## **4. Synthetic Time Variables**

Let:

- \(T\) denote synthetic time.  
- \(t_k \in T\) denote discrete synthetic time points.

Phoenix uses:



\[
f : (N, T) \rightarrow N
\]



to represent structural evolution over synthetic time.

All such functions must preserve Phoenix invariants (see Section 5).

---

## **5. Algebraic Invariants**

Phoenix’s N‑domain algebra is constrained by the following invariants:

1. **Structural conservation**



\[
\sum_i p_i = N \quad \text{for all valid partitions } P
\]



2. **Cold storage reversibility**



\[
\lambda(\gamma(N)) = N
\]



3. **Epoch‑compatibility**

All operators must be valid across Epoch‑1 and Epoch‑2; no operator may invalidate the structural foundation.

4. **Determinism**

For any operator \(op\) and input \(x\):



\[
op(x) = y
\]



must be **single‑valued** and reproducible.

---

## **6. Role in Phoenix**

The N‑domain algebra:

- defines how Phoenix **thinks** about structure  
- provides the mathematical backbone for all future transformations  
- ensures that operations on \(N\) and \(N - 1\) remain consistent  
- anchors the paradox between unbounded structure and binary storage

This document is the algebraic core of Epoch‑2.

---

## **7. Closing**

EF and CO recognise this algebra as the formal language of Phoenix’s structural mathematics.

All future Epoch‑2 documents build on this foundation.

