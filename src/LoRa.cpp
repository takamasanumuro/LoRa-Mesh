#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include "AuxLoRa.h"
OLED_CLASS_OBJ display(OLED_ADDRESS, OLED_SDA, OLED_SCL);

#define WIFI_SSID       "skynet"
#define WIFI_PASSWORD   "12345678"

void setup()
{
    Serial.begin(115200);
    while (!Serial);

    display.init();
    display.flipScreenVertically();
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(display.getWidth() / 2, display.getHeight() / 2, LORA_SENDER ? "LoRa Sender" : "LoRa Receiver");
    display.display();
    delay(2000);

    

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        static int counter=0;
        display.clear();
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "WiFi Attempt: "+String(counter++));
        display.display();
        delay(2000);
        if(counter>5) break;
    }
    if(WiFi.status()==WL_CONNECTED){
        display.clear();
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "WiFi Connected");
        display.display();
        delay(2000);
    }
    else{
        display.clear();
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "WiFi Connect Fail");
        display.display();
        delay(2000);
    }
    
    SPI.begin(CONFIG_CLK, CONFIG_MISO, CONFIG_MOSI, CONFIG_NSS);
    LoRa.setPins(CONFIG_NSS, CONFIG_RST, CONFIG_DIO0);
    LoRa.setTxPower(17);
    if (!LoRa.begin(BAND)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    if (!LORA_SENDER) {
        display.clear();
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "LoraRecv Ready");
        display.display();
    }
}

int count = 0;

void loop()
{
#if LORA_SENDER
    int32_t rssi;
    uint32_t counter;
    if (WiFi.status() == WL_CONNECTED) {
        rssi = WiFi.RSSI();
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "Send RSSI:" + String(rssi));
        display.display();
        LoRa.beginPacket();
        LoRa.print("WiFi RSSI: ");
        LoRa.print(rssi);
        LoRa.endPacket();
    } else {
        display.clear();
        display.setTextAlignment(TEXT_ALIGN_CENTER);
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, "WiFi Disconnected: "+String(count));
        display.display();
        LoRa.beginPacket();
        LoRa.print("No Wifi: "+String(count));
        LoRa.endPacket();
        count++;
    }
    delay(2500);
#else
    if (LoRa.parsePacket()) {
        String recv = "";
        while (LoRa.available()) {
            recv += (char)LoRa.read();
        }
        count++;
        display.clear();
        display.drawString(display.getWidth() / 2, display.getHeight() / 2, recv);
        String info = "[" + String(count) + "]" + "RSSI " + String(LoRa.packetRssi());
        display.drawString(display.getWidth() / 2, display.getHeight() / 2 - 16, info);
        display.display();
    }
#endif
if(Serial.available()){
    String cmd= Serial.readStringUntil('\n');
    if(cmd.substring(0,2)=="SF"){
        int sf=cmd.substring(2).toInt();
        LoRa.setSpreadingFactor(sf);
    }
    else if (cmd.substring(0,2)=="BW"){
        int bw=cmd.substring(2).toInt();
        LoRa.setSignalBandwidth(bw);
    }
    else if(cmd.substring(0,2)=="CR"){
        int cr=cmd.substring(2).toInt();
        LoRa.setCodingRate4(cr);
    }
    else if(cmd.substring(0,2)=="PO"){
        int po=cmd.substring(2).toInt();
        LoRa.setTxPower(po);
    }
    else if(cmd.substring(0,2)=="FR"){
        int frequency=cmd.substring(2).toInt();
        LoRa.setFrequency(frequency);
    }
    else if(cmd.substring(0,2)=="SY"){
        int sync=cmd.substring(2).toInt();
        LoRa.setSyncWord(sync);
    }
    else if(cmd.substring(0,2)=="PR"){
        int preamble=cmd.substring(2).toInt();
        LoRa.setPreambleLength(preamble);
    }
    }
ArduinoOTA.handle();
}


