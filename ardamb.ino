

#include <DHT.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SD.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define DHTTYPE DHT22
const int DHTPin = 2;
DHT dht(DHTPin, DHTTYPE);

float temp, humedad;
int ldr_pin = 0;
int ldr_val = 0;
int suelo = 0;

const int chipSelect = 3;
File myFile; 

Adafruit_BMP280 bme;


void setup() {
  Serial.begin(9600);
  dht.begin();
  bme.begin(0x76);

  if(!SD.begin(chipSelect)){
    Serial.println("No hay MicroSD insertado");
    return;
    }
  }

void loop() {
  humedad = dht.readHumidity();
  temp = dht.readTemperature();
  suelo = analogRead(1);
  float presion = bme.readPressure();
  float altitud = bme.readAltitude(1013.25);
  float temperatura = bme.readTemperature();
  File myFile = SD.open("datos.txt",FILE_WRITE);

  if(isnan(humedad) || isnan(temp)) {
    Serial.println("Fallo de lectura del sensor DHT");
    return;
  }

  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.print("%");
  ldr_val = map(analogRead(ldr_pin),0,1023,100,0);
  Serial.print(" Luz: ");
  Serial.print(ldr_val);
  Serial.print("%");
  Serial.print(" Humedad suelo: ");
  Serial.print(suelo);

  if(suelo <= 300) { Serial.print(" ENCHARCADO"); }
  if((suelo > 300) and (suelo < 700)) { Serial.print(" HUMEDAD "); }
  if(suelo >= 700) { Serial.print(" SECO "); }

  Serial.print(" Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" ºC");
  Serial.print(" Presión: ");
  Serial.print(presion);
  Serial.print(" Pa");
  Serial.print(" Altitud: ");
  Serial.print(altitud);
  Serial.println(" m");

  myFile.print("Humedad (%): ");
  myFile.print(humedad);
  myFile.print(";");

  myFile.print("Luz (%): ");
  myFile.print(ldr_val);
  myFile.print(";");

  myFile.print("Humedad del suelo (%): ");
  myFile.print(suelo);
  myFile.print(";");

  myFile.print("Temperatura (ºC): ");
  myFile.print(temperatura);
  myFile.print(";");

  myFile.print("Presión (Pa): ");
  myFile.print(presion);
  myFile.print(";");

  myFile.print("Altitud (m): ");
  myFile.print(altitud);
  myFile.println(";");

  myFile.close();

  delay(10000);

}
