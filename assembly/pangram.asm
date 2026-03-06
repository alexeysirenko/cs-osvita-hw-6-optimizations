; pangram.asm - check if string contains every letter A-Z
; Uses bitmap: bits 1-26 = letters a-z
section .text
global pangram

pangram:
    xor     eax, eax            ; map = 0
    mov     edx, 0x07fffffe     ; MASK (bits 1-26 set)

.loop:
    movzx   ecx, byte [rdi]     ; load char
    test    cl, cl              ; null terminator?
    jz      .done
    inc     rdi                 ; s++
    cmp     cl, '@'             ; skip if < 64 (non-letter)
    jb      .loop
    and     ecx, 0x1f           ; letter index (1-26)
    bts     eax, ecx            ; set bit in map
    jmp     .loop

.done:
    and     eax, edx            ; map & MASK
    cmp     eax, edx            ; all letters present?
    sete    al
    movzx   eax, al
    ret
