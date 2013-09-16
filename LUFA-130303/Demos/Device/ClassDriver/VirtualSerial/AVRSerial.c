/*
             LUFA Library
     Copyright (C) Dean Camera, 2013.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2013  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the VirtualSerial demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "AVRSerial.h"
#include <util/delay.h>

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial_CDC_Interface =
	{
		.Config =
			{
				.ControlInterfaceNumber   = 0,
				.DataINEndpoint           =
					{
						.Address          = CDC_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};

/** Standard file stream for the CDC interface when set up, so that the virtual CDC COM port can be
 *  used like any regular character stream in the C APIs
 */
static FILE USBSerialStream;


/** Main program entry point. This routine contains the overall program flow, including initial
 *  setup of all components and the main program loop.
 */
int main(void)
{
    #define STRING_LENGTH 30
    char string[STRING_LENGTH];
    char *str = (char *)&string;
    char i = 0;
    
	SetupHardware();

	/* Create a regular character stream for the interface so that it can be used with the stdio.h functions */
	CDC_Device_CreateStream(&VirtualSerial_CDC_Interface, &USBSerialStream);

	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
	GlobalInterruptEnable();
    
    memset(str, '\0', STRING_LENGTH);
    CDC_Device_SendString(&VirtualSerial_CDC_Interface, "Ready\r\n");
    //strcpy(str, "echo: ");

	for (;;)
	{
        char compare[10];
        
		/* Must throw away unused bytes from the host, or it will lock up while waiting for the device */
        while (CDC_Device_BytesReceived
               (&VirtualSerial_CDC_Interface))
        {
            char byte = CDC_Device_ReceiveByte(&VirtualSerial_CDC_Interface);
            
            if(byte == '\r'){
                strncpy(compare, str, 3);
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, compare);
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, ": compare\r\n");
                if(strncmp(compare, "led", 3) == 0){
                    CDC_Device_SendString(&VirtualSerial_CDC_Interface, "Set LED\r\n");
                    PORTD ^= 1<<4;
                }
                string[i] = '\r';
                string[i+1] = '\n';
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, string);
                i = 0;
                memset(str, '\0', STRING_LENGTH);
                //strcpy(str, "echo: ");
                break;
            }
            string[i] = byte;
            i++;
            if(i >= STRING_LENGTH - 4){
                CDC_Device_SendString(&VirtualSerial_CDC_Interface, "Overflow\r\n");
                i = 0;
                memset(str, '\0', STRING_LENGTH);
                //strcpy(str, "echo: ");
            }
        }

		CDC_Device_USBTask(&VirtualSerial_CDC_Interface);
		USB_USBTask();
	}
}

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void SetupHardware(void)
{
	/* Disable watchdog if enabled by bootloader/fuses */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	/* Disable clock division */
	clock_prescale_set(clock_div_1);

	LEDs_Init();
	USB_Init();
    DDRD = (1<<4);
}

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial_CDC_Interface);

	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
	CDC_Device_ProcessControlRequest(&VirtualSerial_CDC_Interface);
}

