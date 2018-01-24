#include <xc.h>
#include <sys/attribs.h>

//configure the clock
#pragma config FNOSC = PRIPLL
#pragma config POSCMOD = EC
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1

#pragma config FPBDIV = DIV_8
#pragma config UPLLIDIV = DIV_1

void IO_Setup();
void Move_Forward();
void Turn_Left();
void Turn_Right();
void Stop();
void LEDsON();
void LEDsBlink();
void LEDsOff();
void Wait500ms();
void FindLine();
void WaitOnBTN1();
void CheckBTN2();

void main()
{
    IO_Setup();
    WaitOnBTN1();
    
    while(1)
    {    
        CheckBTN2();
        if((PORTFbits.RF12 == 1 && PORTFbits.RF13 == 0)||(PORTFbits.RF5 == 1 && PORTFbits.RF4 == 0 && PORTFbits.RF13 == 0))
        {
            Turn_Left();
        }
        else if((PORTFbits.RF12 == 0 && PORTFbits.RF13 == 1)||(PORTFbits.RF12 == 0 && PORTFbits.RF5 == 0 && PORTFbits.RF4 == 1))
        {
            Turn_Right();
        }
        else if(PORTFbits.RF12 == 1 && PORTFbits.RF5 == 1 &&PORTFbits.RF4 == 1 && PORTFbits.RF13 == 1)
        {
            FindLine();
            LEDsON();
        }
        else if(PORTFbits.RF4 == 1)
        {
            Turn_Right();
            if (PORTFbits.RF5 == 1)
            {
                Turn_Right();
            }
        }
        else if(PORTFbits.RF5 == 1)
        {
           Turn_Left();
           if (PORTFbits.RF4 == 1)
           {
               Turn_Left();
           }
        }
        else
        {
            Move_Forward();
        }
    }
    return;
}
void IO_Setup()
{
    //BTNS
    TRISAbits.TRISA6 = 1;
    TRISAbits.TRISA7 = 1;
    
    //LED IO bits
    TRISBbits.TRISB10 = 0;
    TRISBbits.TRISB11 = 0;
    TRISBbits.TRISB12 = 0;
    TRISBbits.TRISB13 = 0;
    
    LATBbits.LATB10 = 0; 
    LATBbits.LATB11 = 0; 
    LATBbits.LATB12 = 0; 
    LATBbits.LATB13 = 0; 
    
    //motors
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD7 = 0;
    
    TRISDbits.TRISD1 = 0;
    LATDbits.LATD1 = 0;
    TRISDbits.TRISD2 = 0;
    LATDbits.LATD2 = 0;
    
    //Timer1
    T1CONbits.TON = 0;
    T1CONbits.TCKPS = 3;
    PR1 = 0xFFFF;
    TMR1 = 0;
    T1CONbits.ON = 1;
    
    //Timer2
    T2CONbits.TON = 0;
    T2CONbits.TCKPS = 7;
    PR2 = 2000;
    TMR2 = 0;
    T2CONbits.ON = 1;
    
    //OC2
    OC2CONbits.OCM = 6;
    OC2R = 0;
    OC2RS = 0;
    OC2CONbits.ON = 1;
    
    //OC3
    OC3CONbits.OCM = 6;
    OC3R = 0;
    OC3RS = 0;
    OC3CONbits.ON = 1;
    
    //sensors
    TRISFbits.TRISF12 = 1;
    TRISFbits.TRISF5 = 1;
    TRISFbits.TRISF13 = 1;
    TRISFbits.TRISF4 = 1;
    
    return;
}

void Move_Forward()
{
    LATDbits.LATD6 = 1;
    LATDbits.LATD7 = 0;
    OC2RS = 1500;
    OC3RS = 1500;
    return;
}

void Turn_Left()
{
    LATDbits.LATD6 = 0;
    LATDbits.LATD7 = 0;
    OC2RS = 800;
    OC3RS = 800;
}

void Turn_Right()
{
    LATDbits.LATD6 = 1;
    LATDbits.LATD7 = 1;
    OC2RS = 800;
    OC3RS = 800;
    
    return;
}

void Stop()
{
    OC2RS = 0;
    OC3RS = 0;
}

void LEDsON()
{
    LATBbits.LATB10 = 1; 
    LATBbits.LATB11 = 1; 
    LATBbits.LATB12 = 1; 
    LATBbits.LATB13 = 1;
    return;
}

void LEDsBlink()
{
    LEDsON();
    Wait500ms();
    LEDsOff();
    Wait500ms();
    
    return; 
}

void LEDsOff()
{
    LATBbits.LATB10 = 0; 
    LATBbits.LATB11 = 0; 
    LATBbits.LATB12 = 0; 
    LATBbits.LATB13 = 0; 
    
    return; 
}

void Wait500ms()
{
    TMR1 = 0; 
    PR1 = 0xFFFF; 
    T1CONbits.ON = 1;
     
    while(TMR1< 11500)
    {
        ;
    }
   
    TMR1 = 0; 
    T1CONbits.ON = 0;
    
    return;
}

void FindLine()
{
    Stop();
    Wait500ms();
    while(PORTFbits.RF12 == 1 && PORTFbits.RF5 == 1 &&PORTFbits.RF4 == 1 && PORTFbits.RF13 == 1)
    {
        CheckBTN2();
        LEDsBlink();
        Move_Forward();
    }
    LEDsOff();
    return;
}

void WaitOnBTN1()
{
    while(PORTAbits.RA6 == 0)
        ;
    OC2CONbits.ON = 1;
    OC3CONbits.ON = 1;
    T1CONbits.ON = 1;
    T2CONbits.ON = 1;
    FindLine();
    LEDsON();
    return;
}

void CheckBTN2()
{
    if(PORTAbits.RA7 == 1)
    {
        OC2CONbits.ON = 0;
        OC3CONbits.ON = 0;
        T1CONbits.ON = 0;
        T2CONbits.ON = 0;
        LEDsOff();
        WaitOnBTN1();
    }
    
    return;
}
