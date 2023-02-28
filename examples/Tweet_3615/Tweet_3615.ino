////////////////////////////////////////////////////////////////////////
/*
   3615 - Version du 28 février 2023 à 21h55
   Copyright 2017-2023 - Eric Sérandour
   
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
  DEBUT DU PROGRAMME (Compile sous Arduino 1.6.13)
*///////////////////////////////////////////////////////////////////////

#include <Minitel1B_Soft.h>
#include <SoftwareSerial.h>

Minitel minitel(8, 9);  // RX, TX

////////////////////////////////////////////////////////////////////////

#define TITRE "3615 TWEET"

String texte="";
int nbCaracteres=0;
const int PREMIERE_LIGNE_EXPRESSION = 4;
const int NB_LIGNES_EXPRESSION = 3;
const String VIDE = ".";

unsigned long touche;

////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);  // Port série matériel de l'ATmega à 9600 bauds.
  minitel.changeSpeed(minitel.searchSpeed());
  minitel.smallMode();
}

////////////////////////////////////////////////////////////////////////

void loop() {
  
  // Affichage de la page
  newPage(TITRE);

  // Lecture du champ expression
  lectureChamp(PREMIERE_LIGNE_EXPRESSION, NB_LIGNES_EXPRESSION);
  Serial.println(texte);
  
  // Message de fin
  minitel.clearScreenFromCursor();
  minitel.noCursor();
  minitel.moveCursorXY(14,24);
  minitel.print("C'est envoyé !");
  delay(2000);
}

////////////////////////////////////////////////////////////////////////

void newPage(String titre) {
  minitel.newScreen();
  minitel.println(titre);
  for (int i=1; i<=40; i++) {
    minitel.writeByte(0x7E);
  }
}

////////////////////////////////////////////////////////////////////////

void champVide(int premiereLigne, int nbLignes)
{
  minitel.noCursor();
  minitel.moveCursorXY(1,premiereLigne);
  minitel.clearScreenFromCursor();
  for (int j=0; j<nbLignes; j++) {
    minitel.attributs(CARACTERE_BLEU);
    minitel.print(VIDE);
    minitel.repeat(39);
    minitel.attributs(CARACTERE_BLANC);
  }
  texte="";
  nbCaracteres=0;
  minitel.moveCursorXY(31,24);
  minitel.print("puis ");
  minitel.attributs(INVERSION_FOND);
  minitel.print("ENVOI");
  minitel.attributs(FOND_NORMAL);
  minitel.moveCursorDown(premiereLigne-1);
  minitel.cursor();
}

////////////////////////////////////////////////////////////////////////

void correction(int nbLignes) {
  if ((nbCaracteres > 0) && (nbCaracteres <= 40*nbLignes)) {
    if (nbCaracteres != 40*nbLignes) { minitel.moveCursorLeft(1); }
    minitel.attributs(CARACTERE_BLEU);
    minitel.print(VIDE);
    minitel.attributs(CARACTERE_BLANC);
    minitel.moveCursorLeft(1);
    texte = texte.substring(0,texte.length()-1);
    nbCaracteres--;
  }
}

////////////////////////////////////////////////////////////////////////

void lectureChamp(int premiereLigne, int nbLignes) {
  champVide(premiereLigne,nbLignes);
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
        // texte += char(touche);
        texte += minitel.getString(touche);
      }
      if (nbCaracteres == 40*nbLignes) {
        minitel.moveCursorXY(40,(premiereLigne-1)+nbLignes);
      }
    }
  switch (touche) {
    case ENVOI : 
      fin = true;
      break;
    case ANNULATION :
      champVide(premiereLigne,nbLignes);
      break;
    case CORRECTION :
      correction(nbLignes);
      break;
  }    
  }
}

////////////////////////////////////////////////////////////////////////
