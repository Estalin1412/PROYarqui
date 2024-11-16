#ifndef GPSREADER_H
#define GPSREADER_H

#include <TinyGPS.h>
#include <HardwareSerial.h>
#include <Adafruit_BMP280.h>
#include <SD.h>

TinyGPS gps;
float latitude, longitude, altitude; 

void FUNCION_LECTURA_GPS(HardwareSerial &ss) {
  while (ss.available()) {
    int c = ss.read();
    if (gps.encode(c)) {
      gps.f_get_position(&latitude, &longitude);
      altitude = gps.f_altitude();
    }
  }
  
  Serial.print("Latitud: ");
  Serial.print(latitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : latitude, 6);
  Serial.print(" ; Longitud: ");
  Serial.print(longitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : longitude, 6);
  Serial.print(" ; Altitud: ");
  Serial.print(altitude == TinyGPS::GPS_INVALID_F_ALTITUDE ? 0.0 : altitude);
  Serial.println(" m; ");

  // Guardar en SD
  File dataFile = SD.open("datos.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("Latitud: ");
    dataFile.print(latitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : latitude, 6);
    dataFile.print(" ; Longitud: ");
    dataFile.print(longitude == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : longitude, 6);
    dataFile.print(" ; Altitud: ");
    dataFile.print(altitude == TinyGPS::GPS_INVALID_F_ALTITUDE ? 0.0 : altitude);
    dataFile.println(" m;");
    dataFile.close();
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

  // Guardar en SD
  File dataFile = SD.open("datos.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("Temperatura: ");
    dataFile.print(temperature);
    dataFile.print(" °C; ");
    dataFile.print("Presión: ");
    dataFile.print(pressure);
    dataFile.println(" hPa;");
    dataFile.close();
  }
}

void FUNCION_LECTURA_MQ135() {
  int mq135State = digitalRead(pinMQ135);  
  int mq135AnalogValue = analogRead(pinMQ135_analog);  

  Serial.print("MQ135 Estado: ");
  Serial.println(mq135State == HIGH ? "Detección" : "No detección");
  Serial.print("MQ135 Valor Analógico: ");
  Serial.println(mq135AnalogValue);

  // Guardar en SD
  File dataFile = SD.open("datos.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("MQ135 Estado: ");
    dataFile.println(mq135State == HIGH ? "Detección" : "No detección");
    dataFile.print("MQ135 Valor Analógico: ");
    dataFile.println(mq135AnalogValue);
    dataFile.close();
  }
}

void FUNCION_LECTURA_SONIDO() {
  int soundAnalogValue = analogRead(pinSoundAnalog);  
  int soundDigitalState = digitalRead(pinSoundDigital);  

  Serial.print("Sonido Valor Analógico: ");
  Serial.println(soundAnalogValue);
  Serial.print("Sonido Estado Digital: ");
  Serial.println(soundDigitalState == HIGH ? "Ruido detectado" : "Silencio");

  // Guardar en SD
  File dataFile = SD.open("datos.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("Sonido Valor Analógico: ");
    dataFile.println(soundAnalogValue);
    dataFile.print("Sonido Estado Digital: ");
    dataFile.println(soundDigitalState == HIGH ? "Ruido detectado" : "Silencio");
    dataFile.close();
  }
}

void FUNCION_LECTURA_GP2Y() {
  int lectura = analogRead(analogPin1); 
  int polvo = (lectura - 1940) * 0.0256;
  Serial.print("Concentracion de polvo: ");
  Serial.print(polvo);
  Serial.print(" ug/m^3; ");

  // Guardar en SD
  File dataFile = SD.open("datos.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("Concentracion de polvo: ");
    dataFile.print(polvo);
    dataFile.println(" ug/m^3;");
    dataFile.close();
  }
}

void FUNCION_LECTURA_IRRADIANCIA() {
  int lectura_sol = analogRead(analogPin2); 
  int irradiancia = lectura_sol;
  Serial.print("Irradiancia: ");
  Serial.print(irradiancia);
  Serial.print(" W/m^2; ");

  // Guardar en SD
  File dataFile = SD.open("datos.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("Irradiancia: ");
    dataFile.print(irradiancia);
    dataFile.println(" W/m^2;");
    dataFile.close();
  }
}

#endif

