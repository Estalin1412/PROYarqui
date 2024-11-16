#include <Arduino.h>

#define MQ135_PIN 36
void FunIniciarMQ135();
void FunLeerDatosMQ135();


class MQ135objetoEstalin{
  public:
    volatile float voltaje = 0;
    volatile float RS = 0;
    volatile double Alchool = 0;
    volatile double CO2 = 0;
};


void setup() {
    Serial.begin(9600);
    FunIniciarMQ135();

}

void loop() {
  FunLeerDatosMQ135();
}

/* Funciones para sensor MQ135---------------------------------------------------------------------------- */
void FunIniciarMQ135(){
      pinMode(MQ135_PIN, INPUT);
    analogReadResolution(12); // Configura la resolución del ADC
    return;

}

void FunLeerDatosMQ135(){
  int analogValue = analogRead(MQ135_PIN);
  float voltaje = analogValue * (3.3 / 4095.0); //Convertimos la lectura en un valor de voltaje
  float Rs=1000*((5-voltaje)/voltaje);  //Calculamos Rs con un RL de 1k
  double alcohol=0.4091*pow(Rs/5463, -1.497); // calculamos la concentración  de alcohol con la ecuación obtenida.
  //-------Enviamos los valores por el puerto serial------------
  Serial.print("adc:");
  Serial.print(analogValue);
  Serial.print("    voltaje:");
  Serial.print(voltaje);
  Serial.print("    Rs:");
  Serial.print(Rs);
  Serial.print("    alcohol:");
  Serial.print(alcohol);
  Serial.println("mg/L");
  delay(1000);
  return;
}
