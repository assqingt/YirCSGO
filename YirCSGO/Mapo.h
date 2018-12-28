#pragma once
//ÂéÆÅÔÆ¼ÓÃÜ¿ªÊ¼
#define MAPO_VM_START __asm\
{\
	__asm _emit 0x50\
	__asm _emit 0x8D\
	__asm _emit 0x80\
	__asm _emit 0x11\
	__asm _emit 0x44\
	__asm _emit 0x33\
	__asm _emit 0x00\
	__asm _emit 0x8D\
	__asm _emit 0x80\
	__asm _emit 0x11\
	__asm _emit 0x44\
	__asm _emit 0x33\
	__asm _emit 0x00\
	__asm _emit 0x58\
};

//ÂéÆÅÔÆ¼ÓÃÜ½áÊø
#define MAPO_VM_END __asm\
{\
	__asm _emit 0x50\
	__asm _emit 0x8D\
	__asm _emit 0x80\
	__asm _emit 0x22\
	__asm _emit 0x44\
	__asm _emit 0x33\
	__asm _emit 0x00\
	__asm _emit 0x8D\
	__asm _emit 0x80\
	__asm _emit 0x22\
	__asm _emit 0x44\
	__asm _emit 0x33\
	__asm _emit 0x00\
	__asm _emit 0x58\
};