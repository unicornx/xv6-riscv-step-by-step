# Initial process
# This code runs in user space.

.globl _start
.globl main
_start:
	call main
stop:
	j stop     # Infinite loop to stop execution
