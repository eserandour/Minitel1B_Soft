Branchements :

Dos du Minitel            Carte Arduino

DIN 1 : RX    <-------->  Digital Pin 8 par exemple (TX)
DIN 4 :       <---//--->
DIN 2 : GND   <-------->  GND
DIN 5 : +8,5V <---//--->
DIN 3 : TX    <----+--->  Digital Pin 7 par exemple (RX)
                   |
                   R
                   |
                  +5V

La ligne TX est à collecteur ouvert : elle nécessite donc une résistance R de rappel (pull-up). J'ai utilisé R = 10 kilohms.
Je récupère le +5V par l'intermédiaire de la broche DIN 5 (+8,5V) sur laquelle je branche un régulateur 5V.
