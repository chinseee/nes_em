	.file	"cpu_debug.cpp"
# GNU C++20 (Ubuntu 13.3.0-6ubuntu2~24.04.1) version 13.3.0 (x86_64-linux-gnu)
#	compiled by GNU C version 13.3.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.26-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -O3 -O3 -std=c++20 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
#APP
	.globl _ZSt21ios_base_library_initv
#NO_APP
	.section	.text._ZNKSt5ctypeIcE8do_widenEc,"axG",@progbits,_ZNKSt5ctypeIcE8do_widenEc,comdat
	.align 2
	.p2align 4
	.weak	_ZNKSt5ctypeIcE8do_widenEc
	.type	_ZNKSt5ctypeIcE8do_widenEc, @function
_ZNKSt5ctypeIcE8do_widenEc:
.LFB2391:
	.cfi_startproc
	endbr64	
# /usr/include/c++/13/bits/locale_facets.h:1092:       do_widen(char __c) const
	movl	%esi, %eax	# tmp87, __c
# /usr/include/c++/13/bits/locale_facets.h:1093:       { return __c; }
	ret	
	.cfi_endproc
.LFE2391:
	.size	_ZNKSt5ctypeIcE8do_widenEc, .-_ZNKSt5ctypeIcE8do_widenEc
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	" A:"
.LC1:
	.string	" X:"
.LC2:
	.string	" Y:"
.LC3:
	.string	" P:"
.LC4:
	.string	" SP:"
.LC5:
	.string	" CYC:"
.LC6:
	.string	"\n"
	.text
	.align 2
	.p2align 4
	.globl	_ZN6nes_em8DebugCPU9exec_instEv
	.type	_ZN6nes_em8DebugCPU9exec_instEv, @function
_ZN6nes_em8DebugCPU9exec_instEv:
.LFB3447:
	.cfi_startproc
	endbr64	
	pushq	%r15	#
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14	#
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13	#
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12	#
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp	#
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx	#
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
# /usr/include/c++/13/ostream:134: 	__pf(*this);
	leaq	_ZSt4cout(%rip), %rbx	#, tmp215
# /home/chenxi/source/repos/nes/src/cpu_debug.cpp:8: void DebugCPU::exec_inst() {
	subq	$40, %rsp	#,
	.cfi_def_cfa_offset 96
# /home/chenxi/source/repos/nes/src/cpu_debug.cpp:9:     uint32_t prev_a = a, prev_x = x, prev_y = y, prev_p = p.to_ulong(), prev_sp = sp;
	movzbl	5(%rdi), %eax	# this_8(D)->D.76412.sp, _4
# /home/chenxi/source/repos/nes/src/cpu_debug.cpp:9:     uint32_t prev_a = a, prev_x = x, prev_y = y, prev_p = p.to_ulong(), prev_sp = sp;
	movzbl	2(%rdi), %r14d	# this_8(D)->D.76412.a, _1
# /home/chenxi/source/repos/nes/src/cpu_debug.cpp:9:     uint32_t prev_a = a, prev_x = x, prev_y = y, prev_p = p.to_ulong(), prev_sp = sp;
	movzbl	3(%rdi), %r13d	# this_8(D)->D.76412.x, _2
# /home/chenxi/source/repos/nes/src/cpu_debug.cpp:9:     uint32_t prev_a = a, prev_x = x, prev_y = y, prev_p = p.to_ulong(), prev_sp = sp;
	movzbl	4(%rdi), %r12d	# this_8(D)->D.76412.y, _3
# /usr/include/c++/13/bitset:501:       { return _M_w; }
	movq	8(%rdi), %rbp	# MEM[(const struct _Base_bitset *)this_8(D) + 8B]._M_w, _50
# /home/chenxi/source/repos/nes/src/cpu_debug.cpp:10:     uint32_t prev_pc = pc;
	movzwl	(%rdi), %r15d	# this_8(D)->D.76412.pc, _5
# /home/chenxi/source/repos/nes/src/cpu_debug.cpp:9:     uint32_t prev_a = a, prev_x = x, prev_y = y, prev_p = p.to_ulong(), prev_sp = sp;
	movb	%al, 7(%rsp)	# _4, %sfp
# /home/chenxi/source/repos/nes/src/cpu_debug.cpp:11:     size_t prev_cycles = cycles;
	movq	65552(%rdi), %rax	# this_8(D)->D.76412.cycles, prev_cycles
	movq	%rax, 8(%rsp)	# prev_cycles, %sfp
# /home/chenxi/source/repos/nes/src/cpu_debug.cpp:13:     CPU::exec_inst();
	call	_ZN6nes_em3CPU9exec_instEv@PLT	#
