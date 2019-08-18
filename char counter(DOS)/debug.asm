assume cs:code

include io.inc


code segment

; void print_memory(word segment, word offset, word length);
public print_memory
print_memory proc far
    push bp
    mov bp, sp

    mov bx, 16                  ; bx must not be changed in this process

    ; print_num(segment, 16)
    push bx
    push word ptr [bp + 6]
    call print_num

    ; putchar('-');
    mov ah, 2
    mov dl, ':'
    int 21h

    ; print_num(offset, 16)
    push bx
    push word ptr [bp + 8]
    call print_num

    ; putchar(' ')
    mov dl, ' '
    int 21h

    cmp word ptr [bp + 8], 0
    jz no_spaces

    ; if (offset != 0) {
        mov ax, 3
        mul word ptr [bp + 8]
        mov cx, ax
        mov ah, 2

        spaces:
            int 21h             ; dl is now ' '
            loop spaces
    ; }

no_spaces:

    mov es, [bp + 6]
    mov si, [bp + 8]

    mov cl, 4
    shr word ptr [bp + 8], cl
    shl word ptr [bp + 8], cl   ; offset: xxx0

    mov cx, [bp + 10]

    mark1:
        mov ah, 0
        mov al, es:[si]

        ; print_num(ax, 16)
        push bx
        push ax
        call print_num

        ; putchar(' ')
        mov dl, ' '
        mov ah, 2
        int 21h

        inc si
        cmp si, 17
        jb continue_0               ; si < 17

        ; if (si == 17) {
            ; si = 0
            mov si, 0

            ; es += 1
            inc word ptr [bp + 6]
            mov es, [bp + 6]

            ; endl
            call endl

            ; print_num(segment, 16)
            push bx
            push word ptr [bp + 6]
            call print_num

            ; putchar(':');
            mov ah, 2
            mov dl, ':'
            int 21h

            ; offset += 10h;
            add word ptr [bp + 8], 10h
            ; print_num(offset, 16)
            push bx
            push word ptr [bp + 8]
            call print_num

            ; putchar(' ');
            mov dl, ' '
            int 21h
        ; }
        
    continue_0:
        cmp si, 8
        jnz continue_1
                                    ; si == 8
        ; putchar('-'); putchar(' ')
        mov dl, '-'
        int 21h
        mov dl, ' '
        int 21h

    continue_1:                     ; si != 8
        loop mark1

    mov sp, bp
    pop bp
    retf 6
print_memory endp

code ends

end

