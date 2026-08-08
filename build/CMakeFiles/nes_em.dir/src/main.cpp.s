	.file	"main.cpp"
# GNU C++20 (Ubuntu 13.3.0-6ubuntu2~24.04.1) version 13.3.0 (x86_64-linux-gnu)
#	compiled by GNU C version 13.3.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.26-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -O3 -O3 -std=c++20 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
#APP
	.globl _ZSt21ios_base_library_initv
#NO_APP
	.section	.text._ZN6nes_em7NesFileD2Ev,"axG",@progbits,_ZN6nes_em7NesFileD5Ev,comdat
	.align 2
	.p2align 4
	.weak	_ZN6nes_em7NesFileD2Ev
	.type	_ZN6nes_em7NesFileD2Ev, @function
_ZN6nes_em7NesFileD2Ev:
.LFB3167:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA3167
	endbr64	
	pushq	%rbx	#
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
# /home/chenxi/source/repos/nes/include/nes_em/nes_file.h:16: struct NesFile {
	movq	%rdi, %rbx	# tmp122, this
# /usr/include/c++/13/bits/stl_vector.h:370: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	544(%rdi), %rdi	# MEM[(struct _Vector_base *)this_2(D) + 544B]._M_impl.D.74853._M_start, _12
# /usr/include/c++/13/bits/stl_vector.h:389: 	if (__p)
	testq	%rdi, %rdi	# _12
	je	.L2	#,
# /usr/include/c++/13/bits/stl_vector.h:370: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	560(%rbx), %rsi	# MEM[(struct _Vector_base *)this_2(D) + 544B]._M_impl.D.74853._M_end_of_storage, tmp102
	subq	%rdi, %rsi	# _12, tmp102
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	call	_ZdlPvm@PLT	#
.L2:
# /usr/include/c++/13/bits/stl_vector.h:370: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	520(%rbx), %rdi	# MEM[(struct _Vector_base *)this_2(D) + 520B]._M_impl.D.74853._M_start, _7
# /usr/include/c++/13/bits/stl_vector.h:389: 	if (__p)
	testq	%rdi, %rdi	# _7
	je	.L3	#,
# /usr/include/c++/13/bits/stl_vector.h:370: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	536(%rbx), %rsi	# MEM[(struct _Vector_base *)this_2(D) + 520B]._M_impl.D.74853._M_end_of_storage, tmp104
	subq	%rdi, %rsi	# _7, tmp104
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	call	_ZdlPvm@PLT	#
.L3:
# /usr/include/c++/13/fstream:607:       { }
	leaq	24+_ZTVSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rax	#, tmp127
	leaq	16(%rbx), %rdi	#, tmp109
	movq	%rax, (%rbx)	# tmp127, MEM[(struct basic_ifstream *)this_2(D)].D.67471._vptr.basic_istream
	addq	$40, %rax	#, tmp128
	movq	%rax, 256(%rbx)	# tmp128, MEM[(struct basic_ios *)this_2(D) + 256B].D.57646._vptr.ios_base
# /usr/include/c++/13/fstream:259:       }
	leaq	16+_ZTVSt13basic_filebufIcSt11char_traitsIcEE(%rip), %rax	#, tmp129
	movq	%rax, 16(%rbx)	# tmp129, MEM[(struct basic_filebuf *)this_2(D) + 16B].D.67279._vptr.basic_streambuf
.LEHB0:
# /usr/include/c++/13/fstream:256: 	  { this->close(); }
	call	_ZNSt13basic_filebufIcSt11char_traitsIcEE5closeEv@PLT	#
.LEHE0:
.L5:
# /usr/include/c++/13/fstream:259:       }
	leaq	120(%rbx), %rdi	#, tmp111
	call	_ZNSt12__basic_fileIcED1Ev@PLT	#
# /usr/include/c++/13/streambuf:205:       { }
	leaq	16+_ZTVSt15basic_streambufIcSt11char_traitsIcEE(%rip), %rax	#, tmp130
	leaq	72(%rbx), %rdi	#, tmp114
	movq	%rax, 16(%rbx)	# tmp130, MEM[(struct basic_streambuf *)this_2(D) + 16B]._vptr.basic_streambuf
	call	_ZNSt6localeD1Ev@PLT	#