# /usr/include/c++/13/ostream:134: 	__pf(*this);
	movq	_ZSt4cout(%rip), %rax	# cout._vptr.basic_ostream, cout._vptr.basic_ostream
	movq	-24(%rax), %rdx	# MEM[(long int *)_46 + -24B], _49
	addq	%rbx, %rdx	# tmp215, _49
# /usr/include/c++/13/bits/ios_base.h:84:   { return _Ios_Fmtflags(static_cast<int>(__a) & static_cast<int>(__b)); }
	movl	24(%rdx), %eax	# _49->_M_flags, tmp148
	andl	$-587, %eax	#, tmp148
	orl	$16392, %eax	#, tmp150
# /usr/include/c++/13/bits/basic_ios.h:372: 	if (!_M_fill_init)
	cmpb	$0, 225(%rdx)	#, MEM[(const struct basic_ios *)_49]._M_fill_init
# /usr/include/c++/13/bits/ios_base.h:84:   { return _Ios_Fmtflags(static_cast<int>(__a) & static_cast<int>(__b)); }
	movl	%eax, 24(%rdx)	# tmp150, MEM[(_Ios_Fmtflags &)_49 + 24]
	movq	%rdx, %rax	# _49, prephitmp_103
# /usr/include/c++/13/bits/basic_ios.h:372: 	if (!_M_fill_init)
	je	.L11	#,
.L4:
# /usr/include/c++/13/bits/basic_ios.h:393: 	_M_fill = __ch;
	movb	$48, 224(%rdx)	#, MEM[(struct basic_ios *)_49]._M_fill
# /usr/include/c++/13/ostream:198: 	return _M_insert(static_cast<unsigned long>(__n));
	movzwl	%r15w, %esi	# _5, _5
	movq	%rbx, %rdi	# tmp215,
# /usr/include/c++/13/bits/ios_base.h:767:       _M_width = __wide;
	movq	$4, 16(%rax)	#, prephitmp_123->_M_width
# /usr/include/c++/13/ostream:198: 	return _M_insert(static_cast<unsigned long>(__n));
	call	_ZNSo9_M_insertImEERSoT_@PLT	#
# /usr/include/c++/13/ostream:667: 	__ostream_insert(__out, __s,
	movl	$3, %edx	#,
	movq	%rbx, %rdi	# tmp215,
	leaq	.LC0(%rip), %rsi	#, tmp163
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT	#
# /usr/include/c++/13/iomanip:242:       __os.width(__f._M_n);
	movq	_ZSt4cout(%rip), %rax	# cout._vptr.basic_ostream, cout._vptr.basic_ostream
# /usr/include/c++/13/ostream:198: 	return _M_insert(static_cast<unsigned long>(__n));
	movzbl	%r14b, %esi	# _1, _1
	movq	%rbx, %rdi	# tmp215,
# /usr/include/c++/13/bits/ios_base.h:767:       _M_width = __wide;
	movq	-24(%rax), %rax	# MEM[(long int *)_37 + -24B], MEM[(long int *)_37 + -24B]
	movq	$2, 16(%rbx,%rax)	#, _40->_M_width
# /usr/include/c++/13/ostream:198: 	return _M_insert(static_cast<unsigned long>(__n));
	call	_ZNSo9_M_insertImEERSoT_@PLT	#
# /usr/include/c++/13/ostream:667: 	__ostream_insert(__out, __s,
	movl	$3, %edx	#,
	movq	%rbx, %rdi	# tmp215,
	leaq	.LC1(%rip), %rsi	#, tmp171
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT	#
# /usr/include/c++/13/iomanip:242:       __os.width(__f._M_n);
	movq	_ZSt4cout(%rip), %rax	# cout._vptr.basic_ostream, cout._vptr.basic_ostream
# /usr/include/c++/13/ostream:198: 	return _M_insert(static_cast<unsigned long>(__n));
	movzbl	%r13b, %esi	# _2, _2
	movq	%rbx, %rdi	# tmp215,
# /usr/include/c++/13/bits/ios_base.h:767:       _M_width = __wide;
	movq	-24(%rax), %rax	# MEM[(long int *)_32 + -24B], MEM[(long int *)_32 + -24B]
	movq	$2, 16(%rbx,%rax)	#, _35->_M_width
# /usr/include/c++/13/ostream:198: 	return _M_insert(static_cast<unsigned long>(__n));
	call	_ZNSo9_M_insertImEERSoT_@PLT	#
# /usr/include/c++/13/ostream:667: 	__ostream_insert(__out, __s,
	movl	$3, %edx	#,
	movq	%rbx, %rdi	# tmp215,
	leaq	.LC2(%rip), %rsi	#, tmp179
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT	#
# /usr/include/c++/13/iomanip:242:       __os.width(__f._M_n);
	movq	_ZSt4cout(%rip), %rax	# cout._vptr.basic_ostream, cout._vptr.basic_ostream
