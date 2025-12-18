#include "comm.h"
#include "protocol.h"
#include "LoRa.h"
#include "../Core/Inc/main.h"

extern SPI_HandleTypeDef hspi1;

/* LoRa instance */
static LoRa lora;

/* RX state */
static CommPacket_t rx_pkt;
static bool rx_ready = false;

void COMM_Init(void)
{
lora.CS_port		= NSS_GPIO_Port;
lora.CS_pin		= NSS_Pin;
lora.reset_port		= LoRa_RST_GPIO_Port;
lora.reset_pin		= LoRa_RST_Pin;
lora.DIO0_port		= DIO0_GPIO_Port;
lora.DIO0_pin		= DIO0_Pin;
lora.hSPIx		= &hspi1;

    LoRa_init(&lora);
    LoRa_startReceiving(&lora);
}

void COMM_Process(void)
{
    uint8_t buf[32];
    uint8_t len = LoRa_receive(&lora, buf, sizeof(buf));

    if (len == 0)
        return;

    ProtocolPacketType_t t = PROTOCOL_GetType(buf);

    if (t == PKT_LOCATION)
    {
        if (PROTOCOL_DecodeLocation(buf, len,
                                    &rx_pkt.latitude,
                                    &rx_pkt.longitude,
                                    &rx_pkt.sender_id))
        {
            rx_pkt.type = COMM_LOCATION;
            rx_ready = true;
        }
    }
    else if (t == PKT_SCAN_REQUEST)
    {
        rx_pkt.type = COMM_SCAN_REQUEST;
        rx_pkt.sender_id = buf[1];
        rx_ready = true;
    }

    LoRa_startReceiving(&lora);
}

bool COMM_SendLocation(float lat, float lon, uint8_t id)
{
    uint8_t buf[32];
    uint8_t len = PROTOCOL_EncodeLocation(buf, lat, lon, id);

    return (LoRa_transmit(&lora, buf, len, 100) == 1);
}

bool COMM_SendScanRequest(uint8_t id)
{
    uint8_t buf[8];
    uint8_t len = PROTOCOL_EncodeScanRequest(buf, id);

    return (LoRa_transmit(&lora, buf, len, 100) == 1);
}

bool COMM_HasPacket(void)
{
    return rx_ready;
}

bool COMM_GetPacket(CommPacket_t *pkt)
{
    if (!rx_ready || pkt == NULL)
        return false;

    *pkt = rx_pkt;
    rx_ready = false;
    return true;
}

