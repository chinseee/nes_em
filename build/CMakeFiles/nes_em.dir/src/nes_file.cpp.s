	.file	"nes_file.cpp"
# GNU C++20 (Ubuntu 13.3.0-6ubuntu2~24.04.1) version 13.3.0 (x86_64-linux-gnu)
#	compiled by GNU C version 13.3.0, GMP version 6.3.0, MPFR version 4.2.1, MPC version 1.3.1, isl version isl-0.26-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64 -O3 -O3 -std=c++20 -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"basic_string::append"
.LC1:
	.string	"basic_string::_M_create"
	.section	.text.unlikely,"ax",@progbits
	.align 2
	.type	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6appendEPKcm.isra.0, @function
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6appendEPKcm.isra.0:
.LFB4246:
	.cfi_startproc
# /usr/include/c++/13/bits/basic_string.h:399: 	if (this->max_size() - (this->size() - __n1) < __n2)
	movabsq	$9223372036854775807, %rax	#, tmp112
# /usr/include/c++/13/bits/basic_string.h:1455:       append(const _CharT* __s, size_type __n)
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
	subq	$24, %rsp	#,
	.cfi_def_cfa_offset 80
# /usr/include/c++/13/bits/basic_string.h:1072:       { return _M_string_length; }
	movq	8(%rdi), %rbp	# MEM[(const struct basic_string *)this_1(D)]._M_string_length, _2
# /usr/include/c++/13/bits/basic_string.h:399: 	if (this->max_size() - (this->size() - __n1) < __n2)
	subq	%rbp, %rax	# _2, tmp111
	cmpq	%rdx, %rax	# __n, tmp111
	jnb	.L2	#,
# /usr/include/c++/13/bits/basic_string.h:400: 	  __throw_length_error(__N(__s));
	leaq	.LC0(%rip), %rdi	#, tmp113
	call	_ZSt20__throw_length_errorPKc@PLT	#
.L2:
# /usr/include/c++/13/bits/basic_string.h:223:       { return _M_dataplus._M_p; }
	movq	(%rdi), %rcx	# MEM[(const struct basic_string *)this_1(D)]._M_dataplus._M_p, _12
	movq	%rdx, %rbx	# tmp143, __n
# /usr/include/c++/13/bits/basic_string.tcc:416:       const size_type __len = __n + this->size();
	leaq	0(%rbp,%rdx), %r15	#, __len
# /usr/include/c++/13/bits/basic_string.h:241: 	return std::pointer_traits<const_pointer>::pointer_to(*_M_local_buf);
	leaq	16(%rdi), %rdx	#, _13
	movq	%rdi, %r12	# tmp141, this
	movq	%rsi, %r13	# tmp142, __s
# /usr/include/c++/13/bits/basic_string.h:1171: 	return _M_is_local() ? size_type(_S_local_capacity)
	movl	$15, %eax	#, iftmp.4_15
# /usr/include/c++/13/bits/basic_string.h:264: 	if (_M_data() == _M_local_data())
	cmpq	%rdx, %rcx	# _13, _12
	je	.L3	#,
# /usr/include/c++/13/bits/basic_string.h:1171: 	return _M_is_local() ? size_type(_S_local_capacity)
	movq	16(%rdi), %rax	# MEM[(const struct basic_string *)this_1(D)].D.47633._M_allocated_capacity, iftmp.4_15
.L3:
# /usr/include/c++/13/bits/basic_string.tcc:418:       if (__len <= this->capacity())
	cmpq	%r15, %rax	# __len, iftmp.4_15
	jb	.L4	#,
# /usr/include/c++/13/bits/basic_string.tcc:420: 	  if (__n)
	testq	%rbx, %rbx	# __n
	je	.L5	#,
# /usr/include/c++/13/bits/basic_string.tcc:421: 	    this->_S_copy(this->_M_data() + this->size(), __s, __n);
	addq	%rcx, %rbp	# _12, _16
# /usr/include/c++/13/bits/basic_string.h:427: 	if (__n == 1)
	cmpq	$1, %rbx	#, __n
	jne	.L6	#,
# /usr/include/c++/13/bits/char_traits.h:358: 	__c1 = __c2;
	movb	0(%r13), %dl	# MEM[(const char_type &)__s_5(D)], _17
# /usr/include/c++/13/bits/char_traits.h:358: 	__c1 = __c2;
	movb	%dl, 0(%rbp)	# _17, MEM[(char_type &)_16]
# /usr/include/c++/13/bits/char_traits.h:359:       }
	jmp	.L5	#
.L6:
# /usr/include/c++/13/bits/char_traits.h:435: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movq	%rbp, %rdi	# _16, _16
	movq	%r13, %rsi	# __s, __s
	movq	%rbx, %rcx	# __n, __n
	rep movsb
# /usr/include/c++/13/bits/char_traits.h:435: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	jmp	.L5	#
.L4:
# /usr/include/c++/13/bits/basic_string.tcc:143:       if (__capacity > max_size())
	testq	%r15, %r15	# __len
	jns	.L7	#,
# /usr/include/c++/13/bits/basic_string.tcc:144: 	std::__throw_length_error(__N("basic_string::_M_create"));
	leaq	.LC1(%rip), %rdi	#, tmp117
	call	_ZSt20__throw_length_errorPKc@PLT	#
.L7:
# /usr/include/c++/13/bits/basic_string.tcc:149:       if (__capacity > __old_capacity && __capacity < 2 * __old_capacity)
	addq	%rax, %rax	# iftmp.4_15, tmp118
# /usr/include/c++/13/bits/basic_string.tcc:331:       size_type __new_capacity = length() + __len2 - __len1;
	movq	%r15, %r14	# __len, __new_capacity
# /usr/include/c++/13/bits/basic_string.tcc:149:       if (__capacity > __old_capacity && __capacity < 2 * __old_capacity)
	cmpq	%rax, %r15	# tmp118, __len
	jnb	.L8	#,
	movabsq	$9223372036854775807, %r14	#, tmp153
	cmpq	%r14, %rax	# tmp153, tmp118
	cmovbe	%rax, %r14	# tmp118,, __new_capacity
.L8:
# /usr/include/c++/13/bits/new_allocator.h:134: 	if (__builtin_expect(__n > this->_M_max_size(), false))
	movq	%r14, %rdi	# __new_capacity, _49
	incq	%rdi	# _49
	jns	.L9	#,
# /usr/include/c++/13/bits/new_allocator.h:140: 	    std::__throw_bad_alloc();
	call	_ZSt17__throw_bad_allocv@PLT	#
.L9:
	movq	%rdx, 8(%rsp)	# _13, %sfp
# /usr/include/c++/13/bits/new_allocator.h:151: 	return static_cast<_Tp*>(_GLIBCXX_OPERATOR_NEW(__n * sizeof(_Tp)));
	call	_Znwm@PLT	#
# /usr/include/c++/13/bits/basic_string.tcc:334:       if (__pos)
	testq	%rbp, %rbp	# _2
	movq	(%r12), %r8	# MEM[(const struct basic_string *)this_1(D)]._M_dataplus._M_p, _30
	movq	8(%rsp), %rdx	# %sfp, _13
	je	.L10	#,
# /usr/include/c++/13/bits/basic_string.h:427: 	if (__n == 1)
	cmpq	$1, %rbp	#, _2
	jne	.L11	#,
# /usr/include/c++/13/bits/char_traits.h:358: 	__c1 = __c2;
	movb	(%r8), %cl	# MEM[(const char_type &)_30], _31
# /usr/include/c++/13/bits/char_traits.h:358: 	__c1 = __c2;
	movb	%cl, (%rax)	# _31, MEM[(char_type &)_55]
# /usr/include/c++/13/bits/char_traits.h:359:       }
	jmp	.L10	#
.L11:
# /usr/include/c++/13/bits/char_traits.h:435: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movq	%rax, %rdi	# _55, _55
	movq	%r8, %rsi	# _30, _30
	movq	%rbp, %rcx	# _2, _2
	rep movsb
.L10:
# /usr/include/c++/13/bits/basic_string.tcc:336:       if (__s && __len2)
	testq	%r13, %r13	# __s
	je	.L12	#,
	testq	%rbx, %rbx	# __n
	je	.L12	#,
# /usr/include/c++/13/bits/basic_string.tcc:337: 	this->_S_copy(__r + __pos, __s, __len2);
	addq	%rax, %rbp	# _55, _35
# /usr/include/c++/13/bits/basic_string.h:427: 	if (__n == 1)
	cmpq	$1, %rbx	#, __n
	jne	.L13	#,
# /usr/include/c++/13/bits/char_traits.h:358: 	__c1 = __c2;
	movb	0(%r13), %cl	# MEM[(const char_type &)__s_5(D)], MEM[(const char_type &)__s_5(D)]
	movb	%cl, 0(%rbp)	# MEM[(const char_type &)__s_5(D)], MEM[(char_type &)_35]
# /usr/include/c++/13/bits/char_traits.h:359:       }
	jmp	.L12	#
.L13:
# /usr/include/c++/13/bits/char_traits.h:435: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movq	%rbp, %rdi	# _35, _35
	movq	%r13, %rsi	# __s, __s
	movq	%rbx, %rcx	# __n, __n
	rep movsb
.L12:
# /usr/include/c++/13/bits/basic_string.h:264: 	if (_M_data() == _M_local_data())
	cmpq	%r8, %rdx	# _30, _13
	je	.L14	#,
	movq	%rax, 8(%rsp)	# _55, %sfp
# /usr/include/c++/13/bits/basic_string.h:289:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	16(%r12), %rax	# this_1(D)->D.47633._M_allocated_capacity, tmp165
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	movq	%r8, %rdi	# _30,
# /usr/include/c++/13/bits/basic_string.h:289:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	leaq	1(%rax), %rsi	#, tmp131
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	call	_ZdlPvm@PLT	#
	movq	8(%rsp), %rax	# %sfp, _55
