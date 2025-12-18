#include "comm.h"
#include "LoRa.h"
#include "../Core/Inc/main.h"
#include <stdio.h>
#include <string.h>

extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart2; // Use your debug UART

/* LoRa instance */
LoRa lora;

/* RX state */
static uint8_t rx_type = 0;
static uint8_t rx_sender = 0;
static bool rx_ready = false;

// Helper to send strings to PC
void DEBUG_Log(char* msg) {
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}

uint16_t COMM_Init(void) {
    lora.CS_port    = NSS_GPIO_Port;
    lora.CS_pin     = NSS_Pin;
    lora.reset_port = LoRa_RST_GPIO_Port;
    lora.reset_pin  = LoRa_RST_Pin;
    lora.DIO0_port  = DIO0_GPIO_Port;
    lora.DIO0_pin   = DIO0_Pin;
    lora.hSPIx      = &hspi1;
    
    lora.frequency      = 434;
    lora.spredingFactor = SF_9;
    lora.bandWidth      = BW_250KHz;
    lora.crcRate        = CR_4_8;
    lora.power          = POWER_17db;
    lora.overCurrentProtection = 130;
    lora.preamble       = 10;

    uint16_t status = LoRa_init(&lora);
    
    if (status == LORA_OK) {
        DEBUG_Log("DEBUG: LoRa Initialized OK\r\n");
        LoRa_startReceiving(&lora);
    } else {
        char buf[50];
        sprintf(buf, "DEBUG: LoRa Init Failed Code: %d\r\n", status);
        DEBUG_Log(buf);
    }
    return status;
}

void COMM_Process(void) {
    uint8_t buf[8];
    uint8_t len = LoRa_receive(&lora, buf, sizeof(buf));

    if (len > 0) {
        char msg[50];
        sprintf(msg, "DEBUG: Received %d bytes from ID %d\r\n", len, buf[1]);
        DEBUG_Log(msg);
        
        if (len < 2) return;

        rx_type = buf[0];
        rx_sender = buf[1];
        rx_ready = true;

        LoRa_startReceiving(&lora);
    }
}

bool COMM_SendTest(uint8_t id) {
    DEBUG_Log("DEBUG: Attempting to send TEST packet...\r\n");
    uint8_t buf[2] = { MSG_TEST, id };
    bool ok = (LoRa_transmit(&lora, buf, 2, 100) == 1);
    
    if (ok) DEBUG_Log("DEBUG: Send SUCCESS\r\n");
    else DEBUG_Log("DEBUG: Send FAILED\r\n");

    LoRa_startReceiving(&lora);
    return ok;
}

bool COMM_SendScan(uint8_t id)
{
	uint8_t buf[2] = { MSG_SCAN, id };
	bool ok = (LoRa_transmit(&lora, buf, 2, 100) == 1);
	LoRa_startReceiving(&lora);
	return ok;
}

bool COMM_HasMessage(void)
{
	return rx_ready;
}

uint8_t COMM_GetMessage(uint8_t *sender_id)
{
	rx_ready = false;
	*sender_id = rx_sender;
	return rx_type;
}

