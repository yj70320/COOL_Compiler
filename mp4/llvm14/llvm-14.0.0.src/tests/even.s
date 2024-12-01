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
	testl	%edi, %edi
	je	.LBB0_5
# %bb.1:                                # %nonzero
	jle	.LBB0_3
# %bb.2:                                # %positive
	decl	%edi
	jmp	.LBB0_4
.LBB0_3:                                # %negative
	incl	%edi
.LBB0_4:                                # %negative
	callq	even@PLT
	testb	$1, %al
	je	.LBB0_5
# %bb.6:                                # %false
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.LBB0_5:                                # %true
	.cfi_def_cfa_offset 16
	movb	$1, %al
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
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
