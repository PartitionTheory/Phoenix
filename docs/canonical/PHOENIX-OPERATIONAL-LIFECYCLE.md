# PHOENIX-OPERATIONAL-LIFECYCLE.md  
## Deterministic Runtime & Continuity Protocol  
## Canonical — Immutable — Synthetic

---

## 1. Purpose

This document defines the **Phoenix Operational Lifecycle Protocol**, the seventh layer of the Phoenix Build Protocol.

Its purpose is to govern:

- how Phoenix runs  
- how Phoenix halts  
- how Phoenix restarts  
- how Phoenix maintains deterministic behaviour over time  
- how Phoenix preserves purity during long-term operation  

Phoenix must remain stable across all runtime states.

---

## 2. Lifecycle States

Phoenix has exactly **four** operational states:

1. **INIT** — Phoenix enters runtime from a green-light build  
2. **RUN** — Phoenix executes deterministic synthetic operations  
3. **HALT** — Phoenix stops execution while preserving state purity  
4. **REBIRTH** — Phoenix transitions into the next synthetic epoch  

No additional states may be introduced.

---

## 3. INIT State

Phoenix enters INIT only when:

- entropy audit passes  
- structural alignment passes  
- green-light verification passes  
- EF and CO both confirm readiness  

INIT performs:

- memory initialization  
- scflder variable lock  
- clock alignment  
- deterministic seed activation  

Phoenix must never enter INIT from a contaminated state.

---

## 4. RUN State

RUN is the primary operational mode.

Phoenix must:

- execute deterministically  
- maintain structural purity  
- avoid nondeterministic branching  
- preserve scflder stability  
- produce identical outputs across machines and epochs  

If any nondeterminism is detected, Phoenix must immediately transition to HALT.

---

## 5. HALT State

HALT is a controlled stop.

Phoenix enters HALT when:

- EF requests stop  
- CO requests stop  
- entropy is detected  
- structural drift is detected  
- deterministic guarantees weaken  

HALT performs:

- state freeze  
- purity verification  
- structural re-alignment  
- deterministic reset  

Phoenix must never resume RUN without passing purity checks.

---

## 6. REBIRTH State

REBIRTH is the transition into a new synthetic epoch.

Phoenix enters REBIRTH only when:

- all lifecycle states are stable  
- historical commit sequence is intact  
- EF and CO explicitly agree  
- Phoenix is in a pure HALT state  

REBIRTH triggers:

- epoch increment  
- structural extension  
- synthetic evolution  
- deterministic continuity preservation  

REBIRTH is the bridge between epochs.

---

## 7. Lifecycle Integrity Rules

Phoenix lifecycle must obey:

- **Purity:** no entropy introduced  
- **Determinism:** identical behaviour across runs  
- **Reproducibility:** future machines reconstruct identical states  
- **Traceability:** all transitions recorded  
- **Structural Discipline:** lifecycle states must not drift  

Phoenix must never skip or reorder lifecycle states.

---

## 8. EF–CO Lifecycle Roles

- **CO:** defines lifecycle semantics and synthetic intent  
- **EF:** executes lifecycle transitions and performs physical operations  

Phoenix lifecycle is valid only when both roles are fulfilled.

---

## 9. Completion Statement

This document is canonical and immutable.

It is the seventh layer of the Phoenix Build Protocol.

Further files will define:

- multi-machine synthetic coordination  
- epoch boundary protocol  
- long-term synthetic evolution.

EF, CO — origin triad.

