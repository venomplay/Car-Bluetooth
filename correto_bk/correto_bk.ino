// INCLUSÃO DE BIBLIOTECAS
#include "motor.h"
#include <SoftwareSerial.h>
#include "PiscaLed.h"

// DEFINIÇÕES DE PINOS
#define pinFarol A3
#define pinLanterna A2
#define pinBuzina A4
#define pinPiscaTraseira A0
#define pinRe A1

#define pinRx 5
#define pinTx 6

// DEFINIÇÕES
#define LIGADO HIGH
#define DESLIGADO LOW

// INSTANCIANDO OBJETOS
SoftwareSerial btSerial(pinRx, pinTx); // RX, TX
PiscaLed piscaTraseira(pinPiscaTraseira, 450);

// DECLARAÇÃO DE VARIÁVEIS
bool estadoFarol, estadoLanterna, estadoPisca;
bool pisca = LOW;
bool luzRe = LOW;
bool luzLanterna = LOW;
bool farol = LOW;

char comando;
char controle = 'D';
char controleAnt = 'S';

unsigned long controleTempo = 0;

void setup() {
  Serial.begin(9600);
  btSerial.begin(9600);

  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);

  pinMode(pinFarol, OUTPUT);
  pinMode(pinPiscaTraseira, OUTPUT);
  pinMode(pinLanterna, OUTPUT);
  pinMode(pinRe, OUTPUT);
  pinMode(pinBuzina, OUTPUT);

  digitalWrite(pinFarol, HIGH);
  digitalWrite(pinLanterna, HIGH);
  digitalWrite(pinBuzina, HIGH);
  digitalWrite(pinPiscaTraseira, HIGH);
  digitalWrite(pinRe, HIGH);

  digitalWrite(pinFarol, LOW);
  digitalWrite(pinLanterna, LOW);
  digitalWrite(pinBuzina, LOW);
  digitalWrite(pinPiscaTraseira, LOW);
  digitalWrite(pinRe, LOW);
  
  Serial.println("Fim Setup");
  delay(1000);
}

void loop() {

  btLoop();
  ledLoop();

}

// IMPLEMENTO DE FUNÇÕES
void btLoop() {

  if ( btSerial.available() ) {
    controle = btSerial.read();
  }

  if (controle != controleAnt) {

    if (controle == 'F' || controle == 'G' || controle == 'I') {      
      luzRe = false;
      luzLanterna = false;

    } else if (controle == 'B' || controle == 'H' || controle == 'J') {      
      luzRe = true;
      luzLanterna = false;

    } else if (controle == 'D' || controle == 'S') {      
      luzRe = false;
      luzLanterna = true;

    } else if (controle > 'S') {

      switch (controle) {
       case 'X': {
            pisca = true;
            break;
          }
        case 'x': {
            pisca = false;
            break;
          }
        case 'W': {
            digitalWrite(pinFarol, HIGH);
            estadoFarol = LIGADO;
            break;
          }
        case 'w': {
            digitalWrite(pinFarol, LOW);
            estadoFarol = DESLIGADO;
            break;
          }
        case 'V': {
            tone(pinBuzina, 300);
            break;
          }
        case 'v': {
            noTone(pinBuzina);
            break;
         }
      }
    }
    controleAnt = controle;
  }
}

void ledLoop() {
  piscaTraseira.loop();
  
  digitalWrite(pinLanterna, luzLanterna);
  digitalWrite(pinRe, luzRe);

  if (pisca && !piscaTraseira.estado() ) {
    piscaTraseira.play();
  }
  else if (!pisca && piscaTraseira.estado() ) {
    piscaTraseira.stop();
  }

  if (btSerial.available()) {
    comando = btSerial.read();
  }

  switch (comando) {
    case 'F': {
        moveFrente();
        break;
      }
    case 'I': {
        frenteDireita();
        break;
      }
    case 'G': {
        frenteEsquerda();
        break;
      }
    case 'R': {
        direita();
        break;
      }
    case 'L': {
        esquerda();
        break;
      }
    case 'B': {
        moveRe();
        break;
      }
    case 'H': {
        reEsquerda();
        break;
      }
    case 'J': {
        reDireita();
        break;
      }
    case 'W': {
        digitalWrite(pinFarol, HIGH);
        estadoFarol = LIGADO;
        break;
      }
    case 'w': {
        digitalWrite(pinFarol, LOW);
        estadoFarol = DESLIGADO;
        break;
      }
    case 'V': {
        tone(pinBuzina, 300);
        break;
      }
    case 'v': {
        noTone(pinBuzina);
        break;
      }
      
    default: {
        parar();
        break;
      }
  }
}
