/*Librerias para BMP280
  Adafruit BMP280 by adafruit
*/
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
/*Libreria para conexion con fireBase
  Firebase ESP32 Cliente by Mobizt
*/
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
/*Objetos y variables para Firebase*/
#define WIFI_SSID "TP-Link_F786"
#define WIFI_PASSWORD "Fund3s*2024"
#define API_KEY "AIzaSyA0iRQ3gyoiOvamQ8S2ApXy9_VK6MhNOAY"
#define DATABASE_URL "conexionesp32app-default-rtdb.firebaseio.com" 

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
unsigned long dataMillis = 0;
int count = 0;
bool signupOK = false;

/*Objetos y variable para BMP280*/
Adafruit_BMP280 bmp;
#define I2C_SDA 21
#define I2C_SCL 22

/*Objetos y variables para Reley*/
#define pinReley 4
#define Encendido true
#define Apagado false
/*Objetos y variables para MQ135*/
#define MQ135_PIN 36

/*Encabezado de funciones*/
void funIniciarBomba();
void funEjecutarBomba(bool prep);

void funIniciarBMP280();
String funLeerDatosBMP280();

void funIniciarMQ135();
String funLeerDatosMQ135();

void funIniciarFirebase();
void funLeerEnviarDatosFirebase(String datos);


/*VOID SETUP--------------------------------------------------------*/
void setup(){
  Serial.begin(9600);
  funIniciarBomba();

  funIniciarBMP280();

  funIniciarMQ135();
  funIniciarFirebase();


}
void loop() {

  String datos = "";
  funEjecutarBomba(Apagado);

  datos += funLeerDatosBMP280();

  datos += funLeerDatosMQ135();
  temperatu
  "Temprearar: 24;42;34;43;43;423;43;;42;432;4234;;424;2342;42"

  funLeerEnviarDatosFirebase(datos);
  delay(1000);
}


/*Funciones para Bomba y reley----------------------------------------------------------------------------------------------------*/
void funIniciarBomba(){
  pinMode(pinReley,OUTPUT);
}
void funEjecutarBomba(bool prep){
  if(prep) digitalWrite(pinReley, HIGH);
  else{
    digitalWrite(pinReley, LOW);
  }
}
/*Funciones para BMP280----------------------------------------------------------------*/
void funIniciarBMP280(){
  if(!bmp.begin(0x76)){
    
  }else{

  }
}
String funLeerDatosBMP280(){
  float temperatura = bmp.readTemperature();
  float pressure = bmp.readPressure()/100.0F;
  float altitude = bmp.readAltitude(1013.25);
  String data = "Temperatura: " + String(temperatura) + "Presion: " + String(pressure) + "Altitud: " + String(altitude);
  return data;

}
/* Funciones para sensor MQ135---------------------------------------------------------------------------- */
void funIniciarMQ135(){
      pinMode(MQ135_PIN, INPUT);
    analogReadResolution(12); // Configura la resolución del ADC
    return;

}

String funLeerDatosMQ135(){
  int analogValue = analogRead(MQ135_PIN);
  float voltaje = analogValue * (3.3 / 4095.0); //Convertimos la lectura en un valor de voltaje
  float Rs=1000*((5-voltaje)/voltaje);  //Calculamos Rs con un RL de 1k
  double alcohol=0.4091*pow(Rs/5463, -1.497); // calculamos la concentración  de alcohol con la ecuación obtenida.
  //-------Enviamos los valores por el puerto serial------------
  String data = "adc: " + String(analogValue)+ "voltaje: " + String(voltaje)+ "Rs: " + String(Rs) + "alcohol: " + String(alcohol) + "mg/L";
  return data;
}
/*Funciones para conexion con  Firebase------------------------------------------*/
void funIniciarFirebase(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){

  }
  WiFi.localIP();
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.reconnectNetwork(true);

  fbdo.setBSSLBufferSize(4096,1024);

  if(Firebase.signUp(&config, &auth, "", ""))  signupOK = true;

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);


}

void funLeerEnviarDatosFirebase(String datos){
  if (millis() - dataMillis > 5000 && signupOK && Firebase.ready())
    {
        dataMillis = millis();
        String path = "Data1";//auth.token.uid.c_str();
        path += "/test/int";
        Serial.printf("Set int... %s\n", Firebase.setString(fbdo, path, datos) ? "ok" : fbdo.errorReason().c_str());

        if (count == 10)
        {
            if (Firebase.deleteUser(&config, &auth ))
            {
                Serial.println("ok");
            }
            else
                Serial.printf("%s\n", config.signer.deleteError.message.c_str());
        }
    }

}

