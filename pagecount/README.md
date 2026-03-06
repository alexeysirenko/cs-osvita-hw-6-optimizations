# Pagecount

Division -> bitshift since page_size is always a power of 2.

```c
return memory_size >> __builtin_ctzll(page_size);
```

**Result:** ~6x speedup
