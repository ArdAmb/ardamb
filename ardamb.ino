

#include <DHT.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <SD.h>
#include <BH1750.h>

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define DHTTYPE DHT22
const int DHTPin = 2;
DHT dht(DHTPin, DHTTYPE);

float temp, humedad;
int suelo = 0;

const int chipSelect = 3;
File myFile; 

Adafruit_BMP280 bme;
BH1750 Bh;

void setup() {
  Serial.begin(9600);
  dht.begin();
  bme.begin(0x76);
  Bh.begin(BH1750_CONTINUOUS_HIGH_RES_MODE);

  if(!SD.begin(chipSelect)){
    Serial.println("No hay MicroSD insertado");
    return;
    }

  File myFile = SD.open("datos.txt",FILE_WRITE); //Abrimos o creamos el archivo llamado datos.txt

  if (myFile.size()<=0)
  {
    myFile.print("Tªbar (ºC);");
    myFile.print("Presión (Pa);");
    myFile.print("Altitud (m);");
    myFile.print("Humedad (%);");
    myFile.print("Luminosidad (lux);");
    myFile.print("Hª suelo (%);");
    myFile.println("TªDHT (ºC);");
    myFile.flush();
  }
  myFile.close(); 
}

void loop() {
  uint16_t lux = Bh.readLightLevel(); 
  humedad = dht.readHumidity();
  temp = dht.readTemperature();
  suelo = analogRead(1);
  float presion = bme.readPressure();
  float altitud = bme.readAltitude(1013.25);
  float temperatura = bme.readTemperature();

  if(isnan(humedad) || isnan(temp)) {
    Serial.println("Fallo de lectura del sensor DHT");
    return;
  }

  File myFile = SD.open("datos.txt",FILE_WRITE); //Abrimos o creamos el archivo llamado datos.txt
  
  Serial.print(" Humedad suelo: ");
  Serial.print(suelo);

  if(suelo <= 300) { Serial.print(" ENCHARCADO "); }
  if((suelo > 300) and (suelo < 700)) { Serial.print(" HUMEDAD "); }
  if(suelo >= 700) { Serial.print(" SECO "); }

  Serial.print(" Tªbar: ");
  Serial.print(temperatura);
  Serial.print(" ºC ");
  Serial.print(" TªDHT: ");
  Serial.print(temp);
  Serial.print(" ºC ");
  Serial.print(" Presión: ");
  Serial.print(presion);
  Serial.print(" Pa");
  Serial.print(" Altitud: ");
  Serial.print(altitud);
  Serial.print(" m ");
  Serial.print(" Humedad (%): ");
  Serial.print(humedad);
  Serial.print(" Luminosidad (lx): ");
  Serial.println(lux);


  //Escribimos datos en fichero
  myFile.print(temperatura);
  myFile.print(";");
  myFile.print(presion);
  myFile.print(";");
  myFile.print(altitud);
  myFile.print(";");
  myFile.print(humedad);
  myFile.print(";");
  myFile.print(lux);
  myFile.print(";");
  myFile.print(suelo);
  myFile.print(";");
  myFile.print(temp);
  myFile.println(";");

  myFile.flush();
  myFile.close();
  delay(10000);

}
