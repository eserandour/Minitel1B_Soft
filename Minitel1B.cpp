////////////////////////////////////////////////////////////////////////
/*
   Minitel1B - Fichier source - Version du 17 février 2016
   Copyright 2016 - Eric Sérandour
   http://bidouille.serandour.com
   
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
*/
////////////////////////////////////////////////////////////////////////

#include "Minitel1B.h"

////////////////////////////////////////////////////////////////////////
/*
   Public
*/
////////////////////////////////////////////////////////////////////////

Minitel::Minitel(int rx, int tx, int vitesse) : SoftwareSerial(rx,tx) {
  // A la mise sous tension du Minitel, la vitesse des échanges entre
  // le Minitel et le périphérique est de 1200 bauds par défaut.
  // L'usager du Minitel peut programmer la vitesse des échanges avec
  // le périphérique quel que soit l'état du terminal grâce aux
  // commandes suivantes :
  // Fnct P + 3 : 300 bauds
  // Fnct P + 1 : 1200 bauds
  // Fnct P + 4 : 4800 bauds
  begin(vitesse);
}
/*--------------------------------------------------------------------*/

void Minitel::writeByte(byte b) {
  // Le bit de parité est mis à 0 si la somme des autres bits est paire
  // et à 1 si elle est impaire.
  boolean parite = 0;
  for (int i=0; i<8; i++) {
    if (bitRead(b,i) == 1)  {
	  parite = !parite;
	}
  }
  if (parite) {
    bitWrite(b,7,1);  // Ecriture du bit de parité
  }
  else {
    bitWrite(b,7,0);  // Ecriture du bit de parité
  }
  write(b);  // Envoi de l'octet sur le port série logiciel
}

/*--------------------------------------------------------------------*/

void Minitel::newScreen() {
  writeByte(FF);
  currentSize = GRANDEUR_NORMALE;
}
/*--------------------------------------------------------------------*/

void Minitel::newXY(byte x, byte y) {
  if (x==1 && y==1) {
    writeByte(RS);
  }
  else {
    // Le code US est suivi de deux caractères non visualisés. Si les
    // octets correspondants à ces deux caractères appartiennent tous deux
    // aux colonnes 4 à 7, ils représentent respectivement (sous forme
    // binaire avec 6 bits utiles) le numéro de rangée et le numéro de
    // colonne du premier caractère du sous-article (voir p.96).
    writeByte(US);        // 
    writeByte(0x40 + y);  // Numéro de rangée
    writeByte(0x40 + x);  // Numéro de colonne
  }
  currentSize = GRANDEUR_NORMALE;
}
/*--------------------------------------------------------------------*/

void Minitel::gotoXY(byte x, byte y) {
  attributs(CSI);            // 0x1B 0x5B
  // Pr
  if (y<=9) {
	writeByte(0x30 + y);     // Voir remarque concernant Pr p.96
  }
  else {
    writeByte(0x30 + y/10);  // Voir remarque concernant Pr p.96
    writeByte(0x30 + y%10);  // Voir remarque concernant Pr p.96
  }
  writeByte(0x3B);           // 0x3B
  // Pc
  if (y<=9) {
	writeByte(0x30 + x);     // Voir remarque concernant Pc p.96
  }
  else {
    writeByte(0x30 + x/10);  // Voir remarque concernant Pc p.96
    writeByte(0x30 + x%10);  // Voir remarque concernant Pc p.96
  }
  writeByte(0x48);           // 0x48
}
/*--------------------------------------------------------------------*/

void Minitel::moveCursorLeft(int n) {  // Voir p.95
  if (n==1) { writeByte(BS); }
  else if (n>1) {
	// Curseur vers la gauche de n colonnes. Arrêt au bord gauche de l'écran.
    attributs(CSI);            // 0x1B 0x5B
    if (n<=9) {
	  writeByte(0x30 + n);     // Voir remarque concernant Pn p.95
	}
	else {
      writeByte(0x30 + n/10);  // Voir remarque concernant Pn p.95
      writeByte(0x30 + n%10);  // Voir remarque concernant Pn p.95
    }  
    writeByte(0x44);
  }
}
/*--------------------------------------------------------------------*/

