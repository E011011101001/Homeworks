assume cs:code, ss:stack, ds:data

include io.inc
include debug.inc
include malloc.inc
include list.inc

stack segment stack
    dw 03fh dup(?)
stack ends

data segment

    msg db 81 dup(?)
    len db 0
    prompt db 'please input a sentence ending with an enter:', 0

data ends

code segment

start proc far
    mov bp, sp

    push ds
    mov ax, 0
    push ax

    mov ax, data
    mov ds, ax

    call set_es

    call main

    retf
start endp

main proc near
    push bp
    mov bp, sp

    ; List* pList;
    sub sp, 2

    ; puts(code, prompt, true);
    mov word ptr [bp - 4], 1
    mov word ptr [bp - 6], offset prompt
    mov word ptr [bp - 8], seg prompt
    sub sp, 6
    call puts

    ; ax = getline(code, msg, 81);
    mov word ptr [bp - 4], 81
    mov word ptr [bp - 6], offset msg
    mov word ptr [bp - 8], seg msg
    sub sp, 6
    call getline

    ; all_lower(msg);
    mov word ptr [bp - 4], offset msg
    sub sp, 2
    call all_lower

    ; pList = List_create();
    call List_create
    mov [bp - 2], ax

    lea si, msg
    mark_main:
        cmp byte ptr [si], 0
        jz sort

        ; if ([si] != '\0') {

            ; List_count(pList, [si])
            push [si]
            push [bp - 2]
            call List_count
        ; }
        inc si
        jmp short mark_main

sort:
    mov word ptr ds:[len], si
    
    ; List_sort(pList);
    push [bp - 2]
    call List_sort

    ; List_display(pList, len);
    push word ptr ds:[len]
    push [bp - 2]
    call List_display

    mov sp, bp
    pop bp
    ret
main endp

; void all_lower(offset)
all_lower proc near
    push bp
    mov bp, sp

    mov si, [bp + 4]
    
    continue:
        cmp byte ptr [si], 0
        jz return_upper

        ; if ([si] != '\0') {
            cmp byte ptr [si], 'A'
            jb mark_0

            ; if ([si] >= 'A') {
                cmp byte ptr [si], 'Z'
                ja mark_0

                ; if ([si] <= 'Z') {
                    or byte ptr [si], 00100000b      ; to_lower
                ; }
            ; }
        ; }

    mark_0:
        inc si
        jmp short continue

return_upper:
    mov sp, bp
    pop bp
    ret 2
all_lower endp

code ends

end start

