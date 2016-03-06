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

La sortie TX au niveau du Minitel est à collecteur ouvert : 
elle nécessite donc une résistance R de rappel (pull-up). J'utilise R = 10 kilohms (soit une valeur comprise entre les 2,3 kilohms et 200 kilohms indiqués sur la documentation technique p.69).
Je récupère le +5V par l'intermédiaire de la broche DIN 5 (+8,5V) sur laquelle je branche un régulateur 5V.
