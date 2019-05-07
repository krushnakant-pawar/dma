
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"


#define SIZE 128
uint32_t SrcBuf[SIZE],DestBuf[SIZE];
int main(void){
    volatile uint32_t delay; uint32_t i,t;
  PLL_Init();  // now running at 80 MHz
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOF; // enable Port F clock
  delay = SYSCTL_RCGCGPIO_R;              // allow time to finish
  GPIO_PORTF_DIR_R |= 0x0E;    // make PF3-1 output (PF3-1 built-in LEDs)
  GPIO_PORTF_AFSEL_R &= ~0x0E; // disable alt funct on PF3-1
  GPIO_PORTF_DEN_R |= 0x0E;    // enable digital I/O on PF3-1
                               // configure PF3-1 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFF000F)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;      // disable analog functionality on PF

  DMA_Init();  // initialize DMA channel 30 for software transfer
  t = 0;
  while(1){
    for(i=0;i<SIZE;i++){
      SrcBuf[i] = i;
      DestBuf[i] = 0;
    }
    while(DMA_Status()); // wait for idle
    DMA_Transfer(SrcBuf,DestBuf,SIZE);
    for(i=0;i<SIZE;i++)
    {
        printf("Dest at %d is %d \n",i,DestBuf[i]);
    }

    for(delay = 0; delay < 600000; delay++){
    }
  }
}