# /usr/include/c++/13/istream:106:       { _M_gcount = streamsize(0); }
	movq	8+_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rax	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 8B], _24
	movq	16+_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rdx	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 16B], MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 16B]
# /usr/include/c++/13/bits/basic_ios.h:282:       ~basic_ios() { }
	leaq	256(%rbx), %rdi	#, tmp120
# /usr/include/c++/13/istream:106:       { _M_gcount = streamsize(0); }
	movq	%rax, (%rbx)	# _24, MEM[(struct basic_istream *)this_2(D)]._vptr.basic_istream
	movq	-24(%rax), %rax	# MEM[(long int *)_24 + -24B], MEM[(long int *)_24 + -24B]
	movq	%rdx, (%rbx,%rax)	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 16B], MEM[(struct basic_ios *)_27].D.57646._vptr.ios_base
# /usr/include/c++/13/bits/basic_ios.h:282:       ~basic_ios() { }
	leaq	16+_ZTVSt9basic_iosIcSt11char_traitsIcEE(%rip), %rax	#, tmp131
# /usr/include/c++/13/istream:106:       { _M_gcount = streamsize(0); }
	movq	$0, 8(%rbx)	#, MEM[(struct basic_istream *)this_2(D)]._M_gcount
# /usr/include/c++/13/bits/basic_ios.h:282:       ~basic_ios() { }
	movq	%rax, 256(%rbx)	# tmp131, MEM[(struct basic_ios *)this_2(D) + 256B].D.57646._vptr.ios_base
# /home/chenxi/source/repos/nes/include/nes_em/nes_file.h:16: struct NesFile {
	popq	%rbx	#
	.cfi_remember_state
	.cfi_def_cfa_offset 8
# /usr/include/c++/13/bits/basic_ios.h:282:       ~basic_ios() { }
	jmp	_ZNSt8ios_baseD2Ev@PLT	#
.L6:
	.cfi_restore_state
	endbr64	
# /usr/include/c++/13/fstream:257: 	__catch(...)
	movq	%rax, %rdi	# tmp123, tmp121
.L4:
	call	__cxa_begin_catch@PLT	#
# /usr/include/c++/13/fstream:257: 	__catch(...)
	call	__cxa_end_catch@PLT	#
	jmp	.L5	#
	.cfi_endproc
.LFE3167:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table._ZN6nes_em7NesFileD2Ev,"aG",@progbits,_ZN6nes_em7NesFileD5Ev,comdat
	.align 4
.LLSDA3167:
	.byte	0xff
	.byte	0x9b
	.uleb128 .LLSDATT3167-.LLSDATTD3167
.LLSDATTD3167:
	.byte	0x1
	.uleb128 .LLSDACSE3167-.LLSDACSB3167
.LLSDACSB3167:
	.uleb128 .LEHB0-.LFB3167
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L6-.LFB3167
	.uleb128 0x1
.LLSDACSE3167:
	.byte	0x1
	.byte	0
	.align 4
	.long	0

.LLSDATT3167:
	.section	.text._ZN6nes_em7NesFileD2Ev,"axG",@progbits,_ZN6nes_em7NesFileD5Ev,comdat
	.size	_ZN6nes_em7NesFileD2Ev, .-_ZN6nes_em7NesFileD2Ev
	.weak	_ZN6nes_em7NesFileD1Ev
	.set	_ZN6nes_em7NesFileD1Ev,_ZN6nes_em7NesFileD2Ev
	.section	.text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv,"axG",@progbits,_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv
	.type	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv, @function
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv:
.LFB3530:
	.cfi_startproc
	endbr64	
# /usr/include/c++/13/bits/basic_string.h:223:       { return _M_dataplus._M_p; }
	movq	(%rdi), %rax	# MEM[(const struct basic_string *)this_4(D)]._M_dataplus._M_p, _5
# /usr/include/c++/13/bits/basic_string.h:241: 	return std::pointer_traits<const_pointer>::pointer_to(*_M_local_buf);
	leaq	16(%rdi), %rdx	#, tmp87
# /usr/include/c++/13/bits/basic_string.h:264: 	if (_M_data() == _M_local_data())
	cmpq	%rdx, %rax	# tmp87, _5
	je	.L15	#,
