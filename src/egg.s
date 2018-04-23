.text
.align 1
.thumb
.thumb_func

egg:
	ldr r3, =(0x0806ACAC | 1)
	bl r3_caller
	mov r0, r5
	mov r1, r6
	bl change_ball
	mov r0, r5
	mov r1, r6
	mov r2, #0x64
	ldr r3, =(0x082E93D4 | 1)
	bl r3_caller
	add sp, #0x50
	ldr r3, =(0x080714A8 | 1)
	
r3_caller:
	bx r3


	