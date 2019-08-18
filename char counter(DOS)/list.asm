assume cs:code

include malloc.inc
include io.inc

    NODE_SIZE equ 6
    LIST_SIZE equ 6

    ; offsets of Node
    _ch equ 0
    _time equ 1
    _next equ 2
    _prior equ 4

    ; offsets of List
    _head equ 0
    _tail equ 2
    _size equ 4

code segment

; Node* Node_create(short ch);
Node_create proc near
    push bp
    mov bp, sp

    ; ax = malloc(NODE_SIZE);
    mov ax, NODE_SIZE
    push ax
    call malloc

    ; initialize
    mov bx, ax
    mov ax, [bp + 4]
    mov byte ptr es:[bx]._ch, al
    mov byte ptr es:[bx]._time, 1
    mov word ptr es:[bx]._next, 0
    mov word ptr es:[bx]._prior, 0

    mov ax, bx

    mov sp, bp
    pop bp
    ret 2
Node_create endp

; List* List_create();
public List_create
List_create proc far
    push bp
    mov bp, sp

    ; ax = malloc(LIST_SIZE);
    mov word ptr [bp - 2], LIST_SIZE
    sub sp, 2
    call malloc

    mov bx, ax
    mov word ptr es:[bx]._head, 0
    mov word ptr es:[bx]._tail, 0
    mov word ptr es:[bx]._size, 0

    mov sp, bp
    pop bp
    retf
List_create endp

; void List_append(List* pList, short ch);
List_append proc near
    push bp
    mov bp, sp

    ; Node* pNewNode;
    sub sp, 2

    ; pNewNode = Node_create(ch);
    push word ptr [bp + 6]
    call Node_create
    mov [bp - 2], ax

    mov bx, [bp + 4]
    cmp word ptr es:[bx]._size, 0
    jz first_node
    ; if (pList->_size != 0) {
        ; pList->_tail->_next = pNewNode;
        mov bx, es:[bx]._tail
        mov ax, [bp - 2]
        mov es:[bx]._next, ax

        ; pNewNode->_prior = pList->_tail;
        mov bx, [bp + 4]
        mov ax, es:[bx]._tail
        mov bx, [bp - 2]
        mov es:[bx]._prior, ax

        jmp short change_tail
    ; } else {
    first_node:
        ; pList->_head = pNewNode;
        mov bx, [bp + 4]
        mov ax, [bp - 2]
        mov es:[bx]._head, ax
    ; }

change_tail:
    ; pList->_tail = pNewNode;
    mov ax, [bp - 2]
    mov bx, [bp + 4]
    mov es:[bx]._tail, ax

    ; ++pList->_size
    inc word ptr es:[bx]._size
    
    mov sp, bp
    pop bp
    ret 2
List_append endp

; Node* List_search(List* pList, short target);
List_search proc near
    push bp
    mov bp, sp

    mov bx, word ptr [bp + 4]
    cmp word ptr es:[_size], 0
    jz found_nothing
    ; if (pList->_size != 0) {
        mov cx, es:[bx]._size
        mov bx, es:[bx]._head      ; bx: pNode
        mov dx, [bp + 6]
        ; do {
        for_each_search:
            cmp es:[bx]._ch, dl
            jz found_target
            mov bx, es:[bx]._next
            loop for_each_search
        ; } while (cx != 0);
    ; }

    found_nothing:
        mov ax, 0
        jmp short return_search
    found_target:
        mov ax, bx

return_search:
    mov sp, bp
    pop bp
    ret 4
List_search endp

; void List_count(List* pList, short ch);
public List_count
List_count proc far
    push bp
    mov bp, sp

    ; List_search(pList, ch);
    push [bp + 8]
    push [bp + 6]
    call List_search

    cmp ax, 0
    jz append_node

    ; if found {
        mov bx, ax
        inc byte ptr es:[bx]._time
        jmp short return_count
    ; } else {
    append_node:
        ; List_append(pList, ch)
        push [bp + 8]
        push [bp + 6]
        call List_append
    ; }

return_count:
    mov sp, bp
    pop bp
    retf 4
List_count endp

