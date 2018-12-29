#pragma once
#ifndef _wsp_sdk_
#define _wsp_sdk_

#define MAPO_PROTECT_BYTES_START	"\x50\x8D\x80\x55\x44\x33\x00\x8D\x80\x55\x44\x33\x00\x58"
#define MAPO_PROTECT_BYTES_END		"\x50\x8D\x80\x88\x77\x66\x00\x8D\x80\x88\x77\x66\x00\x58"

#ifdef __BORLANDC__

#define MAPO_PROTECT_START	__emit__ (0x50, 0x8D, 0x80, 0x55, 0x44, 0x33, 0x00, 0x8D, 0x80, 0x55, 0x44, 0x33, 0x00, 0x58 );
#define MAPO_PROTECT_END	__emit__ (0x50, 0x8D, 0x80, 0x88, 0x77, 0x66, 0x00, 0x8D, 0x80, 0x88, 0x77, 0x66, 0x00, 0x58 );

#else

#define MAPO_PROTECT_START __asm\
	{\
	__asm _emit 0x50\
	__asm _emit 0x8D\
	__asm _emit 0x80\
	__asm _emit 0x55\
	__asm _emit 0x44\
	__asm _emit 0x33\
	__asm _emit 0x00\
	__asm _emit 0x8D\
	__asm _emit 0x80\
	__asm _emit 0x55\
	__asm _emit 0x44\
	__asm _emit 0x33\
	__asm _emit 0x00\
	__asm _emit 0x58\
};

#define MAPO_PROTECT_END __asm\
	{\
	__asm _emit 0x50\
	__asm _emit 0x8D\
	__asm _emit 0x80\
	__asm _emit 0x88\
	__asm _emit 0x77\
	__asm _emit 0x66\
	__asm _emit 0x00\
	__asm _emit 0x8D\
	__asm _emit 0x80\
	__asm _emit 0x88\
	__asm _emit 0x77\
	__asm _emit 0x66\
	__asm _emit 0x00\
	__asm _emit 0x58\
};

//¬È∆≈‘∆º”√‹ø™ º
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

//¬È∆≈‘∆º”√‹Ω· ¯
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

#endif

#endif
