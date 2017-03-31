////////////////////////////////////////////////////////////////////////
/*
   Minitel1B_Soft - Démo - Version du 31 mars 2017 à 21h14
   Copyright 2016 - Eric Sérandour
   
   Documentation utilisée :
   Spécifications Techniques d'Utilisation du Minitel 1B
   http://543210.free.fr/TV/stum1b.pdf
   
////////////////////////////////////////////////////////////////////////
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301 USA.

////////////////////////////////////////////////////////////////////////


Paramètres disponibles pour attributs(byte attribut)
--------------------------------------------------------

// Couleur de caractère
CARACTERE_NOIR / CARACTERE_ROUGE / CARACTERE_VERT / CARACTERE_JAUNE
CARACTERE_BLEU / CARACTERE_MAGENTA / CARACTERE_CYAN / CARACTERE_BLANC

// Couleur de fond
FOND_NOIR / FOND_ROUGE / FOND_VERT / FOND_JAUNE
FOND_BLEU / FOND_MAGENTA / FOND_CYAN / FOND_BLANC

// Taille
GRANDEUR_NORMALE / DOUBLE_HAUTEUR / DOUBLE_LARGEUR / DOUBLE_GRANDEUR

// Clignotement ou fixité
CLIGNOTEMENT / FIXE

// Début et fin de masquage
MASQUAGE / DEMASQUAGE

// Début ou fin de lignage
FIN_LIGNAGE / DEBUT_LIGNAGE

// Fond inversé ou normal
FOND_NORMAL / INVERSION_FOND

// Echappement vers la norme ISO 6429
CSI


Paramètres disponibles pour printDiacriticChar(char caractere)
--------------------------------------------------------------

à â ä è é ê ë î ï ô ö ù û ü ç


Paramètres disponibles pour printSpecialChar(byte b)
----------------------------------------------------

LIVRE / DOLLAR / DIESE / PARAGRAPHE
FLECHE_GAUCHE / FLECHE_HAUT / FLECHE_DROITE / FLECHE_BAS
DEGRE / PLUS_OU_MOINS / DIVISION
UN_QUART / UN_DEMI / TROIS_QUART
OE_MAJUSCULE / OE_MINUSCULE
BETA
                                          
                                                                      */
////////////////////////////////////////////////////////////////////////







////////////////////////////////////////////////////////////////////////

// DEBUT DU PROGRAMME

////////////////////////////////////////////////////////////////////////


#include <Minitel1B_Soft.h>
#include <SoftwareSerial.h>
  
Minitel minitel(8, 9);  // RX, TX

int pause = 10000;

////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);  // Port série matériel de l'ATmega à 9600 bauds.
  minitel.changeSpeed(minitel.searchSpeed());
}

////////////////////////////////////////////////////////////////////////

