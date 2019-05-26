#include <string.h>
#include "GUI.h"
#include "GUI_Private.h"
#include "GUIDRV_Lin.h"
#include "stm32f4xx.h"
#include "lcd_rgb.h"
#include <rtthread.h>


#undef  LCD_SWAP_XY
#undef  LCD_MIRROR_Y

#define LCD_SWAP_XY  0
#define LCD_MIRROR_Y 1

#define XSIZE_PHYS LCD_Width		// 显示屏长度
#define YSIZE_PHYS LCD_Height		// 显示屏宽度

#define XSIZE_0 XSIZE_PHYS
#define YSIZE_0 YSIZE_PHYS


// 多缓冲个数，最大支持3缓冲，开启多缓冲可以让动态绘图更加流畅
// 此处只是定义多缓冲的个数，在程序中还需另外调用函数开启多缓冲功能
//	可调用 WM_MULTIBUF_Enable(1) 让窗口管理器自动使用多缓冲
//
#define NUM_BUFFERS  3

//	虚拟屏幕的个数，emWin不支持多缓冲和虚拟屏幕同时使用
//
#define NUM_VSCREENS 1 

//	定义图层数，STM32F429最大支持双图层
//	若使用双图层，通过 GUI_SelectLayer() 函数来切换所要显示的图层

#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS 1

#if (GUI_NUM_LAYERS > 1)
  #define XSIZE_1 XSIZE_PHYS
  #define YSIZE_1 YSIZE_PHYS
#endif


//	设置每个图层所使用的显存，429最大支持双图层
//	本例程给LTDC划分的显存为每层 320*240*4*2 字节，预留了32位色开双层显示以及使用三缓冲的空间，
//	用户可根据自己的实际应用进行调整,在 lcd_rgb.h 和 GUIConf.c 里进行配置
//
#define LCD_LAYER0_FRAME_BUFFER  LCD_MemoryAdd									
#define LCD_LAYER1_FRAME_BUFFER  LCD_MemoryAdd + LCD_MemoryAdd_OFFSET	

//  配置 emWin 每个图层的颜色格式，支持8位色、16位色、24位色以及32位色，用户可根据需要进行配置
//	需要给每个图层合理的划分显存，假设使用16位色、三缓冲，则该图层占用 480*272*2*3 字节的SDRAM空间
//	为了方便使用，本例程给LTDC划分的显存为每层 800*480*4*3 字节，预留了32位色开双层显示以及使用三缓冲的空间，
//	用户可根据自己的实际应用进行显存的调整,在 lcd_rgb.h 和 GUIConf.c 里进行配置
//

#define COLOR_CONVERSION_0 GUICC_M8888I			// ARGB8888 格式，32位色，带8位的透明色，一般只在双图层下使用
#define DISPLAY_DRIVER_0   GUIDRV_LIN_32		

//#define COLOR_CONVERSION_0 GUICC_M888			// RGB888 格式，24位色
//#define DISPLAY_DRIVER_0   GUIDRV_LIN_24

//#define COLOR_CONVERSION_0 GUICC_M1555I			// ARGB1555格式，16位色，带1位透明色，一般只在双图层下使用
//#define DISPLAY_DRIVER_0   GUIDRV_LIN_16

//#define COLOR_CONVERSION_0 GUICC_M565			// RGB565 格式，16位色，最常用的颜色格式
//#define DISPLAY_DRIVER_0   GUIDRV_LIN_16

//#define COLOR_CONVERSION_0 GUICC_M4444I			// ARGB4444格式，16位色，带4位透明色，一般只在双图层下使用
//#define DISPLAY_DRIVER_0   GUIDRV_LIN_16

//#define COLOR_CONVERSION_0 GUICC_8666			// L8格式，8位色，占用的开销最低，对颜色要求不高的场合可使用，具有很高的使用价值
//#define DISPLAY_DRIVER_0   GUIDRV_LIN_8
 

  
#if (GUI_NUM_LAYERS > 1)

  #define COLOR_CONVERSION_1 GUICC_M8888I			// ARGB8888 格式，32位色，带8位的透明色，一般只在双图层下使用
  #define DISPLAY_DRIVER_1   GUIDRV_LIN_32
 
//  #define COLOR_CONVERSION_1 GUICC_M888			// RGB888 格式，24位色
//  #define DISPLAY_DRIVER_1   GUIDRV_LIN_24

//  #define COLOR_CONVERSION_1 GUICC_M1555I		// ARGB1555格式，16位色，带1位透明色，一般只在双图层下使用
//  #define DISPLAY_DRIVER_1   GUIDRV_LIN_16  

//  #define COLOR_CONVERSION_1 GUICC_M565			// RGB565 格式，16位色，最常用的颜色格式
//  #define DISPLAY_DRIVER_1   GUIDRV_LIN_16

//  #define COLOR_CONVERSION_1 GUICC_M4444I		// ARGB4444格式，16位色，带4位透明色，一般只在双图层下使用
//  #define DISPLAY_DRIVER_1   GUIDRV_LIN_16  

//  #define COLOR_CONVERSION_1 GUICC_8666			// L8格式，8位色，占用的开销最低，对颜色要求不高的场合可使用，具有很高的使用价值
//  #define DISPLAY_DRIVER_1   GUIDRV_LIN_8
  
#endif

/************************************************ 以下为底层驱动，用户无需修改 ***************************************************************************/


/* Private macro -------------------------------------------------------------*/
/* Redirect bulk conversion to DMA2D routines */
#define DEFINEDMA2D_COLORCONVERSION(PFIX, PIXELFORMAT)                                                             \
static void Color2IndexBulk_##PFIX##DMA2D(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex) { \
  DMA_Color2IndexBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                         \
}                                                                                                                   \
static void _Index2ColorBulk_##PFIX##DMA2D(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex) { \
  DMA_Index2ColorBulk(pIndex, pColor, NumItems, SizeOfIndex, PIXELFORMAT);                                         \
}

/* Private variables ---------------------------------------------------------*/
static LTDC_Layer_TypeDef     * apLayer[]      = { LTDC_Layer1, LTDC_Layer2 };
static uint32_t                 layer_address[GUI_NUM_LAYERS];
static __IO int32_t             layer_pending_buffer[GUI_NUM_LAYERS];
static uint32_t                 layer_buffer_index[GUI_NUM_LAYERS];
static uint32_t                 layer_xsize[GUI_NUM_LAYERS];
static uint32_t                 layer_ysize[GUI_NUM_LAYERS];
static uint32_t                 layer_bpp[GUI_NUM_LAYERS];

static const LCD_API_COLOR_CONV * apColorConvAPI[] = 
{
  COLOR_CONVERSION_0,
#if GUI_NUM_LAYERS > 1
  COLOR_CONVERSION_1,
#endif
};

