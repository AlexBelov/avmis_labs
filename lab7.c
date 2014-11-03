#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "msp430.h"
#include "HAL_PMM.h"
#include "HAL_UCS.h"
#include "HAL_Buttons.h"
#include "HAL_Dogs102x6.h"
#include "HAL_Board.h"
#include "HAL_Menu.h"
#include "EchoUsb.h"
#include "MassStorage.h"
#include "Mouse.h"
#include "DemoApps.h"
#include "descriptors.h"
#include "usb.h"
#include "UsbCdc.h"
#include "CTS_Layer.h"

// Forward Declared Function
// char *itoa(int, char *, int);

void DrawCursor(int pos)
{
  char s[16] = "               ";
  s[pos] = '^';
  Dogs102x6_stringDraw(7, 0, s, DOGS102x6_DRAW_NORMAL);
}

void CapLED(void)
{
    uint8_t sliderPosition = 0;

    Board_ledOff(LED_ALL);
    Dogs102x6_clearScreen();
    buttonsPressed = 0;

    char control[16] = "0123456789-+*/=C";
    char lcd_calc[16] = "                ";

    int pos = 1;
    int lcd_pos = 15;
    int sel = 0;
    int stack[1] = {0, 0};
    char selected_control = 0;

    Dogs102x6_stringDraw(1, 0, "    CALCULATOR   ", DOGS102x6_DRAW_NORMAL);
    Dogs102x6_stringDraw(6, 0, control, DOGS102x6_DRAW_NORMAL);

    TI_CAPT_Init_Baseline(&slider);

    while (1)
    {
        sliderPosition = TI_CAPT_Slider(&slider);
        sliderPosition = (sliderPosition + 10) / 20;

        switch (sliderPosition)
        {
            case 0: Board_ledOff(LED_ALL);
                    break;
            case 1: Board_ledOff(LED_ALL);
                    Board_ledOn(LED4);
                    DrawCursor(--pos);
                    break;
            case 2: Board_ledOff(LED_ALL);
                    Board_ledOn(LED5);
                    DrawCursor(++pos);
                    break;
            case 3: Board_ledOff(LED_ALL);
                    Board_ledOn(LED6);
                    sel = pos;
                    break;
            case 4: Board_ledOff(LED_ALL);
                    Board_ledOn(LED7);
                    //write to file
                    break;
            case 5: Board_ledOff(LED_ALL);
                    Board_ledOn(LED8);
                    //read from file
                    break;
            case (255 / 17):
                    break;
        }
        __delay_cycles(5000000);
        Board_ledOff(LED_ALL);

        if(pos < 0)
        {
          pos = 15;
        }
        else if (pos > 16)
        {
          pos = 0;
        }

        if (sel != 0)
        {
          selected_control = control[pos]; // digit or operation



          lcd_calc[lcd_pos--] = control[pos];
          Dogs102x6_stringDraw(3, lcd_pos, lcd_calc, DOGS102x6_DRAW_NORMAL);
        }

        sel = 0;
    }

    Board_ledOff(LED_ALL);
    Dogs102x6_clearScreen();
    buttonsPressed = 0;
}

// /***************************************************************************//**
//  * @brief  Handles UNMI interrupts
//  * @param  none
//  * @return none
//  ******************************************************************************/
//
// #pragma vector = UNMI_VECTOR
// __interrupt void UNMI_ISR(void)
// {
//     switch (__even_in_range(SYSUNIV, SYSUNIV_BUSIFG))
//     {
//         case SYSUNIV_NONE:
//             __no_operation();
//             break;
//         case SYSUNIV_NMIIFG:
//             __no_operation();
//             break;
//         case SYSUNIV_OFIFG:
//             UCSCTL7 &= ~(DCOFFG + XT1LFOFFG + XT2OFFG); // Clear OSC flaut Flags fault flags
//             SFRIFG1 &= ~OFIFG;                          // Clear OFIFG fault flag
//             break;
//         case SYSUNIV_ACCVIFG:
//             __no_operation();
//             break;
//         case SYSUNIV_BUSIFG:
//             // If bus error occurred - the cleaning of flag and re-initializing of USB is required.
//             SYSBERRIV = 0;                              // clear bus error flag
//             USB_disable();                              // Disable
//     }
// }
//
// /***************************************************************************//**
//  * @}
//  ******************************************************************************/
