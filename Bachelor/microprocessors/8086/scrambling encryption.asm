jmp start

please_enter_the_word  db "please enter the word: "
stored_word  db 20 dup ("$")
encrypted_word db "encrypted word: $"
storing_encrypted_word  db 20 dup ("$")
decrypted_word db "decrypted word: $"
storing_decrypted_word  db 20 dup ("$")
new_line db 10,13,"$"

two_letters_encrypted:
lodsb
mov storing_encrypted_word+2,al
lodsb
mov storing_encrypted_word+3,al
jmp encrypted

two_letters_decrypted:
lodsb
mov storing_decrypted_word+2,al
lodsb
mov storing_decrypted_word+3,al
jmp decrypted

four_letters_encrypted:
lodsb
mov storing_encrypted_word+2,al
lodsb
mov storing_encrypted_word+4,al
lodsb
mov storing_encrypted_word+3,al
lodsb
mov storing_encrypted_word+5,al
jmp encrypted

four_letters_decrypted:
lodsb
mov storing_decrypted_word+2,al
lodsb
mov storing_decrypted_word+4,al
lodsb
mov storing_decrypted_word+3,al
lodsb
mov storing_decrypted_word+5,al
jmp decrypted

eight_letters_encrypted:
lodsb
mov storing_encrypted_word+2,al
lodsb
mov storing_encrypted_word+6,al ;001 --> 100 --> 4
lodsb
mov storing_encrypted_word+4,al ;010 --> 010 --> 2
lodsb
mov storing_encrypted_word+8,al ;011 --> 110 --> 6
lodsb
mov storing_encrypted_word+3,al ;100 --> 001 --> 1
lodsb
mov storing_encrypted_word+7,al ;101 --> 101 --> 5
lodsb
mov storing_encrypted_word+5,al ;110 --> 011 --> 3
lodsb
mov storing_encrypted_word+9,al
jmp encrypted

eight_letters_decrypted:
lodsb
mov storing_decrypted_word+2,al
lodsb
mov storing_decrypted_word+6,al ;001 --> 100 --> 4
lodsb
mov storing_decrypted_word+4,al ;010 --> 010 --> 2
lodsb
mov storing_decrypted_word+8,al ;011 --> 110 --> 6
lodsb
mov storing_decrypted_word+3,al ;100 --> 001 --> 1
lodsb
mov storing_decrypted_word+7,al ;101 --> 101 --> 5
lodsb
mov storing_decrypted_word+5,al ;110 --> 011 --> 3
lodsb
mov storing_decrypted_word+9,al
jmp decrypted

sixteen_letters_encrypted:
lodsb
mov storing_encrypted_word+2,al
lodsb
mov storing_encrypted_word+10,al ;0001 --> 1000 --> 8
lodsb
mov storing_encrypted_word+6,al  ;0010 --> 0100 --> 4
lodsb
mov storing_encrypted_word+14,al ;0011 --> 1100 --> 12
lodsb
mov storing_encrypted_word+4,al  ;0100 --> 0010 --> 2
lodsb
mov storing_encrypted_word+12,al ;0101 --> 1010 --> 10
lodsb
mov storing_encrypted_word+8,al  ;0110 --> 0110 --> 6
lodsb
mov storing_encrypted_word+16,al ;0111 --> 1110 --> 14
lodsb
mov storing_encrypted_word+3,al  ;1000 --> 0001 --> 1
lodsb
mov storing_encrypted_word+11,al ;1001 --> 1001 --> 9
lodsb
mov storing_encrypted_word+7,al  ;1010 --> 0101 --> 5
lodsb
mov storing_encrypted_word+15,al ;1011 --> 1101 --> 13
lodsb
mov storing_encrypted_word+5,al  ;1100 --> 0011 --> 3
lodsb
mov storing_encrypted_word+13,al ;1101 --> 1011 --> 11
lodsb
mov storing_encrypted_word+9,al  ;1110 --> 0111 --> 7
lodsb
mov storing_encrypted_word+17,al
jmp encrypted

sixteen_letters_decrypted:
lodsb
mov storing_decrypted_word+2,al
lodsb
mov storing_decrypted_word+10,al ;0001 --> 1000 --> 8
lodsb
mov storing_decrypted_word+6,al  ;0010 --> 0100 --> 4
lodsb
mov storing_decrypted_word+14,al ;0011 --> 1100 --> 12
lodsb
mov storing_decrypted_word+4,al  ;0100 --> 0010 --> 2
lodsb
mov storing_decrypted_word+12,al ;0101 --> 1010 --> 10
lodsb
mov storing_decrypted_word+8,al  ;0110 --> 0110 --> 6
lodsb
mov storing_decrypted_word+16,al ;0111 --> 1110 --> 14
lodsb
mov storing_decrypted_word+3,al  ;1000 --> 0001 --> 1
lodsb
mov storing_decrypted_word+11,al ;1001 --> 1001 --> 9
lodsb
mov storing_decrypted_word+7,al  ;1010 --> 0101 --> 5
lodsb
mov storing_decrypted_word+15,al ;1011 --> 1101 --> 13
lodsb
mov storing_decrypted_word+5,al  ;1100 --> 0011 --> 3
lodsb
mov storing_decrypted_word+13,al ;1101 --> 1011 --> 11
lodsb
mov storing_decrypted_word+9,al  ;1110 --> 0111 --> 7
lodsb
mov storing_decrypted_word+17,al
jmp decrypted


increment:
lodsb
inc cx
jmp counting

start:

lea dx,please_enter_the_word
mov ah,09h
int 21h

lea dx,stored_word
mov ah,0Ah
int 21h

lea dx,stored_word+2

mov si,dx
mov cx,0h
mov al,0h

counting: 
cmp al,'$'
jne increment

sub cx,2h
mov si,dx

cmp cx,2
je two_letters_encrypted
cmp cx,4
je four_letters_encrypted
cmp cx,8
je eight_letters_encrypted
cmp cx,16
je sixteen_letters_encrypted


encrypted:
mov al,0h
lea dx,new_line
mov ah,09h
int 21h
lea dx,encrypted_word
int 21h
lea dx,storing_encrypted_word+2
int 21h

mov si,dx

cmp cx,2
je two_letters_decrypted
cmp cx,4
je four_letters_decrypted
cmp cx,8
je eight_letters_decrypted
cmp cx,16
je sixteen_letters_decrypted

decrypted:
mov al,0h
lea dx,new_line
mov ah,09h
int 21h
lea dx,decrypted_word
int 21h
lea dx,storing_decrypted_word+2
int 21h