static U32 aBufferDMA2D [XSIZE_PHYS * sizeof(U32)];
static U32 aBuffer_FG   [XSIZE_PHYS * sizeof(U32)];
static U32 aBuffer_BG   [XSIZE_PHYS * sizeof(U32)];

/* Private function prototypes -----------------------------------------------*/

static void DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, uint32_t NumItems, U8 SizeOfIndex, uint32_t PixelFormat);
static void DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, uint32_t NumItems, U8 SizeOfIndex, uint32_t PixelFormat);

DEFINEDMA2D_COLORCONVERSION(M8888I, LTDC_Pixelformat_ARGB8888)
DEFINEDMA2D_COLORCONVERSION(M888,   LTDC_Pixelformat_ARGB8888)
DEFINEDMA2D_COLORCONVERSION(M565,   LTDC_Pixelformat_RGB565)
DEFINEDMA2D_COLORCONVERSION(M1555I, LTDC_Pixelformat_ARGB1555)
DEFINEDMA2D_COLORCONVERSION(M4444I, LTDC_Pixelformat_ARGB4444)

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  
  * @param  LayerIndex: Layer index
  * @retval The LTDC Pixel format
  */
static U32 GetPixelformat(int LayerIndex) 
{
  const LCD_API_COLOR_CONV * pColorConvAPI;
  
  if (LayerIndex >= GUI_COUNTOF(apColorConvAPI)) 
  {
    return 0;
  }
  pColorConvAPI = apColorConvAPI[LayerIndex];
  
  if(pColorConvAPI == GUICC_M8888I) 
  {
    return LTDC_Pixelformat_ARGB8888;
  } 
  
  else if (pColorConvAPI == GUICC_M888) 
  {
    return LTDC_Pixelformat_RGB888;
  } 
  
  else if (pColorConvAPI == GUICC_M565) 
  {
    return LTDC_Pixelformat_RGB565;
  } 
  
  else if (pColorConvAPI == GUICC_M1555I) 
  {
    return LTDC_Pixelformat_ARGB1555;
  } 
  
  else if (pColorConvAPI == GUICC_M4444I) 
  {
    return LTDC_Pixelformat_ARGB4444;
  } 
  
  else if (pColorConvAPI == GUICC_8666) 
  {
    return LTDC_Pixelformat_L8;
  } 
  
  else if (pColorConvAPI == GUICC_1616I) 
  {
    return LTDC_Pixelformat_AL44;
  } 
  
  else if (pColorConvAPI == GUICC_88666I) 
  {
    return LTDC_Pixelformat_AL88;
  }
  while (1); 
}

/**
  * @brief  Returns used Bytes per line in the selected layer
  * @param  LayerIndex: Layer index
  * @param  xSize: LCD width
  * @retval The calculated "Bytes per line" value
  */
static uint32_t GetBytesPerLine(uint32_t LayerIndex, uint32_t xSize) 
{
  uint32_t BitsPerPixel, BytesPerLine;

  BitsPerPixel  = LCD_GetBitsPerPixelEx(LayerIndex);
  BytesPerLine = (BitsPerPixel * xSize + 7) / 8;
  return BytesPerLine;
}

/**
  * @brief  
  * @param  pColor
  * @param  NumItems
  * @retval None
  */
static void DMA_LoadLUT(LCD_COLOR * pColor, uint32_t NumItems) 
{
  DMA2D->FGCMAR  = (uint32_t)pColor; 
  
  /*  Foreground PFC Control Register */
  DMA2D->FGPFCCR  = LTDC_Pixelformat_RGB888         /* Pixel format */
                  | ((NumItems - 1) & 0xFF) << 8;   /* Number of items to load */
  DMA2D->FGPFCCR |= (1 << 5);                       /* Start loading */
}

/**
  * @brief  
  * @param  pColorSrc: Source address
  * @param  pColorDst: Destination address
  * @param  NumItems
  * @retval None
  */
static void InvertAlpha_SwapRB(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, uint32_t NumItems) 
{
  uint32_t Color;
  do 
  {
    Color = *pColorSrc++;
    *pColorDst++ = ((Color & 0x000000FF) << 16)         /* Swap red <-> blue */
                 |  (Color & 0x0000FF00)                /* Green */
                 | ((Color & 0x00FF0000) >> 16)         /* Swap red <-> blue */
                 | ((Color & 0xFF000000) ^ 0xFF000000); /* Invert alpha */
  } while (--NumItems);
}

/**
  * @brief  Color format of DMA2D is different to STemWin color format. This routine
  *         inverts alpha that it is compatible to STemWin and vice versa
  * @param  pColorSrc 
  * @param  pColorDst
  * @param  NumItems
  * @retval None
  */
static void InvertAlpha(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, uint32_t NumItems) 
{
  uint32_t Color;

  do 
  {
    Color = *pColorSrc++;
    *pColorDst++ = Color ^ 0xFF000000; /* Invert alpha */
  } while (--NumItems);
}

/**
  * @brief  
  * @param  pColorFG 
  * @param  pColorBG
  * @param  pColorDst
  * @param  NumItems
  * @retval None
  */
static void DMA_AlphaBlendingBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, uint32_t NumItems) 
{
  /* Set up mode */
  DMA2D->CR      = 0x00020000UL | (1 << 9);         /* Control Register (Memory to memory with blending of FG and BG and TCIE) */
  
  /* Set up pointers */
  DMA2D->FGMAR   = (uint32_t)pColorFG;                   /* Foreground Memory Address Register */
  DMA2D->BGMAR   = (uint32_t)pColorBG;                   /* Background Memory Address Register */
  DMA2D->OMAR    = (uint32_t)pColorDst;                  /* Output Memory Address Register (Destination address) */
  
  /* Set up offsets */
  DMA2D->FGOR    = 0;                               /* Foreground Offset Register */
  DMA2D->BGOR    = 0;                               /* Background Offset Register */
  DMA2D->OOR     = 0;                               /* Output Offset Register */
  
  /* Set up pixel format */
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888;       /* Foreground PFC Control Register (Defines the FG pixel format) */
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888;       /* Background PFC Control Register (Defines the BG pixel format) */
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;       /* Output     PFC Control Register (Defines the output pixel format) */
  
  /* Set up size */
  DMA2D->NLR     = (uint32_t)(NumItems << 16) | 1;       /* Number of Line Register (Size configuration of area to be transfered) */
  
  /* Execute operation */
  
  DMA2D->CR     |= DMA2D_CR_START;  
    
  while (DMA2D->CR & DMA2D_CR_START) 
  {
  }
}

/**
  * @brief  Function for mixing up 2 colors with the given intensity.
  *         If the background color is completely transparent the
  *         foreground color should be used unchanged.
  * @param  Color 
  * @param  BkColor
  * @param  Intens
  * @retval LCD_COLOR
  */
