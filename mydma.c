
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"



uint32_t ucControlTable[256] __attribute__ ((aligned(1024)));

#define CH30 (30*4)
#define BIT30 0x40000000

void DMA_Init(void){  int i;
  volatile uint32_t delay;
  for(i=0; i<256; i++){
    ucControlTable[i] = 0;
  }
  SYSCTL_RCGCDMA_R = 0x01;    // µDMA Module Run Mode Clock Gating Control
  delay = SYSCTL_RCGCDMA_R;   // allow time to finish
  UDMA_CFG_R = 0x01;          // MASTEN Controller Master Enable
  UDMA_CTLBASE_R = (uint32_t)ucControlTable;
  UDMA_PRIOCLR_R = BIT30;     // default, not high priority
  UDMA_ALTCLR_R = BIT30;      // use primary control
  UDMA_USEBURSTCLR_R = BIT30; // responds to both burst and single requests
  UDMA_REQMASKCLR_R = BIT30;  // allow the µDMA controller to recognize requests for this channel
}

void DMA_Transfer(uint32_t *source, uint32_t *destination, uint32_t count){
  ucControlTable[CH30]   = (uint32_t)source+count*4-1;       // last address
  ucControlTable[CH30+1] = (uint32_t)destination+count*4-1;  // last address
  ucControlTable[CH30+2] = 0xAA00C002+((count-1)<<4);             // DMA Channel Control Word (DMACHCTL)

  UDMA_ENASET_R = BIT30;  // µDMA Channel 30 is enabled.
  UDMA_SWREQ_R = BIT30;   // software start,
}

uint32_t DMA_Status(void){
  return (UDMA_ENASET_R&BIT30);  // µDMA Channel 30 enable nit is high if active
}
