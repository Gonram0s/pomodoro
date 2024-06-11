#include "max6675.h"  //INCLUSÃO DE BIBLIOTECA
#include "wire.h"
int ktcSO = 0;   //PINO DIGITAL (SO)
int ktcCS = 3;   //PINO DIGITAL (CS)
int ktcCLK = 7;  //PINO DIGITAL (CLK / SCK)

MAX6675 ktc(ktcCLK, ktcCS, ktcSO);  //CRIA UMA INSTÂNCIA UTILIZANDO OS PINOS (CLK, CS, SO)

char asciiToString(uint8_t asciiValue) {
  return (char)asciiValue;
}

void temp() {



  Serial.begin(9600);  //INICIALIZA A SERIAL
                       //INTERVALO DE 500 MILISSEGUNDOS
  char str = asciiToString(248);
  tft.fillRect(-20, 60, 240, 20, ILI9341_BLACK);
  tftTextCentered(String(ktc.readCelsius()) + str + "C", 2, 60, ILI9341_WHITE);



  Serial.print("Temperatura: ");    //IMPRIME O TEXTO NO MONITOR SERIAL
  Serial.print(ktc.readCelsius());  //IMPRIME NO MONITOR SERIAL A TEMPERATURA MEDIDA
  Serial.println("*C");             //IMPRIME O TEXTO NO MONITOR SERIAL
                                    //INTERVALO DE 500 MILISSEGUNDOS
}