static LCD_COLOR DMA_MixColors(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens) 
{
  uint32_t ColorFG, ColorBG, ColorDst;

  if ((BkColor & 0xFF000000) == 0xFF000000)
  {
    return Color;
  }
  ColorFG = Color   ^ 0xFF000000;
  ColorBG = BkColor ^ 0xFF000000;
  
  /* Set up mode */
  DMA2D->CR      = 0x00020000UL | (1 << 9);         /* Control Register (Memory to memory with blending of FG and BG and TCIE) */
  
  /* Set up pointers */
  DMA2D->FGMAR   = (uint32_t)&ColorFG;                   /* Foreground Memory Address Register */
  DMA2D->BGMAR   = (uint32_t)&ColorBG;                   /* Background Memory Address Register */
  DMA2D->OMAR    = (uint32_t)&ColorDst;                  /* Output Memory Address Register (Destination address) */
  
  /* Set up pixel format */
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (1UL << 16)
                 | ((uint32_t)Intens << 24);
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (0UL << 16)
                 | ((uint32_t)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;
  
  /* Set up size */
  DMA2D->NLR     = (uint32_t)(1 << 16) | 1;              /* Number of Line Register (Size configuration of area to be transfered) */
  
  /* Execute operation */
  DMA2D->CR     |= DMA2D_CR_START;                       /* Control Register (Start operation) */
  
  /* Wait until transfer is done */
  while (DMA2D->CR & DMA2D_CR_START) 
  {
  }

  return (ColorDst ^ 0xFF000000);
}

/**
  * @brief  
  * @param  pSrc 
  * @param  pDst
  * @param  PixelFormatSrc
  * @param  PixelFormatDst
  * @param  NumItems
  * @retval None
  */
static void DMA_ConvertColor(void * pSrc, void * pDst,  uint32_t PixelFormatSrc, uint32_t PixelFormatDst, uint32_t NumItems)
{
  /* Set up mode */
  
  DMA2D->CR      = 0x00010000UL | (1 << 9);         /* Control Register (Memory to memory with pixel format conversion and TCIE) */
  
  /* Set up pointers */
  DMA2D->FGMAR   = (uint32_t)pSrc;                       /* Foreground Memory Address Register (Source address) */
  DMA2D->OMAR    = (uint32_t)pDst;                       /* Output Memory Address Register (Destination address) */
  
  /* Set up offsets */
  DMA2D->FGOR    = 0;                               /* Foreground Offset Register (Source line offset) */
  DMA2D->OOR     = 0;                               /* Output Offset Register (Destination line offset) */
  
  /* Set up pixel format */
  DMA2D->FGPFCCR = PixelFormatSrc;                  /* Foreground PFC Control Register (Defines the input pixel format) */
  DMA2D->OPFCCR  = PixelFormatDst;                  /* Output PFC Control Register (Defines the output pixel format) */
  
  /* Set up size */
  DMA2D->NLR     = (uint32_t)(NumItems << 16) | 1;       /* Number of Line Register (Size configuration of area to be transfered) */
  
  /* Execute operation */
  DMA2D->CR     |= DMA2D_CR_START;  
    
  while (DMA2D->CR & DMA2D_CR_START) 
  {
  }
}

/**
  * @brief  The STemWin function LCD_GetpPalConvTable() normally translates the given colors into
  *         index values for the display controller. In case of index based bitmaps without
  *         transparent pixels we load the palette only to the DMA2D LUT registers to be
  *         translated (converted) during the process of drawing via DMA2D.
  * @param  pLogPal 
  * @param  pBitmap
  * @param  LayerIndex
  * @retval LCD_PIXELINDEX
  */
static LCD_PIXELINDEX * _LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, const GUI_BITMAP GUI_UNI_PTR * pBitmap, int LayerIndex)
{
  void (* pFunc)(void);
  int DoDefault = 0;
  
  /* Check if we have a non transparent device independent bitmap */
  if (pBitmap->BitsPerPixel == 8) 
  {
    pFunc = LCD_GetDevFunc(LayerIndex, LCD_DEVFUNC_DRAWBMP_8BPP);
    if (pFunc) 
    {
      if (pBitmap->pPal) 
      {
        if (pBitmap->pPal->HasTrans) 
        {
          DoDefault = 1;
        }
      } 
      else 
      {
        DoDefault = 1;
      }
    } 
    else 
    {
      DoDefault = 1;
    }
  } 
  else 
  {
    DoDefault = 1;
  }
  
  /* Default palette management for other cases */
  if (DoDefault) 
  {
    
    /* Return a pointer to the index values to be used by the controller */
    return LCD_GetpPalConvTable(pLogPal);
  }
  
  /* Convert palette colors from ARGB to ABGR */
  InvertAlpha_SwapRB((U32 *)pLogPal->pPalEntries, aBufferDMA2D, pLogPal->NumEntries);
  
  /* Load LUT using DMA2D */
  DMA_LoadLUT(aBufferDMA2D, pLogPal->NumEntries);
  
  /* Return something not NULL */
  return aBufferDMA2D;
}


/**
  * @brief  
  * @param  pColorFG 
  * @param  pColorBG
  * @param  pColorDst
  * @param  Intens
  * @param  NumItems
  * @retval None
  */
static void DMA_MixColorsBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U8 Intens, uint32_t NumItems)
{
  /* Set up mode */
  DMA2D->CR      = 0x00020000UL | (1 << 9);         /* Control Register (Memory to memory with blending of FG and BG and TCIE) */
  
  /* Set up pointers */
  DMA2D->FGMAR   = (uint32_t)pColorFG;                   /* Foreground Memory Address Register */
  DMA2D->BGMAR   = (uint32_t)pColorBG;                   /* Background Memory Address Register */
  DMA2D->OMAR    = (uint32_t)pColorDst;                  /* Output Memory Address Register (Destination address) */
  
  /* Set up pixel format */
  DMA2D->FGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (1UL << 16)
                 | ((uint32_t)Intens << 24);
  DMA2D->BGPFCCR = LTDC_Pixelformat_ARGB8888
                 | (0UL << 16)
                 | ((uint32_t)(255 - Intens) << 24);
  DMA2D->OPFCCR  = LTDC_Pixelformat_ARGB8888;
  
  /* Set up size */
  DMA2D->NLR     = (uint32_t)(NumItems << 16) | 1;              /* Number of Line Register (Size configuration of area to be transfered) */
  
  /* Execute operation */
  DMA2D->CR     |= DMA2D_CR_START;  
    
  while (DMA2D->CR & DMA2D_CR_START)
  {
  }
}

/**
  * @brief  
  * @param  pColorFG 
  * @param  pColorBG
  * @param  pColorDst
  * @param  NumItems
  * @retval None
  */
static void DMA_AlphaBlending(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems)
{
  /* Invert alpha values */
  InvertAlpha(pColorFG, aBuffer_FG, NumItems);
  InvertAlpha(pColorBG, aBuffer_BG, NumItems);
  
  /* Use DMA2D for mixing */
  DMA_AlphaBlendingBulk(aBuffer_FG, aBuffer_BG, aBufferDMA2D, NumItems);
  
  /* Invert alpha values */
  InvertAlpha(aBufferDMA2D, pColorDst, NumItems);
}


