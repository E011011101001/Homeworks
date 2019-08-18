;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; struct Node {
;     char _ch;
;     char _time;
;     seg*  _next, _prior;
; };
;
; struct List {
;     Node* _head, _tail;
;     short _size;
; }
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

extrn List_create:far ; List* List_create();
extrn List_display:far ; void List_display(List* pList, short total);
extrn List_count:far ; void List_count(List* pList, short ch);
extrn List_sort:far ; void List_sort(List* pList);

