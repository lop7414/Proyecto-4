/***********************************************************************************************************
 * Francisco Javier Lopez Menendez 1414
 * Universidad del Valle de Guatemala
 * Digital 2
 */
//**********************************************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"

//**********************FUNCIONES***************************************************************************

void UART1config(void);
void display(uint8_t valor);

//*********************VARIABLES****************************************************************************

uint8_t PARQ1;
uint8_t PARQ2;
uint8_t PARQ3;
uint8_t PARQ4;
uint8_t VAR = 0;
uint8_t TOTAL = 0;

//****************************MAINLOOP**********************************************************************

void main(void)
{

//*******************************SETUP*********************************************************************

    //configuracion del reloj (40MHz)
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //Activar puertos
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);    //Display
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //Salida LEDS

    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2|GPIO_PIN_3);

    //Entrada push butons

    GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_7);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4);

    //Iniciar UART
    UART1config();

//**********************************************************************************************************

    while(1){

        //leer entradas

        PARQ1 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_4);
        PARQ2 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
        PARQ3 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_2);
        PARQ4 = GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3);

        //Parqueo 1
        //Ocupado
        if (PARQ1 == 0){
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5, 16); //led roja
            VAR |= 1;   //Set del bit 0
        }
        //Disponible
        else{
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4|GPIO_PIN_5, 32); //led verde
            VAR &= ~(1); //Clear del bit 0
        }

        //Parqueo 2
        //Ocupado
        if (PARQ2 == 0){
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6, 32); //led roja
            VAR |= 2;   //Set del bit 1
        }
        //Disponible
        else{
            GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_5|GPIO_PIN_6, 64); //led verde
            VAR &= ~(2); //Clear del bit 1
        }

        //Parqueo 3
        //Ocupado
        if (PARQ3 == 0){
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3, 4); //led roja
            VAR |= 4;   //Set del bit 2
        }
        //Disponible
        else{
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2|GPIO_PIN_3, 8); //led verde
            VAR &= ~(4); //Clear del bit 2
        }

        //Parqueo 4
        //Ocupado
        if (PARQ4 == 0){
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2|GPIO_PIN_3, 4); //led roja
            VAR |= 8;   //Set del bit 3
        }
        //Disponible
        else{
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_2|GPIO_PIN_3, 8); //led verde
            VAR &= ~(8); //Clear del bit 3
        }

        //total de parqueos ocupados

        if (1 & VAR){
                    TOTAL+=1;
                }
        if (2 & VAR){
                    TOTAL+=1;
                }
        if (4 & VAR){
                    TOTAL+=1;
                }
        if (8 & VAR){
                    TOTAL+=1;
                }

        display(TOTAL);
        UARTCharPut(UART1_BASE, VAR); //Se envía el valor al ESP32
        TOTAL = 0;  //Reset para la proxima lectura
    }
}

//**********************************FUNCIONES*********************************************

void UART1config(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1); //Activar clock para UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);//Activar clock para puerto C de la tiva
    GPIOPinConfigure(GPIO_PC4_U1RX);
    GPIOPinConfigure(GPIO_PC5_U1TX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5);
    UARTConfigSetExpClk(UART1_BASE,SysCtlClockGet(), 115200, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    UARTEnable(UART1_BASE);
}

void display(uint8_t valor){

    switch(valor){
        case 0:
            //4
                    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, 113);
                    break;
        case 1:
            //3
                    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, 122);
                    break;
        case 2:
            //2
                    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, 62);
                    break;
        case 3:
            //1
                    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, 80);
                    break;
        case 4:
            //0
                    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, 95);
                    break;
    }
}
