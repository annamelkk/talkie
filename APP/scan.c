#include "scan.h"
#include "comm.h"
#include "stm32f1xx_hal.h"

#define SCAN_DURATION_MS	10000
#define SCAN_TX_PERIOD		5000
//#define NODE_ID			1
#define	NODE_ID			2

static bool scan_active = false;
static uint32_t scan_start_time = 0;
static uint32_t last_tx_time = 0;

void SCAN_Init(void)
{
    scan_active = false;
}

void SCAN_Start(void)
{
    scan_active = true;
    scan_start_time = HAL_GetTick();
    last_tx_time = 0;
}

bool SCAN_IsActive(void)
{
    return scan_active;
}

void SCAN_Process(void)
{
    if (!scan_active)
        return;

    uint32_t now = HAL_GetTick();

    /* Periodic scan broadcast */
    if (now - last_tx_time > SCAN_TX_PERIOD)
    {
        last_tx_time = now;
        COMM_SendScan(NODE_ID);  // sender ID (temporary)
    }

    /* Stop scan */
    if (now - scan_start_time > SCAN_DURATION_MS)
    {
        scan_active = false;
    }
}

