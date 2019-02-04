/*This file is authored by Erik Flink with incorporations of code written by Robin Holmquist 2018 */
#include <pic32mx.h>

/*This function initiallizes the ADC and sets the concerned pins as analog inputs.
Authored by Erik Flink with incorporations of code written by Robin Holmquist.*/
void initiallize_analog(void){
  //Global initiallization

  //A1 = AN4, A2 = AN8, A3 = AN10, A11 = AN15
  TRISBSET = 1 << 4 | 1 << 8 | 1 << 10 | 1 << 15; // Set pins as inputs
  AD1PCFGSET = 1 << 4 | 1 << 8 | 1 << 10 | 1 << 15; // Set pins as analog

  AD1CHSCLR = 1 << 23; // Use VR- as negative input

  AD1CON1 = 0x0;  // Reset settings. Value is integer form.
  AD1CON1SET = 7 << 5; // Auto convert

  AD1CON2 = 0x0; // Reset settings. Use AVDD and AVSS. One 16-word buffer. Disable alternate mode.
  AD1CON2SET = 1 << 10;  // Scan inputs
  AD1CON2SET = 4 << 2; // 4 samples per interrupt

  AD1CSSL = 1 << 4 | 1 << 8 | 1 << 10 | 1 << 15; // Select inputs to be scanned

  AD1CON3 = 1 << 8 | 10; // SAMC = 1 TAD, ADCS = 10

  AD1CON1SET = 1 << 15; // Turn on ADC module
  AD1CON1SET = 1 << 2; // Initiate sampling
}

/*These four functions returns the values of the potentiometes' values. These
versions are authored by Erik Flink.*/
int get_A1() {
  return ADC1BUF0;
}

int get_A2() {
  return ADC1BUF1;
}

int get_A3() {
  return ADC1BUF2;
}

int get_A11() {
  return ADC1BUF3;
}