.L14:
# /usr/include/c++/13/bits/basic_string.h:213:       { _M_dataplus._M_p = __p; }
	movq	%rax, (%r12)	# _55, this_1(D)->_M_dataplus._M_p
# /usr/include/c++/13/bits/basic_string.h:250:       { _M_allocated_capacity = __capacity; }
	movq	%r14, 16(%r12)	# __new_capacity, this_1(D)->D.47633._M_allocated_capacity
.L5:
# /usr/include/c++/13/bits/char_traits.h:358: 	__c1 = __c2;
	movq	(%r12), %rax	# MEM[(const struct basic_string *)this_1(D)]._M_dataplus._M_p, MEM[(const struct basic_string *)this_1(D)]._M_dataplus._M_p
# /usr/include/c++/13/bits/basic_string.h:218:       { _M_string_length = __length; }
	movq	%r15, 8(%r12)	# __len, this_1(D)->_M_string_length
# /usr/include/c++/13/bits/char_traits.h:358: 	__c1 = __c2;
	movb	$0, (%rax,%r15)	#, MEM[(char_type &)_19]
# /usr/include/c++/13/bits/basic_string.h:1460:       }
	addq	$24, %rsp	#,
	.cfi_def_cfa_offset 56
	popq	%rbx	#
	.cfi_def_cfa_offset 48
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
	ret	
	.cfi_endproc
.LFE4246:
	.size	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6appendEPKcm.isra.0, .-_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6appendEPKcm.isra.0
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"invalid nes file: missing NES header magic"
	.section	.rodata.str1.1
.LC3:
	.string	"vector::_M_default_append"
	.section	.rodata.str1.8
	.align 8
.LC4:
	.string	"invalid nes file: truncated rom data"
	.section	.text.unlikely
	.align 2
.LCOLDB5:
	.text
.LHOTB5:
	.align 2
	.p2align 4
	.globl	_ZN6nes_em7NesFile4readEv
	.type	_ZN6nes_em7NesFile4readEv, @function
_ZN6nes_em7NesFile4readEv:
.LFB3434:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA3434
	endbr64	
	pushq	%r15	#
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
# /home/chenxi/source/repos/nes/src/nes_file.cpp:16:     file.read(reinterpret_cast<char*>(header.data()), header.size());
	movl	$16, %edx	#,
# /home/chenxi/source/repos/nes/src/nes_file.cpp:14: void NesFile::read() {
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
	movq	%rdi, %rbx	# tmp245, this
	subq	$72, %rsp	#,
	.cfi_def_cfa_offset 128
# /home/chenxi/source/repos/nes/src/nes_file.cpp:14: void NesFile::read() {
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp254
	movq	%rax, 56(%rsp)	# tmp254, D.95800
	xorl	%eax, %eax	# tmp254
# /home/chenxi/source/repos/nes/src/nes_file.cpp:16:     file.read(reinterpret_cast<char*>(header.data()), header.size());
	leaq	32(%rsp), %rsi	#, tmp164
.LEHB0:
	call	_ZNSi4readEPcl@PLT	#
# /home/chenxi/source/repos/nes/src/nes_file.cpp:17:     if (!file || !std::equal(NES_HEADER.begin(), NES_HEADER.end(), header.begin())) {
	testb	$5, 288(%rbx)	#, MEM[(const struct basic_ios *)this_16(D) + 256B].D.69952._M_streambuf_state
	jne	.L32	#,
# /home/chenxi/source/repos/nes/src/nes_file.cpp:17:     if (!file || !std::equal(NES_HEADER.begin(), NES_HEADER.end(), header.begin())) {
	cmpl	$441664846, 32(%rsp)	#, MEM <unsigned int> [(char * {ref-all})&header]
	jne	.L32	#,
# /home/chenxi/source/repos/nes/src/nes_file.cpp:24:     uint8_t flags6 = header[6];
	movzbl	38(%rsp), %eax	# MEM[(value_type &)&header + 6], flags6
	movl	$2, %r8d	#, tmp243
# /home/chenxi/source/repos/nes/src/nes_file.cpp:21:     size_t prg_rom_units = header[4];
	movzbl	36(%rsp), %r13d	# MEM[(value_type &)&header + 4], prg_rom_units
# /home/chenxi/source/repos/nes/src/nes_file.cpp:22:     size_t chr_rom_units = header[5];
	movzbl	37(%rsp), %ebp	# MEM[(value_type &)&header + 5], chr_rom_units
# /home/chenxi/source/repos/nes/src/nes_file.cpp:27:     has_trainer = flags6 & 0x04;
	movl	%eax, %esi	# flags6, _19
# /home/chenxi/source/repos/nes/src/nes_file.cpp:28:     has_battery = flags6 & 0x02;
	movl	%eax, %ecx	# flags6, tmp170
	movl	%eax, %edx	# flags6, tmp244
# /home/chenxi/source/repos/nes/src/nes_file.cpp:27:     has_trainer = flags6 & 0x04;
	andl	$4, %esi	#, _19
	setne	%dil	#, _5
# /home/chenxi/source/repos/nes/src/nes_file.cpp:28:     has_battery = flags6 & 0x02;
	shrb	%cl	# tmp170
	andl	$1, %edx	#, tmp244
	andl	$1, %ecx	#, _6
	testb	$8, %al	#, flags6
# /home/chenxi/source/repos/nes/src/nes_file.cpp:27:     has_trainer = flags6 & 0x04;
	movb	%dil, 577(%rbx)	# _5, this_16(D)->has_trainer
	cmovne	%r8d, %edx	# tmp244,, tmp243, _224
# /home/chenxi/source/repos/nes/src/nes_file.cpp:36:     mapper = (flags7 & 0xf0) | (flags6 >> 4);
	shrb	$4, %al	#, tmp178
# /home/chenxi/source/repos/nes/src/nes_file.cpp:28:     has_battery = flags6 & 0x02;
	movb	%cl, 576(%rbx)	# _6, this_16(D)->has_battery
# /home/chenxi/source/repos/nes/src/nes_file.cpp:30:         mirroring = Mirroring::FourScreen;
	movl	%edx, 572(%rbx)	# _224, this_16(D)->mirroring
# /home/chenxi/source/repos/nes/src/nes_file.cpp:36:     mapper = (flags7 & 0xf0) | (flags6 >> 4);
	movzbl	39(%rsp), %edx	# MEM[(value_type &)&header + 7], tmp177
	andl	$-16, %edx	#, tmp177
# /home/chenxi/source/repos/nes/src/nes_file.cpp:36:     mapper = (flags7 & 0xf0) | (flags6 >> 4);
	orl	%edx, %eax	# tmp177, tmp179
	movb	%al, 568(%rbx)	# tmp179, this_16(D)->mapper
# /home/chenxi/source/repos/nes/src/nes_file.cpp:38:     if (has_trainer) {
	testb	%sil, %sil	# _19
	jne	.L97	#,
.L38:
# /usr/include/c++/13/bits/stl_vector.h:993:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	528(%rbx), %r14	# MEM[(const struct vector *)this_16(D) + 520B].D.86006._M_impl.D.85316._M_finish, _65
# /usr/include/c++/13/bits/stl_vector.h:993:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	520(%rbx), %r15	# MEM[(const struct vector *)this_16(D) + 520B].D.86006._M_impl.D.85316._M_start, prephitmp_34
# /home/chenxi/source/repos/nes/src/nes_file.cpp:42:     prg_rom.resize(prg_rom_units * PRG_ROM_UNIT_SIZE);
	salq	$14, %r13	#, _11
# /usr/include/c++/13/bits/stl_vector.h:993:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	%r14, %r12	# _65, _67
	subq	%r15, %r12	# prephitmp_34, _67
# /usr/include/c++/13/bits/stl_vector.h:1015: 	if (__new_size > size())
	cmpq	%r13, %r12	# _11, _67
	jb	.L98	#,
# /usr/include/c++/13/bits/stl_vector.h:1017: 	else if (__new_size < size())
	cmpq	%r12, %r13	# _67, _11
	jb	.L99	#,
.L42:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:43:     file.read(reinterpret_cast<char*>(prg_rom.data()), prg_rom.size());
	movq	%r12, %rdx	# _67,
	movq	%r15, %rsi	# prephitmp_34,
	movq	%rbx, %rdi	# this,
# /home/chenxi/source/repos/nes/src/nes_file.cpp:45:     chr_rom.resize(chr_rom_units * CHR_ROM_UNIT_SIZE);
	salq	$13, %rbp	#, _13
# /home/chenxi/source/repos/nes/src/nes_file.cpp:43:     file.read(reinterpret_cast<char*>(prg_rom.data()), prg_rom.size());
	call	_ZNSi4readEPcl@PLT	#
# /usr/include/c++/13/bits/stl_vector.h:993:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	552(%rbx), %r14	# MEM[(const struct vector *)this_16(D) + 544B].D.86006._M_impl.D.85316._M_finish, _59
# /usr/include/c++/13/bits/stl_vector.h:993:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	544(%rbx), %r13	# MEM[(const struct vector *)this_16(D) + 544B].D.86006._M_impl.D.85316._M_start, _60
# /usr/include/c++/13/bits/stl_vector.h:993:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	%r14, %r12	# _59, _61
	subq	%r13, %r12	# _60, _61
# /usr/include/c++/13/bits/stl_vector.h:1015: 	if (__new_size > size())
	cmpq	%rbp, %r12	# _13, _61
	jb	.L100	#,
# /usr/include/c++/13/bits/stl_vector.h:1017: 	else if (__new_size < size())
	cmpq	%r12, %rbp	# _61, _13
	jb	.L101	#,
.L56:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:46:     file.read(reinterpret_cast<char*>(chr_rom.data()), chr_rom.size());
	movq	%r12, %rdx	# _61,
	movq	%r13, %rsi	# _60,
	movq	%rbx, %rdi	# this,
	call	_ZNSi4readEPcl@PLT	#