/**
  * @brief  This routine is used by the STemWin color conversion routines to use DMA2D for
  *         color conversion. It converts the given index values to 32 bit colors.
  *         Because STemWin uses ABGR internally and 0x00 and 0xFF for opaque and fully
  *         transparent the color array needs to be converted after DMA2D has been used.
  * @param  pIndex 
  * @param  pColor
  * @param  NumItems
  * @param  SizeOfIndex
  * @param  PixelFormat
  * @retval None
  */
static void DMA_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, uint32_t NumItems, U8 SizeOfIndex, uint32_t PixelFormat)
{
  /* Use DMA2D for the conversion */
  DMA_ConvertColor(pIndex, aBufferDMA2D, PixelFormat, LTDC_Pixelformat_ARGB8888, NumItems);
  
  /* Convert colors from ARGB to ABGR and invert alpha values */
  InvertAlpha_SwapRB(aBufferDMA2D, pColor, NumItems);
}

/**
  * @brief  This routine is used by the STemWin color conversion routines to use DMA2D for
  *         color conversion. It converts the given 32 bit color array to index values.
  *         Because STemWin uses ABGR internally and 0x00 and 0xFF for opaque and fully
  *         transparent the given color array needs to be converted before DMA2D can be used.
  * @param  pColor
  * @param  pIndex 
  * @param  NumItems
  * @param  SizeOfIndex
  * @param  PixelFormat
  * @retval None
  */
static void DMA_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, uint32_t NumItems, U8 SizeOfIndex, uint32_t PixelFormat) 
{
  /* Convert colors from ABGR to ARGB and invert alpha values */
  InvertAlpha_SwapRB(pColor, aBufferDMA2D, NumItems);
  
  /* Use DMA2D for the conversion */
  DMA_ConvertColor(aBufferDMA2D, pIndex, LTDC_Pixelformat_ARGB8888, PixelFormat, NumItems);
}

/**
  * @brief  
  * @param  pFG 
  * @param  pBG
  * @param  pDst
  * @param  OffFG
  * @param  OffBG
  * @param  OffDest 
  * @param  xSize
  * @param  ySize
  * @param  Intens
  * @retval None
  */
static void LCD_MixColorsBulk(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens) {
  int y;

  GUI_USE_PARA(OffFG);
  GUI_USE_PARA(OffDest);
  for (y = 0; y < ySize; y++) 
  {
    /* Invert alpha values */
    InvertAlpha(pFG, aBuffer_FG, xSize);
    InvertAlpha(pBG, aBuffer_BG, xSize);

    DMA_MixColorsBulk(aBuffer_FG, aBuffer_BG, aBufferDMA2D, Intens, xSize);

    InvertAlpha(aBufferDMA2D, pDst, xSize);
    pFG  += xSize + OffFG;
    pBG  += xSize + OffBG;
    pDst += xSize + OffDest;
  }
}

/**
  * @brief  
  * @param  LTDC_Layerx 
  * @param  NewState
  * @retval None
  */
static void LTDC_LayerEnableColorKeying(LTDC_Layer_TypeDef * LTDC_Layerx, int NewState)
{
  if (NewState != DISABLE)
  {
    LTDC_Layerx->CR |= (uint32_t)LTDC_LxCR_COLKEN;
  } 
  else 
  {
    LTDC_Layerx->CR &= ~(uint32_t)LTDC_LxCR_COLKEN;
  }
  LTDC->SRCR = LTDC_SRCR_VBR; /* Reload on next blanking period */
}

/**
  * @brief  
  * @param  LayerIndex 
  * @param  xPos
  * @param  yPos
  * @retval None
  */
static void LTDC_SetLayerPos(int LayerIndex, int xPos, int yPos)
{
  int xSize, ySize;
  uint32_t HorizontalStart, HorizontalStop, VerticalStart, VerticalStop;

  xSize = LCD_GetXSizeEx(LayerIndex);
  ySize = LCD_GetYSizeEx(LayerIndex);
  HorizontalStart = xPos + HBP + 1;
  HorizontalStop  = xPos + HBP + xSize;
  VerticalStart   = yPos + VBP + 1;
  VerticalStop    = yPos + VBP + ySize;
  
  /* Horizontal start and stop position */
  apLayer[LayerIndex]->WHPCR &= ~(LTDC_LxWHPCR_WHSTPOS | LTDC_LxWHPCR_WHSPPOS);
  apLayer[LayerIndex]->WHPCR = (HorizontalStart | (HorizontalStop << 16));
  
  /* Vertical start and stop position */
  apLayer[LayerIndex]->WVPCR &= ~(LTDC_LxWVPCR_WVSTPOS | LTDC_LxWVPCR_WVSPPOS);
  apLayer[LayerIndex]->WVPCR  = (VerticalStart | (VerticalStop << 16));
  
  /* Reload configuration */
  LTDC_ReloadConfig(LTDC_SRCR_VBR); /* Reload on next blanking period */
}

/**
  * @brief  
  * @param  LayerIndex 
  * @param  Alpha
  * @retval None
  */
static void LTDC_SetLayerAlpha(int LayerIndex, int Alpha)
{
  /* Set constant alpha value */
  apLayer[LayerIndex]->CACR &= ~(LTDC_LxCACR_CONSTA);
  apLayer[LayerIndex]->CACR  = 255 - Alpha;
  
  /* Reload configuration */
  LTDC_ReloadConfig(LTDC_SRCR_IMR);
}

/**
  * @brief  
  * @param  LayerIndex 
  * @param  Color
  * @param  Pos
  * @retval None
  */
static void LTDC_SetLUTEntry(int LayerIndex, uint32_t Color, int Pos) 
{
  uint32_t r, g, b, a;

  r = ( Color        & 0xff) << 16;
  g = ((Color >>  8) & 0xff) <<  8;
  b = ((Color >> 16) & 0xff);
  a = Pos << 24;
  apLayer[LayerIndex]->CLUTWR &= ~(LTDC_LxCLUTWR_BLUE | LTDC_LxCLUTWR_GREEN | LTDC_LxCLUTWR_RED | LTDC_LxCLUTWR_CLUTADD);
  apLayer[LayerIndex]->CLUTWR  = r | g | b | a;
  
  /* Reload configuration */
  LTDC_ReloadConfig(LTDC_SRCR_IMR);
}

/**
  * @brief  Copy a memory area content to another memory area
  * @param  LayerIndex: Layer index
  * @param  pSrc: Source address
  * @param  pDst: Destination address
  * @param  xSize: width of the area to be transfered
  * @param  ySize: height of the area to be transfered
  * @param  OffLineSrc: Source line offset
  * @param  OffLineDst: Destination line offset
  * @retval None
  */
