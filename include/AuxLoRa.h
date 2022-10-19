#pragma once
#include <Arduino.h>
#include <Wire.h> 
#include "SSD1306Wire.h"
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>

#define LORA_SENDER 0
#define LORA_PERIOD 915     
#define OLED_CLASS_OBJ  SSD1306Wire
#define OLED_ADDRESS    0x3C
#define OLED_SDA    21
#define OLED_SCL    22
#define OLED_RST    -1

#define CONFIG_MOSI 27
#define CONFIG_MISO 19
#define CONFIG_CLK  5
#define CONFIG_NSS  18
#define CONFIG_RST  23
#define CONFIG_DIO0 26


#if LORA_PERIOD == 433
#define BAND 433E6
#elif LORA_PERIOD == 868
#define BAND 868E6
#elif LORA_PERIOD == 915
#define BAND 915E6
#else
#error "Please select the correct lora frequency"
#endif
