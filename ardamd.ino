#include <SD.h>  //Librería de la tarjeta de memoria
const int chipSelect = 3;  //Constante para el PIN de la SD
File myFile;  //Variable myFile de tipo File para manejar los archivos

void setup()
{
Serial.begin(9600);

if(!SD.begin(chipSelect)){  //Si no puede inicializar la SD avisa con el error
		Serial.println(“Error leyendo la tarjeta MicroSD”);
		return; //Sale del SETUP()
}
}

void loop()
{
	File myFile = SD.open(“datos.txt”,FILE_WRITE); //Crea o abre el archivo datos.txt
	int variable = 1; //Variable cualquiera, en este caso es un entero con el valor 1

	myFile.print(“Texto guardado”); //Escribimos en el archivo el texto indicado
	myFile.print(variable); //Escribimos en el archivo el valor de la variable
	myFile.println(“;”); //Esto sirve para tabular la información y pasarlo a Excel


	myFile.close();

	delay(10000); //Retrasamos el loop 10s
}
