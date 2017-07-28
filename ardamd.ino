#include <DHT.h>

int SENSOR_TH = 2; // Pin digital al que va enchufado en el arduino
int temp, humedad; // Variables que recogen la información

DHT dht (SENSOR_TH, DHT11); // Objeto de la librería para manejar el sensor

void setup(){
Serial.begin(9600); // Inicializa el objeto Serial, para leer los datos por el monitor serial
dht.begin(); // Iniciamos el objeto
}

void loop(){
humedad = dht.readHumidity(); // Leemos la humedad y la guardamos en nuestra variable humedad
temp = dht.readTemperature(); //Leemos la temperatura y la guardamos en nuestra variable temp

//Mostramos por el monitor serial la información recogida
Serial.print(“Temperatura: “);
Serial.print(temp);
Serial.print(“ºC Humedad: “);
Serial.print(humedad);
Serial.println(“%”);

delay(1000); //Retrasamos el loop 1s
}
