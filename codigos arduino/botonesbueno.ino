#include <SPI.h>
#include "RF24.h"
#include "DFRobot_DF2301Q.h"
#include <Servo.h>

#define CE_PIN 10
#define CSN_PIN 9

#define BTN_ADELANTE   2
#define BTN_ATRAS      3
#define BTN_IZQUIERDA  4
#define BTN_DERECHA    5


RF24 radio(CE_PIN, CSN_PIN);
uint8_t address[][6] = { "1Node", "2Node" };

uint8_t payload;  // comando a enviar

void setup() {
  Serial.begin(115200);

  pinMode(BTN_ADELANTE, INPUT_PULLUP);
  pinMode(BTN_ATRAS, INPUT_PULLUP);
  pinMode(BTN_IZQUIERDA, INPUT_PULLUP);
  pinMode(BTN_DERECHA, INPUT_PULLUP);

  if (!radio.begin()) {
    Serial.println("Radio no responde");
    while (1);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(sizeof(payload));

  radio.openWritingPipe(address[1]);
  radio.stopListening(); // TX
}

void loop() {
 payload = 0; // sin acción

  // COMBINACIONES
if (!digitalRead(BTN_ADELANTE) && !digitalRead(BTN_DERECHA))
  payload = 1;
else if (!digitalRead(BTN_ADELANTE) && !digitalRead(BTN_IZQUIERDA))
  payload = 2;
else if (!digitalRead(BTN_ATRAS) && !digitalRead(BTN_DERECHA))
  payload = 3;
else if (!digitalRead(BTN_ATRAS) && !digitalRead(BTN_IZQUIERDA))
  payload = 4;

// ADELANTE + ATRÁS → PARAR
else if (!digitalRead(BTN_ADELANTE) && !digitalRead(BTN_ATRAS))
{
  payload = 0; // PARAR
}
// BOTONES SIMPLES
else if (!digitalRead(BTN_ADELANTE))
  payload = 5;
else if (!digitalRead(BTN_ATRAS))
  payload = 7;
else if (!digitalRead(BTN_IZQUIERDA))
  payload = 9;
else if (!digitalRead(BTN_DERECHA))
  payload = 8;

  bool report = radio.write(&payload, sizeof(payload));

  if (report) {
    Serial.print("Comando enviado: ");
    Serial.println(payload);
  } else {
    Serial.println("Error al enviar");
  }

  delay(100);
}



