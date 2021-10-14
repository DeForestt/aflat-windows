.global brk
.global sys_create
.global sys_write
.global sys_open
.global sys_read
.global sys_close
.global sys_fstat
.global sys_pause
.global sys_fork
.global sys_sleep

.text

brk:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq    $0x0c, %rax
	syscall
	leave
	ret

sys_write:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq    $0x01, %rax
	syscall
	leave
	ret

sys_create:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq    $0x55, %rax
	syscall
	leave
	ret

sys_open:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq    $0x02, %rax
	syscall
	leave
	ret

sys_read:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq    $0x00, %rax
	syscall
	leave
	ret

sys_close:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq    $0x03, %rax
	syscall
	leave
	ret

sys_fstat:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq    $0x05, %rax
	syscall
	leave
	ret

sys_fork:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq    $57, %rax
	syscall
	leave
	ret

sys_pause:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq    $34, %rax
	syscall
	leave
	ret

sys_sleep:
	pushq	%rbp
	movq	%rsp, %rbp
	subq	$16, %rsp
	movq    $35, %rax
	syscall
	leave
	ret