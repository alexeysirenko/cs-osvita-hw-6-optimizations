# Color Quantize

Replaced ~20 branches with bit ops:

```c
return (red & 0xE0) | ((green & 0xE0) >> 3) | (blue >> 6);
```

Output format: RRRGGGBB (3-3-2 bits)
