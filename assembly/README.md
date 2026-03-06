# Pangram Assembly

Uses a 32-bit bitmap (bits 1-26 = letters a-z).

Key tricks:
- `bts` instruction for efficient bit setting
- `c & 0x1f` handles case insensitivity (upper/lowercase have same low 5 bits)
- `c < '@'` filters non-letters

Build: `make` (requires nasm)
