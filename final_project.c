/* PHYS 319 Guitar Tuner 
 * This code was made to tune a guitar with standard tuning.
 * Standard tuning: E2, A2, D3, G3, B3, E4 from 6th to 1st string
*/

#include "msp430.h"

static unsigned int t;
static unsigned int t0;
static unsigned int t_tot;
static unsigned int t_ave;
static unsigned int T;
static unsigned int f;

unsigned int string;
unsigned int note;
unsigned int oct;
unsigned int sign;

void identify(unsigned int f);
void displayString(unsigned int string);
void displayNote(unsigned int note);
void displayOctave(unsigned int oct);
void displaySign(unsigned int sign);

void main(void){
  WDTCTL = WDTPW + WDTHOLD; // Stop WDT

  P1DIR = 0xF7;  // Set P1.0 & P1.3-1.7 as outputs
  P2DIR = ~0x01; // Set P2.0 as input
  P2SEL = 0x01;  // Set P2SEL to timer   

  /* Configure Clock Modules */
  /* Use internal calibrated 1MHz clock: */
  DCOCTL = CALDCO_1MHZ;  // Set to 1 MHz range
  BCSCTL1 = CALBC1_1MHZ;
  BCSCTL2 &= ~(DIVS_3);  // SMCLK = DCO = 1MHz  

  /* Configure Timer */
  TA1CTL = TASSEL_2 + ID_0 + MC_2; // SMCLK, ID /1, continuous mode
  TA1CCTL0 = CAP + CM_1 + CCIE; // Capture mode (on rising edge),
                                // Capture interrupt enable
  
  /* Main Application Loop */
  while(1){
    __bis_SR_register(LPM0_bits + GIE); // Turn off CPU, keep SMCLK on
                                        // General interrupt enable
    identify(f);
    displayString(string);
    displayNote(note);
    displayOctave(oct);
    displaySign(sign);
  } 
}

/* Function for identifying notes being played:
- Takes in the measured frequency [Hz]
- Explores the frequency ranges following standard tuning 
- Assigns int values for the string, note and octave played
- Determines whether the note is flat, sharp or just right
*/

void identify(unsigned int f) {
  // E2, 6th string
  if (f <= 87) { // primary frequency range
    string = 6;  // string number on the guitar
    note = 14;   // hex number of the note played
    oct = 2;     // octave in which the note falls under

    // secondary frequency ranges for flat, sharp and OK notes
    if (78 <= f && f <= 80) {
      sign = 0;
    }

    else if (80 < f && f <= 84) {
      sign = 1;
    } 
    
    else if (84 < f) {
      sign = 2;
    } 
  }
  // A2, 5th string
  else if (104 <= f && f <= 117) {
    string = 5;
    note = 10;
    oct = 2;
    if (104 <= f && f <= 107) {
      sign = 0;
    }

    else if (107 < f && f <= 114) {
      sign = 1;
    } 
    
    else if (114 < f) {
      sign = 2;
    } 
  }
  // D3, 4th string
  else if (139 <= f && f <= 156) {
    string = 4;
    note = 13;
    oct = 3;
    if (139 <= f && f <= 143) {
      sign = 0;
    }

    else if (143 < f && f <= 152) {
      sign = 1;
    } 
    
    else if (152 < f) {
      sign = 2;
    } 
  }
  // G3, 3rd string
  else if (185 <= f && f <= 208) {
    string = 3;
    note = 6; // G isn't hex, so 6 is displayed instead
    oct = 3;
    if (185 <= f && f <= 191) {
      sign = 0;
    }

    else if (191 < f && f <= 202) {
      sign = 1;
    } 
    
    else if (202 < f) {
      sign = 2;
    } 
  }
  // B3, 2nd string
  else if (233 <= f && f <= 262) {
    string = 2;
    note = 11;
    oct = 3;
    if (233 <= f && f <= 240) {
      sign = 0;
    }

    else if (240 < f && f <= 255) {
      sign = 1;
    } 
    
    else if (255 < f) {
      sign = 2;
    } 
  }
  // E4, 1st string
  else if (311 <= f && f <= 349) {
    string = 1;
    note = 14;
    oct = 4;
    if (311 <= f && f <= 321) {
      sign = 0;
    }

    else if (321 < f && f <= 340) {
      sign = 1;
    } 
    
    else if (340 < f) {
      sign = 2;
    } 
  }
}

/* Displays the string number on fourth display */
void displayString(unsigned int string) {
    P1OUT =  (string << 4) + 0x07;
    P1OUT &= ~0x01;
    P1OUT |= 0x01;
}

/* Displays the note on third display */
void displayNote(unsigned int note) {
    P1OUT = (note << 4) + 0x05;
    P1OUT &= ~0x01;
    P1OUT |= 0x01;
}

/* Displays the octave number on second display */
void displayOctave(unsigned int oct) {
    P1OUT = (oct << 4) + 0x03;
    P1OUT &= ~0x01;
    P1OUT |= 0x01;
}

/* Displays whether the note is sharp, flat or natural on first display */
void displaySign(unsigned int sign) {
  if (sign == 0) { // flat
    P1OUT = 0xB1;
    P1OUT &= ~0x01;
    P1OUT |= 0x01;
  }
  
  else if (sign == 1) { // just right
    P1OUT = 0x01;
    P1OUT &= ~0x01;
    P1OUT |= 0x01;
  }

  else if (sign == 2) { // sharp
    P1OUT = 0x51;
    P1OUT &= ~0x01;
    P1OUT |= 0x01;
  }
}

/* Timer A0 interrupt service routine */
void __attribute__ ((interrupt(TIMER1_A0_VECTOR))) Timer_A1_ISR(void){
//Read the frequency of the guitar signal:  
  TA1CCTL0 &= ~CCIFG; // clear the interrupt flag
  t = TA1CCR0;    // take a time measurement [us] when signal is high
  T = t - t0;     // subtract old measurement from new
  t0 = t;         // set most recent measurement to old measurement
  f = 1000000/T;  // calculate frequency [Hz]

  __bic_SR_register_on_exit(LPM0_bits); // bit clear
}
