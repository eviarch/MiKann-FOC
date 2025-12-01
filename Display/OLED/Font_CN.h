#ifndef __FONT_CN_H
#define __FONT_CN_H

#include "user_def.h"

#define Enable_Cn16Char		1		// 是否启用 16*16 汉字，若启用，oled.c 中对应的显示函数也将启用
#define Enable_Cn32Char		0		// 是否启用 32*32 汉字，若启用，oled.c 中对应的显示函数也将启用

/*------------------------------------- 16*16 汉字 -----------------------------------*/
#if Enable_Cn16Char

	#define Cn16Char_SUMMARY 7

	struct Cn16CharTypedef {
		char Index[3];
		u8 ImageData[32];
	};

	/**
	 * 已有汉字：
	 *  欢迎，起终点，科目
	 */
	extern struct Cn16CharTypedef Cn16Char[Cn16Char_SUMMARY];

#endif

/*------------------------------------- 32*32 汉字 -----------------------------------*/
#if Enable_Cn32Char

	#define Cn32Char_SUMMARY 9

	struct Cn32CharTypedef
	{
		char Index[3];
		u8 ImageData[128];
	};

	/**
	 * 已有汉字：彭宇暄，岳子康，周玄凌
	 */
	extern struct Cn32CharTypedef Cn32Char[Cn32Char_SUMMARY];

#endif

#endif // !__FONT_CN_H