# /home/chenxi/source/repos/nes/src/nes_file.cpp:48:     if (file.fail()) {
	testb	$5, 288(%rbx)	#, MEM[(const struct basic_ios *)this_16(D) + 256B].D.69952._M_streambuf_state
	jne	.L93	#,
# /home/chenxi/source/repos/nes/src/nes_file.cpp:51: }
	movq	56(%rsp), %rax	# D.95800, tmp260
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp260
	jne	.L102	#,
	addq	$72, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx	#
	.cfi_def_cfa_offset 48
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
	ret	
	.p2align 4,,10
	.p2align 3
.L99:
	.cfi_restore_state
# /usr/include/c++/13/bits/stl_vector.h:1018: 	  _M_erase_at_end(this->_M_impl._M_start + __new_size);
	leaq	(%r15,%r13), %rax	#, _70
# /usr/include/c++/13/bits/stl_vector.h:1935: 	if (size_type __n = this->_M_impl._M_finish - __pos)
	cmpq	%rax, %r14	# _70, _65
	je	.L42	#,
# /usr/include/c++/13/bits/stl_vector.h:1939: 	    this->_M_impl._M_finish = __pos;
	movq	%rax, 528(%rbx)	# _70, MEM[(struct vector *)this_16(D) + 520B].D.86006._M_impl.D.85316._M_finish
	movq	%r13, %r12	# _11, _67
	jmp	.L42	#
	.p2align 4,,10
	.p2align 3
.L101:
# /usr/include/c++/13/bits/stl_vector.h:1018: 	  _M_erase_at_end(this->_M_impl._M_start + __new_size);
	leaq	0(%r13,%rbp), %rax	#, _64
# /usr/include/c++/13/bits/stl_vector.h:1935: 	if (size_type __n = this->_M_impl._M_finish - __pos)
	cmpq	%rax, %r14	# _64, _59
	je	.L56	#,
# /usr/include/c++/13/bits/stl_vector.h:1939: 	    this->_M_impl._M_finish = __pos;
	movq	%rax, 552(%rbx)	# _64, MEM[(struct vector *)this_16(D) + 544B].D.86006._M_impl.D.85316._M_finish
	movq	%rbp, %r12	# _13, _61
	jmp	.L56	#
	.p2align 4,,10
	.p2align 3
.L97:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:39:         file.seekg(TRAINER_SIZE, std::ios::cur);
	movl	$1, %edx	#,
	movl	$512, %esi	#,
	movq	%rbx, %rdi	# this,
	call	_ZNSi5seekgElSt12_Ios_Seekdir@PLT	#
	jmp	.L38	#
	.p2align 4,,10
	.p2align 3
.L98:
# /usr/include/c++/13/bits/vector.tcc:641: 					 - this->_M_impl._M_finish);
	movq	536(%rbx), %rax	# MEM[(struct vector *)this_16(D) + 520B].D.86006._M_impl.D.85316._M_end_of_storage, __navail
# /usr/include/c++/13/bits/stl_vector.h:1016: 	  _M_default_append(__new_size - size());
	movq	%r13, %r8	# _11, _172
	subq	%r12, %r8	# _67, _172
# /usr/include/c++/13/bits/vector.tcc:641: 					 - this->_M_impl._M_finish);
	subq	%r14, %rax	# _65, __navail
# /usr/include/c++/13/bits/vector.tcc:646: 	  if (__navail >= __n)
	cmpq	%r8, %rax	# _172, __navail
	jnb	.L103	#,
# /usr/include/c++/13/bits/vector.tcc:643: 	  if (__size > max_size() || __navail > max_size() - __size)
	movabsq	$9223372036854775807, %rax	#, tmp187
	subq	%r12, %rax	# _67, tmp186
# /usr/include/c++/13/bits/stl_vector.h:1898: 	if (max_size() - size() < __n)
	cmpq	%r8, %rax	# _172, tmp186
	jb	.L57	#,
# /usr/include/c++/13/bits/stl_uninitialized.h:668: 	      __first = std::fill_n(__first, __n - 1, *__val);
	leaq	-1(%r8), %rax	#, _237
	movq	%rax, 16(%rsp)	# _237, %sfp
# /usr/include/c++/13/bits/stl_algobase.h:262:       if (__a < __b)
	cmpq	%r8, %r12	# _172, _67
	jb	.L104	#,
# /usr/include/c++/13/bits/stl_vector.h:1901: 	const size_type __len = size() + (std::max)(size(), __n);
	leaq	(%r12,%r12), %rax	#, __len
	movq	%r8, 24(%rsp)	# _172, %sfp
# /usr/include/c++/13/bits/new_allocator.h:151: 	return static_cast<_Tp*>(_GLIBCXX_OPERATOR_NEW(__n * sizeof(_Tp)));
	movq	%rax, %rdi	# __len,
# /usr/include/c++/13/bits/stl_vector.h:1901: 	const size_type __len = size() + (std::max)(size(), __n);
	movq	%rax, 8(%rsp)	# __len, %sfp
# /usr/include/c++/13/bits/new_allocator.h:151: 	return static_cast<_Tp*>(_GLIBCXX_OPERATOR_NEW(__n * sizeof(_Tp)));
	call	_Znwm@PLT	#
# /usr/include/c++/13/bits/stl_algobase.h:1123:       if (__n <= 0)
	movq	24(%rsp), %r8	# %sfp, _172
# /usr/include/c++/13/bits/vector.tcc:668: 		      std::__uninitialized_default_n_a(__new_start + __size,
	leaq	(%rax,%r12), %rdi	#, _10
# /usr/include/c++/13/bits/new_allocator.h:151: 	return static_cast<_Tp*>(_GLIBCXX_OPERATOR_NEW(__n * sizeof(_Tp)));
	movq	%rax, %r14	# tmp248, _88
# /usr/include/c++/13/bits/stl_construct.h:119:       ::new((void*)__p) _Tp(std::forward<_Args>(__args)...);
	movb	$0, (%rdi)	#, *_10
# /usr/include/c++/13/bits/stl_algobase.h:1123:       if (__n <= 0)
	cmpq	$1, %r8	#, _172
	jne	.L105	#,
.L48:
# /usr/include/c++/13/bits/stl_uninitialized.h:1131: 	  __builtin_memmove(__result, __first, __count * sizeof(_Tp));
	movq	%r15, %rsi	# prephitmp_34,
	movq	%r12, %rdx	# _67,
	movq	%r14, %rdi	# _88,
	call	memmove@PLT	#
# /usr/include/c++/13/bits/vector.tcc:704: 			    this->_M_impl._M_end_of_storage - __old_start);
	movq	536(%rbx), %rsi	# MEM[(struct vector *)this_16(D) + 520B].D.86006._M_impl.D.85316._M_end_of_storage, _147
	subq	%r15, %rsi	# prephitmp_34, _147
.L51:
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	movq	%r15, %rdi	# prephitmp_34,
	movq	%r14, %r15	# _88, prephitmp_34
	call	_ZdlPvm@PLT	#
.L52:
# /usr/include/c++/13/bits/vector.tcc:707: 	      this->_M_impl._M_end_of_storage = __new_start + __len;
	movq	8(%rsp), %r9	# %sfp, __len
# /usr/include/c++/13/bits/vector.tcc:706: 	      this->_M_impl._M_finish = __new_start + __size + __n;
	leaq	(%r14,%r13), %rax	#, tmp204
# /usr/include/c++/13/bits/vector.tcc:705: 	      this->_M_impl._M_start = __new_start;
	movq	%r14, 520(%rbx)	# _88, MEM[(struct vector *)this_16(D) + 520B].D.86006._M_impl.D.85316._M_start
	movq	%r13, %r12	# _11, _67
# /usr/include/c++/13/bits/vector.tcc:706: 	      this->_M_impl._M_finish = __new_start + __size + __n;
	movq	%rax, 528(%rbx)	# tmp204, MEM[(struct vector *)this_16(D) + 520B].D.86006._M_impl.D.85316._M_finish
# /usr/include/c++/13/bits/vector.tcc:707: 	      this->_M_impl._M_end_of_storage = __new_start + __len;
	addq	%r14, %r9	# _88, __len
	movq	%r9, 536(%rbx)	# tmp205, MEM[(struct vector *)this_16(D) + 520B].D.86006._M_impl.D.85316._M_end_of_storage
	jmp	.L42	#
	.p2align 4,,10
	.p2align 3
.L100:
# /usr/include/c++/13/bits/vector.tcc:641: 					 - this->_M_impl._M_finish);
	movq	560(%rbx), %rax	# MEM[(struct vector *)this_16(D) + 544B].D.86006._M_impl.D.85316._M_end_of_storage, __navail
# /usr/include/c++/13/bits/stl_vector.h:1016: 	  _M_default_append(__new_size - size());
	movq	%rbp, %rcx	# _13, _141
	subq	%r12, %rcx	# _61, _141
# /usr/include/c++/13/bits/vector.tcc:641: 					 - this->_M_impl._M_finish);
	subq	%r14, %rax	# _59, __navail
# /usr/include/c++/13/bits/vector.tcc:646: 	  if (__navail >= __n)
	cmpq	%rcx, %rax	# _141, __navail
	jnb	.L106	#,
# /usr/include/c++/13/bits/vector.tcc:643: 	  if (__size > max_size() || __navail > max_size() - __size)
	movabsq	$9223372036854775807, %rax	#, tmp213
	subq	%r12, %rax	# _61, tmp212
# /usr/include/c++/13/bits/stl_vector.h:1898: 	if (max_size() - size() < __n)
	cmpq	%rcx, %rax	# _141, tmp212
	jb	.L57	#,
# /usr/include/c++/13/bits/stl_uninitialized.h:668: 	      __first = std::fill_n(__first, __n - 1, *__val);
	leaq	-1(%rcx), %rax	#, _236
	movq	%rax, 8(%rsp)	# _236, %sfp
# /usr/include/c++/13/bits/stl_algobase.h:262:       if (__a < __b)
	cmpq	%rcx, %r12	# _141, _61
	jb	.L107	#,
