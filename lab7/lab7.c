/**************
 * Spring 2015 Lab 7
 * Vincent Chan
 * RedID815909699
 **************/

/**Includes and declarations*************/
/**                                    **/
#include <STM32F0xx.h>

/*Led and button usages*/
#define LED8 8 //Blue Led
#define LED9 9 //Green Led
#define HEXPAD 0

/*Variable declarations*/
volatile int32_t msTick = 0;
uint8_t blinkRate=1;
int validPress=0;
int row, col;

/*Prototype functions*/
void Init();                    //Ln. 54
void SysTick_Handler();         //Ln. 69
uint8_t butPress();             //Ln. 90
void delay(uint32_t);           //Ln. 107
/**                                    **/
/****************************************/

/**Main**********************************/
/**                                    **/
int main(void) {
  /*Initialize the system.*/
  Init();

  /*This is the main function,
   * which will turn the blue LED on
   * and off, according to the current
   * blinkRate. blinkRate is modified by
   * the interrupt.
   */
  while (1) {
    GPIOC->BSRR |= (1<< LED8);
    delay(1000/(blinkRate*2));
    GPIOC->BRR |= (1<< LED8);
    delay(1000/(blinkRate*2));
  }
} //End main()
/**                                    **/
/****************************************/

/**Functions******************************/
/**                                     **/
//This function will handle all initialization of the system.
void Init() {
  /*Configure interrupt to run once every ms*/
  SysTick_Config(SystemCoreClock/1000);

  /*Clock enable port C*/
  RCC->AHBENR |= (1<<19);
  /*Configure 8th and 9th pin as output.
   * Set fist 4 pins to output
   * Set last 4 pins as pullup (input by default)*/
  GPIOC->MODER |= (1<<LED8*2) | (1<<LED9*2);
  GPIOC->MODER |= (0x55<<HEXPAD); 
  GPIOC->PUPDR |= (0x55<<(HEXPAD+4)*2);
} //End Init()

//This function is the interrupt function.
//This will happen every millisecond.
void SysTick_Handler() {
  /*Increase ms tick and debouncer tick.*/
  msTick--;
  validPress--;

  /*This is what will happen when
   * the button is pressed.
   * This includes a debouncer. */
  if(butPress()) {
    GPIOC->BSRR |= (1<< LED9);
    if(validPress<0) {
      blinkRate = butPress();
      validPress=7; //7ms debounce
    }
    while(butPress());
    GPIOC->BRR |= (1<<LED9);
  }
} //End SysTick_Handler()

//Returns the scancode if button is pressed
//Scancode: 4(row) + col + 1
//returns 0 if nothing is pressed
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

//This will delay by specified milliseconds
void delay(uint32_t time) {
  msTick = time;
  while(msTick>0);
} //End delay()
/**                                    **/
/****************************************/

/**************
 * Confidence is quiet
 *  Insecurities are loud.
 **************/
