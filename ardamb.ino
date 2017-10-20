/*


 */

#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "DHT.h"
#include "BH1750.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BMP280.h"
#include "WiFi.h"
#include "SPI.h"

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

//WIFI
char ssid[] = "ConRed_La Villa";
char pass[] = "vmH42u6L2x26fqw";
int status = WL_IDLE_STATUS;  //Para saber el estado de la Wifi


WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  bme.begin(0x76);
  Bh.begin(BH1750_CONTINUOUS_HIGH_RES_MODE);
  
  //Antes de conectarse a la wifi definimos la IP, que habremos puesto previamente en la SD en un fichero llamado IP.txt
  File myFileIP = SD.open("IP.txt",FILE_WRITE); //Abrimos o creamos el archivo llamado IP.txt donde debemos haber introducido la IP
  if (myFileIP) //Si se ha abierto sin problemas asigna la IP que hay en el archivo
  {
    char server[] = myFileIP.print();

  }
  myFileIP.close();

  //Comprobamos si la wifi del arduino está puesta
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
  }

  int i = 0;
  while ((status != WL_CONNECTED) or (i <= 3)) {
    Serial.print("Intentando conectarse a la red");
    Serial.println(ssid);
    status = WiFi.begin(ssid,pass);

    i++;
    delay(3000);
  }

  Serial.print("CONECTADO A LA RED ");
  printCurrentNet();
  printWifiData();

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


  //Si estoy conectado a la red wifi, se conecta al servidor
  if (WiFi.status() == WL_CONNECTED) {
      if (client.connect(server,8000)) {
        client.println("GET /search?q=arduino HTTP/1.1");
        client.println("Host: 192.168.0.108");
        client.println("Connection: close");
        client.println();
      }

      //Leemos la respuesta del servidor
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
  }

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


  //Enviamos los datos por Wifi
  if (WiFi.status() == WL_CONNECTED) {
    if (client.connected()) {
      post("seta","T_bar",String(temperatura));
      post("seta","presion",String(presion));
      post("seta","altitud",String(altitud));
      post("seta","humedad",String(humedad));
      post("seta","irradiancia",String(lux));
      post("seta","humedad_suelo",String(suelo));
      post("seta","T_DHT",String(temp));
    }
  }

  myFile.flush();
  myFile.close();

  //Si se desconecta del servidor, cerramos la conexión


  delay(10000);

}

void post(String sensor_name, String seta_name, String valor)
{
  String PostData="sensor.name=\""+sensor_name+"\""; 
  PostData=PostData+"&seta.name=\""+seta_name+"\"";
  PostData=PostData+"&value=\""+valor"\"";

  Serial.println(PostData);
  if (client.connect()) {
    Serial.println("connected");
    client.println("POST /api/value/ HTTP/1.1");
    client.println("Host: 192.168.0.108");
    //client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(PostData.length());
    client.println();
    client.println(PostData);

    //Leemos la respuesta del servidor
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
   } else {
     Serial.println("connection failed");
  }
}


void printWifiData() {
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}

void printCurrentNet() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  long rssi = WiFi.RSSI();
  Serial.print("intensidad de señal (RSSI):");
  Serial.println(rssi);

  byte encryption = WiFi.encryptionType();
  Serial.print("Tipo de encriptado:");
  Serial.println(encryption, HEX);
  Serial.println();

}
