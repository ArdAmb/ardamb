#include "DHT.h"
#include <Wire.h>
#include <BH1750.h> // Librería del sensor de luz BH1750

#define DHTTYPE DHT22

int SENSOR_TH = 2; // Pin digital al que va enchufado en el arduino
int temp, humedad; // Variables que recogen la información

DHT dht (SENSOR_TH, DHT11); // Objeto de la librería para manejar el sensor
BH1750 Luxometro;

void setup(){
  Serial.begin(9600); // Inicializa el objeto Serial, para leer los datos por el monitor serial
  dht.begin(); // Iniciamos el objeto
  Serial.println("Inicializando sensor...");
  Luxometro.begin(BH1750_CONTINUOUS_HIGH_RES_MODE); //inicializamos el sensor BH1750
}

void loop(){
  humedad = dht.readHumidity(); // Leemos la humedad y la guardamos en nuestra variable humedad
  temp = dht.readTemperature(); //Leemos la temperatura y la guardamos en nuestra variable temp

  //Mostramos por el monitor serial la información recogida
  Serial.print("Temperatura: ");
  Serial.print(temp);
  Serial.print("ºC Humedad: ");
  Serial.print(humedad);
  Serial.println("%");

  uint16_t lux = Luxometro.readLightLevel();//Realizamos una lectura del sensor BH1750

  //Mostramos por el monitor serial los datos obtenidos del BH1750  
  Serial.print("Luz:  ");
  Serial.print(lux);
  Serial.println(" lx");

  delay(1000); //Retrasamos el loop 1s
}
