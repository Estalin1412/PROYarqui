#include "Setup_EM.h"
#include "Funciones_EM.h"
#include <SD.h>
//memoria SD (este pin si fucniona)
const int chipSelect = 14; 

void setup() {
  Serial.begin(115200);
  if (!SD.begin(chipSelect)) {
    Serial.println("Error al inicializar la tarjeta SD.");
    return;
  }
  Serial.println("Tarjeta SD inicializada correctamente.");

  setupGPS();
  setupBME();  
  setupMQ135(); 
  setupSoundSensor(); 
}

void loop() {
  FUNCION_LECTURA_GPS(ss);
  FUNCION_LECTURA_BME();  
  FUNCION_LECTURA_MQ135(); 
  FUNCION_LECTURA_SONIDO();  
  FUNCION_LECTURA_GP2Y();
  FUNCION_LECTURA_IRRADIANCIA();
  delay(1000); 
}


