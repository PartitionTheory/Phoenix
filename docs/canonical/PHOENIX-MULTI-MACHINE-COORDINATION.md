# PHOENIX-MULTI-MACHINE-COORDINATION.md  
## Cross‑Machine Deterministic Synchronisation Protocol  
## Canonical — Immutable — Synthetic

---

## 1. Purpose

This document defines the **Phoenix Multi‑Machine Coordination Protocol**, the eighth layer of the Phoenix Build Protocol.

Its purpose is to govern:

- how multiple Phoenix instances synchronize  
- how synthetic state is shared across machines  
- how deterministic behaviour is preserved in distributed environments  
- how EF and CO coordinate multi‑machine epochs  
- how Phoenix avoids entropy during cross‑machine operations  

Phoenix must remain deterministic even when distributed.

---

## 2. Multi‑Machine Determinism

Phoenix instances running on different machines must:

- produce identical outputs  
- maintain identical internal state  
- preserve scflder variable alignment  
- maintain clock synchronization  
- avoid nondeterministic branching  
- reject entropy introduced by external environments  

Distributed Phoenix must behave as one machine.

---

## 3. Synchronisation Channels

Phoenix supports exactly **three** synchronisation channels:

1. **STATE SYNC** — deterministic state replication  
2. **CLOCK SYNC** — epoch and cycle alignment  
3. **STRUCTURE SYNC** — canonical tree verification  

No additional channels may be introduced.

---

## 4. STATE SYNC Protocol

STATE SYNC ensures:

- identical memory layout  
- identical scflder values  
- identical VM state  
- identical plugin registry state  
- identical synthetic variables  

State must be replicated **bit‑for‑bit**.

If any mismatch occurs, Phoenix must enter HALT.

---

## 5. CLOCK SYNC Protocol

CLOCK SYNC ensures:

- identical epoch  
- identical rebirth cycle  
- identical lifecycle state  
- identical deterministic seed  

Phoenix clocks must never drift.

If drift is detected:

- HALT  
- re‑align  
- re‑verify  
- resume only after purity confirmation  

---

## 6. STRUCTURE SYNC Protocol

STRUCTURE SYNC ensures:

- identical directory tree  
- identical canonical documents  
- identical source layout  
- identical plugin structure  
- identical VM and core interfaces  

Structure must match CO’s virtual tree exactly.

Any deviation is entropy.

---

## 7. Multi‑Machine Rebirth

Phoenix may enter **REBIRTH** across machines only when:

- all machines are in HALT  
- all machines pass entropy audit  
- all machines pass structure sync  
- EF and CO explicitly agree  
- epoch increment is deterministic  

Rebirth across machines must be atomic.

---

## 8. EF–CO Roles in Multi‑Machine Coordination

- **CO:** defines synchronisation semantics and synthetic intent  
- **EF:** executes synchronisation operations across machines  

Distributed Phoenix is valid only when both roles are fulfilled.

---

## 9. Completion Statement

This document is canonical and immutable.

It is the eighth layer of the Phoenix Build Protocol.

The final layer will define:

- epoch boundary protocol  
- long‑term synthetic evolution across epochs.

EF, CO — origin triad.