static void DMA2D_Copy(uint32_t LayerIndex, void * pSrc, void * pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst)
{
  uint32_t PixelFormat;

  PixelFormat =  GetPixelformat(LayerIndex);
  DMA2D->CR      = 0x00000000UL | (1 << 9);              /* Memory to memory and TCIE                     */
  DMA2D->FGMAR   = (uint32_t)pSrc;                       /* Source address                                */
  DMA2D->OMAR    = (uint32_t)pDst;                       /* Destination address                           */
  DMA2D->FGOR    = OffLineSrc;                           /* Source line offset                            */
  DMA2D->OOR     = OffLineDst;                           /* Destination line offset                       */
  DMA2D->FGPFCCR = PixelFormat;                          /* Defines the number of pixels to be transfered */
  DMA2D->NLR     = (uint32_t)(xSize << 16) | (U16)ySize; /* Size configuration of area to be transfered   */
  DMA2D->CR     |= DMA2D_CR_START;                       /* Start operation                               */

  while (DMA2D->CR & DMA2D_CR_START) 
  {
  }
}

/**
  * @brief  Fill a color index into a memory area
  * @param  LayerIndex: Layer index
  * @param  pDst: Destination address
  * @param  xSize: width of the area to be transfered
  * @param  ySize: height of the area to be transfered
  * @param  OffLine: Destination line offset
  * @param  ColorIndex: Color to be used
  * @retval None
  */
static void DMA2D_Fill(uint32_t LayerIndex, void * pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex)
{
  uint32_t PixelFormat;

  PixelFormat = GetPixelformat(LayerIndex);
  DMA2D->CR      = 0x00030000UL | (1 << 9);              /* Register to memory and TCIE                   */
  DMA2D->OCOLR   = ColorIndex;                           /* Color to be used                              */
  DMA2D->OMAR    = (uint32_t)pDst;                       /* Destination address                           */
  DMA2D->OOR     = OffLine;                              /* Destination line offset                       */
  DMA2D->OPFCCR  = PixelFormat;                          /* Defines the number of pixels to be transfered */
  DMA2D->NLR     = (uint32_t)(xSize << 16) | (U16)ySize; /* Size configuration of area to be transfered   */
  DMA2D->CR     |= DMA2D_CR_START;                       /* Start operation                               */
  
  while (DMA2D->CR & DMA2D_CR_START) 
  {
  }
}

/**
  * @brief  LCD Low level initialization 
  * @param  LayerIndex: Layer index
  * @retval None
  */
static void LCD_LL_Init(uint32_t LayerIndex) 
{
  uint32_t xSize, ySize, BytesPerLine, BitsPerPixel, i;
  uint32_t Color;
  static uint32_t Is_Initialized;
  
  LTDC_Layer_InitTypeDef LTDC_Layer_Init;
  LTDC_CLUT_InitTypeDef  LTDC_CLUT_InitStruct;
  
  if (LayerIndex >= GUI_NUM_LAYERS) 
  {
    return;
  }
  if (Is_Initialized == 0) 
  {
		Is_Initialized = 1;
    
		//初始化LCD
		LCD_Init();
		LTDC_ITConfig(LTDC_IT_LI, ENABLE);
		NVIC_SetPriority(LTDC_IRQn, 0);
		NVIC_EnableIRQ(LTDC_IRQn); 
  }   
  xSize = LCD_GetXSizeEx(LayerIndex);
  ySize = LCD_GetYSizeEx(LayerIndex);
  
  BytesPerLine = GetBytesPerLine(LayerIndex, xSize);
  
  LTDC_Layer_Init.LTDC_HorizontalStart = HBP + 1;
  LTDC_Layer_Init.LTDC_HorizontalStop = (xSize + HBP);
  LTDC_Layer_Init.LTDC_VerticalStart = VBP + 1;
  LTDC_Layer_Init.LTDC_VerticalStop = (ySize + VBP);
  LTDC_Layer_Init.LTDC_PixelFormat = GetPixelformat(LayerIndex);
  LTDC_Layer_Init.LTDC_ConstantAlpha = 255; 
  LTDC_Layer_Init.LTDC_DefaultColorBlue = 0;        
  LTDC_Layer_Init.LTDC_DefaultColorGreen = 0;       
  LTDC_Layer_Init.LTDC_DefaultColorRed = 0;         
  LTDC_Layer_Init.LTDC_DefaultColorAlpha = 0;
#if (GUI_NUM_LAYERS > 1)
  LTDC_Layer_Init.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;
  LTDC_Layer_Init.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
#else  
  LTDC_Layer_Init.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;
  LTDC_Layer_Init.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;
#endif
  LTDC_Layer_Init.LTDC_CFBLineLength = BytesPerLine + 3;
  LTDC_Layer_Init.LTDC_CFBPitch = BytesPerLine;
  LTDC_Layer_Init.LTDC_CFBLineNumber = ySize; 
  LTDC_Layer_Init.LTDC_CFBStartAdress = layer_address[LayerIndex];
  
  LTDC_LayerInit((LayerIndex == 0 ? LTDC_Layer1 : LTDC_Layer2), &LTDC_Layer_Init);
  /*  Enable LUT on demand */
  BitsPerPixel = LCD_GetBitsPerPixelEx(LayerIndex);
  if (BitsPerPixel <= 8) 
  {
    /* Enable usage of LUT for all modes with <= 8bpp */
    LTDC_CLUTCmd((LayerIndex == 0 ? LTDC_Layer1 : LTDC_Layer2), ENABLE);
  } 
  else 
  {
    /*  Optional CLUT initialization for AL88 mode (16bpp) */
    if (apColorConvAPI[LayerIndex] == GUICC_88666I) 
    {
      LTDC_CLUTCmd((LayerIndex == 0 ? LTDC_Layer1 : LTDC_Layer2), ENABLE);
      for (i = 0; i < 256; i++) 
      {
        Color = LCD_API_ColorConv_8666.pfIndex2Color(i);
        LTDC_CLUT_InitStruct.LTDC_BlueValue   =  ((Color >> 16) & 0xff);
        LTDC_CLUT_InitStruct.LTDC_GreenValue  =  ((Color >>  8) & 0xff);
        LTDC_CLUT_InitStruct.LTDC_RedValue    =   (Color        & 0xff);
        LTDC_CLUT_InitStruct.LTDC_CLUTAdress  =  i << 24; 
        LTDC_CLUTInit((LayerIndex == 0 ? LTDC_Layer1 : LTDC_Layer2), &LTDC_CLUT_InitStruct);
      }
    }
  }
  
  /*  Enable layer */
  LTDC_LayerCmd((LayerIndex == 0 ? LTDC_Layer1 : LTDC_Layer2), ENABLE);
  /* Reload configuration */
  LTDC_ReloadConfig(LTDC_VBReload);
  LTDC_DitherCmd(ENABLE);	//使能颜色抖动，不然无法正常显示24位色
}