# /usr/include/c++/13/bits/stl_vector.h:1901: 	const size_type __len = size() + (std::max)(size(), __n);
	leaq	(%r12,%r12), %r14	#, __len
	movq	%rcx, 16(%rsp)	# _141, %sfp
# /usr/include/c++/13/bits/new_allocator.h:151: 	return static_cast<_Tp*>(_GLIBCXX_OPERATOR_NEW(__n * sizeof(_Tp)));
	movq	%r14, %rdi	# __len,
	call	_Znwm@PLT	#
# /usr/include/c++/13/bits/stl_algobase.h:1123:       if (__n <= 0)
	movq	16(%rsp), %rcx	# %sfp, _141
# /usr/include/c++/13/bits/vector.tcc:668: 		      std::__uninitialized_default_n_a(__new_start + __size,
	leaq	(%rax,%r12), %rdi	#, _102
# /usr/include/c++/13/bits/new_allocator.h:151: 	return static_cast<_Tp*>(_GLIBCXX_OPERATOR_NEW(__n * sizeof(_Tp)));
	movq	%rax, %r15	# tmp250, _83
# /usr/include/c++/13/bits/stl_construct.h:119:       ::new((void*)__p) _Tp(std::forward<_Args>(__args)...);
	movb	$0, (%rdi)	#, *_102
# /usr/include/c++/13/bits/stl_algobase.h:1123:       if (__n <= 0)
	cmpq	$1, %rcx	#, _141
	jne	.L108	#,
.L61:
# /usr/include/c++/13/bits/stl_uninitialized.h:1131: 	  __builtin_memmove(__result, __first, __count * sizeof(_Tp));
	movq	%r13, %rsi	# _60,
	movq	%r12, %rdx	# _61,
	movq	%r15, %rdi	# _83,
	call	memmove@PLT	#
# /usr/include/c++/13/bits/vector.tcc:704: 			    this->_M_impl._M_end_of_storage - __old_start);
	movq	560(%rbx), %rsi	# MEM[(struct vector *)this_16(D) + 544B].D.86006._M_impl.D.85316._M_end_of_storage, _73
	subq	%r13, %rsi	# _60, _73
.L64:
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	movq	%r13, %rdi	# _60,
	movq	%r15, %r13	# _83, _60
	call	_ZdlPvm@PLT	#
.L65:
# /usr/include/c++/13/bits/vector.tcc:706: 	      this->_M_impl._M_finish = __new_start + __size + __n;
	leaq	(%r15,%rbp), %rax	#, tmp228
# /usr/include/c++/13/bits/vector.tcc:707: 	      this->_M_impl._M_end_of_storage = __new_start + __len;
	leaq	(%r15,%r14), %r8	#, tmp229
# /usr/include/c++/13/bits/vector.tcc:705: 	      this->_M_impl._M_start = __new_start;
	movq	%r15, 544(%rbx)	# _83, MEM[(struct vector *)this_16(D) + 544B].D.86006._M_impl.D.85316._M_start
	movq	%rbp, %r12	# _13, _61
# /usr/include/c++/13/bits/vector.tcc:706: 	      this->_M_impl._M_finish = __new_start + __size + __n;
	movq	%rax, 552(%rbx)	# tmp228, MEM[(struct vector *)this_16(D) + 544B].D.86006._M_impl.D.85316._M_finish
# /usr/include/c++/13/bits/vector.tcc:707: 	      this->_M_impl._M_end_of_storage = __new_start + __len;
	movq	%r8, 560(%rbx)	# tmp229, MEM[(struct vector *)this_16(D) + 544B].D.86006._M_impl.D.85316._M_end_of_storage
	jmp	.L56	#
	.p2align 4,,10
	.p2align 3
.L103:
# /usr/include/c++/13/bits/stl_algobase.h:1123:       if (__n <= 0)
	movq	%r8, %rdx	# _172, _95
# /usr/include/c++/13/bits/stl_construct.h:119:       ::new((void*)__p) _Tp(std::forward<_Args>(__args)...);
	movb	$0, (%r14)	#, *_65
# /usr/include/c++/13/bits/stl_uninitialized.h:667: 	      ++__first;
	leaq	1(%r14), %r12	#, __first
# /usr/include/c++/13/bits/stl_algobase.h:1123:       if (__n <= 0)
	movq	%r8, 8(%rsp)	# _172, %sfp
	subq	$1, %rdx	#, _95
	jne	.L109	#,
.L41:
# /usr/include/c++/13/bits/stl_vector.h:993:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	520(%rbx), %r15	# MEM[(const struct vector *)this_16(D) + 520B].D.86006._M_impl.D.85316._M_start, prephitmp_34
# /usr/include/c++/13/bits/vector.tcc:649: 	      this->_M_impl._M_finish =
	movq	%r12, 528(%rbx)	# __first, MEM[(struct vector *)this_16(D) + 520B].D.86006._M_impl.D.85316._M_finish
# /usr/include/c++/13/bits/stl_vector.h:993:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	subq	%r15, %r12	# prephitmp_34, _67
	jmp	.L42	#
	.p2align 4,,10
	.p2align 3
.L106:
# /usr/include/c++/13/bits/stl_algobase.h:1123:       if (__n <= 0)
	movq	%rcx, %rdx	# _141, _143
# /usr/include/c++/13/bits/stl_construct.h:119:       ::new((void*)__p) _Tp(std::forward<_Args>(__args)...);
	movb	$0, (%r14)	#, *_59
# /usr/include/c++/13/bits/stl_uninitialized.h:667: 	      ++__first;
	leaq	1(%r14), %r12	#, __first
# /usr/include/c++/13/bits/stl_algobase.h:1123:       if (__n <= 0)
	movq	%rcx, 8(%rsp)	# _141, %sfp
	subq	$1, %rdx	#, _143
	jne	.L110	#,
.L55:
# /usr/include/c++/13/bits/stl_vector.h:993:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	movq	544(%rbx), %r13	# MEM[(const struct vector *)this_16(D) + 544B].D.86006._M_impl.D.85316._M_start, _60
# /usr/include/c++/13/bits/vector.tcc:649: 	      this->_M_impl._M_finish =
	movq	%r12, 552(%rbx)	# __first, MEM[(struct vector *)this_16(D) + 544B].D.86006._M_impl.D.85316._M_finish
# /usr/include/c++/13/bits/stl_vector.h:993:       { return size_type(this->_M_impl._M_finish - this->_M_impl._M_start); }
	subq	%r13, %r12	# _60, _61
	jmp	.L56	#
	.p2align 4,,10
	.p2align 3
.L109:
# /usr/include/c++/13/bits/stl_algobase.h:951: 	__builtin_memset(__first, static_cast<unsigned char>(__tmp), __len);
	movq	%r12, %rdi	# __first,
	xorl	%esi, %esi	#
	call	memset@PLT	#
# /usr/include/c++/13/bits/stl_algobase.h:1129:       return __first + __n;
	movq	8(%rsp), %r8	# %sfp, _172
	leaq	(%r14,%r8), %r12	#, __first
	jmp	.L41	#
	.p2align 4,,10
	.p2align 3
.L110:
# /usr/include/c++/13/bits/stl_algobase.h:951: 	__builtin_memset(__first, static_cast<unsigned char>(__tmp), __len);
	movq	%r12, %rdi	# __first,
	xorl	%esi, %esi	#
	call	memset@PLT	#
# /usr/include/c++/13/bits/stl_algobase.h:1129:       return __first + __n;
	movq	8(%rsp), %rcx	# %sfp, _141
	leaq	(%r14,%rcx), %r12	#, __first
	jmp	.L55	#
	.p2align 4,,10
	.p2align 3
.L105:
# /usr/include/c++/13/bits/stl_algobase.h:951: 	__builtin_memset(__first, static_cast<unsigned char>(__tmp), __len);
	movq	16(%rsp), %rdx	# %sfp,
# /usr/include/c++/13/bits/stl_uninitialized.h:667: 	      ++__first;
	addq	$1, %rdi	#, __first
# /usr/include/c++/13/bits/stl_algobase.h:951: 	__builtin_memset(__first, static_cast<unsigned char>(__tmp), __len);
	xorl	%esi, %esi	#
	call	memset@PLT	#
	jmp	.L48	#
	.p2align 4,,10
	.p2align 3
.L108:
	movq	8(%rsp), %rdx	# %sfp,
# /usr/include/c++/13/bits/stl_uninitialized.h:667: 	      ++__first;
	addq	$1, %rdi	#, __first
# /usr/include/c++/13/bits/stl_algobase.h:951: 	__builtin_memset(__first, static_cast<unsigned char>(__tmp), __len);
	xorl	%esi, %esi	#
	call	memset@PLT	#
	jmp	.L61	#
	.p2align 4,,10
	.p2align 3
.L104:
# /usr/include/c++/13/bits/new_allocator.h:151: 	return static_cast<_Tp*>(_GLIBCXX_OPERATOR_NEW(__n * sizeof(_Tp)));
	movq	%r13, %rdi	# _11,
	call	_Znwm@PLT	#
# /usr/include/c++/13/bits/stl_algobase.h:1123:       if (__n <= 0)
	movq	16(%rsp), %rdx	# %sfp, _237
# /usr/include/c++/13/bits/new_allocator.h:151: 	return static_cast<_Tp*>(_GLIBCXX_OPERATOR_NEW(__n * sizeof(_Tp)));
	movq	%rax, %r14	# tmp247, _88
# /usr/include/c++/13/bits/vector.tcc:668: 		      std::__uninitialized_default_n_a(__new_start + __size,
	leaq	(%rax,%r12), %rax	#, _84
# /usr/include/c++/13/bits/stl_construct.h:119:       ::new((void*)__p) _Tp(std::forward<_Args>(__args)...);
	movb	$0, (%rax)	#, *_84
# /usr/include/c++/13/bits/stl_algobase.h:1123:       if (__n <= 0)
	testq	%rdx, %rdx	# _237
	je	.L46	#,
