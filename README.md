# Minitel1B
<b>Minitel library for Arduino</b>

Documentation utilisée : Spécifications Techniques d'Utilisation du Minitel 1B<br>
http://543210.free.fr/TV/stum1b.pdf

Démo en vidéo : http://serandour.com/minitel1b.htm

<b>Historique</b> :

06/03/2016 - Modification du câble<br>
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
