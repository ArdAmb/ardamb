#include "DHT.h"
 
#define DHTTYPE DHT22
 
const int DHTPin = 2; 	// Pin digital en el que conectamos el sensor de Temperatura y Humedad DHT22
 
DHT dht(DHTPin, DHTTYPE); //Se inicializa el objeto DHT
 
void setup() {
   Serial.begin(9600);
 
   dht.begin();	//Se llama al objeto DHT
}
 
void loop() {
  
   // La lectura de los datos del DHT tarda 250 milisegundos
   float h = dht.readHumidity();
   float t = dht.readTemperature()

   //Si falla la lectura del sensor de la humedad o la temperatura
   if (isnan(h) || isnan(t)) {
  	Serial.println("Fallo de lectura del Sensor DHT!"); 
  	return;
   }

   //Mostramos por el monitor serial los datos obtenidos del DHT22
   Serial.print("Humedad: ");   
   Serial.print(h); //Humedad
   Serial.print(" %");
   Serial.print("Temperatura: "); 
   Serial.print(t); //Temperatura
   Serial.println(" ºC "); 

delay(2000); // retrasamos la siguiente lectura 2 segundos 

}
