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
#define WIFI_SSID "JoseMD"//"BIBLIOTECA-LIBRE"
#define WIFI_PASSWORD "soytuperra"
#define API_KEY "AIzaSyA0iRQ3gyoiOvamQ8S2ApXy9_VK6MhNOAY"
#define DATABASE_URL "https://conexionesp32app-default-rtdb.firebaseio.com" 

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

// Pines y constantes
const int analogPin = 36; // Pin analógico donde está conectado el sensor
const double RL = 20.0;    // Resistencia de carga (kΩ)
const double a = 5.5973021420; // Constante de la ecuación
const double b = -0.365425824; // Constante de la ecuación

double R0 = 0.0; // Resistencia de referencia, se calibrará

/*Encabezado de funciones*/



class Data {
private:
    double temperatura = 32.32;
    double altitud = 242.4324;
    double presion = 2432.234;
    double alcohol = 0.430;
    double co2 = 0.0432;
    double humedadDeSuelo = 34.43;
    int estado = 0;

public:
    // Constructor vacío
    Data() {}

    // Setters
    void setTemperatura(double temp) {
        temperatura = temp;
    }

    void setAltitud(double alt) {
        altitud = alt;
    }

    void setPresion(double pres) {
        presion = pres;
    }

    void setAlcohol(double alc) {
        alcohol = alc;
    }

    void setCO2(double gas) {
        co2 = gas;
    }

    void setHumedadDeSuelo(double humSuelo) {
        humedadDeSuelo = humSuelo;
    }

    void setEstado(int est) {
        estado = est;
    }

    // Getters
    double getTemperatura() {
        return temperatura;
    }

    double getAltitud() {
        return altitud;
    }

    double getPresion() {
        return presion;
    }


    double getAlcohol() {
        return alcohol;
    }

    double getCO2() {
        return co2;
    }

    double getHumedadDeSuelo() {
        return humedadDeSuelo;
    }

    int getEstado() {
        return estado;
    }
};
void funIniciarSensorHumedadDeSuelo();
void funIniciarBomba();
void funEjecutarBomba(bool prep);

void funIniciarBMP280();
void funLeerDatosBMP280(Data &);

void funIniciarMQ135();
double calibrateSensor();
String funLeerDatosMQ135(Data &);
double calculateRs(double adcValue);
double calculatePPM(double Rs);

void funIniciarFirebase();
void funLeerEnviarDatosFirebase(Data &);

void funLeerDatosHumedadSuelo(Data &);

Data dataAEnviar = Data();

/*VOID SETUP--------------------------------------------------------*/
void setup(){
  Serial.begin(9600);
  funIniciarSensorHumedadDeSuelo();
  funIniciarBomba();

  funIniciarBMP280();

  funIniciarMQ135();
  funIniciarFirebase();

}

void loop() {

  String datos = "";
  funEjecutarBomba(Apagado);

  funLeerDatosBMP280(dataAEnviar);
  funLeerDatosMQ135(dataAEnviar);
  funLeerDatosHumedadSuelo(dataAEnviar);
  funLeerEnviarDatosFirebase(dataAEnviar);

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
void funLeerDatosBMP280(Data &data){
  double temperatura = bmp.readTemperature();
  double pressure = bmp.readPressure()/100.0F;
  double altitude = bmp.readAltitude(1013.25);
  
  //String data = "Temperatura: " + String(temperatura) + "Presion: " + String(pressure) + "Altitud: " + String(altitude);
  data.setTemperatura(temperatura);
  data.setAltitud(altitude);
  data.setPresion(pressure);

  return ;
}
/* Funciones para sensor MQ135---------------------------------------------------------------------------- */
void funIniciarMQ135(){
    pinMode(MQ135_PIN, INPUT);
    analogReadResolution(12); // Configura la resolución del ADC
    R0 = calibrateSensor();
    return;
}

String funLeerDatosMQ135(Data &datos){
  int analogValue = analogRead(MQ135_PIN);
  float voltaje = analogValue * (3.3 / 4095.0); //Convertimos la lectura en un valor de voltaje
  double Rs = calculateRs(analogValue);   //Calculamos Rs con un RL de 1k
  double alcohol=0.4091*pow(Rs/5463, -1.497); // calculamos la concentración  de alcohol con la ecuación obtenida.
  double ppm = calculatePPM(Rs);
  //-------Enviamos los valores por el puerto serial------------
  //String data = "adc: " + String(analogValue)+ "voltaje: " + String(voltaje)+ "Rs: " + String(Rs) + "alcohol: " + String(alcohol) + "mg/L";
  datos.setAlcohol(alcohol);
  datos.setCO2(ppm);
  String data =  String(alcohol) + ";" + String(ppm);
  return data;
}


double calculateRs(double adcValue) {
  return (1024.0 * (RL / adcValue)) - RL;
}

// Función para calcular las PPM de CO2
double calculatePPM(double Rs) {
  return pow((Rs / R0) / a, 1.0 / b);
}

/*Funciones para conexion con  Firebase------------------------------------------*/
void funIniciarFirebase(){
  analogReadResolution(10);
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

void funLeerEnviarDatosFirebase(Data &datos){
  analogReadResolution(10);
  if (Firebase.ready()) {
        String path = "data"; // Ruta en la base de datos Firebase
        Firebase.setDouble(fbdo, path + "/temperatura", datos.getTemperatura());
        Firebase.setDouble(fbdo, path + "/presion", datos.getPresion());
        Firebase.setDouble(fbdo, path + "/altitud", datos.getAltitud());
        Firebase.setDouble(fbdo,path + "/humedaddesuelo", datos.getHumedadDeSuelo());
        Firebase.setDouble(fbdo, path + "/alcohol", datos.getAlcohol());
        Firebase.setDouble(fbdo, path + "/co2", datos.getCO2());
        Firebase.setInt(fbdo,path + "/estado", datos.getEstado());
        Serial.println("Datos enviados a Firebase.");

    } else {
        Serial.println("Firebase no está listo.");
    }
}

void funIniciarSensorHumedadDeSuelo(){
  pinMode(32, INPUT);
}

void funLeerDatosHumedadSuelo(Data &data){
  analogReadResolution(12);
  double value = analogRead(32);
  double humedadSuelo = 100 - (((value) / ( 4095) )*100);
  Serial.println(value);
  
  data.setHumedadDeSuelo(humedadSuelo);
}

double calibrateSensor() {
  double RsSum = 0.0;
  int samples = 0;
  unsigned long startTime = millis();
  
  while (millis() - startTime < 3000) { // 5 minutos (300000 ms)
    double adcValue = analogRead(analogPin);
    double Rs = calculateRs(adcValue);
    RsSum += Rs;
    samples++;
    delay(500); // Leer cada segundo
  }

  double RsAvg = RsSum / samples; // Calcular promedio de Rs
  double ppmReference = 100.0;   // PPM conocido durante calibración
  return RsAvg / (a * pow(ppmReference, b)); // Calcular R0
}
