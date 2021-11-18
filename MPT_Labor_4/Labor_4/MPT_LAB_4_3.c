/******************************************************************************/
/*                                                                            */
/*              Ostfalia Hochschule für angewandte Wissenschaften             */
/*                        Fakultät für Fahrzeugtechnik                        */
/*           Institut für Fahrzeuginformatik und Fahrzeugelektronik           */
/*                             Fahrzeuginformatik                             */
/*                                                                            */
/* Paket:        MPT_LAB                                                      */
/*                                                                            */
/* Modul:        MPT_LAB_4                                                    */
/*                                                                            */
/* Beschreibung: Aufgaben 4.MPT-Labor                                         */
/*                                                                            */
/* Funktionen:   A_4_3: Einlesen des Messwerts eines IR-Entfernungssensors    */
/*                      (nichtlinear)                                         */
/*                                                                            */
/* Version:      1.0 / 120716 / V. v. Holt                                    */
/*                                                                            */
/******************************************************************************/

//------------------------------------------------------------------------------
//  Headerdateien
//------------------------------------------------------------------------------
// Allgemeine Headerdateien
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Eigene Headerdateien
#include "MPT_LAB_MAIN.h"
#include "MPT_LAB_UTIL.h"
#include "MPT_LAB_4_3.h"



#ifdef ENABLE_A_4
#ifdef ENABLE_A_4_3
//------------------------------------------------------------------------------
//  Makro-/Konstantendefinitionen
//------------------------------------------------------------------------------
#define LED_DDR     DDRC
#define LED_PORT    PORTC
#define LED_DELAY   50
#define TASTER_DDR  DDRB
#define TASTER_PIN  PINB


//------------------------------------------------------------------------------
//  Private Variablen
//------------------------------------------------------------------------------

// Geradenapproximation für IR-Sensor-Kennlinie: Werttupel nach dem Muster {Messwert AD-Wandlung, reale Entfernung [mm]}
// 1 Werttupel ist hierbei ein Punkt, durch den die Gerade für die Approximation läuft.
int32_t GP2D120_1[2][2] = {{300,30},{30,400}};								// 2 Werttupel -> 2 Punkte beschreiben eine Gerade -> nichtlineare Kennlinie des IR-Sensors wird durch eine Gerade vereinfacht (Gültiger Bereich zwischen den beiden Punkten)
int32_t GP2D120_2[4][2] = {{300,30},{125,100},{65,200},{30,400}};			// 4 Werttupel -> 4 Punkte beschreiben 3 Geradenabschnitte mit unterschiedlicher Steigung -> nichtlineare Kennlinie des IR-Sensors wird durch 3 Geraden vereinfacht (Gültiger Bereich zwischen den 2 äußersten Punkten)
//int32_t GP2D120_3[21][2] = {{,},{,},{,},{,},{,},{,},{,},{,},{,},{,},{,},{,},{,},{,},{,},{,},{,},{,},{,},{,},{,}};  //.. 
	
	
//------------------------------------------------------------------------------
//  Interrupt Service Routinen
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  Private Funktionen
//------------------------------------------------------------------------------
static void AdcInit()
{
  // IHR_CODE_HIER ... => Aufgabe A_4_2
}


static uint16_t AdcRead(uint8_t AdcChan)
{
  // IHR_CODE_HIER ... => Aufgabe A_4_2
}


//------------------------------------------------------------------------------
//  Public Funktionen
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Aufgabe A_4_3: Einlesen des Messwerts eines IR-Entfernungssensors (nichtlinear)
// A_4_3_1: Einlesen der Messwerte und Ausgabe mittels linearer Kennlinie.
// A_4_3_2: Einlesen der Messwerte und Ausgabe mittels 3-stufiger Kennlinie.
// A_4_3_3: Einlesen der Messwerte und Ausgabe mittels 20-stufiger Kennlinie.
//------------------------------------------------------------------------------

//##############################################################################

// A_4_3_1: Einlesen der Messwerte und Ausgabe mittels linearer Kennlinie.
void A_4_3_1(void)
{
	// Richtungsregister für LEDs auf Ausgang
	LED_DDR = 0b11111111;

  // Alle LEDs aus
  LED_PORT = 0b11111111;

  // Initialisierung der USART-Schnittstelle
  // 8 Zeichenbits, 1 Stopbit, 0 Paritätsbit, 9600 Baud
	UsartInit(8,1,0,9600);

  AdcInit();	

  while (1)
  {
	uint8_t AdcChan;
	uint16_t AdcRes;
	char String[80];

    // AD-Kanal 2
	// Kanal auswählen und in ADMUX maskieren
    AdcChan = 2;
	AdcRes = AdcRead(AdcChan);

	// Ausgabe der Ergebnisse auf dem Terminal
	// Berechnung der Werte als Festkommazahl
	AdcRes = (AdcRes * (uint32_t)500)/1024;

    sprintf(String, "AD-Wandlung: Kanal[2] = %d\n\r", AdcRes);
	UsartPuts(String);

	// Überführen des Messwertes aus der AD-Wandlung in die reale Entfernung
    {
		int32_t Dist = 0;

  	    // Entfernung < Minimalentfernung?
  	    if (AdcRes > GP2D120_1[0][0])						// GP2D120_1[Index_0: Messwert AD-Wandlung][Index_1: reale Entfernung]
		{
			// Distanz <= 30mm
			Dist = GP2D120_1[0][1];
		}
		
		else
  			// Entfernung > Maximalentfernung?
			if (AdcRes < GP2D120_1[1][0])
			{
				// Distanz >= 400mm
				Dist = GP2D120_1[1][1];
			}
			
			// Entfernung liegt im gültigen Bereich			  			  
			else
			{	
				// liegt der AD-Messwert auf der Sekante zwischen den ersten beiden Werttupeln? (=Gemessene Entfernung liegt im Intervall [0]....[1])
				if ((AdcRes <= GP2D120_1[0][0]) && (AdcRes >= GP2D120_1[1][0]))
				{
					// Berechnung der Entfernung aus der Sekante zwischen [0]..[1]
					Dist = GP2D120_1[0][1] + ((GP2D120_1[1][1]-GP2D120_1[0][1])*(AdcRes-GP2D120_1[0][0])) / (GP2D120_1[1][0]-GP2D120_1[0][0]);
				}
		  }										
		  		  
		  // Ausgabe der Entfernung in [mm]
		  sprintf(String, "Entfernung(1) = %3ld[mm]\n\r", Dist);
		  UsartPuts(String);
	  }	      	  
	  
	  Wait_x_ms(500);
  }	  
}	

//##############################################################################

// A_4_3_2: Einlesen der Messwerte und Ausgabe mittels 3-stufiger Kennlinie.
void A_4_3_2(void)
{
  // IHR_CODE_HIER ...
}	

//##############################################################################

// A_4_3_3: Einlesen der Messwerte und Ausgabe mittels 20-stufiger Kennlinie.
void A_4_3_3(void)
{
  // IHR_CODE_HIER ...
}	

//##############################################################################

#endif /* ENABLE_A_4_3 */
#endif /* ENABLE_A_4 */


/*
 *  EoF
 */