; void List_display(List* pList, short total);
public List_display
List_display proc far
    push bp
    mov bp, sp

    sub sp, 2
    
    mov cx, 5
    mov bx, [bp + 6]
    cmp word ptr es:[bx]._size, cx
    jnb normal
        
    ; if (pList->_size < 5) {
        mov cx, es:[bx]._size
    ; }

    normal:
    mov bx, es:[bx]._head

    mark_0:
        push cx
        cmp bx, 0
        jz display_over

        ; putchar(pNode->_ch);
        mov dl, es:[bx]._ch
        mov ah, 2
        int 21h

        ; putchar(':');
        mov dl, ':'
        int 21h

        ; print_num(pNode->_time, 10);
        mov ax, 10
        push ax
        mov al, es:[bx]._time
        mov ah, 0
        push ax
        call print_num

        ; putchar(',');
        mov dl, ','
        mov ah, 2
        int 21h

        mov [bp - 2], bx

        ; divide(pNode->_time, total);
        push [bp + 8]
        mov al, es:[bx]._time
        mov ah, 0
        push ax
        call divide

        call endl

        mov bx, [bp - 2]

        mov bx, es:[bx]._next
        pop cx
        loop mark_0
    ; }

display_over:
    mov sp, bp
    pop bp
    retf 2
List_display endp

; void Node_swap(Node* pN1, Node* pN2, List* pList);
Node_swap proc near
    push bp
    mov bp, sp

    ; Node* prior; [bp - 2]
    ; Node* next; [bp - 4]
    sub sp, 4

    mov bx, [bp + 4]
    mov ax, es:[bx]._prior
    mov word ptr [bp - 2], ax
    mov bx, [bp + 6]
    mov ax, es:[bx]._next
    mov word ptr [bp - 4], ax

    mov bx, [bp + 4]
    mov ax, word ptr [bp + 6]
    mov word ptr es:[bx]._prior, ax
    mov ax, word ptr [bp - 4]
    mov word ptr es:[bx]._next, ax

    mov bx, [bp + 6]
    mov ax, word ptr [bp - 2]
    mov word ptr es:[bx]._prior, ax
    mov ax, word ptr [bp + 4]
    mov word ptr es:[bx]._next, ax

    cmp word ptr [bp - 2], 0
    jz no_prior
    ; if (prior) {
        mov bx, [bp - 2]
        mov ax, word ptr [bp + 6]
        mov word ptr es:[bx]._next, ax
        jmp short test_next
    ; } else {
    no_prior:
        mov bx, [bp + 8]
        mov ax, word ptr [bp + 6]
        mov word ptr es:[bx]._head, ax
    ; }
test_next:
    cmp word ptr [bp - 4], 0
    jz no_next
    ; if (next) {
        mov bx, [bp - 4]
        mov ax, word ptr [bp + 4]
        mov word ptr es:[bx]._prior, ax
        jmp short return_swap
    ; } else {
    no_next:
        mov bx, [bp + 8]
        mov ax, word ptr [bp + 4]
        mov word ptr es:[bx]._tail, ax
    ; }

return_swap:
    mov sp, bp
    pop bp
    ret 6
Node_swap endp

; void List_sort(List* pList);
public List_sort
List_sort proc far
    push bp
    mov bp, sp

    ; short done;
    ; Node* pNode;
    ; short i;              ; outer loop counter
    sub sp, 6


    ; cx = pList->_size;
    mov bx, [bp + 6]
    mov cx, es:[bx]._size
    cmp cx, 1
    jna return_sort

    dec cx
    mov word ptr [bp - 6], 0        ; i = 0
    bubble_sort:
        mov bx, [bp + 6]
        mov ax, es:[bx]._head
        mov [bp - 4], ax        ; pNode = pList->_head;

        ; done = true;
        mov word ptr [bp - 2], 1
 
        push cx

        ; cx = pList->_size;
        mov bx, [bp + 6]
        mov cx, es:[bx]._size

        ; cx = cx - i - 1;
        dec cx
        sub cx, [bp - 6]
        inner_loop:
            mov bx, [bp - 4]
            mov si, es:[bx]._next
            mov al, byte ptr es:[si]._time
            cmp byte ptr es:[bx]._time, al
            jnb continue_0

            ; done = false;
            mov word ptr [bp - 2], 0

            ; Node_swap(bx, si, pList);
            push [bp + 6]
            push si
            push bx
            call Node_swap
            jmp short continue_1

        continue_0:
            mov [bp - 4], si
        continue_1:
            loop inner_loop
        cmp word ptr [bp - 2], 1
        jz return_sort
        inc word ptr [bp - 6]
        pop cx
        loop bubble_sort

return_sort:
    mov sp, bp
    pop bp
    retf 2
List_sort endp

code ends

end

