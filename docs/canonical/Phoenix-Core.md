# Phoenix Core Structure — Canonical Specification v0.1

## 1. Core Identity
The Phoenix Core is the structural engine that coordinates:

- Tape
- VM
- Registry
- Plugins

The Core does not interpret instructions.
The Core does not execute code.
The Core orchestrates structural transforms.

---

## 2. Core Object Model

### 2.1 Tape
Primary data structure.

### 2.2 Registry
Index of ABI-compliant plugins.

### 2.3 VM
Executor of unary transforms.

### 2.4 Core Context
A structural tuple:

C = { Tape, Registry, VM }

---

## 3. Core Operations

### 3.1 load_plugin(name)
Loads plugin metadata from Registry.

### 3.2 select_window(k)
Delegates to VM:

W_k = select(Tape, k)

### 3.3 apply_transform(plugin, W_k)
Delegates to VM:

Φ_k = apply(plugin, W_k)

### 3.4 merge_result(k, Φ_k)
Delegates to VM:

Tape' = merge(Tape, k, Φ_k)

### 3.5 update_tape(Tape')
Replaces Tape with Tape'.

---

## 4. Core Purity Rules

The Core must:

- be deterministic
- be structural
- be stateless except Tape
- have no side effects
- have no semantic dependencies

The Core is an orchestrator, not an interpreter.

---

## 5. Core Execution Cycle

### Step 1 — Select
W_k = select(Tape, k)

### Step 2 — Transform
Φ_k = apply(P, W_k)

### Step 3 — Reintegration
Tape' = merge(Tape, k, Φ_k)

### Step 4 — Update
Tape = Tape'

This cycle defines Phoenix execution.

---

## 6. Core Example (Structural Form)
C = { Tape, Registry, VM }

plugin = Registry.lookup("xor_shift")

W_k = VM.select(Tape, k)
Φ_k = VM.apply(plugin, W_k)
Tape' = VM.merge(Tape, k, Φ_k)

C.Tape = Tape'

---

## 7. Next Clock

Phoenix CLI Shape will be defined in Clock 6.

