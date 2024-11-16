#ifndef SETUPGPS_H
#define SETUPGPS_H

#include <HardwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SPI.h>
#include <SD.h>

HardwareSerial ss(2);

// Crear un objeto BMP280
Adafruit_BMP280 bmp; 
#define I2C_SDA 21
#define I2C_SCL 22

// Pines del sensor MQ135
const int pinMQ135 = 2;  
const int pinMQ135_analog = 34;

// Pin del sensor de sonido
const int pinSoundAnalog = 35;  
const int pinSoundDigital = 19; 

// Pines analógicos para el GP2Y10
const int analogPin1 = 32;  
// Pin analógico para el sensor de irradiancia (celda solar)
const int analogPin2 = 33;  

void setupGPS() {
  Serial.begin(115200);
  ss.begin(9600, SERIAL_8N1, 16, 17); 
  Serial.println("Leyendo datos del GPS...");
}

void setupBME() {
  if (!bmp.begin(0x76)) { 
    Serial.println("No se encontró el sensor BMP280.");
    while (1);
  } else {
    Serial.println("Sensor BMP280 inicializado correctamente.");
  }
}

void setupMQ135() {
  pinMode(pinMQ135, INPUT);  
}

void setupSoundSensor() {
  pinMode(pinSoundDigital, INPUT); 
}

#endif

