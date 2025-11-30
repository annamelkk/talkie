#ifndef SCAN_H
#define SCAN_H

#include <stdbool.h>

void	Scan_init(void);

void	scan_start(void);
void	scan_stop(void);

bool	scan_is_active(void);
void	scan_process(void);


#endif