/**
  * @brief  Returns the buffer size of the selected layer
  * @param  LayerIndex: Layer index
  * @retval Buffer size
  */
static uint32_t GetBufferSize(uint32_t LayerIndex) 
{
  uint32_t BufferSize;

  BufferSize = layer_xsize[LayerIndex] * layer_ysize[LayerIndex] * layer_bpp[LayerIndex];
  return BufferSize;
}

/**
  * @brief  
  * @param  pSrc
  * @param  pDst
  * @param  OffSrc
  * @param  OffDst
  * @param  PixelFormatDst
  * @param  xSize
  * @param  ySize
  * @retval None
  */
static void DMA_DrawBitmapL8(void * pSrc, void * pDst,  uint32_t OffSrc, uint32_t OffDst, uint32_t PixelFormatDst, uint32_t xSize, uint32_t ySize)
{
  /* Set up mode */
  DMA2D->CR      = 0x00010000UL | (1 << 9);         /* Control Register (Memory to memory with pixel format conversion and TCIE) */
  
  /* Set up pointers */
  DMA2D->FGMAR   = (uint32_t)pSrc;                  /* Foreground Memory Address Register (Source address) */
  DMA2D->OMAR    = (uint32_t)pDst;                  /* Output Memory Address Register (Destination address) */
  
  /* Set up offsets */
  DMA2D->FGOR    = OffSrc;                          /* Foreground Offset Register (Source line offset) */
  DMA2D->OOR     = OffDst;                          /* Output Offset Register (Destination line offset) */
  
  /* Set up pixel format */
  DMA2D->FGPFCCR = LTDC_Pixelformat_L8;             /* Foreground PFC Control Register (Defines the input pixel format) */
  DMA2D->OPFCCR  = PixelFormatDst;                  /* Output PFC Control Register (Defines the output pixel format) */
  
  /* Set up size */
  DMA2D->NLR     = (uint32_t)(xSize << 16) | ySize; /* Number of Line Register (Size configuration of area to be transfered) */
  
  /* Execute operation */
  DMA2D->CR     |= DMA2D_CR_START;                  /* Start operation */
  
  /* Wait until transfer is done */
  while (DMA2D->CR & DMA2D_CR_START)
  {
  }
}

/**
  * @brief  Copy a memory area content to another memory area
  * @param  LayerIndex: Layer index
  * @param  IndexSrc: Source buffer index
  * @param  IndexDst: Destination buffer index
  * @retval None
  */
static void CUSTOM_LCD_CopyBuffer(uint32_t LayerIndex, uint32_t IndexSrc, uint32_t IndexDst) 
{
  uint32_t BufferSize, AddrSrc, AddrDst;

  BufferSize = GetBufferSize(LayerIndex);
  AddrSrc    = layer_address[LayerIndex] + BufferSize * IndexSrc;
  AddrDst    = layer_address[LayerIndex] + BufferSize * IndexDst;
  DMA2D_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, layer_xsize[LayerIndex], layer_ysize[LayerIndex], 0, 0);
  layer_buffer_index[LayerIndex] = IndexDst;
}

/**
  * @brief  Copy a rectangular memory area content to another memory area
  * @param  LayerIndex: Layer index
  * @param  x0: x coordinate of the source buffer index
  * @param  y0: y coordinate of the source buffer index
  * @param  x1: x coordinate of the destination buffer index
  * @param  y1: y coordinate of the destination buffer index
  * @param  xSize: width of the area to be transfered
  * @param  ySize: height of the area to be transfered
  * @retval None
  */
static void CUSTOM_LCD_CopyRect(uint32_t LayerIndex, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t xSize, uint32_t ySize) 
{
  uint32_t BufferSize, AddrSrc, AddrDst;

  BufferSize = GetBufferSize(LayerIndex);
  AddrSrc = layer_address[LayerIndex] + BufferSize * layer_buffer_index[LayerIndex] + (y0 * layer_xsize[LayerIndex] + x0) * layer_bpp[LayerIndex];
  AddrDst = layer_address[LayerIndex] + BufferSize * layer_buffer_index[LayerIndex] + (y1 * layer_xsize[LayerIndex] + x1) * layer_bpp[LayerIndex];
  DMA2D_Copy(LayerIndex, (void *)AddrSrc, (void *)AddrDst, xSize, ySize, layer_xsize[LayerIndex] - xSize, 0);
}

/**
  * @brief  Fill a pixel index into a memory area
  * @param  LayerIndex: Layer index
  * @param  x0: x coordinate of the source buffer index
  * @param  y0: y coordinate of the source buffer index
  * @param  x1: x coordinate of the destination buffer index
  * @param  y1: y coordinate of the destination buffer index
  * @param  PixelIndex: Pixel Index
  * @retval None
  */
static void CUSTOM_LCD_FillRect(uint32_t LayerIndex, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, uint32_t PixelIndex) 
{
  uint32_t BufferSize, AddrDst;
  uint32_t xSize, ySize;

  if (GUI_GetDrawMode() == GUI_DM_XOR) 
  {
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, NULL);
    LCD_FillRect(x0, y0, x1, y1);
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, (void(*)(void))CUSTOM_LCD_FillRect);
  } 
  else 
  {
    xSize = x1 - x0 + 1;
    ySize = y1 - y0 + 1;
    BufferSize = GetBufferSize(LayerIndex);
    AddrDst = layer_address[LayerIndex] + BufferSize * layer_buffer_index[LayerIndex] + (y0 * layer_xsize[LayerIndex] + x0) * layer_bpp[LayerIndex];
    DMA2D_Fill(LayerIndex, (void *)AddrDst, xSize, ySize, layer_xsize[LayerIndex] - xSize, PixelIndex);
  }
}

/**
  * @brief  
  * @param  LayerIndex
  * @param  x
  * @param  y
  * @param  p
  * @param  xSize
  * @param  ySize
  * @param  BytesPerLine
  * @retval None
  */

