 
mov ah, 0
mov al, 12h 
int 10h
mov ax, 0c0fh


;inital cursor position
mov dx,100  
mov cx,300
int 10h
;---------------------------------------

mov si,cx 
mov di,30 ;control the width of the diamond
mov bx,1 ;control the heigth of the diamond
 
sub si,di
     
side1:
int 10h
dec cx
add dx,bx
cmp cx,si
jnz side1
 
add si,di
    
side2:
int 10h
inc cx
add dx,bx   
cmp cx,si
jnz side2

add si,di
     
side3:
int 10h
inc cx
sub dx,bx
cmp cx,si
jnz side3

sub si,di   
   
side4:
int 10h
dec cx
sub dx,bx   
cmp cx,si
jnz side4      