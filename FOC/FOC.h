#ifndef __FOC_H
#define __FOC_H

#include "main.h"

void FOC_Init(void);
void velocityOpenloop(float target_velocity);
void FOC_Task(void);

#endif