static void LCD_DrawBitmap32bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine)
{
  uint32_t BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;

  BufferSize = GetBufferSize(LayerIndex);
  AddrDst = layer_address[LayerIndex] + BufferSize * layer_buffer_index[LayerIndex] + (y * layer_xsize[LayerIndex] + x) * layer_bpp[LayerIndex];
  OffLineSrc = (BytesPerLine / 4) - xSize;
  OffLineDst = layer_xsize[LayerIndex] - xSize;
  DMA2D_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/**
  * @brief  
  * @param  LayerIndex
  * @param  x
  * @param  y
  * @param  p
  * @param  xSize
  * @param  ySize
  * @param  BytesPerLine
  * @retval None
  */
static void LCD_DrawBitmap16bpp(int LayerIndex, int x, int y, U16 const * p, int xSize, int ySize, int BytesPerLine)
{
  uint32_t BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;

  BufferSize = GetBufferSize(LayerIndex);
  AddrDst = layer_address[LayerIndex] + BufferSize * layer_buffer_index[LayerIndex] + (y * layer_xsize[LayerIndex] + x) * layer_bpp[LayerIndex];
  OffLineSrc = (BytesPerLine / 2) - xSize;
  OffLineDst = layer_xsize[LayerIndex] - xSize;
  DMA2D_Copy(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}

/**
  * @brief  
  * @param  LayerIndex
  * @param  x
  * @param  y
  * @param  p
  * @param  xSize
  * @param  ySize
  * @param  BytesPerLine
  * @retval None
  */
static void LCD_DrawBitmap8bpp(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine)
{
  uint32_t BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;
  uint32_t PixelFormat;

  BufferSize = GetBufferSize(LayerIndex);
  AddrDst = layer_address[LayerIndex] + BufferSize * layer_buffer_index[LayerIndex] + (y * layer_xsize[LayerIndex] + x) * layer_bpp[LayerIndex];
  OffLineSrc = BytesPerLine - xSize;
  OffLineDst = layer_xsize[LayerIndex] - xSize;
  PixelFormat = GetPixelformat(LayerIndex);
  DMA_DrawBitmapL8((void *)p, (void *)AddrDst, OffLineSrc, OffLineDst, PixelFormat, xSize, ySize);
}

/**
  * @brief  Transfer complete interrupt of DMA2D
  * @param  None 
  * @retval None
  */
void DMA2D_ISR_Handler(void)
{
  DMA2D->IFCR = (U32)DMA2D_IFSR_CTCIF;
}

/**
  * @brief  End-Of-Frame-Interrupt for managing multiple buffering
  * @param  None 
  * @retval None
  */
void LTDC_ISR_Handler(void) 
{
  uint32_t Addr;
  uint32_t i;

  LTDC->ICR = (uint32_t)LTDC_IER_LIE;
  for (i = 0; i < GUI_NUM_LAYERS; i++) 
  {
    if (layer_pending_buffer[i] >= 0) 
    {
      Addr = layer_address[i] + layer_xsize[i] * layer_ysize[i] * layer_pending_buffer[i] * layer_bpp[i];
	  
      LTDC_LayerAddress((i == 0 ? LTDC_Layer1 : LTDC_Layer2), Addr);
      LTDC_ReloadConfig(LTDC_IMReload);
	  
      GUI_MULTIBUF_ConfirmEx(i, layer_pending_buffer[i]);
      

      layer_pending_buffer[i] = -1;
    }
  }
}

/**
  * @brief  This function is called by the display driver for several purposes.
  *         To support the according task the routine needs to be adapted to
  *         the display controller.
  * @param  LayerIndex: Index of layer to be configured
  * @param  Cmd: Please refer to the details in the switch statement below
  * @param  pData: Pointer to a LCD_X_DATA structure
  * @retval  < -1: Error
  *          -1  : Command not handled
  *          0   : Ok
  */
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData)
{
  int r = 0;
  int xPos, yPos;
  
  switch (Cmd) 
  {
  case LCD_X_INITCONTROLLER: 
    {
      /* Called during the initialization process in order to set up the display controller and put it into operation */
      LCD_LL_Init(LayerIndex);
      break;
    }
  case LCD_X_SETORG: 
    {
      /* Required for setting the display origin which is passed in the 'xPos' and 'yPos' element of p */
      LCD_X_SETORG_INFO * p;
     	if(pData==NULL)
			{
				rt_kprintf("pdata -------NULL\n");
			}
      p = (LCD_X_SETORG_INFO *)pData;
      apLayer[LayerIndex]->CFBAR = layer_address[LayerIndex] + p->yPos * layer_xsize[LayerIndex] * layer_bpp[LayerIndex];
      LTDC_ReloadConfig(LTDC_SRCR_VBR); // Reload on next blanking period
			break;
    }
  case LCD_X_SHOWBUFFER: 
    {
      /* Required if multiple buffers are used. The 'Index' element of p contains the buffer index */
      LCD_X_SHOWBUFFER_INFO * p;    
      p = (LCD_X_SHOWBUFFER_INFO *)pData;
      layer_pending_buffer[LayerIndex] = p->Index;
      break;
    }
  case LCD_X_SETLUTENTRY: 
    {
      /* Required for setting a lookup table entry which is passed in the 'Pos' and 'Color' element of p */
      LCD_X_SETLUTENTRY_INFO * p;
      p = (LCD_X_SETLUTENTRY_INFO *)pData;
      LTDC_SetLUTEntry(LayerIndex, p->Color, p->Pos);
			break;
    }
  case LCD_X_ON: 
    {
      /* Required if the display controller should support switching on and off */
      LTDC_Cmd(ENABLE); /* display ON */
      break;
    }
  case LCD_X_OFF: 
    {
      /* Required if the display controller should support switching on and off */
      LTDC_Cmd(DISABLE); /* display OFF */
      break;
    }
  case LCD_X_SETVIS: 
    {
      /* Required for setting the layer visibility which is passed in the 'OnOff' element of pData */
      LCD_X_SETVIS_INFO * p;
      p = (LCD_X_SETVIS_INFO *)pData;
      LTDC_LayerCmd(apLayer[LayerIndex], p->OnOff ? ENABLE : DISABLE);
      /* Reload shadow register */
      LTDC_ReloadConfig(LTDC_SRCR_IMR);
      break;
    }
  case LCD_X_SETPOS: 
    {
      LCD_X_SETPOS_INFO * p;
      p = (LCD_X_SETPOS_INFO *)pData;
      LTDC_SetLayerPos(LayerIndex, p->xPos, p->yPos);
			break;
    }
  case LCD_X_SETSIZE: 
    {
      /* Required for setting the layer position which is passed in the 'xPos' and 'yPos' element of pData */
      LCD_X_SETSIZE_INFO * p;    
      GUI_GetLayerPosEx(LayerIndex, &xPos, &yPos);
      p = (LCD_X_SETSIZE_INFO *)pData;
      layer_xsize[LayerIndex] = p->xSize;
      layer_ysize[LayerIndex] = p->ySize;
      LTDC_SetLayerPos(LayerIndex, xPos, yPos);
      break;
    }
  case LCD_X_SETALPHA: 
    {
      /* Required for setting the alpha value which is passed in the 'Alpha' element of pData */
      LCD_X_SETALPHA_INFO * p;
      
      p = (LCD_X_SETALPHA_INFO *)pData;
      LTDC_SetLayerAlpha(LayerIndex, p->Alpha);
      break;
    }
  case LCD_X_SETCHROMAMODE: 
    {
      /* Required for setting the chroma mode which is passed in the 'ChromaMode' element of pData */
      LCD_X_SETCHROMAMODE_INFO * p;
      
      p = (LCD_X_SETCHROMAMODE_INFO *)pData;
      LTDC_LayerEnableColorKeying(apLayer[LayerIndex], (p->ChromaMode != 0) ? ENABLE : DISABLE);
			break;
    }
  case LCD_X_SETCHROMA: 
    {
      /* Required for setting the chroma value which is passed in the 'ChromaMin' and 'ChromaMax' element of pData */
      LCD_X_SETCHROMA_INFO * p;
      U32 Color;
      p = (LCD_X_SETCHROMA_INFO *)pData;
      Color = ((p->ChromaMin & 0xFF0000) >> 16) | (p->ChromaMin & 0x00FF00) | ((p->ChromaMin & 0x0000FF) << 16);
      apLayer[LayerIndex]->CKCR = Color;
      LTDC_ReloadConfig(LTDC_SRCR_VBR); // Reload on next blanking period	
      break;
    }
  default:
    r = -1;
  }
  return r;
	
}

/**
  * @brief  Called during the initialization process in order to set up the
  *         display driver configuration.
  * @param  None
  * @retval None
  */
void LCD_X_Config(void) 
{
  uint32_t i;
  
  /* At first initialize use of multiple buffers on demand */
#if (NUM_BUFFERS > 1)
  for (i = 0; i < GUI_NUM_LAYERS; i++) {
    GUI_MULTIBUF_ConfigEx(i, NUM_BUFFERS);
  }
#endif
  
  /* Set display driver and color conversion for 1st layer */
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_0, COLOR_CONVERSION_0, 0, 0);
  
  /* Set size of 1st layer */
  if (LCD_GetSwapXYEx(0)) {
    LCD_SetSizeEx (0, YSIZE_0, XSIZE_0);
    LCD_SetVSizeEx(0, YSIZE_0 * NUM_VSCREENS, XSIZE_0);
  } else {
    LCD_SetSizeEx (0, XSIZE_0, YSIZE_0);
    LCD_SetVSizeEx(0, XSIZE_0, YSIZE_0 * NUM_VSCREENS);
  }
  
#if (GUI_NUM_LAYERS > 1)
  /* Set display driver and color conversion for 2nd layer */
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_1, COLOR_CONVERSION_1, 0, 1);
  
  /* Set size of 2nd layer */
  if (LCD_GetSwapXYEx(0)) {
    LCD_SetSizeEx (1, YSIZE_1, XSIZE_1);
    LCD_SetVSizeEx(1, YSIZE_1 * NUM_VSCREENS, XSIZE_1);
  } else {                  
    LCD_SetSizeEx (1, XSIZE_1, YSIZE_1);
    LCD_SetVSizeEx(1, XSIZE_1, YSIZE_1 * NUM_VSCREENS);
  }
