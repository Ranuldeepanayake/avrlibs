	.file	"main.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.section	.text.blink13,"ax",@progbits
.global	blink13
	.type	blink13, @function
blink13:
.LFB7:
	.file 1 ".././main.c"
	.loc 1 27 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 1 28 0
	ldi r24,lo8(32)
	out 0x5,r24
.LVL0:
.LBB10:
.LBB11:
	.file 2 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\util\\delay.h"
	.loc 2 187 0
	ldi r18,lo8(1599999)
	ldi r24,hi8(1599999)
	ldi r25,hlo8(1599999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
.LBE11:
.LBE10:
	.loc 1 30 0
	out 0x5,__zero_reg__
.LVL1:
.LBB12:
.LBB13:
	.loc 2 187 0
	ldi r18,lo8(1599999)
	ldi r24,hi8(1599999)
	ldi r25,hlo8(1599999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	ret
.LBE13:
.LBE12:
	.cfi_endproc
.LFE7:
	.size	blink13, .-blink13
	.section	.text.main,"ax",@progbits
.global	main
	.type	main, @function
main:
.LFB6:
	.loc 1 17 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	.loc 1 19 0
	sbi 0x4,5
.L3:
	.loc 1 23 0 discriminator 1
	call blink13
.LVL2:
	.loc 1 24 0 discriminator 1
	rjmp .L3
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.text.blink13Advanced,"ax",@progbits
.global	blink13Advanced
	.type	blink13Advanced, @function
blink13Advanced:
.LFB8:
	.loc 1 34 0
	.cfi_startproc
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
.LVL3:
	.loc 1 46 0
	ldi r24,lo8(32)
	out 0x5,r24
.LVL4:
.LBB14:
.LBB15:
	.loc 2 187 0
	ldi r18,lo8(1599999)
	ldi r24,hi8(1599999)
	ldi r25,hlo8(1599999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
.LBE15:
.LBE14:
	.loc 1 48 0
	out 0x5,__zero_reg__
.LVL5:
.LBB16:
.LBB17:
	.loc 2 187 0
	ldi r18,lo8(1599999)
	ldi r24,hi8(1599999)
	ldi r25,hlo8(1599999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	ret
.LBE17:
.LBE16:
	.cfi_endproc
.LFE8:
	.size	blink13Advanced, .-blink13Advanced
	.text
.Letext0:
	.file 3 "c:\\program files (x86)\\arduino\\hardware\\tools\\avr\\avr\\include\\stdint.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x208
	.word	0x2
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF13
	.byte	0x1
	.long	.LASF14
	.long	.LASF15
	.long	.Ldebug_ranges0+0
	.long	0
	.long	0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF0
	.uleb128 0x3
	.long	.LASF4
	.byte	0x3
	.byte	0x7e
	.long	0x3b
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF1
	.uleb128 0x4
	.byte	0x2
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.long	.LASF3
	.uleb128 0x3
	.long	.LASF5
	.byte	0x3
	.byte	0x82
	.long	0x62
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF6
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF7
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF8
	.uleb128 0x5
	.long	.LASF16
	.byte	0x2
	.byte	0xa6
	.byte	0x1
	.byte	0x3
	.long	0xb0
	.uleb128 0x6
	.long	.LASF17
	.byte	0x2
	.byte	0xa6
	.long	0xb0
	.uleb128 0x7
	.long	.LASF9
	.byte	0x2
	.byte	0xa8
	.long	0xb0
	.uleb128 0x7
	.long	.LASF10
	.byte	0x2
	.byte	0xac
	.long	0x57
	.uleb128 0x8
	.byte	0x1
	.long	.LASF18
	.byte	0x2
	.byte	0xad
	.byte	0x1
	.byte	0
	.byte	0
	.uleb128 0x2
	.byte	0x4
	.byte	0x4
	.long	.LASF11
	.uleb128 0x9
	.byte	0x1
	.long	.LASF19
	.byte	0x1
	.byte	0x1b
	.long	.LFB7
	.long	.LFE7
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x143
	.uleb128 0xa
	.long	0x77
	.long	.LBB10
	.long	.LBE10
	.byte	0x1
	.byte	0x1d
	.long	0x10b
	.uleb128 0xb
	.long	0x84
	.byte	0x4
	.long	0x447a0000
	.uleb128 0xc
	.long	.LBB11
	.long	.LBE11
	.uleb128 0xd
	.long	0x8f
	.byte	0x4
	.long	0x4af42400
	.uleb128 0xe
	.long	0x9a
	.long	0x7a1200
	.byte	0
	.byte	0
	.uleb128 0xf
	.long	0x77
	.long	.LBB12
	.long	.LBE12
	.byte	0x1
	.byte	0x1f
	.uleb128 0xb
	.long	0x84
	.byte	0x4
	.long	0x447a0000
	.uleb128 0xc
	.long	.LBB13
	.long	.LBE13
	.uleb128 0xd
	.long	0x8f
	.byte	0x4
	.long	0x4af42400
	.uleb128 0xe
	.long	0x9a
	.long	0x7a1200
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x10
	.byte	0x1
	.long	.LASF20
	.byte	0x1
	.byte	0x10
	.byte	0x1
	.long	0x42
	.long	.LFB6
	.long	.LFE6
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.long	0x16b
	.uleb128 0x11
	.long	.LVL2
	.long	0xb7
	.byte	0
	.uleb128 0x12
	.byte	0x1
	.long	.LASF21
	.byte	0x1
	.byte	0x22
	.long	.LFB8
	.long	.LFE8
	.byte	0x3
	.byte	0x92
	.uleb128 0x20
	.sleb128 2
	.byte	0x1
	.uleb128 0x13
	.long	.LASF12
	.byte	0x1
	.byte	0x2a
	.long	0x30
	.byte	0x20
	.uleb128 0x14
	.string	"low"
	.byte	0x1
	.byte	0x2b
	.long	0x30
	.byte	0
	.uleb128 0xa
	.long	0x77
	.long	.LBB14
	.long	.LBE14
	.byte	0x1
	.byte	0x2f
	.long	0x1d3
	.uleb128 0xb
	.long	0x84
	.byte	0x4
	.long	0x447a0000
	.uleb128 0xc
	.long	.LBB15
	.long	.LBE15
	.uleb128 0xd
	.long	0x8f
	.byte	0x4
	.long	0x4af42400
	.uleb128 0xe
	.long	0x9a
	.long	0x7a1200
	.byte	0
	.byte	0
	.uleb128 0xf
	.long	0x77
	.long	.LBB16
	.long	.LBE16
	.byte	0x1
	.byte	0x31
	.uleb128 0xb
	.long	0x84
	.byte	0x4
	.long	0x447a0000
	.uleb128 0xc
	.long	.LBB17
	.long	.LBE17
	.uleb128 0xd
	.long	0x8f
	.byte	0x4
	.long	0x4af42400
	.uleb128 0xe
	.long	0x9a
	.long	0x7a1200
	.byte	0
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x55
	.uleb128 0x6
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x52
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x20
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x34
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0xa
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x34
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0xa
	.uleb128 0x2117
	.uleb128 0xc
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.word	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0
	.word	0
	.word	0
	.long	.LFB7
	.long	.LFE7-.LFB7
	.long	.LFB6
	.long	.LFE6-.LFB6
	.long	.LFB8
	.long	.LFE8-.LFB8
	.long	0
	.long	0
	.section	.debug_ranges,"",@progbits
.Ldebug_ranges0:
	.long	.LFB7
	.long	.LFE7
	.long	.LFB6
	.long	.LFE6
	.long	.LFB8
	.long	.LFE8
	.long	0
	.long	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF14:
	.string	".././main.c"
.LASF12:
	.string	"high"
.LASF18:
	.string	"__builtin_avr_delay_cycles"
.LASF19:
	.string	"blink13"
.LASF1:
	.string	"unsigned char"
.LASF6:
	.string	"long unsigned int"
.LASF16:
	.string	"_delay_ms"
.LASF11:
	.string	"double"
.LASF20:
	.string	"main"
.LASF2:
	.string	"unsigned int"
.LASF8:
	.string	"long long unsigned int"
.LASF4:
	.string	"uint8_t"
.LASF13:
	.string	"GNU C 4.9.2 -fpreprocessed -mn-flash=1 -mno-skip-bug -mmcu=avr5 -g2 -O1 -std=gnu99 -funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections -fpack-struct -fshort-enums"
.LASF7:
	.string	"long long int"
.LASF10:
	.string	"__ticks_dc"
.LASF15:
	.string	"c:\\\\users\\\\ranul\\\\Documents\\\\Atmel Studio\\\\7.0\\\\Tutorial\\\\Tutorial\\\\Debug"
.LASF9:
	.string	"__tmp"
.LASF5:
	.string	"uint32_t"
.LASF3:
	.string	"long int"
.LASF0:
	.string	"signed char"
.LASF17:
	.string	"__ms"
.LASF21:
	.string	"blink13Advanced"
	.ident	"GCC: (GNU) 4.9.2"
