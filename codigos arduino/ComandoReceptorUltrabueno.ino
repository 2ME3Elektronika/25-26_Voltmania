#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

/* ===== RADIO ===== */
#define CE_PIN 10
#define CSN_PIN 9
RF24 radio(CE_PIN, CSN_PIN);
uint8_t address[][6] = { "1Node", "2Node" };

/* ===== SERVO ===== */
#define SERVO_PIN 3
#define ANGULO_RECTO     90
#define ANGULO_DERECHA   30
#define ANGULO_IZQUIERDA 150
Servo direccion;

/* ===== TB6612FNG ===== */
#define AIN1 A0
#define AIN2 A1
#define BIN1 A2
#define BIN2 A3
#define STBY 5

uint8_t comando;

void setup() {
  Serial.begin(115200);

  // Servo
  
  direccion.attach(SERVO_PIN);
  direccion.write(ANGULO_RECTO);

  // Motores
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);  // activar puente H

  // Radio
  if (!radio.begin()) {
    Serial.println("Radio no responde");
    while (1);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setPayloadSize(sizeof(comando));
  radio.openReadingPipe(1, address[1]);
  radio.startListening();

  Serial.println("Receptor listo");
}

void loop() {
  if (radio.available()) {
    radio.read(&comando, sizeof(comando));

    Serial.print("Comando recibido: ");
    Serial.println(comando);

    switch (comando) {

      case 5: // AVANZAR
        avanzar();
        direccion.write(ANGULO_RECTO);
        break;

      case 7: // ATRÁS
        retroceder();
        direccion.write(ANGULO_RECTO);
        break;

      case 8: // DERECHA
        parar();
        direccion.write(ANGULO_DERECHA);
        break;

      case 9: // IZQUIERDA
        parar();
        direccion.write(ANGULO_IZQUIERDA);
        break;

      case 1: // AVANZAR + DERECHA
        avanzar();
        direccion.write(ANGULO_DERECHA);
        break;

      case 2: // AVANZAR + IZQUIERDA
        avanzar();
        direccion.write(ANGULO_IZQUIERDA);
        break;

      case 3: // ATRÁS + DERECHA
        retroceder();
        direccion.write(ANGULO_DERECHA);
        break;

      case 4: // ATRÁS + IZQUIERDA
        retroceder();
        direccion.write(ANGULO_IZQUIERDA);
        break;

      default: // PARAR
        parar();
        direccion.write(ANGULO_RECTO);
        break;
    }
  }
}

/* ===== FUNCIONES MOTORES (SIN PWM) ===== */
void avanzar() {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void retroceder() {
  digitalWrite(STBY, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void parar() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}
