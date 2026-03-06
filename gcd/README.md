# GCD: Big-O vs Reality

| Algorithm | Complexity | Instruction |
|-----------|------------|-------------|
| Subtraction | O(max(a,b)) | SUB (~1 cycle) |
| Modulo | O(log min) | DIV (~30-90 cycles) |

**Close inputs:** Subtraction wins (few iterations)
**Far inputs:** Modulo wins (way fewer iterations)

Big-O doesn't account for instruction cost.
