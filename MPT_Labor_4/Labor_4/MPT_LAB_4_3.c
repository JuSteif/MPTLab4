/******************************************************************************/
/*                                                                            */
/*              Ostfalia Hochschule f�r angewandte Wissenschaften             */
/*                        Fakult�t f�r Fahrzeugtechnik                        */
/*           Institut f�r Fahrzeuginformatik und Fahrzeugelektronik           */
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

// Geradenapproximation f�r IR-Sensor-Kennlinie: Werttupel nach dem Muster {Messwert AD-Wandlung, reale Entfernung [mm]}
// 1 Werttupel ist hierbei ein Punkt, durch den die Gerade f�r die Approximation l�uft.
int32_t GP2D120_1[2][2] = {{300,30},{30,400}};								// 2 Werttupel -> 2 Punkte beschreiben eine Gerade -> nichtlineare Kennlinie des IR-Sensors wird durch eine Gerade vereinfacht (G�ltiger Bereich zwischen den beiden Punkten)
int32_t GP2D120_2[4][2] = {{300,30},{125,100},{65,200},{30,400}};			// 4 Werttupel -> 4 Punkte beschreiben 3 Geradenabschnitte mit unterschiedlicher Steigung -> nichtlineare Kennlinie des IR-Sensors wird durch 3 Geraden vereinfacht (G�ltiger Bereich zwischen den 2 �u�ersten Punkten)
int32_t GP2D120_3[17][2] = {{300,30},{270,40},{235,50},{200,60},{175,70},{155,80},{140,90},{130,100},{110,120},{90,140},{80,160},{75,180},{65,200},{50,250},{45,300},{35,350},{30,400}};  //..


//------------------------------------------------------------------------------
//  Interrupt Service Routinen
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  Private Funktionen
//------------------------------------------------------------------------------
static void AdcInit()
{
	// IHR_CODE_HIER ... => Aufgabe A_4_2
	ADMUX = 0b00100000;
	ADCSRA = 0b10000111;
	SFIOR &= ~(111 << ADTS0);
}


static uint16_t AdcRead(uint8_t AdcChan)
{
	// IHR_CODE_HIER ... => Aufgabe A_4_2
	uint16_t AdcRes;
	if(AdcChan > 7){
		return 0;
	}
	
	ADMUX &= 0b11111000;
	ADMUX |= AdcChan;
	
	SET_BIT(ADCSRA, ADSC);
	while(BIT_IS_SET(ADCSRA, ADSC)){
		
	}
	
	AdcRes = (ADCL >> 6) | (ADCH << 2);

	return AdcRes;
}