void Minitel::moveCursorRight(int n) {  // Voir p.94
  if (n==1) { writeByte(HT); }
  else if (n>1) {
	// Curseur vers la droite de n colonnes. Arrêt au bord droit de l'écran.
    attributs(CSI);            // 0x1B 0x5B
    if (n<=9) {
	  writeByte(0x30 + n);     // Voir remarque concernant Pn p.95
	}
	else {
      writeByte(0x30 + n/10);  // Voir remarque concernant Pn p.95
      writeByte(0x30 + n%10);  // Voir remarque concernant Pn p.95
    }  
    writeByte(0x43);
  }
}
/*--------------------------------------------------------------------*/

void Minitel::moveCursorDown(int n) {
  if (n==1) { writeByte(LF); }
  else if (n>1) {
	// Curseur vers le bas de n rangées. Arrêt en bas de l'écran.
    attributs(CSI);            // 0x1B 0x5B
    if (n<=9) {
	  writeByte(0x30 + n);     // Voir remarque concernant Pn p.95
	}
	else {
      writeByte(0x30 + n/10);  // Voir remarque concernant Pn p.95
      writeByte(0x30 + n%10);  // Voir remarque concernant Pn p.95
    }  
    writeByte(0x42);
  }
}
/*--------------------------------------------------------------------*/

void Minitel::moveCursorUp(int n) {
  if (n==1) { writeByte(VT); }
  else if (n>1) {
	// Curseur vers le haut de n rangées. Arrêt en haut de l'écran.
    attributs(CSI);            // 0x1B 0x5B
    if (n<=9) {
	  writeByte(0x30 + n);     // Voir remarque concernant Pn p.95
	}
	else {
      writeByte(0x30 + n/10);  // Voir remarque concernant Pn p.95
      writeByte(0x30 + n%10);  // Voir remarque concernant Pn p.95
    }  
    writeByte(0x41);
  }	
}
/*--------------------------------------------------------------------*/

void Minitel::moveCursorReturn(int n) {
  writeByte(CR);
  moveCursorDown(n);
}
/*--------------------------------------------------------------------*/

void Minitel::textMode() {
  writeByte(SI);  // Accès au jeu G0 (voir p.100)
}
/*--------------------------------------------------------------------*/

void Minitel::graphicMode() {
  writeByte(SO);  // Accès au jeu G1 (voir p.101 & 102)
}
/*--------------------------------------------------------------------*/

void Minitel::specialMode() {
  writeByte(SS2);  // Accès au jeu G2 (voir p.103 & 104)
}
/*--------------------------------------------------------------------*/

void Minitel::attributs(byte attribut) {
  writeByte(ESC);  // Accès à la grille C1 (voir p.92)
  writeByte(attribut);
  if (attribut == DOUBLE_HAUTEUR || attribut == DOUBLE_GRANDEUR) {
    moveCursorDown(1);
    currentSize = attribut;
  }
  else if (attribut == GRANDEUR_NORMALE || attribut == DOUBLE_LARGEUR) {
    currentSize = attribut;
  }
}
/*--------------------------------------------------------------------*/

void Minitel::cursor(boolean cursor) {
  if(cursor) {
    writeByte(CON);
  }
  else {
    writeByte(COFF);
  }
}
/*--------------------------------------------------------------------*/

void Minitel::bip() {
  writeByte(BEL);
}
/*--------------------------------------------------------------------*/

void Minitel::print(String chaine) {
  for (int i=0; i<chaine.length(); i++) {
    char caractere = chaine.charAt(i);           
    if (!isDiacritic(caractere)) {
	  printChar(caractere);
	}
	else {	  	  
	  i+=1;  // Un caractère accentué prend la place de 2 caractères
	  caractere = chaine.charAt(i);
	  printDiacriticChar(caractere);
	}
  }
}
/*--------------------------------------------------------------------*/

