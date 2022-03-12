# MSP-430-Guitar-Tuner

## About the Project
I built a guitar tuner for physics lab course I took back in my undergrad in 2017. Aside from needing to complete a final project of some sort, I thought it would be neat to make my own tuner because the display of the tuner integrated into my acoustic guitar had fogged up during air travel. Plus, acoustic physics was fun and different from all the other areas of physics I was studying at the time.

The course centered around microcontroller-based device development, so this project was built with an MSP 430 microcontroller for signal analysis. The MSP430 was accompanied by a circuit for signal processing. 

## Built With
### Hardware
- MSP430 Microcontroller

### Software
- C
- msp430.h

## How It Worked
### The Circuit
The guitar tuner circuit consisted of three main components, namely an AC-coupled non-inverting amplifier, a low-pass filter, and a square wave converter. The AC coupler removed the DC voltage offset of the acoustic signal and improved its resolution. The low-pass filter applied a threshold to remove unwanted frequencies based on its RC time constant. The signal was then converted using a comparator from a sinusoid to a square wave to improve signal readability and analysis for the microcontroller. The comparator was AC coupled to reduce noise in the square wave output. 

### final_project.c
Upon receiving the processed signal, the microcontroller identifies its frequency by calculating the time difference between the rising edges of the input signal and getting the reciprocal of this value. The frequency of the signal needed to be rounded off to the nearest integer to prevent using floats due to the limited memory of the MSP430. The calculated frequency was then sent to a 4-digit 7-segment display formatted to show the string being tuned, the frequency's corresponding letter note, and its octave. LED lights were used to indicate whether the note from the plucked string was sharp, flat, or just right.

## Contact
Andie Buenconsejo   
Email: al.buenco@gmail.com  
[Project Link](https://github.com/abuenco/Guitar-Tuner)
