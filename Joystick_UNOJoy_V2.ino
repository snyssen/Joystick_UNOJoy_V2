// V2 du joystick utilisant UNOJoy ( https://code.google.com/archive/p/unojoy/ )
// Unojoy va utiliser l'Arduino pour simuler une manette de Playstation 3 reconnue par le PC.
// Ceci veut également dire que notre joystick devrait être compatible avec une PS3 (non testé vu que je dispose pas de ladite console)
//
// On récupère ici 3 axes -> pin A0 = Axe X du joystick
//                        -> pin A1 = Axe Y du joystick
//                        -> pin A2 = throttle
//
// Et 2 bouton -> pin 5 = gâchette de tir
//             -> pin 6 = tir missile
//
// Les boutons doivent être reliés à la masses via une R de pulldown extérieurement

#include "UnoJoy.h"

const short pAxeX = 14; // pin A0 | Fil orange (/!\ pas confondre avec Vcc)
const short pAxeY = 15; // pin A1 | Fil jaune
const short pThrottle = 16; // pin A2
const short pGachette = 5; // pin 5
const short pMissile = 6; // pin 6
const short pSecVcc = 2; // pin 2 | sert de second pin d'alimentation, utilisé pour la throttle


void setup()
{
  // setup des pins utilisé, voir plus bas
  setupPins();
  // Méthode externe, propre à la librairie
  setupUnoJoy();
}

void loop()
{
  // Récupération des données
  dataForController_t controllerData = getControllerData();
  // Envoi des données (méthode de la librairie)
  setControllerData(controllerData);
}

void setupPins(void)
{
  pinMode(pAxeX, INPUT);
  pinMode(pAxeY, INPUT);
  pinMode(pThrottle, INPUT);
  pinMode(pGachette, INPUT);
  pinMode(pMissile, INPUT);
  pinMode(pSecVcc, OUTPUT);
  digitalWrite(pSecVcc, HIGH);
}

dataForController_t getControllerData(void){
  // Création d'une nouvelle donnée controllerData qui va récupérer les infos des différents boutons et axes de notre joystick
  // On utilise la méthode getBlankDataForController() qui va s'assurer que les données de notre joystick sont bien vides avant de commencer
  // Ces données ont en effet tendance à être remplies avec d'autres données dans le cas contraire
  dataForController_t controllerData = getBlankDataForController();

  // Setup des boutons de la manette PS3

  // Nos boutons passent à un état haut quand ils sont pressés
  // On récupère leur état actuel
  // Hijack de la croix en gachette de tir
  controllerData.crossOn = digitalRead(pGachette);
  // et du rond pour les missiles
  controllerData.circleOn = digitalRead(pMissile);

  // Tous les autres boutons de la manette sont évidemment également accessibles mais je ne m'en sers pas ici
  /*
  controllerData.triangleOn = !digitalRead(2);
  controllerData.dpadUpOn = !digitalRead(6);
  controllerData.dpadDownOn = !digitalRead(7);
  controllerData.dpadLeftOn = !digitalRead(8);
  controllerData.dpadRightOn = !digitalRead(9);
  controllerData.l1On = !digitalRead(10);
  controllerData.r1On = !digitalRead(11);
  controllerData.selectOn = !digitalRead(12);
  controllerData.startOn = !digitalRead(A4);
  controllerData.homeOn = !digitalRead(A5);
  controllerData.squareOn = !digitalRead(4);
  */

  // Setup des sticks de la manette
  // analogRead() récupère une tension sur une valeur codée sur 10 bits,
  // or les manettes PS3 ne travaillent que sur 8 bits.
  // Nous reculons donc les valeurs récupérées de 2 bits, perdant ainsi les 2 bits de poids faible

  // Hijack du leftStick en joystick
  // Déplacement des potentiomètres limité => mapping nécessaire (les limites entrées ont été définies expérimentalement)
  // Axe X
  controllerData.leftStickX = map(analogRead(pAxeX), 415, 605, 0, 1023) >> 2;
  // Axe Y
  controllerData.leftStickY = map(analogRead(pAxeY), 375, 640, 0, 1023) >> 2;

  // Hijack du rightStickX pour la throttle
  controllerData.rightStickX = analogRead(pThrottle) >> 2;

  // Nous n'utiliserons pas ici l'axe Y du stick droit
  //controllerData.rightStickY = analogRead(A3) >> 2;

  return controllerData;
}