uint8_t getInd(uint16_t AdcRes){
	uint8_t res = 0;
	
	while(AdcRes < GP2D120_3[res][0]){
		res++;
	}
	res--;
	
	return res;
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
	// Richtungsregister f�r LEDs auf Ausgang
	LED_DDR = 0b11111111;

	// Alle LEDs aus
	LED_PORT = 0x00;

	// Initialisierung der USART-Schnittstelle
	// 8 Zeichenbits, 1 Stopbit, 0 Parit�tsbit, 9600 Baud
	UsartInit(8,1,0,9600);

	AdcInit();
	
	uint8_t AdcChan;
	uint16_t AdcRes;
	char String[80];

	while (1)
	{
		// AD-Kanal 2
		// Kanal ausw�hlen und in ADMUX maskieren
		AdcChan = 2;
		AdcRes = AdcRead(AdcChan);

		// Ausgabe der Ergebnisse auf dem Terminal
		// Berechnung der Werte als Festkommazahl
		AdcRes = (AdcRes * (uint32_t)500)/1024;

		sprintf(String, "AD-Wandlung: Kanal[2] = %d\n\r", AdcRes);
		UsartPuts(String);

		// �berf�hren des Messwertes aus der AD-Wandlung in die reale Entfernung
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
			
			// Entfernung liegt im g�ltigen Bereich
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
	// Richtungsregister f�r LEDs auf Ausgang
	LED_DDR = 0b11111111;

	// Alle LEDs aus
	LED_PORT = 0b11111111;

	// Initialisierung der USART-Schnittstelle
	// 8 Zeichenbits, 1 Stopbit, 0 Parit�tsbit, 9600 Baud
	UsartInit(8,1,0,9600);

	AdcInit();
	
	uint8_t AdcChan;
	uint16_t AdcRes;
	char String[80];

	while (1)
	{
		// AD-Kanal 2
		// Kanal ausw�hlen und in ADMUX maskieren
		AdcChan = 2;
		AdcRes = AdcRead(AdcChan);

		// Ausgabe der Ergebnisse auf dem Terminal
		// Berechnung der Werte als Festkommazahl
		AdcRes = (AdcRes * (uint32_t)500)/1024;

		sprintf(String, "AD-Wandlung: Kanal[2] = %d\n\r", AdcRes);
		UsartPuts(String);

		// �berf�hren des Messwertes aus der AD-Wandlung in die reale Entfernung
		{
			int32_t Dist = 0;

			// Entfernung < Minimalentfernung?
			if (AdcRes > GP2D120_2[0][0])						// GP2D120_1[Index_0: Messwert AD-Wandlung][Index_1: reale Entfernung]
			{
				// Distanz <= 30mm
				Dist = GP2D120_2[0][1];
			}
			
			else
			// Entfernung > Maximalentfernung?
			if (AdcRes < GP2D120_2[3][0])
			{
				// Distanz >= 400mm
				Dist = GP2D120_2[3][1];
			}
			
			// Entfernung liegt im g�ltigen Bereich
			else
			{
				// liegt der AD-Messwert auf der Sekante zwischen den ersten beiden Werttupeln? (=Gemessene Entfernung liegt im Intervall [0]....[1])
				if ((AdcRes <= GP2D120_2[0][0]) && (AdcRes >= GP2D120_2[1][0]))
				{
					// Berechnung der Entfernung aus der Sekante zwischen [0]..[1]
					Dist = GP2D120_2[0][1] + ((GP2D120_2[1][1]-GP2D120_2[0][1])*(AdcRes-GP2D120_2[0][0])) / (GP2D120_2[1][0]-GP2D120_2[0][0]);
				}
				else if ((AdcRes <= GP2D120_2[1][0]) && (AdcRes >= GP2D120_2[2][0]))
				{
					// Berechnung der Entfernung aus der Sekante zwischen [1]..[2]
					Dist = GP2D120_2[1][1] + ((GP2D120_2[2][1]-GP2D120_2[1][1])*(AdcRes-GP2D120_2[1][0])) / (GP2D120_2[2][0]-GP2D120_2[1][0]);
				}
				else if ((AdcRes <= GP2D120_2[2][0]) && (AdcRes >= GP2D120_2[3][0]))
				{
					// Berechnung der Entfernung aus der Sekante zwischen [2]..[3]
					Dist = GP2D120_2[2][1] + ((GP2D120_2[3][1]-GP2D120_2[2][1])*(AdcRes-GP2D120_2[2][0])) / (GP2D120_2[3][0]-GP2D120_2[2][0]);
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

// A_4_3_3: Einlesen der Messwerte und Ausgabe mittels 20-stufiger Kennlinie.
void A_4_3_3(void)
{
	LED_DDR = 0b11111111;

	// Alle LEDs aus
	LED_PORT = 0b11111111;

	// Initialisierung der USART-Schnittstelle
	// 8 Zeichenbits, 1 Stopbit, 0 Parit�tsbit, 9600 Baud
	UsartInit(8,1,0,9600);

	AdcInit();
	
	uint8_t AdcChan;
	uint16_t AdcRes;
	char String[80];
	
	while(1){
		// AD-Kanal 2
		// Kanal ausw�hlen und in ADMUX maskieren
		AdcChan = 2;
		AdcRes = AdcRead(AdcChan);

		// Ausgabe der Ergebnisse auf dem Terminal
		// Berechnung der Werte als Festkommazahl
		AdcRes = (AdcRes * (uint32_t)500)/1024;

		sprintf(String, "AD-Wandlung: Kanal[2] = %d\n\r", AdcRes);
		UsartPuts(String);

		// �berf�hren des Messwertes aus der AD-Wandlung in die reale Entfernung
		{
			int32_t Dist = 0;

			// Entfernung < Minimalentfernung?
			if (AdcRes > GP2D120_3[0][0])						// GP2D120_1[Index_0: Messwert AD-Wandlung][Index_1: reale Entfernung]
			{
				// Distanz <= 30mm
				Dist = GP2D120_3[0][1];
			}
			
			else
			// Entfernung > Maximalentfernung?
			if (AdcRes < GP2D120_3[16][0])
			{
				// Distanz >= 400mm
				Dist = GP2D120_3[16][1];
			}
			
			// Entfernung liegt im g�ltigen Bereich
			else
			{
				//Index des x0 Punktes bekommen
				uint8_t ind = getInd(AdcRes);
				Dist = GP2D120_3[ind][1] + ((GP2D120_3[ind+1][1]-GP2D120_3[ind][1])*(AdcRes-GP2D120_3[ind][0])) / (GP2D120_3[ind+1][0]-GP2D120_3[ind][0]);
			}
			
			// Ausgabe der Entfernung in [mm]
			sprintf(String, "Entfernung(1) = %3ld[mm]\n\r", Dist);
			UsartPuts(String);
		}
		
		Wait_x_ms(500);
	}
}

//##############################################################################

#endif /* ENABLE_A_4_3 */
#endif /* ENABLE_A_4 */


/*
*  EoF
*/
