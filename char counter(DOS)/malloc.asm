assume cs:code, es:alloc

public malloc

alloc segment

    allocPtr dw 2
    dw 0ffh dup(0)

alloc ends

code segment

; short malloc(short size);
public malloc
malloc proc far
    push bp
    mov bp, sp

    mov ax, es:[allocPtr]
    mov cx, [bp + 6]
    add word ptr es:[allocPtr], cx

    mov sp, bp
    pop bp
    retf 2
malloc endp

; void set_es();
public set_es
set_es proc far
    mov ax, alloc
    mov es, ax
    retf
set_es endp

code ends

end

