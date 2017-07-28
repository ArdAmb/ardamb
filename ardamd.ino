#include <Wire.h>
#include <BH1750.h> // Librería del sensor de luz BH1750

BH1750 Luxometro;


void setup(){
  Serial.begin(9600);
  Serial.println("Inicializando sensor...");
  Luxometro.begin(BH1750_CONTINUOUS_HIGH_RES_MODE); //inicializamos el sensor BH1750
}


void loop() {
  uint16_t lux = Luxometro.readLightLevel();//Realizamos una lectura del sensor BH1750

  //Mostramos por el monitor serial los datos obtenidos del BH1750  
  Serial.print("Luz:  ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(2000);
}