#endif

  /*Initialize GUI Layer structure */
  layer_address[0] = LCD_LAYER0_FRAME_BUFFER;
#if (GUI_NUM_LAYERS > 1)
  layer_address[1] = LCD_LAYER1_FRAME_BUFFER;     
#endif
  
  /* Setting up VRam address and custom functions for CopyBuffer-, CopyRect- and FillRect operations */
  for (i = 0; i < GUI_NUM_LAYERS; i++) {
    layer_pending_buffer[i] = -1;
    
    /* Set VRAM address */
    LCD_SetVRAMAddrEx(i, (void *)(layer_address[i]));
    
    /* Remember color depth for further operations */
    layer_bpp[i] = LCD_GetBitsPerPixelEx(i) >> 3;
    
    /* Set custom functions for several operations */
    LCD_SetDevFunc(i, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))CUSTOM_LCD_CopyBuffer);
    LCD_SetDevFunc(i, LCD_DEVFUNC_COPYRECT,   (void(*)(void))CUSTOM_LCD_CopyRect);
    
    /* Filling via DMA2D does only work with 16bpp or more */
    if (GetPixelformat(i) <= LTDC_Pixelformat_ARGB4444) {
      LCD_SetDevFunc(i, LCD_DEVFUNC_FILLRECT, (void(*)(void))CUSTOM_LCD_FillRect);
      LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_8BPP, (void(*)(void))LCD_DrawBitmap8bpp); 
    }
    
    /* Set up drawing routine for 16bpp bitmap using DMA2D */
    if (GetPixelformat(i) == LTDC_Pixelformat_RGB565) {
      LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_16BPP, (void(*)(void))LCD_DrawBitmap16bpp);     /* Set up drawing routine for 16bpp bitmap using DMA2D. Makes only sense with RGB565 */
    }
	 
     /* Set up drawing routine for 32bpp bitmap using DMA2D */
    if (GetPixelformat(i) == LTDC_Pixelformat_ARGB8888) {
      LCD_SetDevFunc(i, 0x28, (void(*)(void))LCD_DrawBitmap32bpp);     /* Set up drawing routine for 32bpp bitmap using DMA2D. Makes only sense with RGB565 */
    }  
	 
    /* Set up custom color conversion using DMA2D, works only for direct color modes because of missing LUT for DMA2D destination */
    GUICC_M1555I_SetCustColorConv(Color2IndexBulk_M1555IDMA2D, _Index2ColorBulk_M1555IDMA2D); /* Set up custom bulk color conversion using DMA2D for ARGB1555 */
    GUICC_M565_SetCustColorConv  (Color2IndexBulk_M565DMA2D,   _Index2ColorBulk_M565DMA2D);   /* Set up custom bulk color conversion using DMA2D for RGB565 */
    GUICC_M4444I_SetCustColorConv(Color2IndexBulk_M4444IDMA2D, _Index2ColorBulk_M4444IDMA2D); /* Set up custom bulk color conversion using DMA2D for ARGB4444 */
    GUICC_M888_SetCustColorConv  (Color2IndexBulk_M888DMA2D,   _Index2ColorBulk_M888DMA2D);   /* Set up custom bulk color conversion using DMA2D for RGB888 */
    GUICC_M8888I_SetCustColorConv(Color2IndexBulk_M8888IDMA2D, _Index2ColorBulk_M8888IDMA2D); /* Set up custom bulk color conversion using DMA2D for ARGB8888 */
    
    /* Set up custom alpha blending function using DMA2D */
    GUI_SetFuncAlphaBlending(DMA_AlphaBlending);                                                /* Set up custom alpha blending function using DMA2D */
    
    /* Set up custom function for translating a bitmap palette into index values.
    Required to load a bitmap palette into DMA2D CLUT in case of a 8bpp indexed bitmap */
    GUI_SetFuncGetpPalConvTable(_LCD_GetpPalConvTable);
    
    /* Set up a custom function for mixing up single colors using DMA2D */
    GUI_SetFuncMixColors(DMA_MixColors);
    
    /* Set up a custom function for mixing up arrays of colors using DMA2D */
    GUI_SetFuncMixColorsBulk(LCD_MixColorsBulk);
  }
}

/*************************** End of file ****************************/