# /usr/include/c++/13/bits/stl_uninitialized.h:667: 	      ++__first;
	leaq	1(%rax), %rdi	#, __first
# /usr/include/c++/13/bits/stl_algobase.h:951: 	__builtin_memset(__first, static_cast<unsigned char>(__tmp), __len);
	xorl	%esi, %esi	#
	call	memset@PLT	#
.L46:
# /usr/include/c++/13/bits/stl_uninitialized.h:1119:       if (__count > 0)
	testq	%r12, %r12	# _67
	jne	.L111	#,
# /usr/include/c++/13/bits/stl_vector.h:389: 	if (__p)
	testq	%r15, %r15	# prephitmp_34
	jne	.L112	#,
	movq	%r13, 8(%rsp)	# _11, %sfp
	movq	%r14, %r15	# _88, prephitmp_34
	jmp	.L52	#
	.p2align 4,,10
	.p2align 3
.L107:
# /usr/include/c++/13/bits/new_allocator.h:151: 	return static_cast<_Tp*>(_GLIBCXX_OPERATOR_NEW(__n * sizeof(_Tp)));
	movq	%rbp, %rdi	# _13,
	call	_Znwm@PLT	#
# /usr/include/c++/13/bits/stl_algobase.h:1123:       if (__n <= 0)
	movq	8(%rsp), %rdx	# %sfp, _236
# /usr/include/c++/13/bits/new_allocator.h:151: 	return static_cast<_Tp*>(_GLIBCXX_OPERATOR_NEW(__n * sizeof(_Tp)));
	movq	%rax, %r15	# tmp249, _83
# /usr/include/c++/13/bits/vector.tcc:668: 		      std::__uninitialized_default_n_a(__new_start + __size,
	leaq	(%rax,%r12), %rax	#, _132
# /usr/include/c++/13/bits/stl_construct.h:119:       ::new((void*)__p) _Tp(std::forward<_Args>(__args)...);
	movb	$0, (%rax)	#, *_132
# /usr/include/c++/13/bits/stl_algobase.h:1123:       if (__n <= 0)
	testq	%rdx, %rdx	# _236
	je	.L59	#,
# /usr/include/c++/13/bits/stl_uninitialized.h:667: 	      ++__first;
	leaq	1(%rax), %rdi	#, __first
# /usr/include/c++/13/bits/stl_algobase.h:951: 	__builtin_memset(__first, static_cast<unsigned char>(__tmp), __len);
	xorl	%esi, %esi	#
	call	memset@PLT	#
.L59:
# /usr/include/c++/13/bits/stl_uninitialized.h:1119:       if (__count > 0)
	testq	%r12, %r12	# _61
	jne	.L113	#,
# /usr/include/c++/13/bits/stl_vector.h:389: 	if (__p)
	testq	%r13, %r13	# _60
	jne	.L114	#,
	movq	%r15, %r13	# _83, _60
	movq	%rbp, %r14	# _13, __len
	jmp	.L65	#
	.p2align 4,,10
	.p2align 3
.L112:
# /usr/include/c++/13/bits/vector.tcc:704: 			    this->_M_impl._M_end_of_storage - __old_start);
	movq	536(%rbx), %rsi	# MEM[(struct vector *)this_16(D) + 520B].D.86006._M_impl.D.85316._M_end_of_storage, _147
# /usr/include/c++/13/bits/vector.tcc:703: 	      _M_deallocate(__old_start,
	movq	%r13, 8(%rsp)	# _11, %sfp
# /usr/include/c++/13/bits/vector.tcc:704: 			    this->_M_impl._M_end_of_storage - __old_start);
	subq	%r15, %rsi	# prephitmp_34, _147
	jmp	.L51	#
	.p2align 4,,10
	.p2align 3
.L114:
	movq	560(%rbx), %rsi	# MEM[(struct vector *)this_16(D) + 544B].D.86006._M_impl.D.85316._M_end_of_storage, _73
# /usr/include/c++/13/bits/vector.tcc:703: 	      _M_deallocate(__old_start,
	movq	%rbp, %r14	# _13, __len
# /usr/include/c++/13/bits/vector.tcc:704: 			    this->_M_impl._M_end_of_storage - __old_start);
	subq	%r13, %rsi	# _60, _73
	jmp	.L64	#
.L111:
	movq	%r13, 8(%rsp)	# _11, %sfp
	jmp	.L48	#
.L113:
	movq	%rbp, %r14	# _13, __len
	jmp	.L61	#
.L102:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:51: }
	call	__stack_chk_fail@PLT	#
.L57:
# /usr/include/c++/13/bits/stl_vector.h:1899: 	  __throw_length_error(__N(__s));
	movq	56(%rsp), %rax	# D.95800, tmp256
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp256
	jne	.L115	#,
	leaq	.LC3(%rip), %rdi	#, tmp189
	call	_ZSt20__throw_length_errorPKc@PLT	#
.LEHE0:
.L115:
	call	__stack_chk_fail@PLT	#
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA3434:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE3434-.LLSDACSB3434
.LLSDACSB3434:
	.uleb128 .LEHB0-.LFB3434
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
.LLSDACSE3434:
	.text
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC3434
	.type	_ZN6nes_em7NesFile4readEv.cold, @function
_ZN6nes_em7NesFile4readEv.cold:
.LFSB3434:
.L93:
	.cfi_def_cfa_offset 128
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
# /home/chenxi/source/repos/nes/src/nes_file.cpp:49:         throw std::runtime_error("invalid nes file: truncated rom data");
	movl	$16, %edi	#,
	call	__cxa_allocate_exception@PLT	#
# /home/chenxi/source/repos/nes/src/nes_file.cpp:49:         throw std::runtime_error("invalid nes file: truncated rom data");
	leaq	.LC4(%rip), %rsi	#, tmp233
	movq	%rax, %rdi	# _41,
# /home/chenxi/source/repos/nes/src/nes_file.cpp:49:         throw std::runtime_error("invalid nes file: truncated rom data");
	movq	%rax, %rbx	# tmp251, _41
.LEHB1:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:49:         throw std::runtime_error("invalid nes file: truncated rom data");
	call	_ZNSt13runtime_errorC1EPKc@PLT	#
.LEHE1:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:49:         throw std::runtime_error("invalid nes file: truncated rom data");
	movq	56(%rsp), %rax	# D.95800, tmp257
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp257
	jne	.L116	#,
	movq	_ZNSt13runtime_errorD1Ev@GOTPCREL(%rip), %rdx	#,
	leaq	_ZTISt13runtime_error(%rip), %rsi	#, tmp237
	movq	%rbx, %rdi	# _41,
.LEHB2:
	call	__cxa_throw@PLT	#
.LEHE2:
.L32:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:18:         throw std::runtime_error("invalid nes file: missing NES header magic");
	movl	$16, %edi	#,
	call	__cxa_allocate_exception@PLT	#
# /home/chenxi/source/repos/nes/src/nes_file.cpp:18:         throw std::runtime_error("invalid nes file: missing NES header magic");
	leaq	.LC2(%rip), %rsi	#, tmp168
	movq	%rax, %rdi	# _46,
# /home/chenxi/source/repos/nes/src/nes_file.cpp:18:         throw std::runtime_error("invalid nes file: missing NES header magic");
	movq	%rax, %rbx	# tmp246, _46
.LEHB3:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:18:         throw std::runtime_error("invalid nes file: missing NES header magic");
	call	_ZNSt13runtime_errorC1EPKc@PLT	#
.LEHE3:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:18:         throw std::runtime_error("invalid nes file: missing NES header magic");
	movq	56(%rsp), %rax	# D.95800, tmp255
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp255
	jne	.L117	#,
	movq	_ZNSt13runtime_errorD1Ev@GOTPCREL(%rip), %rdx	#,
	leaq	_ZTISt13runtime_error(%rip), %rsi	#, tmp175
	movq	%rbx, %rdi	# _46,
.LEHB4:
	call	__cxa_throw@PLT	#
.L116:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:49:         throw std::runtime_error("invalid nes file: truncated rom data");
	call	__stack_chk_fail@PLT	#
.L117:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:18:         throw std::runtime_error("invalid nes file: missing NES header magic");
	call	__stack_chk_fail@PLT	#
.L76:
	endbr64	
# /home/chenxi/source/repos/nes/src/nes_file.cpp:18:         throw std::runtime_error("invalid nes file: missing NES header magic");
	movq	%rbx, %rdi	# _46,
	movq	%rax, %rbp	# tmp252, tmp238
	call	__cxa_free_exception@PLT	#
	movq	56(%rsp), %rax	# D.95800, tmp258
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp258
	jne	.L118	#,
	movq	%rbp, %rdi	# tmp238,
	call	_Unwind_Resume@PLT	#
.L77:
	endbr64	
# /home/chenxi/source/repos/nes/src/nes_file.cpp:49:         throw std::runtime_error("invalid nes file: truncated rom data");
	movq	%rbx, %rdi	# _41,
	movq	%rax, %rbp	# tmp253, tmp240
	call	__cxa_free_exception@PLT	#
	movq	56(%rsp), %rax	# D.95800, tmp259
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp259
	jne	.L119	#,
	movq	%rbp, %rdi	# tmp240,
	call	_Unwind_Resume@PLT	#
.LEHE4:
.L118:
	call	__stack_chk_fail@PLT	#
.L119:
	call	__stack_chk_fail@PLT	#
	.cfi_endproc
.LFE3434:
	.section	.gcc_except_table
.LLSDAC3434:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC3434-.LLSDACSBC3434
.LLSDACSBC3434:
	.uleb128 .LEHB1-.LCOLDB5
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L77-.LCOLDB5
	.uleb128 0
	.uleb128 .LEHB2-.LCOLDB5
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB3-.LCOLDB5
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L76-.LCOLDB5
	.uleb128 0
	.uleb128 .LEHB4-.LCOLDB5
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
.LLSDACSEC3434:
	.section	.text.unlikely
	.text
	.size	_ZN6nes_em7NesFile4readEv, .-_ZN6nes_em7NesFile4readEv
	.section	.text.unlikely
	.size	_ZN6nes_em7NesFile4readEv.cold, .-_ZN6nes_em7NesFile4readEv.cold