# /usr/include/c++/13/bits/basic_string.h:289:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	16(%rdi), %rsi	# this_4(D)->D.35691._M_allocated_capacity, this_4(D)->D.35691._M_allocated_capacity
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	movq	%rax, %rdi	# _5,
# /usr/include/c++/13/bits/basic_string.h:289:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	addq	$1, %rsi	#, tmp88
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	jmp	_ZdlPvm@PLT	#
	.p2align 4,,10
	.p2align 3
.L15:
# /usr/include/c++/13/bits/basic_string.h:284:       }
	ret	
	.cfi_endproc
.LFE3530:
	.size	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv, .-_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv
	.section	.text.unlikely,"ax",@progbits
.LCOLDB0:
	.section	.text.startup,"ax",@progbits
.LHOTB0:
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB3162:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA3162
	endbr64	
	pushq	%r12	#
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	pushq	%rbp	#
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	pushq	%rbx	#
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	leaq	-65536(%rsp), %r11	#,
	.cfi_def_cfa 11, 65568
.LPSRL0:
	subq	$4096, %rsp	#,
	orq	$0, (%rsp)	#,
	cmpq	%r11, %rsp	#,
	jne	.LPSRL0
	.cfi_def_cfa_register 7
	subq	$656, %rsp	#,
	.cfi_def_cfa_offset 66224
# /home/chenxi/source/repos/nes/include/nes_em/cpu_debug.h:5: class DebugCPU: public CPU {
	leaq	624(%rsp), %rbp	#, tmp110
# /usr/include/c++/13/bits/basic_string.h:189: 	: allocator_type(__a), _M_p(__dat) { }
	movq	%rsp, %rbx	#, tmp109
# /home/chenxi/source/repos/nes/src/main.cpp:6:     nes_em::NesFile file("nestest.nes");
	leaq	32(%rsp), %r12	#, tmp111
# /home/chenxi/source/repos/nes/src/main.cpp:4: int main(void) {
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp114
	movq	%rax, 66184(%rsp)	# tmp114, D.83991
	xorl	%eax, %eax	# tmp114
# /home/chenxi/source/repos/nes/include/nes_em/cpu_debug.h:5: class DebugCPU: public CPU {
	movq	%rbp, %rdi	# tmp110,
.LEHB1:
	call	_ZN6nes_em3CPUC2Ev@PLT	#
.LEHE1:
# /usr/include/c++/13/bits/basic_string.h:189: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	16(%rsp), %rax	#, tmp89
# /home/chenxi/source/repos/nes/src/main.cpp:6:     nes_em::NesFile file("nestest.nes");
	movq	%rbx, %rsi	# tmp109,
	movq	%r12, %rdi	# tmp111,
# /usr/include/c++/13/bits/basic_string.h:189: 	: allocator_type(__a), _M_p(__dat) { }
	movq	%rax, (%rsp)	# tmp89, MEM[(struct _Alloc_hider *)&D.76564]._M_p
# /usr/include/c++/13/bits/char_traits.h:435: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movabsq	$3347427302625731950, %rax	#, tmp119
	movq	%rax, 16(%rsp)	# tmp119, MEM <char[1:11]> [(void *)&D.76564 + 16B]
# /usr/include/c++/13/bits/basic_string.h:218:       { _M_string_length = __length; }
	movq	$11, 8(%rsp)	#, D.76564._M_string_length
# /usr/include/c++/13/bits/char_traits.h:435: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movl	$1936027182, 23(%rsp)	#, MEM <char[1:11]> [(void *)&D.76564 + 16B]
# /usr/include/c++/13/bits/char_traits.h:358: 	__c1 = __c2;
	movb	$0, 27(%rsp)	#, MEM[(char_type &)&D.76564 + 27]
.LEHB2:
# /home/chenxi/source/repos/nes/src/main.cpp:6:     nes_em::NesFile file("nestest.nes");
	call	_ZN6nes_em7NesFileC1ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@PLT	#
.LEHE2:
# /usr/include/c++/13/bits/basic_string.h:804:       { _M_dispose(); }
	movq	%rbx, %rdi	# tmp109,
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv	#
# /home/chenxi/source/repos/nes/src/main.cpp:7:     cpu.load(file);
	movq	%r12, %rsi	# tmp111,
	movq	%rbp, %rdi	# tmp110,
