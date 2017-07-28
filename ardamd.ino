#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

Adafruit_BMP280 bme;  //Creamos variable para presión y temperatura de BMP280

void setup() {
	Serial.begin(9600);
	Serial.println(F("BMP280 test”));
}

void loop() {
	
	float presion = bme.readPressure(); //Recoge la presión en pascales
	float altitud = bme.readAltitude(1013.25); //Recoge la altitud dependiente de la presión de referencia (1013.25) en metros (debería estar ajustado a tu localidad)
	float temperatura = bme.readTemperature(); //Recoge la temperatura en ºCelsius


Serial.print("Temperatura: ");
	Serial.print(temperatura);
	Serial.print(" ºC ”);

	Serial.print("Presión: ");
	Serial.print(presion);
	Serial.print(" Pa ");

	Serial.print("Altitud: ");
	Serial.print(altitud);
	Serial.println(" m ");

	delay(2000); //Retrasamos el loop 2 segundos
}
