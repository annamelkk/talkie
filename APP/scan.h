#ifndef SCAN_H
#define SCAN_H

#include <stdbool.h>

void SCAN_Init(void);
void SCAN_Start(void);
void SCAN_Stop(void);
void SCAN_Process(void);

bool SCAN_IsActive(void);

#endif