.LEHB3:
	call	_ZN6nes_em3CPU4loadERKNS_7NesFileE@PLT	#
# /home/chenxi/source/repos/nes/src/main.cpp:8:     cpu.reset();
	movq	%rbp, %rdi	# tmp110,
	call	_ZN6nes_em3CPU5resetEv@PLT	#
# /home/chenxi/source/repos/nes/src/main.cpp:10:     cpu.pc = 0xc000;
	movl	$-16384, %eax	#,
	movl	$8991, %ebx	#, ivtmp_1
	movw	%ax, 624(%rsp)	#, cpu.D.76420.pc
	.p2align 4,,10
	.p2align 3
.L18:
# /home/chenxi/source/repos/nes/src/main.cpp:12:         cpu.exec_inst();
	movq	%rbp, %rdi	# tmp110,
	call	_ZN6nes_em8DebugCPU9exec_instEv@PLT	#
.LEHE3:
# /home/chenxi/source/repos/nes/src/main.cpp:11:     for (size_t i = 0; i < 8991; ++i)
	subq	$1, %rbx	#, ivtmp_1
	jne	.L18	#,
# /home/chenxi/source/repos/nes/src/main.cpp:13: }
	movq	%r12, %rdi	# tmp111,
	call	_ZN6nes_em7NesFileD1Ev	#
	movq	66184(%rsp), %rax	# D.83991, tmp115
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp115
	jne	.L29	#,
	addq	$66192, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 32
	xorl	%eax, %eax	#
	popq	%rbx	#
	.cfi_def_cfa_offset 24
	popq	%rbp	#
	.cfi_def_cfa_offset 16
	popq	%r12	#
	.cfi_def_cfa_offset 8
	ret	
.L29:
	.cfi_restore_state
	call	__stack_chk_fail@PLT	#
.L25:
	endbr64	
	movq	%rax, %rbx	# tmp113, tmp106
	jmp	.L21	#
.L24:
	endbr64	
# /usr/include/c++/13/bits/basic_string.h:804:       { _M_dispose(); }
	movq	%rax, %rbp	# tmp112, tmp103
	jmp	.L19	#
	.section	.gcc_except_table,"a",@progbits
.LLSDA3162:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE3162-.LLSDACSB3162
.LLSDACSB3162:
	.uleb128 .LEHB1-.LFB3162
	.uleb128 .LEHE1-.LEHB1
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB2-.LFB3162
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L24-.LFB3162
	.uleb128 0
	.uleb128 .LEHB3-.LFB3162
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L25-.LFB3162
	.uleb128 0
.LLSDACSE3162:
	.section	.text.startup
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC3162
	.type	main.cold, @function
main.cold:
.LFSB3162:
.L21:
	.cfi_def_cfa_offset 66224
	.cfi_offset 3, -32
	.cfi_offset 6, -24
	.cfi_offset 12, -16
# /home/chenxi/source/repos/nes/src/main.cpp:13: }
	movq	%r12, %rdi	# tmp111,
	call	_ZN6nes_em7NesFileD1Ev	#
	movq	66184(%rsp), %rax	# D.83991, tmp117
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp117
	jne	.L30	#,
	movq	%rbx, %rdi	# tmp106,
.LEHB4:
	call	_Unwind_Resume@PLT	#
.L19:
# /usr/include/c++/13/bits/basic_string.h:804:       { _M_dispose(); }
	movq	%rbx, %rdi	# tmp109,
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv	#
	movq	66184(%rsp), %rax	# D.83991, tmp116
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp116
	jne	.L31	#,
	movq	%rbp, %rdi	# tmp103,
	call	_Unwind_Resume@PLT	#
.LEHE4:
.L30:
	call	__stack_chk_fail@PLT	#
.L31:
	call	__stack_chk_fail@PLT	#
	.cfi_endproc
.LFE3162:
	.section	.gcc_except_table
.LLSDAC3162:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC3162-.LLSDACSBC3162
.LLSDACSBC3162:
	.uleb128 .LEHB4-.LCOLDB0
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
.LLSDACSEC3162:
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
