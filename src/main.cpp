#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "M5Stack.h"

static bool SoftAP_setup(void);

const char ssid[] = "ESP32_wifi"; // SSID
const char pass[] = "esp32pass";  // password
const int localPort = 10000;      // ポート番号

const IPAddress ip(192, 168, 4, 1);       // IPアドレス(ゲートウェイも兼ねる)
const IPAddress subnet(255, 255, 255, 0); // サブネットマスク

WiFiUDP udp;

void setup()
{
    M5.begin(true, false, false, false);
    Serial.begin(115200);
    
    if (SoftAP_setup())
    {
        Serial.println("Starting UDP");
        udp.begin(localPort);               // UDP通信の開始(引数はポート番号)

        Serial.print("Local port: ");
        Serial.println(localPort);
    }
}

void loop()
{
    if (udp.parsePacket())
    {
        int i = udp.read();
        Serial.print("rx : ");
        Serial.println(i);   // UDP通信で来た値を表示
    }
}

bool SoftAP_setup(void)
{
    bool ret;

    Serial.print("setup softAP");
    ret = false;
    for (int i = 0; i < 10; i++)
    {
        Serial.print(".");
        ret = WiFi.softAP(ssid, pass);           // set SSID and passward
        if (ret)
        {
            Serial.println(" Ready");
            break;
        }
        else
        {
            delay(100);
        }
    }
    if (!ret)
    {
        Serial.println(" Failed");
        return false;
    }
    
    Serial.print("configure softAP");
    ret = false;
    for (int i = 0; i < 10; i++)
    {
        ret = WiFi.softAPConfig(ip, ip, subnet); // IPアドレス、ゲートウェイ、サブネットマスクの設定
        Serial.print(".");
        if (ret)
        {
            Serial.println(" Ready");
            break;
        }
        else
        {
            delay(100);
        }
    }
    if (!ret)
    {
        Serial.println(" Failed");
        return false;
    }

    Serial.print("AP IP address: ");
    IPAddress myIP = WiFi.softAPIP();
    Serial.println(myIP);

    return true;
}