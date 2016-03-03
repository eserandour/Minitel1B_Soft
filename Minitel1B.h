////////////////////////////////////////////////////////////////////////
/*
   Minitel1B - Fichier d'en-tête - Version du 3 mars 2016 à 22 h 11
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

#ifndef MINITEL1B_H  // Si la constante MINITEL1B_H n'est pas définie,
#define MINITEL1B_H  // on la définit.

// Selon la version d'Arduino
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif  // Fin Si (ARDUINO)

#include "SoftwareSerial.h"

////////////////////////////////////////////////////////////////////////

// Le standard Télétel

// 1 Mode Vidéotex

// 1.2.3 Codage des caractères visualisables
// Jeu G0 => alphanumérique (voir p.100)
// Jeu G1 => semi-graphique (voir p.101 et 102)
// Jeu G2 => complément à G0 (voir p.103)
// Les caractères du jeu G2 sont obtenus si précédés du code SS2 (0x19).
// On peut les afficher directement en utilisant printSpecialChar(byte b) :
#define LIVRE              0x23
#define DOLLAR             0x24
#define DIESE              0x26
#define PARAGRAPHE         0x27
#define FLECHE_GAUCHE      0x2C
#define FLECHE_HAUT        0x2D
#define FLECHE_DROITE      0x2E
#define FLECHE_BAS         0x2F
#define DEGRE              0x30
#define PLUS_OU_MOINS      0x31
#define DIVISION           0x38
#define UN_QUART           0x3C
#define UN_DEMI            0x3D
#define TROIS_QUART        0x3E
#define OE_MAJUSCULE       0x6A
#define OE_MINUSCULE       0x7A
#define BETA               0x7B
// Les diacritiques ne peuvent pas être affichés seuls.
// printSpecialChar(byte b) n'aura donc aucun effet ici.
#define ACCENT_GRAVE       0x41
#define ACCENT_AIGU        0x42
#define ACCENT_CIRCONFLEXE 0x43
#define TREMA              0x48
#define CEDILLE            0x4B


// 1.2.4 Codage des attributs de visualisation (voir p.91)
// Ces fonctions sont obtenues si précédées du code ESC (0x1B).
// Nous avons alors accès à la grille C1. On peut y accéder directement
// en utilisant attributs(byte attribut).
// Couleur de caractère
#define CARACTERE_NOIR    0x40
#define CARACTERE_ROUGE   0x41
#define CARACTERE_VERT    0x42
#define CARACTERE_JAUNE   0x43
#define CARACTERE_BLEU    0x44
#define CARACTERE_MAGENTA 0x45
#define CARACTERE_CYAN    0x46
#define CARACTERE_BLANC   0x47
// Couleur de fond              // En mode texte, l'espace (0x20) est l'élément déclencheur du changement de couleur de fond (voir p.93). Ce changement est valide jusqu'à la fin d'une rangée.
#define FOND_NOIR         0x50  // Pour éviter d'avoir cet espace à l'écran, une autre solution (dans ce cas le caractère sera noir), est de mettre en oeuvre le fond inversé.
#define FOND_ROUGE        0x51  // Par exemple :
#define FOND_VERT         0x52  // minitel.attributs(CARACTERE_VERT);
#define FOND_JAUNE        0x53  // minitel.attributs(INVERSION_FOND);
#define FOND_BLEU         0x54  // minitel.print("J'ECRIS ICI MON TEXTE");
#define FOND_MAGENTA      0x55  // minitel.attributs(FOND_NORMAL);
#define FOND_CYAN         0x56
#define FOND_BLANC        0x57
// Taille
#define GRANDEUR_NORMALE  0x4C  // Non utilisable en mode graphique
#define DOUBLE_HAUTEUR    0x4D  // Non utilisable en mode graphique
#define DOUBLE_LARGEUR    0x4E  // Non utilisable en mode graphique
#define DOUBLE_GRANDEUR   0x4F  // Non utilisable en mode graphique
// Clignotement ou fixité
#define CLIGNOTEMENT      0x48
#define FIXE              0x49
// Début et fin de masquage
#define MASQUAGE          0x58
#define DEMASQUAGE        0x5F
// Début ou fin de lignage
#define FIN_LIGNAGE       0x59
#define DEBUT_LIGNAGE     0x5A  // En mode texte, l'espace (0x20) marque le début d'une zone de lignage. C'est l'élément déclencheur (voir p.93).
// Fond inversé ou normal
#define FOND_NORMAL       0x5C  // Non utilisable en mode graphique
#define INVERSION_FOND    0x5D  // Non utilisable en mode graphique
// Echappement vers la norme ISO 6429
#define CSI               0x5B


// 1.2.5 Fonctions de mise en page (voir p.94)
#define BS   0x08  // BackSpace : Déplacement du curseur d'un emplacement de caractère à gauche.
#define HT   0x09  // Horizontal Tab : Déplacement du curseur d'un emplacement de caractère à droite.
#define LF   0x0A  // Line Feed : Déplacement du curseur d'un emplacement de caractère vers le bas.
#define VT   0x0B  // Vertical Tab : Déplacement du curseur d'un emplacement de caractère vers le haut.
#define CR   0x0D  // Carriage Return : Retour du curseur au début de la rangée courante.
// Les fonctions de type CSI sont développées à l'intérieur de la classe Minitel (plus bas).
#define RS   0x1E  // Record Separator : Retour du curseur en première position de la rangée 01. Ce code est un séparateur explicite d'article.
#define FF   0x0C  // Form Feed : Retour du curseur en première position de la rangée 01 avec effacement complet de l'écran.
#define US   0x1F  // Unit Separator : Séparateur de sous-article.
#define CAN  0x18  // Cancel : Remplissage à partir de la position courante du curseur et jusqu'à la fin de la rangée par des espaces du jeu courant ayant l'état courant des attributs. La position courante du curseur n'est pas déplacée.


// 1.2.6 Autres fonctions (voir p.98)
// 1.2.6.1 Fonctions diverses :
#define REP  0x12  // Repetition : Permet de répéter le dernier caractère visualisé avec les attributs courants de la position active d'écriture.
#define NUL  0x00  // Null :
#define SP   0x20  // Space :
#define DEL  0x7F  // Delete :
#define BEL  0x07  // Bell : Provoque l'émission d'un signal sonore
// 1.2.6.2 Demande de position du curseur
//...
// 1.2.6.3 Fonctions d'extension de code
#define SO   0x0E  // Shift Out : Accès au jeu G1. => Mode semi-graphique
#define SI   0x0F  // Shift In : Accès au jeu G0.  => Mode alphanumérique
#define SS2  0x19  // Single Shift 2 : Appel d'un caractère unique du jeu G2.
#define ESC  0x1B  // Escape : Echappement et accès à la grille C1.
// 1.2.6.4 Visualisation du curseur
#define CON  0x11  // Visualisation de la position active du curseur (curseur actif).
#define COFF 0x14  // Arrêt de la visualisation de la position active (curseur inactif).


// 1.2.7 Filtages particuliers (voir p.99)
//...


// 1.2.8 Comportements en cas d'erreur et resynchronisation (voir p.99)
// ...




// 6 Le Protocole

#define PROG                0x6B
#define STATUS_VITESSE      0x74
#define REP_STATUS_VITESSE  0x75




// Constantes personnelles pour hline et vline
#define CENTER  0
#define TOP     1
#define BOTTOM  2
#define LEFT    3
#define RIGHT   4
#define UP      5
#define DOWN    6



////////////////////////////////////////////////////////////////////////

class Minitel : public SoftwareSerial
{
public:
  Minitel(int rx, int tx);
  
  // Ecrire / Lire un octet
  void writeByte(byte b);
  byte readByte();
  
  // Vitesse de la liaison série
  // A la mise sous tension du Minitel, la vitesse des échanges entre
  // le Minitel et le périphérique est de 1200 bauds par défaut.
  // L'usager du Minitel peut programmer au clavier la vitesse des
  // échanges avec le périphérique quel que soit l'état du terminal
  // grâce aux commandes suivantes :
  // Fnct P + 3 : 300 bauds
  // Fnct P + 1 : 1200 bauds
  // Fnct P + 4 : 4800 bauds
  // Fnct P + 9 : 9600 bauds (pour le Minitel 2 seulement)
  // Attention ! Si le Minitel et le périphérique ne communiquent pas
  // à la même vitesse, on perd la liaison.
  int changeSpeed(int bauds);  // A tout moment, un périphérique peut modifier les vitesses d'échange de la prise (vitesses possibles : 300, 1200, 4800 bauds ; également 9600 bauds pour le Minitel 2).
  int currentSpeed();  // Pour connaitre la vitesse d'échange en cours, le Minitel et le périphérique échangeant à la même vitesse.
  int searchSpeed();  // Pour connaitre la vitesse du Minitel, le Minitel et le périphérique n'échangeant pas nécessairement à la même vitesse.
  
  // Séparateurs
  void newScreen();  // Attention ! newScreen réinitialise les attributs de visualisation.
  void newXY(int x, int y);  // Attention ! newXY réinitialise les attributs de visualisation.
  
  // Curseur
  void cursor();  // Curseur visible
  void noCursor();  // Curseur invisible
  void moveCursorXY(int x, int y);  // Adressage direct du curseur en colonne x et rangée y.
  void moveCursorLeft(int n);  // Curseur vers la gauche de n colonnes. Arrêt au bord gauche de l'écran.
  void moveCursorRight(int n);  // Curseur vers la droite de n colonnes. Arrêt au bord droit de l'écran.
  void moveCursorDown(int n);  // Curseur vers le bas de n rangées. Arrêt en bas de l'écran.
  void moveCursorUp(int n);  // Curseur vers le haut de n rangées. Arrêt en haut de l'écran.
  void moveCursorReturn(int n);  // Retour du curseur au début de la rangée courante puis curseur vers le bas de n rangées. Arrêt en bas de l'écran.
  
  // Effacements, Suppressions, Insertions
  void cancel();  // Remplissage à partir de la position courante du curseur et jusqu'à la fin de la rangée par des espaces du jeu courant ayant l'état courant des attributs. Le position courante du curseur n'est pas déplacée.
  void clearScreenFromCursor();  // Effacement depuis le curseur inclus jusqu'à la fin de l'écran.
  void clearScreenToCursor();  // Effacement depuis le début de l'écran jusqu'au curseur inclus.
  void clearScreen();  // Effacement de tout l'écran (la position du curseur n'est pas modifiée).
  void clearLineFromCursor();  // Effacement depuis le curseur inclus jusqu'à la fin de la rangée.
  void clearLineToCursor();  // Effacement depuis le début de la rangée jusqu'au curseur inclus.
  void clearLine();  // Effacement total de la rangée où est le curseur.
  void deleteChars(int n);  // Suppression de n caractères en commençant à la position curseur incluse.
  void insertChars(int n);  // Insertion de n caractères en commençant à la position curseur incluse (modèle RTIC uniquement, pas le MATRA ou le TELIC).
  void startInsert();  // Début du mode insertion de caractères.
  void stopInsert();  // Fin du mode insertion de caractères.
  void deleteLines(int n);  // Suppression de n rangées à partir de celle où est le curseur.
  void insertLines(int n);  // Insertion de n rangées à partir de celle où est le curseur.
  
  // Modes
  void textMode();     // Accès au jeu G0
  void graphicMode();  // Accès au jeu G1
  
  // Contenu
  void attributs(byte attribut); 
  void print(String chaine);
  void println(String chaine);
  void println();
  void printChar(char caractere);  // Caractère du jeu G0 exceptés ceux codés 0x60, 0x7B à 0x7F.
  void printDiacriticChar(char caractere);  // Caractère avec accent, tréma ou cédille.  
  void printSpecialChar(byte b);  // Caractère du jeu G2. Voir plus haut, au niveau de 1.2.3, les constantes possibles.
  void repeat(int n);  // Permet de répéter le dernier caractère visualisé avec les attributs courants de la position active d'écriture.
  void bip();  // Bip sonore 
  byte getCharByte(char caractere);
  
  // Géométrie
  void rect(int x1, int y1, int x2, int y2);  // Rectangle défini par 2 points.
  void hLine(int x1, int y, int x2, int position);  // Ligne horizontale. position = TOP, CENTER ou BOTTOM.
  void vLine(int x, int y1, int y2, int position, int sens);  // Ligne verticale. position = LEFT, CENTER ou RIGHT. sens = DOWN ou UP.
  

private: 
  byte currentSize = GRANDEUR_NORMALE;
  boolean isValidChar(byte index);
  boolean isDiacritic(char caractere);
  void writeBytesP(int n);  // Pn, Pr, Pc
  
  // Protocole
  void writeBytesPRO1();  // PRO1
  void writeBytesPRO2();  // PRO2
  void writeBytesPRO3();  // PRO3
  int trameSpeed();
};

////////////////////////////////////////////////////////////////////////

#endif // Fin Si (MINITEL1B_H)
