#include "Arduino.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "clockTeste.h"
#include "temperature.h"
#include "music.h";

#define SCREEN_WIDTH 240   // horizontalmente
#define SCREEN_HEIGHT 320  // verticalmente

// largura de 320 pixels e altura de 240 pixels

const int TFT_DC = 6;
const int TFT_CS = 5;
const int TFT_RST = 8;

const int red = A1;
const int green = 4;
const int yellowButton = 2;
const int redButton = 1;


Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

int yellowButtonState = 0;
int redButtonState = 0;
int previousStateButton = 0;
int previousRedState = 0;
int cycleCounter = 0;
bool cycle = false;

String timesUp = "00:00";

void setup() {
  // put your setup code here, to run once:
  music();
  Serial.begin(9600);
  tft.begin();
  wifi();
  printWiFiStatus();
  tft.fillScreen(ILI9341_BLACK);

  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(redButton, INPUT);
  pinMode(yellowButton, INPUT);

  tft.setRotation(0);
  tft.setTextColor(ILI9341_WHITE);

  drawHeart(20, 300);
}


void loop() {
  
  printDate();
  printTime();
  temp();
  Serial.println();
  delay(1000);
  yellowButtonState = digitalRead(yellowButton);
  redButtonState = digitalRead(redButton);  // read the value of buttonPin and get buttonState


  if ((yellowButtonState == HIGH && previousStateButton == LOW) || cycle ) {
    Serial.println("Botão pressionado! Iniciando operações...");
    if (yellowButtonState == HIGH) {
      cycleCounter = 0;
      updateLegend(cycleCounter);
    }


    String currentTime = printTime();
    String currentDate = printDate();
    temp();

    //countdown de estudo
    digitalWrite(green, HIGH);
    music();
    redButtonState = countdown(10, false, currentTime, currentDate);

    digitalWrite(green, LOW);
    tft.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ILI9341_BLACK);
    if (redButtonState != 1) {
      //delay(1000);
      cycleCounter++;
      updateLegend(cycleCounter);
      // countdown de pausa
      digitalWrite(red, HIGH);
      music();
      redButtonState = countdown(5, true, currentTime, currentDate);
      digitalWrite(red, LOW);
      tft.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ILI9341_BLACK);
      if (redButtonState != 1){
        cycle = true;
      }
      else {
        cycle = false;
      }
      

    }
    else {
      cycle = false;
    }
    Serial.print("Número de ciclos completos: ");
    Serial.println(cycleCounter);

    updateLegend(cycleCounter);
  }
  previousStateButton = yellowButtonState;
}




int countdown(int time, bool showPause, String currentTime, String currentDate) {
  int textSize = 4;

  tft.setTextSize(textSize);
  int textHeight = 8 * textSize;
  int posY = (SCREEN_HEIGHT - textHeight) / 2;

  for (int i = time; i >= 0; i--) {
    temp();
    if (i > 0) {
      int minutes = i / 60;
      int seconds = i % 60;

      char timeStr[6];
      sprintf(timeStr, "%02d:%02d", minutes, seconds);

      tft.fillRect(0, 160, SCREEN_WIDTH, textHeight, ILI9341_BLACK);

      // Exibir "PAUSA" centralizado se showPause for verdadeiro
      if (showPause) {
        tftTextCentered("PAUSA", 2, 200, ILI9341_RED);
        tftTextCentered(timeStr, 4, 160, ILI9341_WHITE);
      } else {
        tftTextCentered("ESTUDO", 2, 200, ILI9341_GREEN);
        tftTextCentered(timeStr, 4, 160, ILI9341_WHITE);
      }

      tftTextCentered(currentTime, 2, 20, ILI9341_WHITE);
      tftTextCentered(currentDate, 2, 40, ILI9341_WHITE);
      delay(1000);

      currentTime = printTime();
      currentDate = printDate();

      redButtonState = digitalRead(redButton);
      if (redButtonState == HIGH) {
        Serial.println(redButtonState);
        // Se o botão de parada foi pressionado, pare o pomodoro
        //stopPomodoro();
        break;
      }
    }

    previousRedState = redButtonState;
  }
  return redButtonState;
}

void drawHeart(int x, int y) {
  // Define o tamanho do coração
  int heartSize = 20;

  // Desenha o coração na posição especificada
  tft.fillTriangle(x, y + heartSize / 2, x - heartSize / 2, y - heartSize / 3, x + heartSize / 2, y - heartSize / 3, ILI9341_RED);
  tft.fillTriangle(x, y + heartSize / 2, x + heartSize / 2, y - heartSize / 3, x - heartSize / 2, y - heartSize / 3, ILI9341_RED);
  tft.fillCircle(x - heartSize / 4, y - heartSize / 4, heartSize / 4, ILI9341_RED);
  tft.fillCircle(x + heartSize / 4, y - heartSize / 4, heartSize / 4, ILI9341_RED);
}

void updateLegend(int numCycles) {
  // Define a posição e o tamanho do coração
  int heartX = 20;
  int heartY = 300;

  // Limpa a área do coração
  tft.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ILI9341_BLACK);

  // Escreve o número de ciclos ao lado do coração
  drawHeart(heartX, heartY);
  tft.setCursor(40, 295);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print(numCycles);
}

void stopPomodoro() {
  // Adicione aqui quaisquer ações que você deseja realizar ao parar o pomodoro
  // Por exemplo, desligar o LCD, redefinir variáveis, etc.

  // Exemplo:
  tft.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, ILI9341_BLACK);
  Serial.println("Contagem parada.");
  printTime();
  printDate();
  Serial.println();
  delay(1000);

  // Você pode adicionar mais ações aqui, se necessário

  // Use um loop infinito para parar a execução do programa
  while (true) {
    // Você pode optar por piscar um LED para indicar que o pomodoro foi parado
    digitalWrite(red, HIGH);
    delay(500);
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    delay(500);
    digitalWrite(green, LOW);
  }
}