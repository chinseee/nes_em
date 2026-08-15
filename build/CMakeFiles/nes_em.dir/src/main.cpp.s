	.file	"main.cpp"
# GNU C++20 (Ubuntu 13.3.0-6ubuntu2~24.04.1) version 13.3.0 (x86_64-linux-gnu)
#	compiled by GNU C version 13.3.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.26-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -O3 -std=c++20 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
#APP
	.globl _ZSt21ios_base_library_initv
#NO_APP
	.section	.text.unlikely,"ax",@progbits
.LCOLDB0:
	.section	.text.startup,"ax",@progbits
.LHOTB0:
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB4717:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA4717
	endbr64	
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx	#
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$88, %rsp	#,
	.cfi_def_cfa_offset 112
# /home/chenxi/source/repos/nes/src/main.cpp:4: int main(void) {
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp95
	movq	%rax, 72(%rsp)	# tmp95, D.120799
	xorl	%eax, %eax	# tmp95
# /home/chenxi/source/repos/nes/src/main.cpp:5:     nes_client::Client c;
	movq	%rsp, %rbx	#, tmp93
	movq	%rbx, %rdi	# tmp93,
.LEHB0:
	call	_ZN10nes_client6ClientC1Ev@PLT	#
.LEHE0:
# /home/chenxi/source/repos/nes/src/main.cpp:6:     c.run();
	movq	%rbx, %rdi	# tmp93,
.LEHB1:
	call	_ZN10nes_client6Client3runEv@PLT	#
.LEHE1:
# /home/chenxi/source/repos/nes/src/main.cpp:8: }
	movq	%rbx, %rdi	# tmp93,
	call	_ZN10nes_client6ClientD1Ev@PLT	#
	movq	72(%rsp), %rax	# D.120799, tmp96
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp96
	jne	.L9	#,
	addq	$88, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax	#
	popq	%rbx	#
	.cfi_def_cfa_offset 16
	popq	%rbp	#
	.cfi_def_cfa_offset 8
	ret	
.L9:
	.cfi_restore_state
	call	__stack_chk_fail@PLT	#
.L5:
	endbr64	
	movq	%rax, %rbp	# tmp94, tmp90
	jmp	.L2	#
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA4717:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4717-.LLSDACSB4717
.LLSDACSB4717:
	.uleb128 .LEHB0-.LFB4717
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB4717
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L5-.LFB4717
	.uleb128 0
.LLSDACSE4717:
	.section	.text.startup
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC4717
	.type	main.cold, @function
main.cold:
.LFSB4717:
.L2:
	.cfi_def_cfa_offset 112
	.cfi_offset 3, -24
	.cfi_offset 6, -16
	movq	%rbx, %rdi	# tmp93,
	call	_ZN10nes_client6ClientD1Ev@PLT	#
	movq	72(%rsp), %rax	# D.120799, tmp97
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp97
	jne	.L10	#,
	movq	%rbp, %rdi	# tmp90,
.LEHB2:
	call	_Unwind_Resume@PLT	#
.LEHE2:
.L10:
	call	__stack_chk_fail@PLT	#
	.cfi_endproc
.LFE4717:
	.section	.gcc_except_table
.LLSDAC4717:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC4717-.LLSDACSBC4717
.LLSDACSBC4717:
	.uleb128 .LEHB2-.LCOLDB0
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSEC4717:
	.section	.text.unlikely
	.section	.text.startup
	.size	main, .-main
	.section	.text.unlikely
	.size	main.cold, .-main.cold
.LCOLDE0:
	.section	.text.startup
.LHOTE0:
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
