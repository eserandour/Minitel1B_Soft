////////////////////////////////////////////////////////////////////////
/*
   Minitel1B - 3615 MORUE - Version du 11 juin 2017 à 16h00
   Copyright 2016, 2017 - Eric Sérandour
   
   Documentation utilisée :
   Spécifications Techniques d'Utilisation du Minitel 1B
   http://543210.free.fr/TV/stum1b.pdf
   
////////////////////////////////////////////////////////////////////////
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
   
////////////////////////////////////////////////////////////////////////
   Compile sous Arduino 1.6.12
////////////////////////////////////////////////////////////////////////
  DEBUT DU PROGRAMME
*///////////////////////////////////////////////////////////////////////


#include <Minitel1B_Soft.h>
#include <SoftwareSerial.h>


  
Minitel minitel(8, 9);  // RX, TX

unsigned long touche;

String texte="";
int nbCaracteres=0;
const int nbLignes = 4;

////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);  // Port série matériel de l'ATmega à 9600 bauds.
  minitel.changeSpeed(minitel.searchSpeed());
  minitel.smallMode();
}

////////////////////////////////////////////////////////////////////////

void loop() {
  demoClavier(); 
}

////////////////////////////////////////////////////////////////////////

void newPage(String titre) {
  minitel.newScreen();
  minitel.println(titre);
  for (int i=1; i<=40; i++) {
    minitel.writeByte(0x7E);
  }
  minitel.moveCursorReturn(1); 
}

////////////////////////////////////////////////////////////////////////

void formulaireVide()
{
  minitel.noCursor();
  minitel.moveCursorXY(1,7);
  minitel.clearScreenFromCursor();
  for (int j=0; j<nbLignes; j++) {
    minitel.print(".");
    minitel.repeat(39);
  }
  minitel.moveCursorUp(nbLignes);
  minitel.cursor();
  texte="";
  nbCaracteres=0;
}

////////////////////////////////////////////////////////////////////////

void demoClavier() {
  // Affichage de la page
  newPage("3615 MORUE");
  minitel.println("Exprimez votre ressenti en 160 signes");
  minitel.println("puis ENVOI :");
  formulaireVide();

  // Utilisation du clavier
  boolean fin = false;
  while (!fin) {
    touche = minitel.getKeyCode();
    
    if ((touche != 0) &&
        (touche != CONNEXION_FIN) &&
        (touche != SOMMAIRE) &&
        (touche != ANNULATION) &&
        (touche != RETOUR) &&
        (touche != REPETITION) &&
        (touche != GUIDE) &&
        (touche != CORRECTION) &&
        (touche != SUITE) &&
        (touche != ENVOI)) {
      if (nbCaracteres < 40*nbLignes) {
        nbCaracteres++;
        texte += char(touche);
        if (nbCaracteres == 40*nbLignes) {
          minitel.moveCursorXY(40,6+nbLignes);
        }
      }
      else {
        minitel.moveCursorXY(40,6+nbLignes);
      }
      //Serial.println(texte);
    }
    
    switch (touche) {
      case ENVOI :
        Serial.println(texte);
        fin = true;
        break;
      case ANNULATION :
        formulaireVide();
        break;
      case CORRECTION :
        if (nbCaracteres != 40*nbLignes) {minitel.moveCursorLeft(1);}
        minitel.print(".");
        minitel.moveCursorLeft(1);
        texte = texte.substring(0,texte.length()-1);
        nbCaracteres--;
        break;
    }
  }
  
  // On valide l'enregistrement
  if (touche == ENVOI) {
      minitel.moveCursorXY(6,24);
      minitel.print("Merci de votre participation");
      minitel.noCursor();
    }
    delay(3000); 
  }

////////////////////////////////////////////////////////////////////////
