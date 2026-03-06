# Vector Dot Product

Optimizations:
1. Cache `vec_length()` outside loop
2. Direct array access via `get_vec_start()`
3. 4x loop unrolling with 4 accumulators

Multiple accumulators break the dependency chain, letting CPU execute in parallel.
