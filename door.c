/**
 *
 * Requires the bcm2835 gpio library found at
 * http://www.airspayce.com/mikem/bcm2835/index.html
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <bcm2835.h>

void on_high();
void on_low();

#define PIN_15 RPI_GPIO_P1_15
#define PIN_22 RPI_GPIO_P1_22

int run;
//Check counter runs from 0-255
//on each 0 the changed script is run regardless of status
//this means that its run every ~2 min
uint8_t counter;
uint8_t status; //Current status of door

#define SLEEP_TIME 500

int main(int argc, char** argv){

	if (!bcm2835_init())
        return 1;

	//Set Pin 15 to be output
	bcm2835_gpio_fsel(PIN_15, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(PIN_15, HIGH);
	
	// Set Pin 22 to be input with pullup
    bcm2835_gpio_fsel(PIN_22, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(PIN_22, BCM2835_GPIO_PUD_UP);
    
	run = 1;
	counter = 0;
	status = LOW;
	while(run){
		//Check if door is open or closed
        uint8_t value = bcm2835_gpio_lev(PIN_22);
        if(value == HIGH)
			on_high();
		else
			on_low();
        
		//Sleep
		counter++;
        delay(SLEEP_TIME);
	}
	return 1;
}

/** Action to perform when input is high(Door closed) */
void on_high(){
	if(status == LOW || counter == 0){
		counter = 0;
		status = HIGH;
		//Call script, easier to add new separate functionality in script.
		system("doorchanged 1");
	}
}

/** Action to perform when input is low(Door open) */
void on_open(){
	if(status == HIGH || counter == 0){
		counter = 0;
		status = LOW;
		//Call script, easier to add new separate functionality in script.
		system("doorchanged 0");
	}
}