# /usr/include/c++/13/ostream:198: 	return _M_insert(static_cast<unsigned long>(__n));
	movzbl	%r12b, %esi	# _3, _3
	movq	%rbx, %rdi	# tmp215,
# /usr/include/c++/13/bits/ios_base.h:767:       _M_width = __wide;
	movq	-24(%rax), %rax	# MEM[(long int *)_27 + -24B], MEM[(long int *)_27 + -24B]
	movq	$2, 16(%rbx,%rax)	#, _30->_M_width
# /usr/include/c++/13/ostream:198: 	return _M_insert(static_cast<unsigned long>(__n));
	call	_ZNSo9_M_insertImEERSoT_@PLT	#
# /usr/include/c++/13/ostream:667: 	__ostream_insert(__out, __s,
	movl	$3, %edx	#,
	movq	%rbx, %rdi	# tmp215,
	leaq	.LC3(%rip), %rsi	#, tmp187
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT	#
# /usr/include/c++/13/iomanip:242:       __os.width(__f._M_n);
	movq	_ZSt4cout(%rip), %rax	# cout._vptr.basic_ostream, cout._vptr.basic_ostream
# /usr/include/c++/13/ostream:198: 	return _M_insert(static_cast<unsigned long>(__n));
	movl	%ebp, %esi	# _50, _50
	movq	%rbx, %rdi	# tmp215,
# /usr/include/c++/13/bits/ios_base.h:767:       _M_width = __wide;
	movq	-24(%rax), %rax	# MEM[(long int *)_22 + -24B], MEM[(long int *)_22 + -24B]
	movq	$2, 16(%rbx,%rax)	#, _25->_M_width
# /usr/include/c++/13/ostream:198: 	return _M_insert(static_cast<unsigned long>(__n));
	call	_ZNSo9_M_insertImEERSoT_@PLT	#
# /usr/include/c++/13/ostream:667: 	__ostream_insert(__out, __s,
	movl	$4, %edx	#,
	movq	%rbx, %rdi	# tmp215,
	leaq	.LC4(%rip), %rsi	#, tmp195
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT	#
# /usr/include/c++/13/iomanip:242:       __os.width(__f._M_n);
	movq	_ZSt4cout(%rip), %rax	# cout._vptr.basic_ostream, cout._vptr.basic_ostream
# /usr/include/c++/13/ostream:198: 	return _M_insert(static_cast<unsigned long>(__n));
	movzbl	7(%rsp), %esi	# %sfp, _4
	movq	%rbx, %rdi	# tmp215,
# /usr/include/c++/13/bits/ios_base.h:767:       _M_width = __wide;
	movq	-24(%rax), %rax	# MEM[(long int *)_18 + -24B], MEM[(long int *)_18 + -24B]
	movq	$2, 16(%rbx,%rax)	#, _21->_M_width
# /usr/include/c++/13/ostream:198: 	return _M_insert(static_cast<unsigned long>(__n));
	call	_ZNSo9_M_insertImEERSoT_@PLT	#
# /usr/include/c++/13/ostream:134: 	__pf(*this);
	movq	_ZSt4cout(%rip), %rax	# cout._vptr.basic_ostream, cout._vptr.basic_ostream
# /usr/include/c++/13/ostream:667: 	__ostream_insert(__out, __s,
	movq	%rbx, %rdi	# tmp215,
	leaq	.LC5(%rip), %rsi	#, tmp210
# /usr/include/c++/13/ostream:134: 	__pf(*this);
	movq	-24(%rax), %rdx	# MEM[(long int *)_13 + -24B], _16
	addq	%rbx, %rdx	# tmp215, _16
# /usr/include/c++/13/bits/ios_base.h:84:   { return _Ios_Fmtflags(static_cast<int>(__a) & static_cast<int>(__b)); }
	movl	24(%rdx), %eax	# _16->_M_flags, tmp207
	andl	$-75, %eax	#, tmp207
# /usr/include/c++/13/bits/ios_base.h:88:   { return _Ios_Fmtflags(static_cast<int>(__a) | static_cast<int>(__b)); }
	orl	$2, %eax	#, tmp209
	movl	%eax, 24(%rdx)	# tmp209, MEM[(_Ios_Fmtflags &)_16 + 24]
# /usr/include/c++/13/ostream:667: 	__ostream_insert(__out, __s,
	movl	$5, %edx	#,
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT	#
# /usr/include/c++/13/ostream:173:       { return _M_insert(__n); }
	movq	8(%rsp), %rsi	# %sfp,
	movq	%rbx, %rdi	# tmp215,
	call	_ZNSo9_M_insertImEERSoT_@PLT	#
# /home/chenxi/source/repos/nes/src/cpu_debug.cpp:24: };
	addq	$40, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 56
# /usr/include/c++/13/ostream:667: 	__ostream_insert(__out, __s,
	movq	%rbx, %rdi	# tmp215,
	movl	$1, %edx	#,
