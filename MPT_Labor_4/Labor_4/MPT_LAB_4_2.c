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
/* Funktionen:   A_4_2: A/D-Wandlung mehrerer Kanäle mit Terminalausgabe und  */
/*                      Hilfsfunktionen                                       */
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
#include "MPT_LAB_4_2.h"



#ifdef ENABLE_A_4
#ifdef ENABLE_A_4_2
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
static void AdcInit()
{
  // IHR_CODE_HIER ...
}


static uint16_t AdcRead(uint8_t AdcChan)
{
  // IHR_CODE_HIER ...
}


//------------------------------------------------------------------------------
//  Public Funktionen
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Aufgabe A_4_2: A/D-Wandlung mehrerer Kanäle mit Terminalausgabe und Hilfsfunktionen.
// A_4_2_1: A/D-Wandlung im Multiplexverfahren von 2 Kanälen und Ausgabe auf den
//          LEDs 0..3 bzw. den LEDs 4..7 für beide Kanäle.
// A_4_2_2: A/D-Wandlung mit Hilfsfunktionen zur Initialisierung und zum Einlesen.
// A_4_2_3: A/D-Wandlung mit Ausgabe der Bitwerte auf dem Terminal.
// A_4_2_4: A/D-Wandlung mit Ausgabe der umgerechneten Spannungswerte auf dem Terminal.
//------------------------------------------------------------------------------

//##############################################################################

// A_4_2_1: A/D-Wandlung im Multiplexverfahren von 2 Kanälen und Ausgabe auf den
//          LEDs 0..3 bzw. den LEDs 4..7 für beide Kanäle.
void A_4_2_1(void)
{
  // IHR_CODE_HIER ...
  ADMUX = 0b00100000;
  ADCSRA = 0b10000111;
  SFIOR &= ~(111 << ADTS0);
  
  LED_DDR = 0xff;
  LED_PORT = 0xff;
  
  uint8_t LEDRR;
  
  while (1)
  {
	  LEDRR = 0;
	  
	  CLR_BIT(ADMUX, 0);
	  ADCSRA |= 1 << ADSC;
	  
	  while (BIT_IS_CLR(ADCSRA, ADSC))
	  {
	  }
	  
	  LEDRR = ADCH & 0xf0;
	  
	  SET_BIT(ADMUX, 0);
	  ADCSRA |= 1 << ADSC;
	  
	  while (BIT_IS_CLR(ADCSRA, ADSC))
	  {
	  }
	  
	  LEDRR |= ADCH >> 4;
	  
	  LED_PORT = ~(LEDRR);
  }
}	

//##############################################################################
	
// A_4_2_2: A/D-Wandlung mit Hilfsfunktionen zur Initialisierung und zum Einlesen.
void A_4_2_2(void)
{
  // IHR_CODE_HIER ...
}	

//##############################################################################

// A_4_2_3: A/D-Wandlung mit Ausgabe auf dem Terminal.
void A_4_2_3(void)
{
  // IHR_CODE_HIER ...
}	

//##############################################################################

// A_4_2_4: A/D-Wandlung mit Ausgabe der umgerechneten Spannungswerte auf dem Terminal.
void A_4_2_4(void)
{
  // IHR_CODE_HIER ...
}	

//##############################################################################

#endif /* ENABLE_A_4_2 */
#endif /* ENABLE_A_4 */

/*
 *  EoF
 */
