#include "kernellog.h"

#define LOG_DEPTH    (4 * 512)
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

uint8_t  *pLOG_CacheBuffer;  
uint32_t LOG_IN_ptr = 0;

void KernelLogInit(void)
{
  pLOG_CacheBuffer = (uint8_t *)rt_malloc(LOG_DEPTH);
  memset (pLOG_CacheBuffer, 0, LOG_DEPTH);
  LOG_IN_ptr = 0; 
}

PUTCHAR_PROTOTYPE
{
   uint32_t cnt = 0;
   
   if( LOG_IN_ptr++ >= LOG_DEPTH)
   {
    for (cnt = 1; cnt <  LOG_DEPTH; cnt ++)
    {
      pLOG_CacheBuffer[cnt -1 ] = pLOG_CacheBuffer[cnt];
    }
    LOG_IN_ptr = LOG_DEPTH;
   }
   pLOG_CacheBuffer [LOG_IN_ptr - 1] = ch;
   
   if(ch == '\n')
   {
     k_UpdateLog ((char *)pLOG_CacheBuffer);
   }
   return ch;
}
