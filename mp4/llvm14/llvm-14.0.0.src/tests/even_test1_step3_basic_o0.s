	.text
	.file	"even.ll"
	.globl	even                            # -- Begin function even
	.p2align	4, 0x90
	.type	even,@function
even:                                   # @even
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	%edi, 4(%rsp)                   # 4-byte Spill
	cmpl	$0, %edi
	je	.LBB0_4
# %bb.1:                                # %nonzero
	movl	4(%rsp), %eax                   # 4-byte Reload
	cmpl	$0, %eax
	jle	.LBB0_3
# %bb.2:                                # %positive
	movl	4(%rsp), %edi                   # 4-byte Reload
	subl	$1, %edi
	callq	even@PLT
	testb	$1, %al
	jne	.LBB0_5
	jmp	.LBB0_4
.LBB0_3:                                # %negative
	movl	4(%rsp), %edi                   # 4-byte Reload
	addl	$1, %edi
	callq	even@PLT
	testb	$1, %al
	jne	.LBB0_5
.LBB0_4:                                # %true
	movb	$1, %al
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.LBB0_5:                                # %false
	.cfi_def_cfa_offset 16
	xorl	%eax, %eax
                                        # kill: def $al killed $al killed $eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	even, .Lfunc_end0-even
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
	movl	$-11, %edi
	callq	even@PLT
                                        # kill: def $cl killed $al
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
