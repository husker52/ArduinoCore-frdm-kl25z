/*
  Copyright (c) 2015 Arduino LLC.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"

#ifdef __cplusplus
 extern "C" {
#endif

void pinMode( uint32_t ulPin, uint32_t ulMode )
{
  // Handle the case the pin isn't usable as PIO
  if ( g_APinDescription[ulPin].ulPinType == PIO_NOT_A_PIN )
  {
    return ;
  }

  // Set pin mode according to chapter '22.6.3 I/O Pin Configuration'
  switch ( ulMode )
  {
    case INPUT:
      // Set pin to input mode
      PORT[g_APinDescription[ulPin].ulPort]->PCR[g_APinDescription[ulPin].ulPin] = (PORT_PCR_MUX(1) ) ;
      FGPIO[g_APinDescription[ulPin].ulPort]->PDDR &=  ~(1<<g_APinDescription[ulPin].ulPin) ;
    break ;

    case INPUT_PULLUP:
      // Set pin to input mode with pull-up resistor enabled
      PORT[g_APinDescription[ulPin].ulPort]->PCR[g_APinDescription[ulPin].ulPin] = (PORT_PCR_MUX(1) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK ) ;
      FGPIO[g_APinDescription[ulPin].ulPort]->PDDR &=  ~(1<<g_APinDescription[ulPin].ulPin) ;
    break ;

    case INPUT_PULLDOWN:
      // Set pin to input mode with pull-down resistor enabled
      PORT[g_APinDescription[ulPin].ulPort]->PCR[g_APinDescription[ulPin].ulPin] |= (PORT_PCR_MUX(1) | PORT_PCR_PE_MASK ) ;
      FGPIO[g_APinDescription[ulPin].ulPort]->PDDR &=  ~(1<<g_APinDescription[ulPin].ulPin) ;
    break ;

    case OUTPUT:
      // enable input, to support reading back values, with pullups disabled
      PORT[g_APinDescription[ulPin].ulPort]->PCR[g_APinDescription[ulPin].ulPin] |= (PORT_PCR_MUX(1) ) ;
      FGPIO[g_APinDescription[ulPin].ulPort]->PDDR |= (uint32_t)(1<<g_APinDescription[ulPin].ulPin) ;
    break ;
    default:
      // do nothing
    break ;
  }
}


void digitalWrite( uint32_t ulPin, uint32_t ulVal )
{
  // Handle the case the pin isn't usable as PIO
  if ( g_APinDescription[ulPin].ulPinType == PIO_NOT_A_PIN )
  {
    return ;
  }

  EPortType port = g_APinDescription[ulPin].ulPort;
  uint32_t pin = g_APinDescription[ulPin].ulPin;
  uint32_t pinMask = (1ul << pin);

  if ( (FGPIO[port]->PDDR & pinMask) == 0 ) {
    // the pin is not an output, disable pull-up if val is LOW, otherwise enable pull-up
    if(ulVal == LOW)
    	PORT[port]->PCR[pin] &= ~PORT_PCR_PS_MASK;
    else
    	PORT[port]->PCR[pin] |= PORT_PCR_PS_MASK ;

  }

  switch ( ulVal )
  {
    case LOW:
      FGPIO[port]->PCOR = pinMask;
    break ;

    default:
      FGPIO[port]->PSOR = pinMask;
    break ;
  }

  return ;
}

int digitalRead( uint32_t ulPin )
{
  // Handle the case the pin isn't usable as PIO
  if ( g_APinDescription[ulPin].ulPinType == PIO_NOT_A_PIN )
  {
    return LOW ;
  }

  if ( (FGPIO[g_APinDescription[ulPin].ulPort]->PDIR & (1ul << g_APinDescription[ulPin].ulPin)) != 0 )
  {
    return HIGH ;
  }

  return LOW ;
}

#ifdef __cplusplus
}
#endif

