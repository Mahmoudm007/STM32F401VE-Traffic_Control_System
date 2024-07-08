#include "Gpio.h"
#include "Rcc.h"
#include "Std_Types.h"
#include "Bit_Operations.h"


// Define the patterns for each digit
const uint8 digitPatterns[10][7] = {
    {0, 1, 1, 1, 1, 1, 1}, // 0
    {0, 0, 0, 0, 1, 1, 0}, // 1
    {1, 0, 1, 1, 0, 1, 1}, // 2
    {1, 0, 0, 1, 1, 1, 1}, // 3
    {1, 1, 0, 0, 1, 1, 0}, // 4
    {1, 1, 0, 1, 1, 0, 1}, // 5
    {1, 1, 1, 1, 1, 0, 1}, // 6
    {0, 0, 0, 0, 1, 1, 1}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 0, 1, 1, 1, 1}  // 9
};


// Function to set the GPIO pins based on the digit
void setDigit(uint8 digit, uint8 segment) {
    for (int i = 0; i < 7; i++) {
    	Gpio_WritePin(segment == 0 ? GPIO_A : GPIO_B, i, digitPatterns[digit][i] ? HIGH : LOW);
    }
}


void DELAY(unsigned long timeinMILLS){
    volatile unsigned int i = 0;
    volatile unsigned int j = 0;

    for(i = 0; i < timeinMILLS; i++)
        // to avoid the warning of the for loop is not guard
        for(j = 0; j< 2 ; j++);
}

void INIT_GPIOS_PINS(){
	uint32 i = 0;
	  for (i = 0; i < 7; i++) {
	    Gpio_ConfigPin(GPIO_A, i, GPIO_OUTPUT, GPIO_PUSH_PULL);
	    Gpio_ConfigPin(GPIO_B, i, GPIO_OUTPUT, GPIO_PUSH_PULL);
	  }
	    Gpio_ConfigPin(GPIO_C, 0, GPIO_OUTPUT, GPIO_PUSH_PULL);
	    Gpio_ConfigPin(GPIO_C, 1, GPIO_OUTPUT, GPIO_PUSH_PULL);
	    Gpio_ConfigPin(GPIO_D, 0, GPIO_INPUT, GPIO_PULL_UP);
	    // Initially set pin 1 of GPIO_C high --> Red led
	    Gpio_WritePin(GPIO_C, 1, HIGH);
}

void DISABLE_GPIO_PINS(){
	GPIO_DISABLE_PINS(GPIO_A);
	GPIO_DISABLE_PINS(GPIO_B);
	GPIO_DISABLE_PINS(GPIO_C);
	GPIO_DISABLE_PINS(GPIO_D);
	GPIO_DISABLE_PINS(GPIO_E);
}
void RCC_ENABLE(){
	Rcc_Init();
	Rcc_Enable(RCC_GPIOA);
	Rcc_Enable(RCC_GPIOB);
	Rcc_Enable(RCC_GPIOC);
	Rcc_Enable(RCC_GPIOD);
	Rcc_Enable(RCC_GPIOE);
}

void RCC_DISABLE(){
	Rcc_Disable(RCC_GPIOA);
	Rcc_Disable(RCC_GPIOB);
	Rcc_Disable(RCC_GPIOC);
	Rcc_Disable(RCC_GPIOD);
	Rcc_Disable(RCC_GPIOE);
}

int main() {
	uint8 tens_counter = 2;
	uint8 ones_counter = 0;
	uint8 toggle_state = 0; // Flag to track toggle state

	RCC_ENABLE();
	INIT_GPIOS_PINS();

	while (1) {
	        // Display tens counter
		/*
		 * @param : decimal to display
		 * @flag : if TRUE will display in GPIOA, else will display in GPIOB
		 * */
	        setDigit(tens_counter, 0);
//	        DELAY(12000);
	        // Display ones counter
	        setDigit(ones_counter, 1);
//	        DELAY(12000);
	        ones_counter--;
	        DELAY(150000);
	        if (ones_counter > 9) {
	            ones_counter = 9;
	        	DELAY(150000);
	            tens_counter--;
	        }
	        if (tens_counter == 0xFF) { // reached 00
	            tens_counter = 2;
	            ones_counter = 0;
	            // Toggle LED on pin 0 of GPIO_C
	            toggle_state = !toggle_state;
	            Gpio_WritePin(GPIO_C, 0, toggle_state ? HIGH : LOW);
	            // Toggle LED on pin 1 of GPIO_C
	            Gpio_WritePin(GPIO_C, 1, !toggle_state ? HIGH : LOW);
	        	DELAY(150000);

	        }
	    }
  return 0;
}