void Minitel::println(String chaine) {
  print(chaine);
  if (currentSize == DOUBLE_HAUTEUR || currentSize == DOUBLE_GRANDEUR) {
    moveCursorReturn(2);
  }
  else {
    moveCursorReturn(1);
  }
}

/*--------------------------------------------------------------------*/

void Minitel::println() {
  if (currentSize == DOUBLE_HAUTEUR || currentSize == DOUBLE_GRANDEUR) {
    moveCursorReturn(2);
  }
  else {
    moveCursorReturn(1);
  }
}

/*--------------------------------------------------------------------*/

void Minitel::printChar(char caractere) {
  byte charByte = getCharByte(caractere);
  if (isValidChar(charByte)) {
    writeByte(charByte);
  }
}
/*--------------------------------------------------------------------*/

void Minitel::printDiacriticChar(char caractere) {		
  specialMode();  // Accès au jeu G2 (voir p.103)
  String diacritics = "àâäèéêëîïôöùûüç";
  // Dans une chaine de caractères, un caractère diacritique prend la
  // place de 2 caractères simples, ce qui explique le /2.
  int index = (diacritics.indexOf(caractere)-1)/2;
  char car;
  switch (index) {
    case( 0): car = 'a'; writeByte(ACCENT_GRAVE); break;
    case( 1): car = 'a'; writeByte(ACCENT_CIRCONFLEXE); break;
    case( 2): car = 'a'; writeByte(TREMA); break;
    case( 3): car = 'e'; writeByte(ACCENT_GRAVE); break;
    case( 4): car = 'e'; writeByte(ACCENT_AIGU); break;      
    case( 5): car = 'e'; writeByte(ACCENT_CIRCONFLEXE); break;
    case( 6): car = 'e'; writeByte(TREMA); break;
    case( 7): car = 'i'; writeByte(ACCENT_CIRCONFLEXE); break;
    case( 8): car = 'i'; writeByte(TREMA); break;
    case( 9): car = 'o'; writeByte(ACCENT_CIRCONFLEXE); break;
    case(10): car = 'o'; writeByte(TREMA); break;
    case(11): car = 'u'; writeByte(ACCENT_GRAVE); break;
    case(12): car = 'u'; writeByte(ACCENT_CIRCONFLEXE); break;      
    case(13): car = 'u'; writeByte(TREMA); break;
    case(14): car = 'c'; writeByte(CEDILLE); break;
  }
  printChar(car);
}
/*--------------------------------------------------------------------*/

void Minitel::printSpecialChar(byte b) {
  // N'est pas fonctionnelle pour les diacritiques (accents, tréma et cédille)
  specialMode();  // Accès au jeu G2 (voir p.103)
  writeByte(b);
}
/*--------------------------------------------------------------------*/

byte Minitel::getCharByte(char caractere) {
  // Voir les codes et séquences émis en mode Vidéotex (Jeu G0 p.100).
  // Dans la chaine ci-dessous, on utilise l'échappement (\) :
  // \" rend au guillemet sa signification littérale.
  // \\ donne à l'antislash sa signification littérale .
  String caracteres = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_xabcdefghijklmnopqrstuvwxyz";
  return (byte) caracteres.lastIndexOf(caractere);
}
/*--------------------------------------------------------------------*/






////////////////////////////////////////////////////////////////////////
/*
   Private
*/
////////////////////////////////////////////////////////////////////////

boolean Minitel::isValidChar(byte index) {
  // On vérifie que le caractère appartient au jeu G0 (voir p.100).
  // SP (0x20) correspond à un espace et DEL (0x7F) à un pavé plein.
  if (index >= SP && index <= DEL) {
    return true;
  }
  return false;
}
/*--------------------------------------------------------------------*/

boolean Minitel::isDiacritic(char caractere) {
  String accents = "àâäèéêëîïôöùûüç";
  if (accents.indexOf(caractere) >= 0) {
    return true; 
  }
  return false;
}
/*--------------------------------------------------------------------*/





