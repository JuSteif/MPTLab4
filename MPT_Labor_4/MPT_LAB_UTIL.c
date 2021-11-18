/*
 * MPT_LAB_UTIL.c
 *
 * Created: 03.06.2012 13:22:11
 *  Author: holtv
 */ 

#include <avr/io.h>

#include "MPT_LAB_UTIL.h"


void Wait_x_ms(uint16_t x_ms)
{
	uint16_t i_w, j_w;

  for (j_w=0; j_w < x_ms; j_w++)
	{
    for (i_w=0; i_w < 600; i_w++)
	  {
	  }		  
	}		
}

//#ifdef NM
void LedOn(uint8_t Led)
{
  if (Led == LED_ALL)
    PORTC = ~0b11111111;
	else
	  CBI(PORTC,Led);
}	


void LedOff(uint8_t Led)
{
  if (Led == LED_ALL)
    PORTC = 0b11111111;
	else
    SBI(PORTC,Led);
}	


void LedToggle(uint8_t Led)
{
  if (Led == LED_ALL)
    PORTC ^= 0b11111111;
	else
    TBI(PORTC,Led);
}	
//#endif


void UsartInit(uint8_t CharBits, uint8_t ParBit, uint8_t StopBits, uint32_t Baudrate)
{
	// Vorhandensein und Art des Paritäts-Bits festlegen:
	// Gerade   Parität: Anzahl der '1' wird auf gerade Anzahl ergänzt
	// Ungerade Parität: Anzahl der '1' wird auf ungerade Anzahl ergänzt
	// Keine    Parität: Paritäts-Bit entfällt
  if (ParBit == USART_EVEN_PARITY)
    UCSRC |= (1 << UPM1) | (0 << UPM0);  // äquivalent: UCSRC |= (1 << UPM1);
	else
  if (ParBit == USART_ODD_PARITY)
    UCSRC |= (1 << UPM1) | (1 << UPM0);
	else
    UCSRC |= (0 << UPM1) | (0 << UPM0);  // brauchen wir eigentlich nicht...

  // Anzahl der Stop-Bits festlegen: 1 oder 2
  if (StopBits == 1)
    UCSRC |= (0 << USBS);
  else
  if (StopBits == 2)
    UCSRC |= (1 << USBS);

  // Anzahl der Zeichenbits: 5..9
  // Üblich sind 8 Bits / Zeichen.
  // Bei 9 Zeichenbits muss man die besondere Behandlung des 9.Bits beim
  // Lesen und Schreiben beachten. Diese ist in unseren Lese- und Schreibroutinen
  // nicht berücksichtigt.
  switch (CharBits)
  {
    case 5: // 5 Zeichenbits
      break;

    case 6: // 6 Zeichenbits
      UCSRC |= (1 << UCSZ0);
      break;

    case 7: // 7 Zeichenbits
      UCSRC |= (1 << UCSZ1);
      break;

    case 8: // 8 Zeichenbits
      UCSRC |= (1 << UCSZ1) | (1 << UCSZ0);
      break;

    case 9: // 9 Zeichenbits
      UCSRB |= (1 << UCSZ2);
      UCSRC |= (1 << UCSZ1) | (1 << UCSZ0);
      break;
  }

  // Einstellen der Bitrate:
  // Bei der Wahl der Bitrate muss der relative Bitratenfehler
  // aufgrund der Taktfrequenz des uC beachtet werden.
  // Dieser wirkt sich bei höheren Bitraten stärker aus, daher
  // darf bei großem Bitratenfehler die Bitrate nicht zu groß
  // gewählt werden.
  UBRRL = (F_CPU/(16*Baudrate)-1) % 256;
  UBRRH = (F_CPU/(16*Baudrate)-1) / 256;

  // Freigabe der Sende-/Empfangs-Kanäle und uC-Pins
  UCSRB |= (1 << RXEN) | (1 << TXEN);
}


/*
 *  1 Zeichen auf USART-Kanal ausgeben
 */
void UsartPutc(uint8_t Data)
{
  while (!(UCSRA & (1 << UDRE)));
  UDR = Data;
}


/*
 *  String auf USART-Kanal ausgeben
 */
void UsartPuts(char* pString)
{
	char* pData = pString;
	
  while (*pData != 0)
  {
    UsartPutc(*pData);
	  pData++;
  }	
}


/*
 *  1 Zeichen von USART-Kanal einlesen
 */
uint8_t UsartGetc(void)
{
	uint8_t Data;
	
  while (!(UCSRA & (1 << RXC)));
  Data = UDR;
  
  return(Data);
}


