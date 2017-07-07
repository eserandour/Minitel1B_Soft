# Minitel1B_Soft
<b>Minitel Library for Arduino (with SoftwareSerial)</b>

Documentation utilisée : Spécifications Techniques d'Utilisation du Minitel 1B<br>
http://543210.free.fr/TV/stum1b.pdf

Démo en vidéo : http://serandour.com/minitel1b.htm

<b>Historique</b> :

28/06/2017 - Fichier Fritzing shield-3615.fzz (http://3615.entropie.org)<br>

18/06/2017 - Remplacement de l'exemple Morue_3615.ino par Tweet_3615.ino<br>

15/06/2017 - 2 nouvelles fonctions qui donnent la position du curseur :<br>
int getCursorX()<br>
int getCursorY()<br>

06/06/2017 - 2 nouvelles fonctions graphiques :<br>
void graphic(byte b, int x, int y)<br>
void graphic(byte b)<br>

31/03/2017 - Modification de l'interfaçage : RX (D8) / TX (D9)<br>

04/11/2016 - Exemple d'utilisation du clavier avec Morue_3615.ino<br>

14/10/2016 - La bibliothèque Minitel1B est renommée Minitel1B_Soft<br>

12/03/2016 - Nouvelle version de Minitel1B (sans SoftwareSerial) :<br>
https://github.com/eserandour/Minitel1B_Hard<br>

07/03/2016 - 6 nouvelles fonctions :<br>
int pageMode()<br>
int scrollMode()<br>
int smallMode()<br>
int capitalMode()<br>
int extendedKeyboard()<br>
int standardKeyboard()<br>

06/03/2016 - Modification du câble :<br>
Ajout d'une résistance de tirage de 10 kilohms sur la sortie TX du minitel.

05/03/2016 - 1 nouvelle fonction :<br>
unsigned long getKeyCode()<br>

03/03/2016 - 1 nouvelle fonction :<br>
int searchSpeed()<br>

22/02/2016 - 3 nouvelles fonctions géométriques :<br>
void rect(int x1, int y1, int x2, int y2)<br>
void hLine(int x1, int y, int x2, int position)<br>
void vLine(int x, int y1, int y2, int position, int sens)<br>

21/02/2016 - 3 nouvelles fonctions :<br>
byte readByte()<br>
int changeSpeed(int bauds)<br>
int currentSpeed()<br>

19/02/2016 - 13 nouvelles fonctions de mise en page :<br>
void cancel()<br>
void clearScreenFromCursor()<br>
void clearScreenToCursor()<br>
void clearScreen()<br>
void clearLineFromCursor()<br>
void clearLineToCursor()<br>
void clearLine()<br>
void deleteChars(int n)<br>
void insertChars(int n)<br>
void startInsert()<br>
void stopInsert()<br>
void deleteLines(int n)<br>
void insertLines(int n)<br>

17/02/2016 - Début du projet sur GitHub<br>

02/02/2016 - Fabrication du câble
