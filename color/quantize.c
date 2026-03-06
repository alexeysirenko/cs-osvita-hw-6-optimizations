// 3-3-2 RGB quantization (RRRGGGBB) - branch-free
unsigned char quantize(unsigned char red, unsigned char green,
                       unsigned char blue) {
    return (red & 0xE0) | ((green & 0xE0) >> 3) | (blue >> 6);
}

