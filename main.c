//This file is authored by Robin Holmquist and Erik Flink 2018.
#include <pic32mx.h>
#include <stdint.h>
#include "game_logic_header.h"
#include "oled_i2c_communication_header.h"


/*This function initiallizes everything that has to be initialliezed
for the entire game. Both Robin Holmquist and Erik Flink contributed to this function.*/
void initiallize(void){
	oled_init();
	display_init();

	//Initiallizes the game logic on difficulty 0
	initiallize_logic();
	//Timers
	//Timer and interrupts
	T3CON = 0x070; //Prescale = 256
	PR3 = 20833; // = (80 000 000/256)/15 --> will set flag 15 times per second
	TMR3 = 0; // Reset timer

	IFSCLR(0) = 1 << 12; //Sets the flag of the timer to 0

	IEC(0) = 1 << 12; //Enable interrupts from TMR3
	IPC(3) = 7 << 2 | 3; //Set interrupt priority

	enable_interrupt();

	T3CONSET = 1<<15; //Starting timer
}
/*This function updates everything that happens in the game, and is authored
by Robin Holmquist*/
void update(void){
	//Updates 15 times per second
	if(((IFS(0)&0x01000)>>12) == 1){
		IFS(0) = IFS(0) & ~0x01000; //Resets the flag to 0
		update_logic();
	}
}

/*This is where the program starts executing. Authored by Robin Holmquist */
int main(void) {
	initiallize();
	while(1){

	}
	return 0;
}
