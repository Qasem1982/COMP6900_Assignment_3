// COMP 6900: Embdedded Systems Programming/Assignemnt_3:Design & implement a traffic light system
// Submitted By Student:Qasem Abu Al-Haija(T00565951)/Assigned By: Dr. Manar Samad/Oct_28_2019/

#include "tm4c123gh6pm.h"
#include <stdint.h>

void PortB_Init(void); 				//For port B initialization
uint32_t PortB_Input(void);   //For acquiring PB7/PB6 for normal/emegency operations
void Normal_Mode(int r);			//For running the traffic light in the normal mode
void Emergency_Mode(void);		//For running the traffic light in the emergency mode
void delay(int tm); 					//For generating delay amounts

int Norm_SW_New=0x00;					// Maintain the new status of Normal Mode Switch 
int Norm_SW_Old=0x00;					// Maintain the old status of Normal Mode Switch 
int n =0;											// counting the number of Normal Mode Switch presess. 
int c =0;											// Global Variable for counting in the Normal Mode
	
int Emrg_SW_New=0x00;					// Maintain the new status of Emergency Mode Switch 
int Emrg_SW_Old=0x00;					// Maintain the old status of Emergency Mode Switch 
int t =0;											// counting the number of Emergency Mode Switch presess.
	
int main(){
PortB_Init();                           //Call Port_B Intialization at the begining of Main
while(1){
		Norm_SW_New=PortB_Input();          // Read PB7: Normal Swicth State
		if(Norm_SW_New==0x80 && Norm_SW_Old==0x00) n++;          // If Normal SW is pressed then Increment n;
		if(Norm_SW_New==0x00 && Norm_SW_Old==0x80) delay(10000); // If Normal SW is released then ==> small delay;
		
		Norm_SW_Old = Norm_SW_New;          // Maintain the current state for the normal mode switch
		if(n%2==1){													// If the press count is odd then
			Emrg_SW_New =PortB_Input();				// Read PB6: Emergency Swicth State
			if(Emrg_SW_New==0x40 && Emrg_SW_Old==0x00) t++;	       //If Emergency SW is pressed then Increment t;
			if(Emrg_SW_New==0x00 && Emrg_SW_Old==0x40) delay(5000);//If Emergency SW is released then small delay;
			
			Emrg_SW_Old = Emrg_SW_New;		    // Maintain the current state for the Emergency mode switch
			if(t%2==0) Normal_Mode(1);				// If Emergency Swicth is NOT pressed (released==> run the normal mode;
			else if(t%2==1) Emergency_Mode(); // If Emergency Swicth is pressed (released==> run the emergency mode;
		}
		else	t=0;  									      // Clear counts for emergency state presses;
}
	return 0; 
}

void PortB_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x02;  //Activate clock for part B	
	GPIO_PORTB_DIR_R   = 0x3F;  //6 LEDs (PB0,... PB5: Outputs) and 2 External SWs (PB6, PB7: Inputs)
	GPIO_PORTB_DEN_R  |= 0xFF;  //Enable digital function at pins 0-7 of Port_B
  GPIO_PORTB_AMSEL_R = 0x00;  //Disable digital function at pins 0-7 of Port_B
}

uint32_t PortB_Input(void){    //Pass along PB6 & PB7 as the input data triggered by SWs(+ve logic)
	uint32_t data = GPIO_PORTB_DATA_R & 0xC0;
	return(data); 
}

void Normal_Mode(int r){			// Run traffic light in Normal Mode
	if (c<r){										
	  GPIO_PORTB_DATA_R = 0xE1; 										// Turn On North Red and East Green
		delay(10000000); 		     											// Delay
		GPIO_PORTB_DATA_R &= 0xC0; 										// Turn On North Yellow and East Red
		c++;																					// Increment the count value
	}
	else if(c<2*r){
    GPIO_PORTB_DATA_R = 0xE2; 										// Turn On North Yellow and East Green
		delay(10000000); 		    											// Delay
		GPIO_PORTB_DATA_R &= 0xC0; 										// Turn On North Yellow and East Red
		c++;																					// Increment the count value
	}
	else if(c<3*r){
		GPIO_PORTB_DATA_R = 0xCC; 										// Turn On North Green and East Red
		delay(10000000); 		    											// Delay
		GPIO_PORTB_DATA_R &= 0xC0; 										// Turn On North Yellow and East Red
		c++;																					// Increment the count value
	}
	else if(c<4*r){
		GPIO_PORTB_DATA_R = 0xCA; 										// Turn On North Yellow and East Red
		delay(10000000); 		    											// Delay
		GPIO_PORTB_DATA_R &= 0xC0; 										// Turn On North Yellow and East Red
		c++;																					// Increment the count value
	}
	else if(c<5*r){
		GPIO_PORTB_DATA_R = 0xE1; 										// Turn On North Red and East Green
		delay(10000000); 		     											// Delay
		GPIO_PORTB_DATA_R &= 0xC0; 										// Turn On North Yellow and East Red
		c++;																					// Increment the count value
		}
	else{
		c=0;																					// Clear the count value
	}
}

void Emergency_Mode(void){												// Blinking the North Yellow and East Red
  	GPIO_PORTB_DATA_R |= 0xCA; 										// Turn On the North Yellow and East Red
		delay(500000); 															  // Short Delay
	  GPIO_PORTB_DATA_R &= ~0x0A; 									// Turn Off the North Yellow and East Red
		delay(500000); 															  // Short Delay
}

void delay (int tm){															// Delay function deffinition
  int cnt =0;
  while (cnt<tm)
		++cnt;																			 // Just loop and increment
}