.LCOLDE5:
	.text
.LHOTE5:
	.section	.text._ZNSt6vectorIhSaIhEED2Ev,"axG",@progbits,_ZNSt6vectorIhSaIhEED5Ev,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt6vectorIhSaIhEED2Ev
	.type	_ZNSt6vectorIhSaIhEED2Ev, @function
_ZNSt6vectorIhSaIhEED2Ev:
.LFB3793:
	.cfi_startproc
	endbr64	
# /usr/include/c++/13/bits/stl_vector.h:370: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	(%rdi), %rax	# MEM[(struct _Vector_base *)this_3(D)]._M_impl.D.85316._M_start, _7
# /usr/include/c++/13/bits/stl_vector.h:389: 	if (__p)
	testq	%rax, %rax	# _7
	je	.L120	#,
# /usr/include/c++/13/bits/stl_vector.h:370: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	movq	16(%rdi), %rsi	# MEM[(struct _Vector_base *)this_3(D)]._M_impl.D.85316._M_end_of_storage, MEM[(struct _Vector_base *)this_3(D)]._M_impl.D.85316._M_end_of_storage
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	movq	%rax, %rdi	# _7,
# /usr/include/c++/13/bits/stl_vector.h:370: 		      _M_impl._M_end_of_storage - _M_impl._M_start);
	subq	%rax, %rsi	# _7, tmp87
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	jmp	_ZdlPvm@PLT	#
	.p2align 4,,10
	.p2align 3
.L120:
# /usr/include/c++/13/bits/stl_vector.h:738:       }
	ret	
	.cfi_endproc
.LFE3793:
	.size	_ZNSt6vectorIhSaIhEED2Ev, .-_ZNSt6vectorIhSaIhEED2Ev
	.weak	_ZNSt6vectorIhSaIhEED1Ev
	.set	_ZNSt6vectorIhSaIhEED1Ev,_ZNSt6vectorIhSaIhEED2Ev
	.section	.text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv,"axG",@progbits,_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv
	.type	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv, @function
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv:
.LFB3827:
	.cfi_startproc
	endbr64	
# /usr/include/c++/13/bits/basic_string.h:223:       { return _M_dataplus._M_p; }
	movq	(%rdi), %rax	# MEM[(const struct basic_string *)this_4(D)]._M_dataplus._M_p, _5
# /usr/include/c++/13/bits/basic_string.h:241: 	return std::pointer_traits<const_pointer>::pointer_to(*_M_local_buf);
	leaq	16(%rdi), %rdx	#, tmp87
# /usr/include/c++/13/bits/basic_string.h:264: 	if (_M_data() == _M_local_data())
	cmpq	%rdx, %rax	# tmp87, _5
	je	.L122	#,
# /usr/include/c++/13/bits/basic_string.h:289:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	16(%rdi), %rsi	# this_4(D)->D.47633._M_allocated_capacity, this_4(D)->D.47633._M_allocated_capacity
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	movq	%rax, %rdi	# _5,
# /usr/include/c++/13/bits/basic_string.h:289:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	addq	$1, %rsi	#, tmp88
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	jmp	_ZdlPvm@PLT	#
	.p2align 4,,10
	.p2align 3
.L122:
# /usr/include/c++/13/bits/basic_string.h:284:       }
	ret	
	.cfi_endproc
.LFE3827:
	.size	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv, .-_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv
	.section	.rodata.str1.1
.LC6:
	.string	"failed to open nes file: "
	.section	.text.unlikely
	.align 2
.LCOLDB7:
	.text
.LHOTB7:
	.align 2
	.p2align 4
	.globl	_ZN6nes_em7NesFileC2ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	.type	_ZN6nes_em7NesFileC2ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE, @function
_ZN6nes_em7NesFileC2ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE:
.LFB3432:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA3432
	endbr64	
	pushq	%r15	#
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
# /usr/include/c++/13/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	leaq	16+_ZTVSt9basic_iosIcSt11char_traitsIcEE(%rip), %r15	#, tmp202
# /home/chenxi/source/repos/nes/src/nes_file.cpp:7: NesFile::NesFile(std::string file_name): file(file_name, std::ios::binary) {
	pushq	%r14	#
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
# /usr/include/c++/13/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	leaq	256(%rdi), %r14	#, _43
# /home/chenxi/source/repos/nes/src/nes_file.cpp:7: NesFile::NesFile(std::string file_name): file(file_name, std::ios::binary) {
	pushq	%r13	#
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12	#
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp	#
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%rsi, %rbp	# tmp206, file_name
	pushq	%rbx	#
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movq	%rdi, %rbx	# tmp205, this
# /usr/include/c++/13/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	movq	%r14, %rdi	# _43,
# /home/chenxi/source/repos/nes/src/nes_file.cpp:7: NesFile::NesFile(std::string file_name): file(file_name, std::ios::binary) {
	subq	$72, %rsp	#,
	.cfi_def_cfa_offset 128
# /home/chenxi/source/repos/nes/src/nes_file.cpp:7: NesFile::NesFile(std::string file_name): file(file_name, std::ios::binary) {
	movq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp217
	movq	%rax, 56(%rsp)	# tmp217, D.95948
	xorl	%eax, %eax	# tmp217
# /usr/include/c++/13/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	call	_ZNSt8ios_baseC2Ev@PLT	#
# /usr/include/c++/13/istream:697:       : _M_gcount(streamsize(0))
	movq	8+_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %r12	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 8B], _45
# /usr/include/c++/13/bits/basic_ios.h:461:       : ios_base(), _M_tie(0), _M_fill(char_type()), _M_fill_init(false), 
	xorl	%esi, %esi	#
# /usr/include/c++/13/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	pxor	%xmm0, %xmm0	# tmp129
# /usr/include/c++/13/bits/basic_ios.h:461:       : ios_base(), _M_tie(0), _M_fill(char_type()), _M_fill_init(false), 
	movw	%si, 480(%rbx)	#, MEM <unsigned short> [(void *)this_7(D) + 480B]
# /usr/include/c++/13/istream:697:       : _M_gcount(streamsize(0))
	movq	16+_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rcx	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 16B], tmp225
# /usr/include/c++/13/istream:698:       { this->init(0); }
	xorl	%esi, %esi	#
# /usr/include/c++/13/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	movups	%xmm0, 488(%rbx)	# tmp129, MEM <vector(2) long unsigned int> [(void *)this_7(D) + 488B]
	movups	%xmm0, 504(%rbx)	# tmp129, MEM <vector(2) long unsigned int> [(void *)this_7(D) + 504B]
# /usr/include/c++/13/istream:697:       : _M_gcount(streamsize(0))
	movq	-24(%r12), %rax	# MEM[(long int *)_45 + -24B], MEM[(long int *)_45 + -24B]
# /usr/include/c++/13/bits/basic_ios.h:462: 	_M_streambuf(0), _M_ctype(0), _M_num_put(0), _M_num_get(0)
	movq	%r15, 256(%rbx)	# tmp202, MEM[(struct basic_ios *)this_7(D) + 256B].D.69952._vptr.ios_base
# /usr/include/c++/13/bits/basic_ios.h:461:       : ios_base(), _M_tie(0), _M_fill(char_type()), _M_fill_init(false), 
	movq	$0, 472(%rbx)	#, MEM[(struct basic_ios *)this_7(D) + 256B]._M_tie
# /usr/include/c++/13/istream:697:       : _M_gcount(streamsize(0))
	movq	%r12, (%rbx)	# _45, MEM[(struct basic_istream *)this_7(D)]._vptr.basic_istream
	movq	%rcx, (%rbx,%rax)	# tmp225, MEM[(struct basic_ios *)_48].D.69952._vptr.ios_base
# /usr/include/c++/13/istream:697:       : _M_gcount(streamsize(0))
	movq	$0, 8(%rbx)	#, MEM[(struct basic_istream *)this_7(D)]._M_gcount
# /usr/include/c++/13/istream:698:       { this->init(0); }
	movq	-24(%r12), %rdi	# MEM[(long int *)_45 + -24B], tmp134
	addq	%rbx, %rdi	# this, tmp134
.LEHB5:
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E@PLT	#
.LEHE5:
# /usr/include/c++/13/fstream:571:       : __istream_type(), _M_filebuf()
	leaq	24+_ZTVSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rax	#, tmp227
# /usr/include/c++/13/fstream:571:       : __istream_type(), _M_filebuf()
	leaq	16(%rbx), %r13	#, _53
# /usr/include/c++/13/fstream:571:       : __istream_type(), _M_filebuf()
	movq	%rax, (%rbx)	# tmp227, MEM[(struct basic_ifstream *)this_7(D)].D.77934._vptr.basic_istream
	addq	$40, %rax	#, tmp228
# /usr/include/c++/13/fstream:571:       : __istream_type(), _M_filebuf()
	movq	%r13, %rdi	# _53,
# /usr/include/c++/13/fstream:571:       : __istream_type(), _M_filebuf()
	movq	%rax, 256(%rbx)	# tmp228, MEM[(struct basic_ios *)this_7(D) + 256B].D.69952._vptr.ios_base
.LEHB6:
# /usr/include/c++/13/fstream:571:       : __istream_type(), _M_filebuf()
	call	_ZNSt13basic_filebufIcSt11char_traitsIcEEC1Ev@PLT	#
.LEHE6:
# /usr/include/c++/13/fstream:573: 	this->init(&_M_filebuf);
	movq	%r13, %rsi	# _53,
	movq	%r14, %rdi	# _43,
