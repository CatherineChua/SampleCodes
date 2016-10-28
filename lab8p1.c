#pragma config OSC = INTIO2
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config BOR =OFF
#pragma interrupt chk_isr
#pragma config CCP2MX = RB3

#include <p18f4321.h>
#include <stdio.h>
#include <math.h>
#include <usart.h>

#define SEC_LED PORTEbits.RE2
#define NSR PORTAbits.RA3
#define NSG PORTAbits.RA4
#define	NSLTR PORTAbits.RA5
#define NSLTG PORTBbits.RB0
#define EWR PORTBbits.RB1
#define EWG PORTBbits.RB2
#define EWLTR PORTBbits.RB4
#define EWLTG PORTBbits.RB5
#define NSDP PORTEbits.RE1
#define EWDP PORTEbits.RE0
#define R 0
#define Y 1
#define G 2
#define Off 3 
#define EW_PED PORTAbits.RA1
#define NS_PED PORTAbits.RA2
#define EW_LT PORTAbits.RA1
#define NS_LT PORTAbits.RA2
#define EMRGNCY PORTAbits.RA7
#define MODE PORTDbits.RD7
//#define buzzer

void chk_isr(void); //prototypeing 
void T0ISR(void);
void Wait_Half_Second();
void Wait_One_Second();
void Wait_N_Seconds(char n);
void SET_EW_LT(char color);
void SET_EW(char color);
void SET_NS_LT(char color);
void SET_NS(char color);
void Day_Mode(void);
void Evening_Mode(void);
void gen_beep(int n);
void EmergecnyCheck(void);

int TMRFLAG;
int beep;
char SNS;
char SNSLT;
char SEW;
char SEWLT;
char PORTC_TEMP;
char PORTD_TEMP;

char PR2_ARRAY[7] ={0x7C, 0x8E, 0xA6, 0x31, 0x62, 0xF9, 0x7C};
char T2CON_ARRAY[7] ={0x04, 0x04, 0x04, 0x05, 0x05, 0x04, 0x085};
char CCPR2L_ARRAY[7] ={0x3E, 0x47, 0x53, 0x18, 0x29, 0x7C, 0x3E};
char CCP2CON_ARRAY[7] ={0x1C, 0x1C, 0x1C, 0x3C, 0x1C, 0x3C, 0x1C};


void init_ADC(void)
{
	OSCCON = 0x60;	
	TRISA = 0x80;		
	TRISB = 0x00;															//Initializes the port we are going to use, makes port input
	TRISC = 0x00;															//Initializes the port we are going to use, makes port output
	TRISD = 0x80;															//Initializes the port we are going to use, makes port output	
	TRISE = 0X00;															//Initializes the port we are going to use, makes port output
	
	ADCON0=0x11; 															// analong to digital conversion
	ADCON1=0x0F;															 																		
	ADCON2=0xA9;			
							
	T0CON = 0x07;														//Set Timer 0 in Timer mode
	INTCONbits.TMR0IE = 1;												//Enable Timer 0 interrupt
	INTCONbits.TMR0IF = 0;												//Clear Timer 0 interupt flag
	INTCONbits.GIE = 1;													//Enable Glbal interrupt
	TMR0H = 0xFA;														//Program Timer high byte
	TMR0L = 0x2F;														//Program Timer low byte
	T0CONbits.TMR0ON = 1;	
																		
	
}

unsigned int get_full_ADC(void)
{
int result;
	ADCON0bits.GO = 1;			 										// Start Conversion
	while(ADCON0bits.DONE==1); 											// wait for conversion to be completed
	result = (ADRESH * 0x100) + ADRESL; 								// combine result of upper byte and
																		//lower byte into result
	return result; 														// return the result.
}

void Wait_Half_Second()													 //wait half second by nested for loop													
{
	while(TMRFLAG == 0);
	TMRFLAG = 0;
}

