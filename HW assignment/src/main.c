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

// Variables to use /* Change it as you want RECOMMENDED Delay_time = 10000*/
#define Delay_time     1000
volatile unsigned int Status = 1;

// Function to set the GPIO pins based on the digit
void setDigit(uint8 digit) {
    for (int i = 0; i < 7; i++) {
        Gpio_WritePin(GPIO_A, i, digitPatterns[digit][i] ? HIGH : LOW);
        DELAY(1000);
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
	uint32  j = 0;
	  for (i = 0; i < 7; i++) {
	    Gpio_ConfigPin(GPIO_A, i, GPIO_OUTPUT, GPIO_PUSH_PULL);
	  }
	  for (j = 5; j < 8; j++) {
		Gpio_ConfigPin(GPIO_C, j, GPIO_OUTPUT, GPIO_PUSH_PULL);
		  }
	    Gpio_ConfigPin(GPIO_C, 4, GPIO_INPUT, GPIO_PULL_UP);

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

void LED_SetColor(volatile unsigned int button_clicked, unsigned int red, unsigned int green, unsigned int blue, uint8 counter) {
    switch (button_clicked) {
        case 0: // BLACK
            red = 0;
            green = 0;
            blue = 0;
            break;

        case 1: // RED
            red = 255;
            green = 0;
            blue = 0;
            break;

        case 2: // GREEN
            red = 0;
            green = 255;
            blue = 0;
            break;

        case 3: // YELLOW
            red = 255;
            green = 255;
            blue = 0;
            break;

        case 4: // BLUE
            red = 0;
            green = 0;
            blue = 255;
            break;

        case 5: // MAGENTA
            red = 255;
            green = 0;
            blue = 255;
            break;

        case 6:  // CYAN
            red = 0;
            green = 255;
            blue = 255;
            break;

        default: // WHITE
            red = 255;
            green = 255;
            blue = 255;
            break;
    }

    // Set RGB LED colors
    Gpio_WritePin(GPIO_C, 5, red > 0 ? HIGH : LOW);
    Gpio_WritePin(GPIO_C, 6, green > 0 ? HIGH : LOW);
    Gpio_WritePin(GPIO_C, 7, blue > 0 ? HIGH : LOW);
    setDigit(counter);
}

int main() {
	uint8 counter = 0;
	 // Variables to store previous and current button state
	uint8 prevButtonState = 1; // Initial value HIGH --> Active LOW
	uint8 currentButtonState = 1; // Initial value HIGH --> Active LOW
	int button_clicked = 0; // The counter that toggles the colors

	uint8 red = 0;
	uint8 green = 0;
	uint8 blue = 0;
	RCC_ENABLE();
	INIT_GPIOS_PINS();

	while (Status == 1) {
	        // Read current button state
	        currentButtonState = GPIO_ReadPin(GPIO_C, 4);

	        // Check for falling edge (button press)
	        if (prevButtonState == 1 && currentButtonState == 0) {
	            currentButtonState = GPIO_ReadPin(GPIO_C, 4);
//                counter++;
	            if (button_clicked == 7 || counter == 7) {
	                // Reset the counter
	                button_clicked = 0;
	                counter = 0;
	            }
	            else{
	                // Increase button click counter
	                button_clicked++;
	                counter++;

	            }
	        }

	        /* This delay means the time which needed for toggle between state you need to press the button for 0.5sec*/

	                DELAY(0.5 * Delay_time);
	                // Set LED colors
	                LED_SetColor(button_clicked, red,green , blue, counter);
	                // Update previous button state
	                prevButtonState = currentButtonState;
	    }
  return 0;
}
