## Talkie device with LoRa

**This project is software implementation for LoRa communication device implemented with STM32F103 NUCLEO board. The main function of the device is to send and receive data in very ling ranges.**

### Configuration of microcontroller pins 

![](/assets/stm.png?raw=true)

First step was configuration of STM32 pins ind CubeMX.
The following libraries will be used for the project

```
#include "LoRa.h"
#include "gps.h"
#include "ssd1306.h"
#include "ssd1306_tests.h"
#include "ssd1306_fonts.h"
#include <string.h>
#include <stdio.h>
```

I configured

- **UART for GPS module**
	- PA10 as receiving pin
	- PA9 as transmitting pin

```c
// ====================== GPS SETUP ==========================
GPS_Init();

void GPS_display_status(GPS_t *gps)
{
    ssd1306_SetCursor(0, 16); // adjust position as needed

    if (gps->lock == 0)
    {
        // GPS module is connected but no fix yet
        ssd1306_WriteString("GPS: no fix", Font_11x18, White);
    }
    else if (gps->lock > 0 && gps->satelites >= 3) 
    {
        // GPS has a valid fix (3+ satellites)
        ssd1306_WriteString("GPS OK!", Font_11x18, White);
    }
    else
    {
        // if GPS string is invalid or module not responding
        ssd1306_WriteString("GPS not found!", Font_11x18, White);
    }

    ssd1306_UpdateScreen();
}

char gps_buffer[GPSBUFSIZE];

GPS_parse(gps_buffer); // this fills the gps struct

GPS_display_status(&gps);
```

If the GPS module hasn't found sufficient amount of satellites, it is not fixed.
If 3 or more satellites are fixed GPS module data will parsed.
If there are wiring errors, GPS module will not be interfaced by the controller.

- **For LoRa SPI protocol is used**
	- PA4 for NSS
	  PA5 for clock
	- PA6 for MISO
	- PA7 for MOSI
	- PA8 for LoRa_RST
	- PA0 for DI0

```c
// ====================== LORA SETUP ==========================
// configuring SPI pins for lora

lora.CS_port		= NSS_GPIO_Port;
lora.CS_pin		= NSS_Pin;
lora.reset_port		= LoRa_RST_GPIO_Port;
lora.reset_pin		= LoRa_RST_Pin;
lora.DIO0_port		= DIO0_GPIO_Port;
lora.DIO0_pin		= DIO0_Pin;
lora.hSPIx		= &hspi1;

char		send_data[200];
uint16_t	status = LoRa_init(&lora);

memset(send_data, 0, 200);

// checing status
if (status == LORA_OK)
	{
		/*	
		snprintf(send_data, sizeof(send_data), "\n\r LoRa OK! :)");
		LoRa_transmit(&lora, (uint8_t*)send_data, 120, 100);
		HAL_UART_Transmit(&huart1, (uint8_t*)send_data, 200, 200);
		*/
		ssd1306_SetCursor(16, 16);
		ssd1306_WriteString("LoRa OK!", Font_11x18, White);
		ssd1306_UpdateScreen();
		HAL_Delay(500);
	}
else if (status == LORA_NOT_FOUND)
	{
		// mcu cant communicate with lora	
		ssd1306_WriteString("LoRa not found, check wiring!", Font_11x18, White);
		ssd1306_UpdateScreen();
		HAL_Delay(500);
	}
else if (status == LORA_UNAVAILABLE)
	{
		// setup is not correct
		ssd1306_WriteString("Setup is not correct..", Font_11x18, White);
		ssd1306_UpdateScreen();
		ssd1306_UpdateScreen();
		HAL_Delay(500);

	}


lora.frequency      = 434;
lora.spredingFactor = SF_9;
lora.bandWidth      = BW_250KHz;
lora.crcRate        = CR_4_8;
lora.power          = POWER_17db;
lora.overCurrentProtection = 130;
lora.preamble       = 10;


```

Similarly to GPS, LoRa modules wiring and status is checked in this section.

- **For OLED screen I2C protocol was used**
	- PB9 for serial data line
	- PB8 for serial clock

```c
// ======================OLED SETUP =============================

// Initialize the display
ssd1306_Init();
    
// clear the display buffer
ssd1306_Fill(Black);
    
// set cursor position and write text
ssd1306_SetCursor(16, 16);
ssd1306_WriteString("Startup...", Font_11x18, White);

// update the display
ssd1306_UpdateScreen();
HAL_Delay(500);
ssd1306_Fill(Black);
```

OLED is initialized, and on boot a startup screen is displayed


Additionally two push buttons are connected for control (one for scanning available data, one for sending)
- scan_Pin GPIO input with pull up resistor
- send_Pin GPIO input with pull up resistor

After this the code was generated through Cube MX.


On boot (powering)

![[Pasted image 20251129213306.png| center | 600]]

Next the wiring and setting of LoRa module are checked, in case of success we get this screen

![[Pasted image 20251129213347.png| center | 600]]

For GPS, the no fix status is displayed until the enough satellites are found for a lock.

![[Pasted image 20251129213444.png| center | 600]]