.LEHB7:
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE4initEPSt15basic_streambufIcS1_E@PLT	#
# /usr/include/c++/13/fstream:339:       { return open(__s.c_str(), __mode); }
	movq	0(%rbp), %rsi	# MEM[(char * *)file_name_9(D)], MEM[(char * *)file_name_9(D)]
	movl	$12, %edx	#,
	movq	%r13, %rdi	# _53,
	call	_ZNSt13basic_filebufIcSt11char_traitsIcEE4openEPKcSt13_Ios_Openmode@PLT	#
# /usr/include/c++/13/fstream:707: 	  this->setstate(ios_base::failbit);
	movq	(%rbx), %rdx	# MEM[(struct basic_ifstream *)this_7(D)].D.77934._vptr.basic_istream, MEM[(struct basic_ifstream *)this_7(D)].D.77934._vptr.basic_istream
	movq	-24(%rdx), %rdi	# MEM[(long int *)pretmp_60 + -24B], _86
	addq	%rbx, %rdi	# this, _86
# /usr/include/c++/13/fstream:706: 	if (!_M_filebuf.open(__s, __mode | ios_base::in))
	testq	%rax, %rax	# tmp207
	je	.L165	#,
# /usr/include/c++/13/fstream:711: 	  this->clear();
	xorl	%esi, %esi	#
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate@PLT	#
.LEHE7:
.L126:
# /usr/include/c++/13/bits/stl_vector.h:100: 	: _M_start(), _M_finish(), _M_end_of_storage()
	pxor	%xmm0, %xmm0	# tmp143
# /home/chenxi/source/repos/nes/src/nes_file.cpp:7: NesFile::NesFile(std::string file_name): file(file_name, std::ios::binary) {
	xorl	%ecx, %ecx	#
	movb	$0, 568(%rbx)	#, *this_7(D).mapper
	movl	$0, 572(%rbx)	#, *this_7(D).mirroring
	movw	%cx, 576(%rbx)	#, MEM <vector(2) unsigned char> [(bool *)this_7(D) + 576B]
# /usr/include/c++/13/bits/stl_vector.h:100: 	: _M_start(), _M_finish(), _M_end_of_storage()
	movups	%xmm0, 520(%rbx)	# tmp143, MEM <vector(2) long unsigned int> [(unsigned char * *)this_7(D) + 520B]
	movups	%xmm0, 536(%rbx)	# tmp143, MEM <vector(2) long unsigned int> [(unsigned char * *)this_7(D) + 536B]
	movups	%xmm0, 552(%rbx)	# tmp143, MEM <vector(2) long unsigned int> [(unsigned char * *)this_7(D) + 552B]
# /home/chenxi/source/repos/nes/src/nes_file.cpp:8:     if (!file) {
	testb	$5, 288(%rbx)	#, MEM[(const struct basic_ios *)this_7(D) + 256B].D.69952._M_streambuf_state
	jne	.L166	#,
# /home/chenxi/source/repos/nes/src/nes_file.cpp:11:     read();
	movq	%rbx, %rdi	# this,
.LEHB8:
	call	_ZN6nes_em7NesFile4readEv	#
.LEHE8:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:12: }
	movq	56(%rsp), %rax	# D.95948, tmp223
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp223
	jne	.L167	#,
	addq	$72, %rsp	#,
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx	#
	.cfi_def_cfa_offset 48
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
	ret	
	.p2align 4,,10
	.p2align 3
.L165:
	.cfi_restore_state
# /usr/include/c++/13/bits/ios_base.h:171:   { return _Ios_Iostate(static_cast<int>(__a) | static_cast<int>(__b)); }
	movl	32(%rdi), %esi	# MEM[(const struct basic_ios *)_86].D.69952._M_streambuf_state, tmp141
	orl	$4, %esi	#, tmp141
.LEHB9:
# /usr/include/c++/13/bits/basic_ios.h:158:       { this->clear(this->rdstate() | __state); }
	call	_ZNSt9basic_iosIcSt11char_traitsIcEE5clearESt12_Ios_Iostate@PLT	#
.LEHE9:
	jmp	.L126	#
.L167:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:12: }
	call	__stack_chk_fail@PLT	#
.L151:
	endbr64	
# /usr/include/c++/13/bits/basic_ios.h:282:       ~basic_ios() { }
	movq	%rax, %rbp	# tmp210, tmp154
	jmp	.L131	#
.L153:
	endbr64	
# /usr/include/c++/13/fstream:575:       }
	movq	%rax, %rbp	# tmp208, tmp150
	jmp	.L129	#
.L158:
	endbr64	
	jmp	.L159	#
.L152:
	endbr64	
# /usr/include/c++/13/istream:106:       { _M_gcount = streamsize(0); }
	movq	%rax, %rbp	# tmp209, tmp149
	jmp	.L130	#
.L166:
	jmp	.L127	#
	.section	.gcc_except_table
.LLSDA3432:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE3432-.LLSDACSB3432
.LLSDACSB3432:
	.uleb128 .LEHB5-.LFB3432
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L151-.LFB3432
	.uleb128 0
	.uleb128 .LEHB6-.LFB3432
	.uleb128 .LEHE6-.LEHB6
	.uleb128 .L152-.LFB3432
	.uleb128 0
	.uleb128 .LEHB7-.LFB3432
	.uleb128 .LEHE7-.LEHB7
	.uleb128 .L153-.LFB3432
	.uleb128 0
	.uleb128 .LEHB8-.LFB3432
	.uleb128 .LEHE8-.LEHB8
	.uleb128 .L158-.LFB3432
	.uleb128 0
	.uleb128 .LEHB9-.LFB3432
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L153-.LFB3432
	.uleb128 0
.LLSDACSE3432:
	.text
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC3432
	.type	_ZN6nes_em7NesFileC2ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE.cold, @function
_ZN6nes_em7NesFileC2ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE.cold:
.LFSB3432:
.L129:
	.cfi_def_cfa_offset 128
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
# /usr/include/c++/13/fstream:575:       }
	movq	%r13, %rdi	# _53,
	call	_ZNSt13basic_filebufIcSt11char_traitsIcEED1Ev@PLT	#
.L130:
# /usr/include/c++/13/istream:106:       { _M_gcount = streamsize(0); }
	movq	-24(%r12), %rax	# MEM[(long int *)_45 + -24B], MEM[(long int *)_45 + -24B]
	movq	16+_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE(%rip), %rcx	# MEM[(const void * *)&_ZTTSt14basic_ifstreamIcSt11char_traitsIcEE + 16B], tmp231
# /usr/include/c++/13/istream:106:       { _M_gcount = streamsize(0); }
	xorl	%edx, %edx	#
# /usr/include/c++/13/istream:106:       { _M_gcount = streamsize(0); }
	movq	%r12, (%rbx)	# _45, MEM[(struct basic_istream *)this_7(D)]._vptr.basic_istream
	movq	%rcx, (%rbx,%rax)	# tmp231, MEM[(struct basic_ios *)_58].D.69952._vptr.ios_base
# /usr/include/c++/13/istream:106:       { _M_gcount = streamsize(0); }
	movq	%rdx, 8(%rbx)	#, MEM[(struct basic_istream *)this_7(D)]._M_gcount
.L131:
# /usr/include/c++/13/bits/basic_ios.h:282:       ~basic_ios() { }
	movq	%r15, 256(%rbx)	# tmp202, MEM[(struct basic_ios *)this_7(D) + 256B].D.69952._vptr.ios_base
	movq	%r14, %rdi	# _43,
	call	_ZNSt8ios_baseD2Ev@PLT	#
	movq	56(%rsp), %rax	# D.95948, tmp218
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp218
	jne	.L168	#,
	movq	%rbp, %rdi	# tmp154,
.LEHB10:
	call	_Unwind_Resume@PLT	#
.L149:
	endbr64	
.L159:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:12: }
	movq	%rax, %rbp	# tmp215, tmp192
.L146:
	leaq	544(%rbx), %rdi	#, tmp194
	call	_ZNSt6vectorIhSaIhEED1Ev	#
	leaq	520(%rbx), %rdi	#, vectp.197
	call	_ZNSt6vectorIhSaIhEED1Ev	#
	movq	%rbx, %rdi	# this,
	call	_ZNSt14basic_ifstreamIcSt11char_traitsIcEED1Ev@PLT	#
	movq	56(%rsp), %rax	# D.95948, tmp222
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp222
	jne	.L169	#,
	movq	%rbp, %rdi	# tmp192,
	call	_Unwind_Resume@PLT	#
.LEHE10:
.L127:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:9:         throw std::runtime_error("failed to open nes file: " + file_name);
	movl	$16, %edi	#,
# /usr/include/c++/13/bits/basic_string.h:189: 	: allocator_type(__a), _M_p(__dat) { }
	leaq	32(%rsp), %r15	#, tmp203
# /home/chenxi/source/repos/nes/src/nes_file.cpp:9:         throw std::runtime_error("failed to open nes file: " + file_name);
	call	__cxa_allocate_exception@PLT	#
	movq	8(%rbp), %r14	# MEM[(long unsigned int *)file_name_9(D) + 8B], _42
# /usr/include/c++/13/bits/basic_string.h:189: 	: allocator_type(__a), _M_p(__dat) { }
	movq	%r15, 16(%rsp)	# tmp203, MEM[(struct _Alloc_hider *)&D.86151]._M_p
# /home/chenxi/source/repos/nes/src/nes_file.cpp:9:         throw std::runtime_error("failed to open nes file: " + file_name);
	movq	%rax, %r13	# tmp211, _18
	movq	0(%rbp), %rax	# MEM[(char * *)file_name_9(D)], _41
# /usr/include/c++/13/bits/char_traits.h:358: 	__c1 = __c2;
	movb	$0, 32(%rsp)	#, MEM[(char_type &)&D.86151 + 16]
# /usr/include/c++/13/bits/basic_string.h:3537:       __str.reserve(__lhs_len + __rhs_len);
	leaq	25(%r14), %r12	#, _80
	movq	%rax, (%rsp)	# _41, %sfp
# /usr/include/c++/13/bits/basic_string.h:218:       { _M_string_length = __length; }
	xorl	%eax, %eax	#
	movq	%rax, 24(%rsp)	#, D.86151._M_string_length
