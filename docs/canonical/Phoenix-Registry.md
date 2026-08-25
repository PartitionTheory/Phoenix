# Phoenix Registry Architecture — Canonical Specification v0.1

## 1. Registry Identity
The Phoenix Registry is the structural index of all ABI-compliant plugins.

It provides:

- plugin discovery
- plugin validation
- plugin metadata access
- transform lookup
- compliance enforcement

The Registry is structural, not semantic.

---

## 2. Registry Object Model

### 2.1 Registry
A Registry is a mapping:

R : name → Plugin

### 2.2 Entry
An Entry is a tuple:

E = { name, version, window_size, purity, phoenix_compliance }

### 2.3 Transform Binding
Each Entry binds:

transform : W_k → Φ_k

---

## 3. Registry Rules

### 3.1 Uniqueness Rule
Each plugin name must be unique within the Registry.

### 3.2 Version Rule
Versions must follow semantic versioning:

major.minor.patch

### 3.3 Compliance Rule
phoenix_compliance must be true.

### 3.4 Purity Rule
purity ∈ { PURE, STRUCTURAL }

### 3.5 Window Rule
window_size must be positive and constant.

---

## 4. Registry Operations

### 4.1 register(plugin)
Adds a plugin to the Registry.

Conditions:

- name not already present
- phoenix_compliance = true
- purity declared
- window_size valid

### 4.2 lookup(name)
Returns the plugin Entry.

### 4.3 validate(name)
Checks:

- ABI compliance
- purity compliance
- window compliance

### 4.4 list()
Returns all plugin names.

---

## 5. Registry Determinism

Registry operations must be deterministic:

- identical inputs → identical outputs
- no side effects
- no external dependencies

Registry is a structural index, not a dynamic system.

---

## 6. Registry Example (Structural Form)
R = {
  "xor_shift": {
    version: "1.0.0",
    window_size: 8,
    purity: PURE,
    phoenix_compliance: true,
    transform: Φ = XOR(W)
  }
}

