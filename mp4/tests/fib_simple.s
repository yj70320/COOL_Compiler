	.text
	.file	"fib.ll"
	.globl	fib                             # -- Begin function fib
	.p2align	4, 0x90
	.type	fib,@function
fib:                                    # @fib
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	pushq	%rax
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -24
	.cfi_offset %rbp, -16
	movl	%edi, %eax
	cmpl	$2, %eax
	movq	%rax, (%rsp)                    # 8-byte Spill
	jg	.LBB0_3
# %bb.1:                                # %then
	movl	$1, %eax
	jmp	.LBB0_2
.LBB0_3:                                # %else
	movq	(%rsp), %rbx                    # 8-byte Reload
	leal	-1(%rbx), %ecx
	addl	$-2, %ebx
	movl	%ecx, %edi
	callq	fib@PLT
	movl	%eax, %ebp
	movl	%ebx, %edi
	callq	fib@PLT
	movl	%eax, %ecx
	addl	%ebp, %ecx
	movl	%ecx, %eax
.LBB0_2:                                # %then
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	fib, .Lfunc_end0-fib
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$12, %edi
	callq	fib@PLT
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
