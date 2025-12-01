#ifndef __PAGE_MAIN_H
#define __PAGE_MAIN_H

#include "Display.h"

#define PageMain_Ptr_MAX     ((u8)(PAGE_TOTAL - 1))   // 主页面的页面指针最大值


/*------------------------------- Exported Vars & Funcs -----------------------------*/

extern u8 Page_Main_Ptr;

void Page_Main_Subtrate(void);
void Page_Main_Task(void);


/*---------------------------------------------- User Determine -------------------------------------------*/

void Page_Main_Key_ShortPress(void);
void Page_Main_Key_LongPress(void);
void Page_Main_Key_DoublePress(void);

#endif // !__PAGE_MAIN_H
