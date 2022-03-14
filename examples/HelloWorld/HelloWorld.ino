#include <Minitel1B_Soft.h>  // Voir https://github.com/eserandour/Minitel1B_Soft
#include <SoftwareSerial.h>

// Port série logiciel utilisé pour la connexion avec le Minitel :
// Broches 8 et 9 de la carte Arduino Uno.
Minitel minitel(8, 9);  // RX, TX

///////////////////////////////////////////////////////////////////////////////////

void setup() {  
  // Le port série matériel de l'ATMega 328P (Serial)
  // est utilisé pour la connexion avec le PC.
  Serial.begin(9600);  

  // A la mise sous tension du Minitel, la vitesse des échanges entre
  // le Minitel et le périphérique est de 1200 bauds par défaut.
  // On envisage cependant le cas où le Minitel se trouve dans un autre état.
  minitel.changeSpeed(minitel.searchSpeed());
  
  minitel.newScreen();
}

///////////////////////////////////////////////////////////////////////////////////

void loop() {
  minitel.print("Hello World ! ");
}

///////////////////////////////////////////////////////////////////////////////////
