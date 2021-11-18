/*
 * MPT.c
 *
 * Created: 25.05.2012 18:12:01
 *  Author: holtv
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "MPT_LAB_MAIN.h"
#include "MPT_LAB_UTIL.h"

#include "Labor_4/MPT_LAB_4_1.h"
#include "Labor_4/MPT_LAB_4_2.h"
#include "Labor_4/MPT_LAB_4_3.h"


int main(void)
{
	//##############################################################################
	//
	// Aufgabe A_4
	//
	//##############################################################################
  //------------------------------------------------------------------------------
  // Aufgabe A_4_1: A/D-Wandler in Betrieb nehmen
  // A_4_1_1: A/D-Wandler Kanal 0 kontinuierlich einlesen und als 8-Bit-Wert auf
  //          den LEDs ausgeben.
  //------------------------------------------------------------------------------
  A_4_1_1();


  //------------------------------------------------------------------------------
  // Aufgabe A_4_2: A/D-Wandlung mehrerer Kanäle mit Terminalausgabe und Hilfsfunktionen
  // A_4_2_1: A/D-Wandlung im Multiplexverfahren von 2 Kanälen und Ausgabe auf den
  //          LEDs 0..3 bzw. den LEDs 4..7 für beide Kanäle.
  // A_4_2_2: A/D-Wandlung mit Hilfsfunktionen zur Initialisierung und zum Einlesen.
  // A_4_2_3: A/D-Wandlung mit Ausgabe der Bitwerte auf dem Terminal.
  // A_4_2_4: A/D-Wandlung mit Ausgabe der umgerechneten Spannungswerte auf dem Terminal.
  //------------------------------------------------------------------------------
  //A_4_2_1();
  //A_4_2_2();
  //A_4_2_3();
  //A_4_2_4();


  //------------------------------------------------------------------------------
  // Aufgabe A_4_3: Einlesen des Messwerts eines IR-Entfernungssensors (nichtlinear)
  // A_4_3_1: Einlesen der Messwerte und Ausgabe mittels linearer Kennlinie.
  // A_4_3_2: Einlesen der Messwerte und Ausgabe mittels 3-stufiger Kennlinie.
  // A_4_3_3: Einlesen der Messwerte und Ausgabe mittels 20-stufiger Kennlinie.
  //------------------------------------------------------------------------------
  //A_4_3_1();
  //A_4_3_2();
  //A_4_3_3();


  while(1)
  {
      //TODO:: Please write your application code 
  }
}

