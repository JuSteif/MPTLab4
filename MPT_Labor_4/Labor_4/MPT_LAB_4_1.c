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
/* Funktionen:   A_4_1: A/D-Wandler in Betrieb nehmen                         */
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
#include "MPT_LAB_4_1.h"



#ifdef ENABLE_A_4
#ifdef ENABLE_A_4_1
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


//------------------------------------------------------------------------------
//  Interrupt Service Routinen
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  Private Funktionen
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
//  Public Funktionen
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Aufgabe A_4_1: A/D-Wandler in Betrieb nehmen
// A_4_1_1: A/D-Wandler Kanal 0 kontinuierlich einlesen und als 8-Bit-Wert auf
//          den LEDs ausgeben.
//------------------------------------------------------------------------------

//##############################################################################

// A_4_1_1: A/D-Wandler Kanal 0 kontinuierlich einlesen und als 8-Bit-Wert auf
//          den LEDs ausgeben.
void A_4_1_1(void)
{
	// IHR_CODE_HIER ...
	ADMUX = 0b00100000;
	ADCSRA = 0b10000111;
	SFIOR &= ~(111 << ADTS0);
	
	LED_DDR = 0xff;
	LED_PORT = 0xff;
	
	while (1)
	{
		ADCSRA |= 1 << ADSC;
		
		while (BIT_IS_CLR(ADCSRA, ADSC))
		{
		}
		
		LED_PORT = ~(ADCH);
	}
}

//##############################################################################

#endif /* ENABLE_A_4_1 */
#endif /* ENABLE_A_4 */


/*
*  EoF
*/
