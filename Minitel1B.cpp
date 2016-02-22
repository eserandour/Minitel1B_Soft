////////////////////////////////////////////////////////////////////////
/*
   Minitel1B - Fichier source - Version du 22 février 2016 à 22 h 17
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

Minitel::Minitel(int rx, int tx) : SoftwareSerial(rx,tx) {
  // A la mise sous tension du Minitel, la vitesse des échanges entre
  // le Minitel et le périphérique est de 1200 bauds par défaut.
  begin(1200);
}
/*--------------------------------------------------------------------*/

void Minitel::writeByte(byte b) {
  // Le bit de parité est mis à 0 si la somme des autres bits est paire
  // et à 1 si elle est impaire.
  boolean parite = 0;
  for (int i=0; i<7; i++) {
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

byte Minitel::readByte() {
  byte b = read();
  // Le bit de parité est à 0 si la somme des autres bits est paire
  // et à 1 si elle est impaire.	
  boolean parite = 0;
  for (int i=0; i<7; i++) {
    if (bitRead(b,i) == 1)  {
	  parite = !parite;
	}
  }
  if (bitRead(b,7) == parite) {  // La transmission est bonne, on peut récupérer la donnée.
	if (bitRead(b,7) == 1) {  // Cas où le bit de parité vaut 1.
      b = b ^ 0b10000000;  // OU exclusif pour mettre le bit de parité à 0 afin de récupérer la donnée.
    }
    return b;
  }
  else {
    return 0xFF;  // Pour indiquer une erreur de parité.
  }
}
/*--------------------------------------------------------------------*/

int Minitel::changeSpeed(int bauds) {  // Voir p.141
  // Format de la commande
  writeBytesPRO2();  // 0x1B 0x3A
  writeByte(PROG);   // 0x6B
  switch (bauds) {
    case  300 : writeByte(0b1010010); begin( 300); break;  // 0x52
    case 1200 : writeByte(0b1100100); begin(1200); break;  // 0x64
    case 4800 : writeByte(0b1110110); begin(4800); break;  // 0x76
    case 9600 : writeByte(0b1111111); begin(9600); break;  // 0x7F (pour le Minitel 2 seulement)
  }
  // Acquittement
  return trameSpeed();  // En bauds (voir section Private ci-dessous)
}
/*--------------------------------------------------------------------*/

int Minitel::currentSpeed() {  // Voir p.141
  // Demande
  writeBytesPRO1();
  writeByte(STATUS_VITESSE);
  // Réponse
  return trameSpeed();  // En bauds (voir section Private ci-dessous)
}
/*--------------------------------------------------------------------*/

void Minitel::newScreen() {
  writeByte(FF);
  currentSize = GRANDEUR_NORMALE;
}
/*--------------------------------------------------------------------*/

void Minitel::newXY(int x, int y) {
  if (x==1 && y==1) {
    writeByte(RS);
  }
  else {
    // Le code US est suivi de deux caractères non visualisés. Si les
    // octets correspondants à ces deux caractères appartiennent tous deux
    // aux colonnes 4 à 7, ils représentent respectivement (sous forme
    // binaire avec 6 bits utiles) le numéro de rangée et le numéro de
    // colonne du premier caractère du sous-article (voir p.96).
    writeByte(US);
    writeByte(0x40 + y);  // Numéro de rangée
    writeByte(0x40 + x);  // Numéro de colonne
  }
  currentSize = GRANDEUR_NORMALE;
}
/*--------------------------------------------------------------------*/

void Minitel::cursor() {
  writeByte(CON);
}
/*--------------------------------------------------------------------*/

void Minitel::noCursor() {
  writeByte(COFF);
}
/*--------------------------------------------------------------------*/

void Minitel::moveCursorXY(int x, int y) {  // Voir p.95
  attributs(CSI);   // 0x1B 0x5B
  writeBytesP(y);   // Pr : Voir section Private ci-dessous
  writeByte(0x3B);
  writeBytesP(x);   // Pc : Voir section Private ci-dessous
  writeByte(0x48);
}
/*--------------------------------------------------------------------*/

void Minitel::moveCursorLeft(int n) {  // Voir p.94 et 95
  if (n==1) { writeByte(BS); }
  else if (n>1) {
	// Curseur vers la gauche de n colonnes. Arrêt au bord gauche de l'écran.
    attributs(CSI);   // 0x1B 0x5B
    writeBytesP(n);   // Pn : Voir section Private ci-dessous
    writeByte(0x44);
  }
}
/*--------------------------------------------------------------------*/

void Minitel::moveCursorRight(int n) {  // Voir p.94
  if (n==1) { writeByte(HT); }
  else if (n>1) {
	// Curseur vers la droite de n colonnes. Arrêt au bord droit de l'écran.
    attributs(CSI);   // 0x1B 0x5B
    writeBytesP(n);   // Pn : Voir section Private ci-dessous
    writeByte(0x43);
  }
}
/*--------------------------------------------------------------------*/

void Minitel::moveCursorDown(int n) {  // Voir p.94
  if (n==1) { writeByte(LF); }
  else if (n>1) {
	// Curseur vers le bas de n rangées. Arrêt en bas de l'écran.
    attributs(CSI);   // 0x1B 0x5B
    writeBytesP(n);   // Pn : Voir section Private ci-dessous
    writeByte(0x42);
  }
}
/*--------------------------------------------------------------------*/

void Minitel::moveCursorUp(int n) {  // Voir p.94
  if (n==1) { writeByte(VT); }
  else if (n>1) {
	// Curseur vers le haut de n rangées. Arrêt en haut de l'écran.
    attributs(CSI);   // 0x1B 0x5B
    writeBytesP(n);   // Pn : Voir section Private ci-dessous
    writeByte(0x41);
  }	
}
/*--------------------------------------------------------------------*/

void Minitel::moveCursorReturn(int n) {  // Voir p.94
  writeByte(CR);
  moveCursorDown(n);  // Pour davantage de souplesse
}
/*--------------------------------------------------------------------*/

void Minitel::cancel() {  // Voir p.95
  writeByte(CAN);
}
/*--------------------------------------------------------------------*/

void Minitel::clearScreenFromCursor() {  // Voir p.95
  attributs(CSI);  // 0x1B 0x5B
  // writeByte(0x30);  Inutile
  writeByte(0x4A);
}
/*--------------------------------------------------------------------*/

void Minitel::clearScreenToCursor() {  // Voir p.95
  attributs(CSI);  // 0x1B 0x5B
  writeByte(0x31);
  writeByte(0x4A);
}
/*--------------------------------------------------------------------*/

void Minitel::clearScreen() {  // Voir p.95
  attributs(CSI);  // 0x1B 0x5B
  writeByte(0x32);
  writeByte(0x4A);
}
/*--------------------------------------------------------------------*/

void Minitel::clearLineFromCursor() {  // Voir p.95
  attributs(CSI);  // 0x1B 0x5B
  // writeByte(0x30);  Inutile
  writeByte(0x4B);
}
/*--------------------------------------------------------------------*/

void Minitel::clearLineToCursor() {  // Voir p.95
  attributs(CSI);  // 0x1B 0x5B
  writeByte(0x31);
  writeByte(0x4B);
}
/*--------------------------------------------------------------------*/

void Minitel::clearLine() {  // Voir p.95
  attributs(CSI);  // 0x1B 0x5B
  writeByte(0x32);
  writeByte(0x4B);
}
/*--------------------------------------------------------------------*/

void Minitel::deleteChars(int n) {  // Voir p.95
  attributs(CSI);  // 0x1B 0x5B
  writeBytesP(n);  // Voir section Private ci-dessous
  writeByte(0x50);
}
/*--------------------------------------------------------------------*/

void Minitel::insertChars(int n) {  // Voir p.95
  attributs(CSI);  // 0x1B 0x5B
  writeBytesP(n);  // Voir section Private ci-dessous
  writeByte(0x40);
}
/*--------------------------------------------------------------------*/

void Minitel::startInsert() {  // Voir p.95
  attributs(CSI);  // 0x1B 0x5B
  writeByte(0x34);
  writeByte(0x68);
}
/*--------------------------------------------------------------------*/

void Minitel::stopInsert() {  // Voir p.95
  attributs(CSI);  // 0x1B 0x5B
  writeByte(0x34);
  writeByte(0x6C);
}
/*--------------------------------------------------------------------*/

void Minitel::deleteLines(int n) {  // Voir p.95
  attributs(CSI);  // 0x1B 0x5B
  writeBytesP(n);  // Voir section Private ci-dessous
  writeByte(0x4D);
}
/*--------------------------------------------------------------------*/

void Minitel::insertLines(int n) {  // Voir p.95
  attributs(CSI);  // 0x1B 0x5B
  writeBytesP(n);  // Voir section Private ci-dessous
  writeByte(0x4C);
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
  writeByte(SS2);  // // Accès au jeu G2 (voir p.103)
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
  writeByte(SS2);  // Accès au jeu G2 (voir p.103)
  writeByte(b);
}
/*--------------------------------------------------------------------*/

void Minitel::repeat(int n) {  // Voir p.98
  writeByte(REP);
  writeByte(0x40 + n);
}
/*--------------------------------------------------------------------*/

void Minitel::bip() {  // Voir p.98
  writeByte(BEL);
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

void Minitel::rect(int x1, int y1, int x2, int y2) {
  hLine(x1,y1,x2,BOTTOM);
  vLine(x2,y1+1,y2,RIGHT,DOWN);
  hLine(x1,y2,x2,TOP);
  vLine(x1,y1,y2-1,LEFT,UP);
}
/*--------------------------------------------------------------------*/

void Minitel::hLine(int x1, int y, int x2, int position) {
  textMode();
  moveCursorXY(x1,y);
  switch (position) {
    case TOP    : writeByte(0x7E); break;
    case CENTER : writeByte(0x60); break;
    case BOTTOM : writeByte(0x5F); break;
  }
  repeat(x2-x1);
}
/*--------------------------------------------------------------------*/

void Minitel::vLine(int x, int y1, int y2, int position, int sens) {
  textMode();
  switch (sens) {
	case DOWN : moveCursorXY(x,y1); break;
    case UP   : moveCursorXY(x,y2); break;
  }
  for (int i=0; i<y2-y1; i++) {
    switch (position) {
      case LEFT   : writeByte(0x7B); break;
      case CENTER : writeByte(0x7C); break;
      case RIGHT  : writeByte(0x7D); break;
    }
    switch (sens) {
	  case DOWN : moveCursorLeft(1); moveCursorDown(1); break;
      case UP   : moveCursorLeft(1); moveCursorUp(1); break;
    }
  }
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

void Minitel::writeBytesP(int n) {
  // Pn, Pr, Pc : Voir remarques p.95 et 96
  if (n<=9) {
    writeByte(0x30 + n);
  }
  else {
    writeByte(0x30 + n/10);
    writeByte(0x30 + n%10);
  }
}
/*--------------------------------------------------------------------*/

void Minitel::writeBytesPRO1() {  // Voir p.134
  writeByte(ESC);  // 0x1B
  writeByte(0x39);	
}
/*--------------------------------------------------------------------*/

void Minitel::writeBytesPRO2() {  // Voir p.134
  writeByte(ESC);  // 0x1B
  writeByte(0x3A);
}
/*--------------------------------------------------------------------*/

void Minitel::writeBytesPRO3() {  // Voir p.134
  writeByte(ESC);  // 0x1B
  writeByte(0x3B);	
}
/*--------------------------------------------------------------------*/

int Minitel::trameSpeed() {
  int bauds = -1;
  while (!isListening());   // On attend que le port soit sur écoute.
  unsigned long trame = 0;  // 32 bits = 4 octets
  while (trame >> 8 != 0x1B3A75) {  // Voir p.141
	if (available() > 0) {
      trame = (trame << 8) + readByte();
      //Serial.println(trame, HEX);
    }
  }
  switch (trame) {
    case 0x1B3A7552 : bauds =  300; break;
    case 0x1B3A7564 : bauds = 1200; break;
    case 0x1B3A7576 : bauds = 4800; break;
    case 0x1B3A757F : bauds = 9600; break;  // Pour le Minitel 2 seulement
  }
  return bauds;	
}
/*--------------------------------------------------------------------*/