void loop() {
  demoCaracteres();
  demoTailles();
  demoCouleurs();
  demoCurseur();
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

void demoCaracteres() {
  newPage("LES CARACTERES");
  
  // Mode texte 
  
  minitel.println("MODE TEXTE SANS LIGNAGE :");
  for (int i=0x20; i<=0x7F; i++) {
    minitel.writeByte(i);
  }
  minitel.moveCursorReturn(2);
  
  minitel.println("MODE TEXTE AVEC LIGNAGE :");
  minitel.attributs(DEBUT_LIGNAGE);  // En mode texte, le lignage est déclenché par le premier espace rencontré (0x20).
  for (int i=0x20; i<=0x7F; i++) {   
    minitel.writeByte(i);   
  }
  minitel.attributs(FIN_LIGNAGE);
  minitel.moveCursorReturn(2);  

  // Mode semi-graphique
  
  minitel.textMode();
  minitel.println("MODE SEMI-GRAPHIQUE SANS LIGNAGE :");
  minitel.graphicMode();  
  for (int i=0x20; i<=0x7F; i++) {
    minitel.writeByte(i);
  }
  minitel.moveCursorReturn(2);
  
  minitel.textMode();
  minitel.println("MODE SEMI-GRAPHIQUE AVEC LIGNAGE :");
  minitel.graphicMode();
  minitel.attributs(DEBUT_LIGNAGE);
  for (int i=0x20; i<=0x7F; i++) {
    minitel.writeByte(i);
  }
  minitel.attributs(FIN_LIGNAGE);
  minitel.moveCursorReturn(2);
    
  delay(pause); 
}

////////////////////////////////////////////////////////////////////////

void demoTailles() { 
  newPage("LES TAILLES");
  minitel.println("GRANDEUR_NORMALE");
  minitel.attributs(DOUBLE_HAUTEUR);
  minitel.print("DOUBLE_HAUTEUR");  
  minitel.attributs(DOUBLE_LARGEUR);
  minitel.println();
  minitel.println("DOUBLE_LARGEUR");  
  minitel.attributs(DOUBLE_GRANDEUR); 
  minitel.println("DOUBLE_GRANDEUR");
  minitel.println();
  minitel.attributs(GRANDEUR_NORMALE);
  minitel.attributs(DEBUT_LIGNAGE);  // En mode texte, le lignage est déclenché par le premier espace rencontré (0x20).
  minitel.println("SEULEMENT EN MODE TEXTE");  
  minitel.attributs(FIN_LIGNAGE);
  minitel.println();
  delay(pause);  
}

////////////////////////////////////////////////////////////////////////

void demoCouleurs() {
  newPage("LES COULEURS");
  for (int i=0; i<=1; i++) {   
    if (i==0) { minitel.textMode(); }
    if (i==1) { minitel.graphicMode(); }
    minitel.attributs(INVERSION_FOND);
    minitel.print("CARACTERE_NOIR, FOND_BLANC");
    minitel.attributs(FOND_NORMAL);
    minitel.println(" (INVERSION)");      
    minitel.attributs(CARACTERE_ROUGE);
    minitel.println("CARACTERE_ROUGE");
    minitel.attributs(CARACTERE_VERT);
    minitel.println("CARACTERE_VERT");
    minitel.attributs(CARACTERE_JAUNE);
    minitel.println("CARACTERE_JAUNE");
    minitel.attributs(CARACTERE_BLEU);
    minitel.println("CARACTERE_BLEU");
    minitel.attributs(CARACTERE_MAGENTA);
    minitel.println("CARACTERE_MAGENTA");
    minitel.attributs(CARACTERE_CYAN);
    minitel.println("CARACTERE_CYAN");
    minitel.attributs(CARACTERE_BLANC);
    minitel.println("CARACTERE_BLANC");
    minitel.println();
  }
  delay(pause);
}

////////////////////////////////////////////////////////////////////////

void demoCurseur() {
  minitel.cursor();
  newPage("DEPLACER LE CURSEUR");
  minitel.moveCursorXY(20,12);
  for (int i=1; i<=100; i++) {
    delay(100);
    switch (random(4)) {
      case 0: minitel.moveCursorRight(1+random(3)); break;
      case 1: minitel.moveCursorLeft(1+random(3)); break;
      case 2: minitel.moveCursorDown(1+random(3)); break;
      case 3: minitel.moveCursorUp(1+random(3)); break;
    }
  }
  newPage("POSITIONNER LE CURSEUR");
  minitel.textMode();
  for (int i=1; i<1000; i++) {
    minitel.moveCursorXY(1+random(40),3+random(22));
    minitel.writeByte(0x20 + random(0x60));
  }
  
  minitel.newScreen();
  minitel.textMode();
  minitel.noCursor();
  for (int i=1; i<1000; i++) {
    if (random(4)<3) { minitel.textMode(); }
    else {
      minitel.graphicMode();
      minitel.attributs(DEBUT_LIGNAGE);
    }
    minitel.attributs(0x4C+random(5));
    minitel.writeByte(0x20 + random(0x60));
    minitel.attributs(FIN_LIGNAGE);
  }
}

////////////////////////////////////////////////////////////////////////
