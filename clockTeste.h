#include <SPI.h>
//#include <WiFi101.h>
#include <WiFiNINA.h>  //Include this instead of WiFi101.h as needed
#include <WiFiUdp.h>
#include <RTCZero.h>
#include <LiquidCrystal.h>

#include "Arduino.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#include "arduino_secrets.h"  //documento com a informacao privada relativa a rede wifi utilizada
extern Adafruit_ILI9341 tft;
RTCZero rtc;

#define SCREEN_WIDTH 240   // horizontalmente
#define SCREEN_HEIGHT 320  // verticalmente

char ssid[] = SECRET_SSID;  // network SSID (nome da rede)
char pass[] = SECRET_PASS;  // network password (palavra-passe da rede)
int keyIndex = 0;           // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

const int GMT = 1;  //change this to adapt it to your time zone
void wifi() {


  Serial.begin(115200);

  // check if the WiFi module works
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");  // Information about the WiFI status
    // don't continue:
    while (true)
      ;
  }

  // attempt to connect to WiFi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the status:
  //printWiFiStatus();

  rtc.begin();

  unsigned long epoch;
  int numberOfTries = 0, maxTries = 6;
  do {
    epoch = WiFi.getTime();
    numberOfTries++;
  } while ((epoch == 0) && (numberOfTries < maxTries));

  if (numberOfTries > maxTries) {
    Serial.print("NTP unreachable!!");
    while (1)
      ;
  } else {
    Serial.print("Epoch received: ");
    Serial.println(epoch);
    rtc.setEpoch(epoch);

    Serial.println();
  }
}


void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void tftTextCentered(String text, int textSize, int posY, int16_t textColor) {
  int16_t textWidth, textHeight;
  tft.setTextSize(textSize);
  tft.setTextColor(textColor);

  textWidth = text.length() * 6 * textSize;
  textHeight = 8 * textSize;

  int posX = (SCREEN_WIDTH - textWidth) / 2;
  // int posY = (SCREEN_HEIGHT - textHeight) / 2;

  tft.setCursor(posX, posY);
  tft.print(text);
}

String printTime() {
  char timeStr[9];  // String para armazenar a hora formatada (HH:MM:SS)

  // Formatando os valores da hora, minutos e segundos em uma única string
  sprintf(timeStr, "%02d:%02d:%02d", rtc.getHours() + GMT, rtc.getMinutes(), rtc.getSeconds());
  Serial.println(timeStr);
  tft.fillRect(0, 0, SCREEN_WIDTH, 20, ILI9341_BLACK);
  tftTextCentered(timeStr, 2, 20, ILI9341_WHITE);
  return String(timeStr);
}

String printDate() {
  char dateStr[9];  // Array de caracteres para armazenar a data formatada (DD/MM/YYYY)

  // Formatando os valores do dia, mês e ano em uma única string
  sprintf(dateStr, "%02d/%02d/%02d", rtc.getDay(), rtc.getMonth(), rtc.getYear());

  // Imprimindo via Serial
  Serial.println(dateStr);
  tft.fillRect(0, 20, SCREEN_WIDTH, 20, ILI9341_BLACK);
  tftTextCentered(dateStr, 2, 40, ILI9341_WHITE);
  return String(dateStr);
}
