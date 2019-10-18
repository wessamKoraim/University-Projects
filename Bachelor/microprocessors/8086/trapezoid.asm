mov cx,100
mov dx,100
;set cursor position
mov ah,0h
mov al,12h
int 10h
;entered the graphical mode
mov ax,0c0fh
;entered the pixel mode
side1:
int 10h
inc cx
cmp cx,200
jne side1

side2:
int 10h
inc cx
inc dx
cmp dx,180
jne side2

side3:
int 10h
dec cx
cmp cx,100
jne side3

side4:
int 10h
dec dx
cmp dx,100
jne side4