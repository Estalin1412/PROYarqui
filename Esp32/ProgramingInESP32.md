## Entradas y salidas

```cpp
#define PinIN 18    //Periferico 18 del esp32 como entrada
#define PinOUT 2    //Periferico 2 como salida

//PinOUT= numero de pin de ESP32
//OUTPUT = modo salida
pinMode(PinOUT, OUTPUT);
```

```cpp
//Estado de lectuda del PinIN
digitalRead(PinIN)
//EStado de salida
digitalWrite(PinOUT, HIGH);

```

## DAC 
### Signal analog
valor maximo     
$$
     2^{12} = 4095
$$

```cpp
#define PinADC 34

int Resolution = 4095;
void setup(){
    //Resolucion de valores de 9 a 12 bytes(512 a 4096 )
    analogReadResolution(12);
    //puede tomar valores de 1.1 a 3.3 V
    analogSetPinAttenuation(PinADC, ADC_12db);
}
```

    