void Wait_One_Second() 													// wait one second function
{																		// waits one sec by calling half second 2 times
	Wait_Half_Second();
	Wait_Half_Second();
	EmergecnyCheck();
		
}
void Wait_N_Seconds(char n)												// function to wain 'n' number of seconds
{
	int i;
	for(i = n; i > 0; i--)
	{
		Wait_One_Second();												// uses the "wait one second function"
	}	
}	

void SET_NS(char color)													// set north/south direction cases
{
SNS = color;
	switch(color)
	{
		case R:															// case red light (only red is high)
			NSR = 1; 
			NSG = 0;
		break;
		case Y:															// case yellow light (both are high)
			NSR = 1;
			NSG = 1;
		break;
		case G:															// case green light (only green is high)
			NSR = 0;
			NSG = 1;
		break;
		case Off:
			NSR = 0;
			NSG = 0;
		break;
		default:														// defauklt (both are off)
			NSR = 0;
			NSG = 0;
		break;
	}	
}	
void SET_NS_LT(char color)												// north/south left turn cases
{
SNSLT = color;
	switch(color)															
	{
		case R:															// case red light (only red is high)
			NSLTR = 1;	
			NSLTG = 0;
		break;
		case Y:														  	// case yellow light (both are is high)
			NSLTR = 1;
			NSLTG = 1;
		break;
		case G:															// case green light (only green is high)
			NSLTR = 0;
			NSLTG = 1;
		break;
		case Off:
			NSLTR = 0;
			NSLTG = 0;
		break;
		default:														// default case (both are off)
			NSLTR = 0;
			NSLTG = 0;
		break;
	}	
}
void SET_EW(char color)                                                  // east/west direction cases
{
SEW = color;
	switch(color)
	{
		case R:															// case red light (only red is high)
			EWR = 1;
			EWG = 0;
		break;
		case Y:															// case yellow light (both are is high)
			EWR = 1;
			EWG = 1;
		break;
		case G:															// case green light (only green is high)
			EWR = 0;
			EWG = 1;
		break;
		case Off:
			EWR = 0;
			EWG = 0;
		break;
		default:														// default case (both are off)
			EWR = 0;
			EWG = 0;
		break;
	}	
}
void SET_EW_LT(char color)												// east/west left turn cases
{
SEWLT = color;
	switch(color)
	{
		case R:															// case red light (only red is high)
			EWLTR = 1;
			EWLTG = 0;
		break;
		case Y:															// case yellow light (both are is high)
			EWLTR = 1;
			EWLTG = 1;
		break;
		case G:															// case green light (only green is high)
			EWLTR = 0;
			EWLTG = 1;
		break;
		case Off:
			EWLTR = 0;
			EWLTG = 0;
		break;
		default:														// default case (both are off)
			EWLTR = 0;
			EWLTG = 0;
		break;
	}	
}



//High Priority Interrupt
#pragma code My_HiPriority_Int=0x08
void My_HiPrio_Int()
{
	_asm													
	GOTO chk_isr														//When Priority occurs, go to chk_isr routine
	_endasm													
}	

#pragma code

//Check ISR routine
#pragma interrupt chk_isr
void chk_isr()
{
	T0ISR();															//Go to Timer 0 ISR
}	
	

void T0ISR()
{
	INTCONbits.TMR0IF = 0;
	TMRFLAG = 1;
	TMR0H = 0xF8;
	TMR0L = 0x5F;
	SEC_LED = ~SEC_LED;
}