# /usr/include/c++/13/bits/basic_string.tcc:312:       if (__res <= __capacity)
	cmpq	$15, %r12	#, _80
	ja	.L170	#,
.L133:
# /usr/include/c++/13/bits/basic_string.h:3538:       __str.append(__lhs, __lhs_len);
	leaq	16(%rsp), %rbp	#, tmp204
	movl	$25, %edx	#,
	leaq	.LC6(%rip), %rsi	#, tmp176
	movq	%rbp, %rdi	# tmp204,
.LEHB11:
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6appendEPKcm.isra.0	#
# /usr/include/c++/13/bits/basic_string.h:3539:       __str.append(__rhs, __rhs_len);
	movq	(%rsp), %rsi	# %sfp,
	movq	%r14, %rdx	# _42,
	movq	%rbp, %rdi	# tmp204,
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE6appendEPKcm.isra.0	#
.LEHE11:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:9:         throw std::runtime_error("failed to open nes file: " + file_name);
	movq	%rbp, %rsi	# tmp204,
	movq	%r13, %rdi	# _18,
.LEHB12:
	call	_ZNSt13runtime_errorC1ERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE@PLT	#
.LEHE12:
# /usr/include/c++/13/bits/basic_string.h:804:       { _M_dispose(); }
	movq	%rbp, %rdi	# tmp204,
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv	#
# /home/chenxi/source/repos/nes/src/nes_file.cpp:9:         throw std::runtime_error("failed to open nes file: " + file_name);
	movq	56(%rsp), %rax	# D.95948, tmp221
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp221
	jne	.L171	#,
	movq	_ZNSt13runtime_errorD1Ev@GOTPCREL(%rip), %rdx	#,
	leaq	_ZTISt13runtime_error(%rip), %rsi	#, tmp188
	movq	%r13, %rdi	# _18,
.LEHB13:
	call	__cxa_throw@PLT	#
.LEHE13:
.L170:
# /usr/include/c++/13/bits/basic_string.tcc:143:       if (__capacity > max_size())
	testq	%r12, %r12	# _80
	jns	.L134	#,
# /usr/include/c++/13/bits/basic_string.tcc:144: 	std::__throw_length_error(__N("basic_string::_M_create"));
	movq	56(%rsp), %rax	# D.95948, tmp219
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp219
	jne	.L172	#,
	leaq	.LC1(%rip), %rdi	#, tmp162
	leaq	16(%rsp), %rbp	#, tmp204
.LEHB14:
	call	_ZSt20__throw_length_errorPKc@PLT	#
.L134:
# /usr/include/c++/13/bits/basic_string.tcc:149:       if (__capacity > __old_capacity && __capacity < 2 * __old_capacity)
	movl	$30, %eax	#, tmp216
	cmpq	%rax, %r12	# tmp216, _80
	cmovb	%rax, %r12	# _80,, tmp216, _118
# /usr/include/c++/13/bits/new_allocator.h:134: 	if (__builtin_expect(__n > this->_M_max_size(), false))
	movq	%r12, %rdi	# _118, _98
	addq	$1, %rdi	#, _98
	js	.L173	#,
	leaq	16(%rsp), %rbp	#, tmp204
# /usr/include/c++/13/bits/new_allocator.h:151: 	return static_cast<_Tp*>(_GLIBCXX_OPERATOR_NEW(__n * sizeof(_Tp)));
	call	_Znwm@PLT	#
	movq	%rax, %rbp	# tmp212, _104
# /usr/include/c++/13/bits/basic_string.tcc:316:       this->_S_copy(__tmp, _M_data(), length() + 1);
	movq	24(%rsp), %rax	# D.86151._M_string_length, D.86151._M_string_length
# /usr/include/c++/13/bits/basic_string.h:223:       { return _M_dataplus._M_p; }
	movq	16(%rsp), %rcx	# D.86151._M_dataplus._M_p, _90
# /usr/include/c++/13/bits/basic_string.tcc:316:       this->_S_copy(__tmp, _M_data(), length() + 1);
	leaq	1(%rax), %rdx	#, _89
# /usr/include/c++/13/bits/basic_string.h:427: 	if (__n == 1)
	testq	%rax, %rax	# D.86151._M_string_length
	jne	.L138	#,
# /usr/include/c++/13/bits/char_traits.h:358: 	__c1 = __c2;
	movzbl	(%rcx), %eax	# MEM[(const char_type &)_90], _91
# /usr/include/c++/13/bits/char_traits.h:358: 	__c1 = __c2;
	movb	%al, 0(%rbp)	# _91, MEM[(char_type &)_104]
.L139:
# /usr/include/c++/13/bits/basic_string.h:264: 	if (_M_data() == _M_local_data())
	cmpq	%r15, %rcx	# tmp203, _90
	je	.L140	#,
# /usr/include/c++/13/bits/basic_string.h:289:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	movq	32(%rsp), %rax	# D.86151.D.47633._M_allocated_capacity, tmp234
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	movq	%rcx, %rdi	# _90,
# /usr/include/c++/13/bits/basic_string.h:289:       { _Alloc_traits::deallocate(_M_get_allocator(), _M_data(), __size + 1); }
	leaq	1(%rax), %rsi	#, tmp173
# /usr/include/c++/13/bits/new_allocator.h:172: 	_GLIBCXX_OPERATOR_DELETE(_GLIBCXX_SIZED_DEALLOC(__p, __n));
	call	_ZdlPvm@PLT	#
.L140:
# /usr/include/c++/13/bits/basic_string.h:213:       { _M_dataplus._M_p = __p; }
	movq	%rbp, 16(%rsp)	# _104, D.86151._M_dataplus._M_p
# /usr/include/c++/13/bits/basic_string.h:250:       { _M_allocated_capacity = __capacity; }
	movq	%r12, 32(%rsp)	# _118, D.86151.D.47633._M_allocated_capacity
# /usr/include/c++/13/bits/basic_string.h:250:       { _M_allocated_capacity = __capacity; }
	jmp	.L133	#
.L173:
# /usr/include/c++/13/bits/new_allocator.h:140: 	    std::__throw_bad_alloc();
	movq	56(%rsp), %rax	# D.95948, tmp220
	subq	%fs:40, %rax	# MEM[(<address-space-1> long unsigned int *)40B], tmp220
	jne	.L174	#,
	leaq	16(%rsp), %rbp	#, tmp204
	call	_ZSt17__throw_bad_allocv@PLT	#
.LEHE14:
.L138:
# /usr/include/c++/13/bits/char_traits.h:429: 	if (__n == 0)
	testq	%rdx, %rdx	# _89
	je	.L139	#,
# /usr/include/c++/13/bits/char_traits.h:435: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	movq	%rcx, %rsi	# _90,
	movq	%rbp, %rdi	# _104,
	movq	%rcx, 8(%rsp)	# _90, %sfp
	call	memcpy@PLT	#
	movq	8(%rsp), %rcx	# %sfp, _90
# /usr/include/c++/13/bits/char_traits.h:435: 	return static_cast<char_type*>(__builtin_memcpy(__s1, __s2, __n));
	jmp	.L139	#
.L172:
# /usr/include/c++/13/bits/basic_string.tcc:144: 	std::__throw_length_error(__N("basic_string::_M_create"));
	call	__stack_chk_fail@PLT	#
.L174:
# /usr/include/c++/13/bits/new_allocator.h:140: 	    std::__throw_bad_alloc();
	call	__stack_chk_fail@PLT	#
.L171:
# /home/chenxi/source/repos/nes/src/nes_file.cpp:9:         throw std::runtime_error("failed to open nes file: " + file_name);
	call	__stack_chk_fail@PLT	#
.L150:
	endbr64	
.L164:
# /usr/include/c++/13/bits/basic_string.h:804:       { _M_dispose(); }
	movq	%rbp, %rdi	# tmp204,
	movq	%rax, %r12	# tmp214, tmp190
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv	#
# /home/chenxi/source/repos/nes/src/nes_file.cpp:9:         throw std::runtime_error("failed to open nes file: " + file_name);
	movq	%r13, %rdi	# _18,
	movq	%r12, %rbp	# tmp179, tmp192
	call	__cxa_free_exception@PLT	#
	jmp	.L146	#
.L154:
	endbr64	
	jmp	.L164	#
.L168:
	call	__stack_chk_fail@PLT	#
.L169:
	call	__stack_chk_fail@PLT	#
	.cfi_endproc
.LFE3432:
	.section	.gcc_except_table
.LLSDAC3432:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC3432-.LLSDACSBC3432
.LLSDACSBC3432:
	.uleb128 .LEHB10-.LCOLDB7
	.uleb128 .LEHE10-.LEHB10
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB11-.LCOLDB7
	.uleb128 .LEHE11-.LEHB11
	.uleb128 .L154-.LCOLDB7
	.uleb128 0
	.uleb128 .LEHB12-.LCOLDB7
	.uleb128 .LEHE12-.LEHB12
	.uleb128 .L150-.LCOLDB7
	.uleb128 0
	.uleb128 .LEHB13-.LCOLDB7
	.uleb128 .LEHE13-.LEHB13
	.uleb128 .L149-.LCOLDB7
	.uleb128 0
	.uleb128 .LEHB14-.LCOLDB7
	.uleb128 .LEHE14-.LEHB14
	.uleb128 .L154-.LCOLDB7
	.uleb128 0
.LLSDACSEC3432:
	.section	.text.unlikely
	.text
	.size	_ZN6nes_em7NesFileC2ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE, .-_ZN6nes_em7NesFileC2ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	.section	.text.unlikely
	.size	_ZN6nes_em7NesFileC2ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE.cold, .-_ZN6nes_em7NesFileC2ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE.cold
.LCOLDE7:
	.text
.LHOTE7:
	.globl	_ZN6nes_em7NesFileC1ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	.set	_ZN6nes_em7NesFileC1ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE,_ZN6nes_em7NesFileC2ENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
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
