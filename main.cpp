
/*
  Titre      : piezo ADC
  Auteur     : Pidjieu dibril
  Date       : 05/12/2022
  Description: Une description du programme
  Version    : 0.0.1
*/

//Pour contrôler l’écran LCD, nous utiliserons la   bibliothèque LiquidCrystal
// inclusion de la librairie LCD
#include <LiquidCrystal.h> 
// la bibliothèque Wire permet de communiquer avec les périphériques I2C
#include <Wire.h>
#include <Arduino.h>
#include "WIFIConnector_MKR1000.h"
#include "MQTTConnector.h"


#define PIN_LED1   6
#define PIN_LED2   7
float etatLed =0.0;


const int PIEZO_PIN = A0; // Piezo output
//initialise la librairie LCD avec les broches utilisées
LiquidCrystal lcd = LiquidCrystal (12, 11, 5, 4, 3, 2);// déclare une variable LiquidCrystal appelée lcd avec mode 4 bits - RW non connectée (le plus simple!)

// Instruction d'initialisation de la carte Arduino
//--------------- FONCTION SETUP = Code d'initialisation -----------------------------
// La fonction setup() est exécutée en premier et 1 seule fois, au démarrage du programme
void setup() 
{
  // Commencez la communication série à un débit de 9600 bauds:
  Serial.begin(9600); 
  lcd.begin(16, 2); // initialise le LCD en mode 16 colonnes x 2 lignes
  wifiConnect();              // branchement au reseau wifi 
  MQTTConnect();              // branchement au broker MQTT
  pinMode(PIN_LED1, OUTPUT);
  pinMode(PIN_LED2, OUTPUT);
  
}

void loop() 
{
  //Lire la valeur Piezo ADC et la convertir en tension
   // Lecture du capteur 
  int piezoValeur = analogRead(PIEZO_PIN);
  // Conversion en Volt
  float piezoVoltage = piezoValeur * 5.0 / 1023.0;
  // Mise à jour luminosité du LED
  analogWrite(PIN_LED1, piezoValeur >> 2);


  if ( piezoValeur > 0.2)
  {
    etatLed = 1;
}
  


  //affichage
  Serial.println(piezoVoltage); // Print the voltage.
  // afficher la distance a l'ecran 
    lcd.clear();// efface l'écran et met le curseur en haut à gauche
    lcd.setCursor(2,0); // 1ère colonne - 1ère ligne - positionne le curseur à l'endroit voulu (colonne, ligne) (1ère=0 !)
    lcd.print("  VOTRE!!! "); // affiche la chaîne texte - message de test (Ici Temps:)
    lcd.setCursor(0,1);// 1ère colonne - 2ème ligne - positionne le curseur à l'endroit voulu (colonne, ligne) (1ère=0 !)
    lcd.print("TENSION A "); // affiche la chaîne texte - message de test (Ici un espace)
    lcd.print("");
    lcd.print(piezoVoltage);// affiche le calcul de la distance en cm  entre l'émétteur et l'objet
    lcd.print(" V");
      //lcd.write(12); // affiche le caractère µ pour micro
    //lcd.write(11); // affiche le caractère s pour seconde

  appendPayload("Tension de sortie",piezoVoltage); // ajout de la donnnée temperature au message MQTT
  sendPayload();                                 // Envoie du message via le protocole MQTT 
    delay(500); //La fonction delay() permet de mettre en pause le programme pendant 2 seconde. C'est une fonction bloquante.   
}