void Seven_Seg_Control( char Mode1, char Direction, char Num_Sec)
{
char ARRAY[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};   // seven segment display array
int U;																	// variable declarations
int L;
int i;
int temp;
//char freq[7] = {}

	if(Mode1 == 1)														// if mode is 1 == TRAFFIC
	{
		if(Direction == 0)												// if direction is 0 (east/west)
		{
			NSDP = 1;													// set N/W dp off and E/W dp on
			EWDP = 0;
			PORTD = 0xFF;												// make sure port D is off
			for(i = Num_Sec; i>0; i--)									// sev seg display countdown from num_sec on port D
			{
				PORTC = ARRAY[i];
				PORTC_TEMP = PORTC;
				Wait_One_Second();										// wait for one second
			}	
			PORTC = 0xFF;												// turn off port C
		}	
		else															// else direction is 1 (north/south)
		{
			NSDP = 0;													// set N/W dp on and E/W dp off
			EWDP = 1;
			PORTC = 0xFF;												// make sure port c is off
			for(i = Num_Sec; i>0; i--)									// sev seg display countdown from num_sec on port C
			{
				PORTD = ARRAY[i];
				PORTD_TEMP = PORTD;
				Wait_One_Second();										// wait one second 
			}
			PORTD = 0xFF;												// turn port D off
		}	
	}
	else																// else mode is 0 == PEDESTRIAN
	{
		if(Direction == 0)												// if direction is 0 (east/west)
		{
			NSDP = 1;													// set N/W dp off and E/W dp on
			EWDP = 0;
		}
		else															// else direction is 1
		{
			NSDP = 0;													// set N/W dp on and E/W dp off
			EWDP = 1;
		}		
				
		for(i = Num_Sec; i>0; i--)										// for loop for proper upper and lower number
		{																// seven segment display countdown from Num_Sec
			U = i/10;
			L = i%10;
			PORTC = ARRAY[U];
			PORTD = ARRAY[L];
			PORTC_TEMP = PORTC;
			PORTD_TEMP = PORTD;
			if(i<=7 && beep == 1)
			{
				gen_beep(i-1);
			}	
			else
				Wait_One_Second();	
		}

		PORTC = 0xFF;													// Make sure port C & D are off, then wait 1 sec
		PORTD = 0xFF;	
		Wait_One_Second();
	}
		
}


void gen_beep(int n)
{
	PR2 = PR2_ARRAY[n]; 												// values for short beep (8000Herz - 0b01111100)  
	CCPR2L = CCPR2L_ARRAY[n];											// (8000Herz - 0b00111110)
	CCP2CON = CCP2CON_ARRAY[n];											// (8000Herz - 0b00011100)  
	T2CON = T2CON_ARRAY[n];												// Turn T2 on here (8000Herz - 0b00000100)
	Wait_Half_Second();													// Wait half-second 
	T2CON = 0x00; 			    										// Turn T2 off here
	Wait_Half_Second();		    										// Wait half-second
	EmergecnyCheck();
}

void main()
{
int i;
init_ADC();																	

	while (1)															// while(1) to repeat the same process = infinite loop
	{
		if(MODE == 0)
		{
			Day_Mode();														
		}
		if(MODE == 1)
		{
			Evening_Mode();
		}	
	}																		
	
}
	
