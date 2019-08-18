assume cs:code

public puts
public endl
public print_num
public getline

code segment

; void puts(seg, offset, short endl);
puts proc far
    push bp
    mov bp, sp
    push ax
    push dx
    push ds
    push si
    pushf

    mov ax, [bp + 6]
    mov ds, ax
    mov si, [bp + 8]

    mov ah, 2
for_each_char:
    mov dl, [si]
    cmp dl, 0
    jz endstr
    int 21h
    inc si
    jmp short for_each_char

endstr:
    cmp word ptr [bp + 10], 0
    jz puts_return          ; endl = false
    call far ptr endl
    
puts_return:
    popf
    pop si
    pop ds
    pop dx
    pop ax
    mov sp, bp
    pop bp
    retf 6
puts endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void endl();
;
; Considering of efficiency, endl does not push any
;   registers.
; Procs that call endl must push ax and dx (maybe
;   along with flags) themselves. 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
endl proc far
    mov ah, 2
    mov dl, 0dh
    int 21h
    mov dl, 0ah
    int 21h
    retf
endl endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void print_num(unsigned num, unsigned numSys)
;       num: the number to output
;       numSys: the numeral system
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
print_num proc far
    push bp
    mov bp, sp
    push ax
    push bx
    mov bh, 0
    push cx
    push dx

    mov ax, [bp + 6]
    mov bx, [bp + 8]

    mov cx, 0
continue_division:
    mov dx, 0
    div bx
    push dx
    inc cx
    cmp ax, 0
    jnz continue_division

    DIFF equ 'A' - 10 - '0'
    mov ah, 2
output_char_rev:
    pop dx
    add dl, '0'
    cmp dl, '9'
    jna putchar_print_num
    add dl, DIFF
    
putchar_print_num:
    int 21h
    loop output_char_rev
    
    pop dx
    pop cx
    pop bx
    pop ax
    mov sp, bp
    pop bp
    retf 4
print_num endp


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; short getline(seg, off, short size);
; return: 0 for success, 1 for overflow
;
; If 1 is returned, size - 1 chars are saved at seg:off,
;   following by '\0'. Remaining chars in the input buffer
;   are cleared.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

tooLong: db 'Warning: Your input has exceeded the buffer limit. Results may be unexpected.', 0
msgLen dw $ - tooLong

getline proc far
    push bp
    mov bp, sp
    push cx
    push ds
    push si
    pushf
    
    mov ax, [bp + 6]
    mov ds, ax
    mov si, [bp + 8]

    mov ah, 1
    mov cx, [bp + 10]
    cmp cx, 1
    ja space_enough     ; cx >= 2
    cmp cx, 0
    jz getline_fail     ; cx == 0
    jmp short overflow  ; cx == 1

space_enough:
    dec cx
get_each_char:
    int 21h

    cmp al, 0dh
    jnz not_end_of_input
    mov byte ptr [si], 0
    jmp short getline_success

not_end_of_input:
    mov [si], al
    inc si
    loop get_each_char

; already overflow here
overflow:
    mov byte ptr [si], 0
getline_fail:
    int 21h
    cmp al, 0dh
    jnz getline_fail

    call endl

    mov ax, 1
    push ax
    mov ax, offset tooLong
    push ax
    mov ax, seg tooLong
    push ax
    call puts

    call endl

    mov ax, 1           ; error code

    jmp short getline_return

getline_success:
    mov ax, 0

getline_return:
    popf
    pop si
    pop ds
    pop cx
    mov sp, bp
    pop bp
    retf 6
getline endp

; void divide(short a, short b)
public divide
divide proc far
    push bp
    mov bp, sp

    sub sp, 4
    ; short k1;
    ; short k2;

    mov ax, [bp + 6]
    mov bl, 100
    mul bl

    mov bx, [bp + 8]
    mov dx, 0
    div bx
    mov [bp - 2], ax


    mov ax, dx
    mov cl, 10
    mul cl
    mov dx, 0
    div bx
    mov [bp - 4], ax

    mov ax, dx
    mul cl
    mov dx, 0
    div bx

    cmp ax, 5
    jb rounding_end

    inc word ptr [bp - 4]
    cmp word ptr [bp - 4], 10
    jb rounding_end

    inc word ptr [bp - 2]
    mov word ptr [bp - 4], 0

rounding_end:
    ; print_num([bp - 2], 10);
    mov ax, 10
    push ax
    push [bp - 2]
    call far ptr print_num

    ; putchar('.');
    mov ah, 2
    mov dl, '.'
    int 21h

    ; putchar(k2);
    mov dx, [bp - 4]
    add dx, 30h
    int 21h

    ; putchar('%');
    mov dl, '%'
    int 21h

    mov sp, bp
    pop bp
    retf 4
divide endp

code ends

end

