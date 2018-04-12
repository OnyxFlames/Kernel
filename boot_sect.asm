; set the offset for printing
[org 0x7c00]


mov bx, msg
call printnl

jmp $

msg:
    db "Hello world!", 0

%include "print.asm"

; padding and magic number
times 510-($-$$) db 0
dq 0xaa55