void Day_Mode(void)
{
	beep = 1;
	SET_NS(G);															// First, The NS LED will show the GREEN color while the other three RGB LEDs will
	SET_NS_LT(R);														// be on RED
	SET_EW(R);															
	SET_EW_LT(R);
	
	if(NS_PED == 0)														//If NS_PED is ‘0’, activate a delay for pedestrian crossing
	{
		Seven_Seg_Control (0, 0, 13); 									// Set mode 0 with direction 0 and do for 13 seconds
	}	

	Seven_Seg_Control (1, 0, 8);										// Set mode 1 with direction 0 for 8 seconds	

	SET_NS(Y);															// NS LED to be changed to YELLOW and stays there for 3											
	Wait_N_Seconds(3);													// seconds

	SET_NS(R);															//Turn NS LED to RED. 
	SET_NS_LT(R);														//Turn NS_LT LED to RED
	SET_EW(R);															//Turn EW_LED to RED
	SET_EW_LT(G);														// Turn EW_LT LED to GREEN and wait for 5 seconds
	Wait_N_Seconds(5);	
	
	SET_EW_LT(Y);														//Turn EW_LT to YELLOW and stays on that color for 3 seconds.
	Wait_N_Seconds(3);
	
	SET_EW_LT(R);														// Change EW_LT to RED and go directly to step 10)
	SET_EW(G);															//Make EW change to GREEN and go to step 11).
		
	if(NS_PED == 0)														//If the EW_PED switch is ‘0’, we have a ped crossing on the EW direction
	{
		Seven_Seg_Control (0, 1, 15);									// Set mode 0 with direction 1 for 15 seconds
	}
		
	Seven_Seg_Control (1, 1, 7);										// Set mode 1 with direction 1 for 7 seconds	
	SET_EW(Y);															//Change EW to YELLOW and stays in that state for 3 seconds
	Wait_N_Seconds(3);
	
	SET_EW(R);															// The program will change the EW LED to RED
	SET_NS_LT(G);														// NS-LT LED will change to GREEN and stays there for 5 seconds
	Wait_N_Seconds(5);
		
	SET_NS_LT(Y);														// Turn NS_LT LED to YELLOW and wait for 3 seconds
	Wait_N_Seconds(3);
	
	SET_NS_LT(R);														// Turn NS_LT LED from YELLOW to RED
}	
void Evening_Mode(void)
{
	//NS_PED = NS_LT
	//EW_PED = EW_LT
	beep = 0;
	SET_NS(G);															// First, The NS LED will show the GREEN color while the other three RGB LEDs will
	SET_NS_LT(R);														// be on RED
	SET_EW(R);															
	SET_EW_LT(R);
	if(NS_LT == 0)														//If NS_PED is ‘0’, activate a delay for pedestrian crossing
	{
		Seven_Seg_Control (0, 1, 18); 									// Set mode 0 with direction 0 and do for 13 seconds
	}
	
	SET_NS(Y);															// NS LED to be changed to YELLOW and stays there for 3											
	Wait_N_Seconds(3);													// seconds
	
	SET_NS(R);	
	
	if(EW_LT == 0)														//If NS_PED is ‘0’, activate a delay for pedestrian crossing
	{
		SET_EW_LT(G);
		Seven_Seg_Control (0, 0, 8); 									// Set mode 0 with direction 0 and do for 13 seconds
		SET_EW_LT(Y);
		Wait_N_Seconds(3);
		SET_EW_LT(R);
	}
	
	SET_EW(G);
	Seven_Seg_Control (0, 0, 15);
	
	SET_EW(Y);
	Wait_N_Seconds(3);
	
	SET_EW(R);
	
	if(NS_LT == 0)														//If NS_PED is ‘0’, activate a delay for pedestrian crossing
	{
		SET_NS_LT(G);
		Seven_Seg_Control (0, 1, 8); 									// Set mode 0 with direction 0 and do for 13 seconds
		SET_NS_LT(Y);
		Wait_N_Seconds(3);
		SET_NS_LT(R);
		SET_NS(G);
	}
	
}	

void EmergecnyCheck(void)
{
	while(EMRGNCY == 1)
	{
			NSR = 1; 
			NSG = 0;
			NSLTR = 1;	
			NSLTG = 0;	
			EWR = 1;
			EWG = 0;
			EWLTR = 1;
			EWLTG = 0;
		PORTC = PORTC_TEMP;
		PORTD = PORTD_TEMP;
		Wait_Half_Second();
			NSR = 0; 
			NSG = 0;
			NSLTR = 0;	
			NSLTG = 0;	
			EWR = 0;
			EWG = 0;
			EWLTR = 0;
			EWLTG = 0;
		PORTC = 0xFF;
		PORTD = 0XFF;	
		Wait_Half_Second();	
	}
	PORTC_TEMP = PORTC;
	PORTD_TEMP = PORTD;
	SET_NS(SNS);
	SET_NS_LT(SNSLT);	
	SET_EW(SEW);
	SET_EW_LT(SEWLT);
		
}	