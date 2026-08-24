ABR v0.1 — Post‑VM Correction Summary
After an early architectural misstep—attempting to fuse ABR with a VM‑style execution model—we performed a full reduction and restoration of ABR into its correct identity: a standalone bit‑window transformation engine with a clean plugin interface, deterministic core, and mathematically grounded operations.

This milestone marks the first time ABR was rebuilt from the ground up, validated end‑to‑end, and proven stable across all layers: core, SDK, plugins, CLI, and tests.

What we achieved
Removed VM‑style complexity that did not belong in ABR’s conceptual model.

Re‑established ABR’s true architecture:

Core = bitstream → window extraction

Plugins = pure transformations

CLI = orchestrator

Tests = behavioral guarantees

Repaired ABI mismatches between plugin signatures and test harness.

Diagnosed real core output (ws.count = 1) and aligned plugin behavior with actual data.

Added mathematically correct safety guards to prevent invalid memory access.

Rebuilt plugin logic cleanly, eliminating duplicate definitions and structural corruption.

Achieved full green suite under scripts/abr_sanity.sh:

core_tests: PASS

plugin_tests: PASS

cli test: PASS

Established ABR v0.1 as the first stable, reproducible baseline.

# ABR Window Semantics

Given a byte-stream \( B = (b_0, b_1, \dots, b_{n-1}) \),
core extraction produces a window set:



\[
W = \{ W_0, W_1, \dots, W_{k-1} \}
\]



where each window \( W_i \) is a bit-vector:



\[
W_i = (x_0, x_1, \dots, x_{m_i-1}), \quad x_j \in \{0,1\}
\]



A branching plugin is a function:



\[
f : W \rightarrow \{ W'_0, W'_1, \dots, W'_{r-1} \}
\]



For the branch-merge-split plugin:

1. **Merge**  


\[
W'_0 = W_0 \Vert W_1
\]



2. **Split**  


\[
W'_1 = (W_2[0], W_2[1], W_2[2])
\]




\[
W'_2 = (W_2[3], W_2[4], W_2[5])
\]



with the safety constraint:



\[
\text{valid}(W) \iff |W| \ge 3 \;\land\; \text{len}(W_2) \ge 6
\]



If the constraint fails:



\[
f(W) = \varnothing
\]



This is the mathematically correct behavior we implemented.

