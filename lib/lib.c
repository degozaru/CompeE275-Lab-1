#include "lib.h"

/**General Function***********************************************/
/**                                                             **/
/* This will enable the specified port. */
void portEnable(char port) {
  RCC->AHBENR |= (1<<(17+(port-'A')));
} //End portEnable();

/*This will set the pin to output 1.*/
void pinHigh(char port, int pin) {
  switch(port) {
    case 'A':
      GPIOA->BSRR |= (1<<pin);
      break;
    case 'B':
      GPIOB->BSRR |= (1<<pin);
      break;
    case 'C':
      GPIOC->BSRR |= (1<<pin);
      break;
    case 'D':
      GPIOD->BSRR |= (1<<pin);
      break;
    case 'E':
      GPIOE->BSRR |= (1<<pin);
      break;
    case 'F':
      GPIOF->BSRR |= (1<<pin);
      break;
  }
} //End pinHigh()

/*This will reset the pin to output 0.*/
void pinLow(char port, int pin) {
  switch(port) {
    case 'A':
      GPIOA->BRR |= (1<<pin);
      break;
    case 'B':
      GPIOB->BRR |= (1<<pin);
      break;
    case 'C':
      GPIOC->BRR |= (1<<pin);
      break;
    case 'D':
      GPIOD->BRR |= (1<<pin);
      break;
    case 'E':
      GPIOE->BRR |= (1<<pin);
      break;
    case 'F':
      GPIOF->BRR |= (1<<pin);
      break;
  }
} //End pinLow()

/*This will initialize the pin*/
void initPin(char port, int pin, int mode) {
  switch(port) {
    case 'A':
      GPIOA->MODER |= (mode<<pin*2);
      break;
    case 'B':
      GPIOB->MODER |= (mode<<pin*2);
      break;
    case 'C':
      GPIOC->MODER |= (mode<<pin*2);
      break;
    case 'D':
      GPIOD->MODER |= (mode<<pin*2);
      break;
    case 'E':
      GPIOE->MODER |= (mode<<pin*2);
      break;
    case 'F':
      GPIOF->MODER |= (mode<<pin*2);
      break;
  }
} //End initpin*2()
/**                                                             **/
/*****************************************************************/

/**HEXPAD FUNCTIONS***********************************************/
/**                                                             **/
/*This will initialize the hexpad for use.
 * note that port C must be enabled before
 * the hexpad will work.*/
void initHex() {
  /*Configure 8th and 9th pin as output.
   * Set fist 4 pins to output
   * Set last 4 pins as pullup (input by default)*/
  GPIOC->MODER |= (1<<LED8*2) | (1<<LED9*2);
  GPIOC->MODER |= (0x55<<HEXPAD); 
  GPIOC->PUPDR |= (0x55<<(HEXPAD+4)*2);
} //End initHex()

/*Returns the scancode if button is pressed
 * Scancode: 4(row) + col + 1
 * returns 0 if nothing is pressed*/
uint8_t butPress() {
	GPIOC->BSRR |= (0xf<<(HEXPAD));
	for(row=0; row<4; row++) {
    GPIOC->BRR |= (1<<(HEXPAD+row));
    for(col=0; col<4; col++)
      if(~GPIOC->IDR & (1<<((HEXPAD+4)+col))) {
        GPIOC->BSRR |= (1<<(HEXPAD+row));
        return (row*4)+col+1;
      }
    GPIOC->BSRR |= (1<<(HEXPAD+row));
  }
  return 0;
} //End butPress()
/**                                                             **/
/*****************************************************************/

