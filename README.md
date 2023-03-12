# Minitel1B_Soft
<b>Minitel Library for Arduino (with SoftwareSerial)</b>

Documentation utilisée : Spécifications Techniques d'Utilisation du Minitel 1B<br>
http://543210.free.fr/TV/stum1b.pdf

<b>Historique</b> :

<b>Dernière Version :</b> 12/03/2023 à 00h35.<br>

12/03/2023<br>
<b>Correction d'un bug mineur</b> dans getKeyCode(bool unicode).<br>

06/03/2023<br>
<b>Mise à jour de l'exemple :</b><br>
Tweet_3615.ino<br>

05/03/2023<br>
<b>Ajout d'une nouvelle fonction :</b><br>
int getNbBytes(unsigned long code)<br>
à utiliser (si besoin) en association avec String getString(unsigned long code) afin de déterminer le nombre d'octets utilisés pour un caractère codé en UTF-8.<br>

28/02/2023<br>
Modification de l'exemple :<br>
Tweet_3615.ino<br>

28/02/2023 - Sur proposition de iodeo :<br>
Pour une meilleure gestion des <b>caractères spéciaux</b> (notamment dans la console série) :<br>
<b>Ajout de nouvelles fonctions :</b><br>
void writeCode(unsigned long code)<br>
String getString(unsigned long code)<br>
boolean isVisualisable(unsigned long code)<br>
<b>Modification de :</b><br>
void print(String chaine)<br>
unsigned long getKeyCode(bool unicode = true)<br>
<b>Suppression, car devenues inutiles, de :</b><br>
void printDiacriticChar(unsigned char caractere)<br>
boolean isDiacritic(unsigned char caractere)<br>
<b>Ajout d'une fonction pour identifier le Minitel :</b><br>
unsigned long identifyDevice()<br>

25/06/2022 - Sur proposition de iodeo :<br>
Correction d'un bug dans la fonction byte reset()

24/04/2022<br>
Ajout d'un exemple : Portrait.ino (affichage d'une image)<br>

14/03/2022 - Sur proposition de iodeo :<br>
Correction de bugs mineurs dans echo(boolean commande) et changeSpeed(int bauds).<br>
Ajout d'une nouvelle fonction liée au modem (équivalent de la touche CONNEXION / FIN) :<br>
byte connexion(boolean commande)<br>
Ajout d'une option qui permet de désactiver la conversion des caractères en ascii étendue (cela sert en particulier pour la communication avec des services Minitel externes) :<br>
unsigned long getKeyCode(bool ascii = true)<br>
Ajout d'un exemple basique : HelloWorld.ino

02/07/2021<br>
1 nouvelle fonction :<br>
byte reset()<br>

01/07/2021<br>
1 nouvelle fonction :<br>
byte statusAiguillage(byte module)<br>
et mise à jour de :<br>
byte pageMode()<br>
byte scrollMode()<br>
byte modeMixte()<br>
byte modeVideotex()<br>
byte standardTeleinformatique()<br>
byte standardTeletel()<br>
byte smallMode()<br>
byte capitalMode()<br>
byte extendedKeyboard()<br>
byte standardKeyboard()<br>
byte echo(boolean commande)<br>
byte aiguillage(boolean commande, byte emetteur, byte recepteur)<br>

29/06/2021<br>
5 nouvelles fonctions :<br>
void writeWord(word w)<br>
int modeMixte()<br>
int modeVideotex()<br>
int standardTeleinformatique()<br>
int standardTeletel()<br>

28/06/2021<br>
Modification de la fonction :<br>
unsigned long getKeyCode()<br>
pour tenir compte des codes du clavier étendu.<br>
Dans le fichier d'entête (.h), ajout de plusieurs #define dans la partie "Clavier".<br>
2 nouvelles fonctions :<br>
void echo(boolean commande)<br>
void aiguillage(boolean commande, byte emetteur, byte recepteur)

11/05/2018<br>
Modification de deux fonctions :<br>
void printDiacriticChar(unsigned char caractere)<br>
boolean isDiacritic(unsigned char caractere)<br>
pour tenir compte de la remarque de BorisFR sur les caractères diacritiques majuscules.

28/06/2017<br>
Fichier Fritzing shield-3615.fzz (http://3615.entropie.org)<br>
![alt text](http://entropie.org/3615/wp-content/uploads/2017/06/shield-3615-top-300x233bis.png)

18/06/2017<br>
Remplacement de l'exemple Morue_3615.ino par Tweet_3615.ino<br>

15/06/2017<br>
2 nouvelles fonctions qui donnent la position du curseur :<br>
int getCursorX()<br>
int getCursorY()<br>

06/06/2017<br>
2 nouvelles fonctions graphiques :<br>
void graphic(byte b, int x, int y)<br>
void graphic(byte b)<br>

31/03/2017<br>
Modification de l'interfaçage : RX (D8) / TX (D9)<br>

04/11/2016<br>
Exemple d'utilisation du clavier avec Morue_3615.ino<br>

14/10/2016<br>
La bibliothèque Minitel1B est renommée Minitel1B_Soft<br>

12/03/2016<br>
Nouvelle version de Minitel1B (sans SoftwareSerial) :<br>
https://github.com/eserandour/Minitel1B_Hard<br>

07/03/2016<br>
6 nouvelles fonctions :<br>
int pageMode()<br>
int scrollMode()<br>
int smallMode()<br>
int capitalMode()<br>
int extendedKeyboard()<br>
int standardKeyboard()<br>

06/03/2016<br>
Modification du câble :<br>
Ajout d'une résistance de tirage de 10 kilohms sur la sortie TX du minitel.

05/03/2016<br>
1 nouvelle fonction :<br>
unsigned long getKeyCode()<br>

03/03/2016<br>
1 nouvelle fonction :<br>
int searchSpeed()<br>

22/02/2016<br>
3 nouvelles fonctions géométriques :<br>
void rect(int x1, int y1, int x2, int y2)<br>
void hLine(int x1, int y, int x2, int position)<br>
void vLine(int x, int y1, int y2, int position, int sens)<br>

21/02/2016<br>
3 nouvelles fonctions :<br>
byte readByte()<br>
int changeSpeed(int bauds)<br>
int currentSpeed()<br>

19/02/2016<br>
13 nouvelles fonctions de mise en page :<br>
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

17/02/2016<br>
Début du projet sur GitHub<br>

02/02/2016<br>
Fabrication du câble
