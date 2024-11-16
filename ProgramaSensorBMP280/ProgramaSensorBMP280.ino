#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; 
#define I2C_SDA 21
#define I2C_SCL 22


void setupBME();
void FUNCION_LECTURA_BME();
void setup() 
{
  Serial.begin(9600);
  setupBME();
}

void loop()
{
  FUNCION_LECTURA_BME();
  delay(1000);
}


void setupBME() {
  if (!bmp.begin(0x76)) { 
    Serial.println("No se encontró el sensor BMP280.");
    while (1);
  } else {
    Serial.println("Sensor BMP280 inicializado correctamente.");
  }
}

void FUNCION_LECTURA_BME() {
  float temperature = bmp.readTemperature();
  float pressure = bmp.readPressure() / 100.0F; 
  float altitude = bmp.readAltitude(1013.25); 

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" °C; ");
  Serial.print("Presión: ");
  Serial.print(pressure);
  Serial.println(" hPa; ");

}