# /home/chenxi/source/repos/nes/src/cpu_debug.cpp:24: };
	popq	%rbx	#
	.cfi_def_cfa_offset 48
# /usr/include/c++/13/ostream:667: 	__ostream_insert(__out, __s,
	leaq	.LC6(%rip), %rsi	#, tmp213
# /home/chenxi/source/repos/nes/src/cpu_debug.cpp:24: };
	popq	%rbp	#
	.cfi_def_cfa_offset 40
	popq	%r12	#
	.cfi_def_cfa_offset 32
	popq	%r13	#
	.cfi_def_cfa_offset 24
	popq	%r14	#
	.cfi_def_cfa_offset 16
	popq	%r15	#
	.cfi_def_cfa_offset 8
# /usr/include/c++/13/ostream:667: 	__ostream_insert(__out, __s,
	jmp	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT	#
	.p2align 4,,10
	.p2align 3
.L11:
	.cfi_restore_state
	movq	240(%rdx), %rdi	# MEM[(const struct __ctype_type * *)_49 + 240B], _87
# /usr/include/c++/13/bits/basic_ios.h:49:       if (!__f)
	testq	%rdi, %rdi	# _87
	je	.L12	#,
# /usr/include/c++/13/bits/locale_facets.h:882: 	if (_M_widen_ok)
	cmpb	$0, 56(%rdi)	#, MEM[(const struct ctype *)_87]._M_widen_ok
	je	.L13	#,
.L6:
# /usr/include/c++/13/bits/basic_ios.h:375: 	    _M_fill_init = true;
	movb	$1, 225(%rdx)	#, MEM[(const struct basic_ios *)_49]._M_fill_init
	jmp	.L4	#
	.p2align 4,,10
	.p2align 3
.L13:
	movq	%rdx, 24(%rsp)	# _49, %sfp
# /usr/include/c++/13/bits/locale_facets.h:884: 	this->_M_widen_init();
	movq	%rdi, 16(%rsp)	# _87, %sfp
	call	_ZNKSt5ctypeIcE13_M_widen_initEv@PLT	#
# /usr/include/c++/13/bits/locale_facets.h:885: 	return this->do_widen(__c);
	movq	16(%rsp), %rdi	# %sfp, _87
	movq	24(%rsp), %rdx	# %sfp, _49
	leaq	_ZNKSt5ctypeIcE8do_widenEc(%rip), %rcx	#, tmp152
	movq	(%rdi), %rax	# MEM[(const struct ctype *)_87].D.55920._vptr.facet, MEM[(const struct ctype *)_87].D.55920._vptr.facet
	movq	48(%rax), %rax	# MEM[(int (*) () *)_104 + 48B], _105
	cmpq	%rcx, %rax	# tmp152, _105
	jne	.L7	#,
# /usr/include/c++/13/iomanip:242:       __os.width(__f._M_n);
	movq	_ZSt4cout(%rip), %rax	# cout._vptr.basic_ostream, cout._vptr.basic_ostream
	movq	-24(%rax), %rcx	# MEM[(long int *)pretmp_99 + -24B], prephitmp_103
# /usr/include/c++/13/bits/basic_ios.h:375: 	    _M_fill_init = true;
	movb	$1, 225(%rdx)	#, MEM[(const struct basic_ios *)_49]._M_fill_init
# /usr/include/c++/13/iomanip:242:       __os.width(__f._M_n);
	addq	%rbx, %rcx	# tmp215, prephitmp_103
	movq	%rcx, %rax	# prephitmp_103, prephitmp_103
	jmp	.L4	#
	.p2align 4,,10
	.p2align 3
.L7:
	movq	%rdx, 16(%rsp)	# _49, %sfp
# /usr/include/c++/13/bits/locale_facets.h:885: 	return this->do_widen(__c);
	movl	$32, %esi	#,
	call	*%rax	# _105
# /usr/include/c++/13/iomanip:242:       __os.width(__f._M_n);
	movq	_ZSt4cout(%rip), %rax	# cout._vptr.basic_ostream, cout._vptr.basic_ostream
	movq	16(%rsp), %rdx	# %sfp, _49
	movq	-24(%rax), %rcx	# MEM[(long int *)pretmp_116 + -24B], prephitmp_103
	addq	%rbx, %rcx	# tmp215, prephitmp_103
	movq	%rcx, %rax	# prephitmp_103, prephitmp_103
	jmp	.L6	#
.L12:
# /usr/include/c++/13/bits/basic_ios.h:50: 	__throw_bad_cast();
	call	_ZSt16__throw_bad_castv@PLT	#
	.cfi_endproc
.LFE3447:
	.size	_ZN6nes_em8DebugCPU9exec_instEv, .-_ZN6nes_em8DebugCPU9exec_instEv
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
