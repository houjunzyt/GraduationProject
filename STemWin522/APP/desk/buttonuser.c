/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter for emWin (Demo version) V5.46.            *
*        Compiled Dec 12 2017, 15:43:56                              *
*                                                                    *
*        (c) 1998 - 2017 Segger Microcontroller GmbH & Co. KG        *
*                                                                    *
*        May not be used in a product                                *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: button                                                *
* Dimensions:  31 * 20                                               *
* NumColors:   16bpp: 65536                                          *
*                                                                    *
**********************************************************************
*/

#include <stdlib.h>

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

static GUI_CONST_STORAGE unsigned short _acbutton1[] = {
  0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xD616, 0xDE57, 0xC572, 0xB4CE, 0xAC8D, 0xB4CF, 0xC593, 0xDE78, 0xD616, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5,
  0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xACB0, 0xACD0, 0x93AA, 0xA42C, 0xCDD4, 0xE6B8, 0xE6D9, 0xE6B8, 0xC5B3, 0xA40A, 0x93AA, 0xB4F1, 0xA48F, 0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xA46F,
  0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xA48F, 0xB4F1, 0xA44D, 0xAC8D, 0xDED9, 0xDEB9, 0xD657, 0xCE16, 0xCE16, 0xCE16, 0xD657, 0xDEB9, 0xDEB8, 0xA42B, 0xAC8F, 0xACD1, 0xA48F, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xAC90, 0xACD1, 0xA44D, 0xBD52, 0xD657, 0xC5B3, 0xC571, 0xC550, 0xC550, 0xC551, 0xBD52, 0xBD52, 0xBD52, 0xC5B3, 0xD657, 0xB4EF, 0xAC8F, 0xACB0, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xACB0, 0xAC90, 0xACB0, 0xACAF, 0xB4F0, 0xBD93, 0xB4EF, 0xA48E, 0x6B31, 0x4293, 0x4293, 0x7370, 0xAC8F, 0xB4CF, 0xB4CF, 0xB4CF, 0xB4F0, 0xC5D4, 0xAC8E, 0xACD1, 0xACB0, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xACB0, 0xA48F, 0xB512, 0xA42C, 0xB512, 0xA44D, 0xA44C, 0x62F0, 0x1179, 0x1A79, 0x22DA, 0x1ADB, 0x738F, 0x944C, 0xA40B, 0xA42C, 0x9C0C, 0x940C, 0xB512, 0xA42C, 0xB4F1, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xAC90, 0xACB0, 0xAC8F, 0xA4AF, 0x93EB, 0x93AA, 0x9BC8, 0x3A74, 0x22FA, 0x2BFB, 0x2C9C, 0x2C7D, 0x7C2C, 0x4613, 0x5C8D, 0x63EA, 0x4BC7, 0x53E9, 0x93CC, 0xA44D, 0xACD0, 0xAC90, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xAC90, 0xACD1, 0xAC8E, 0x93CC, 0x8328, 0x9367, 0x8328, 0x2AD8, 0x2C1C, 0x2D3D, 0x2DDE, 0x44B8, 0x850E, 0x6E96, 0x4613, 0x454E, 0x3CA9, 0x6368, 0x8328, 0x940C, 0xACAF, 0xACD0, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xA48F, 0xB512, 0x93CA, 0x8BAA, 0x8306, 0x9345, 0x62CB, 0x12FD, 0x33B7, 0x2C59, 0x1E9F, 0x5BEF, 0x9676, 0x86B7, 0x5E35, 0x4591, 0x44EC, 0x7AE6, 0x8306, 0x93EB, 0x9BEB, 0xB4F1, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xA48F, 0xBD53, 0x82C5, 0x8328, 0x7A83, 0x8281, 0x7263, 0x9B42, 0xC446, 0xCC88, 0x7AC7, 0x6B06, 0xAFFE, 0x96F9, 0x6E96, 0x4613, 0x4CAB, 0x7A40, 0x7A83, 0x8328, 0x8B27, 0xB512, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xA48F, 0xBD53, 0x7221, 0x5980, 0x7220, 0x7A40, 0xBBE2, 0xD505, 0xE5CF, 0xF6F7, 0xFF58, 0x8345, 0x7364, 0x854F, 0x6D2F, 0x5C28, 0x6AC2, 0x8280, 0x6A00, 0x5160, 0x82C5, 0xB512, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xA48F, 0xB533, 0x82C4, 0x61C0, 0x7A40, 0x82A1, 0xBBE3, 0xCCA3, 0xDD4C, 0xEE73, 0xBCEC, 0x8E18, 0xA71E, 0x7CF2, 0x64B1, 0x2D3C, 0x7B88, 0x8AA0, 0x7220, 0x61C0, 0x9367, 0xB512, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xAC90, 0xB4F1, 0xA40A, 0x61C0, 0x7A40, 0x8AE2, 0xBBE2, 0xC442, 0xD4E6, 0xDD8E, 0xA3E6, 0x7EFF, 0x86FF, 0x66DF, 0x2EBF, 0x257E, 0x9B01, 0x92E0, 0x7240, 0x69C0, 0xAC6D, 0xACB1, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xAC90, 0xACB0, 0xB4F0, 0x7A40, 0x7220, 0xA363, 0xAB40, 0xABA2, 0xBC01, 0xD4A4, 0x742B, 0x5EFF, 0x4E9F, 0x367F, 0x25DF, 0x4475, 0xB360, 0x92E0, 0x7240, 0x7A40, 0xB511, 0xACB0, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xACB0, 0xA48F, 0xB533, 0x9365, 0x7A40, 0x8AE1, 0xA362, 0xBC01, 0xBC01, 0xBBC0, 0x4D15, 0x1E5F, 0x25DF, 0x259F, 0x0D1F, 0x73EC, 0xBBA0, 0x9301, 0x7A40, 0xA3E9, 0xB512, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xACB0, 0xAC90, 0xACB0, 0xB4F1, 0x92E0, 0x9B01, 0xB3C1, 0xCC82, 0xD4C2, 0xDD01, 0xBCA3, 0x6490, 0x3CD7, 0x3C96, 0x5C0F, 0xB402, 0xB3C1, 0x92E0, 0x9302, 0xB512, 0xACB0, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xAC90, 0xB4F1, 0xAC8E, 0xA361, 0xBBE2, 0xCC82, 0xDCE2, 0xE5A1, 0xF600, 0xF600, 0xEDA0, 0xE540, 0xDCC0, 0xCC61, 0xB3C1, 0xA361, 0xB4F0, 0xACB1, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xA48F, 0xACF2, 0xACAF, 0xABE2, 0xD4C1, 0xE541, 0xEE21, 0xF681, 0xF6C0, 0xF681, 0xEE21, 0xE541, 0xD4C1, 0xA3C3, 0xACD1, 0xACD1, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xA48F, 0xACD1, 0xB513, 0xA429, 0xB441, 0xE5E0, 0xFEE0, 0xFF40, 0xFEE0, 0xE5C0, 0xB421, 0xA44B, 0xB513, 0xACB0, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0,
  0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xAC90, 0xA48F, 0xACF2, 0xB513, 0xA470, 0x9C2D, 0xA42C, 0xA42E, 0xA470, 0xB513, 0xACD1, 0xA48F, 0xAC90, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0, 0xACB0
};

GUI_CONST_STORAGE GUI_BITMAP bmbuttonuser1 = {
  31, // xSize
  20, // ySize
  62, // BytesPerLine
  16, // BitsPerPixel
  (unsigned char *)_acbutton1,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMP565
};

static GUI_CONST_STORAGE unsigned short _acbutton2[] = {
  0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xC5B5, 0xBD96, 0xBD96, 0xB576, 0xB576, 0xBDD8, 0xBD96, 0xACD1, 0xAC90, 0xACCF, 0xB510, 0xCE16, 0xD657, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5, 0xCDD5,
  0xA46F, 0xA46F, 0xA46F, 0xA46F, 0x9C70, 0x9C70, 0x9451, 0x8C32, 0x9474, 0x8C11, 0x8C10, 0xA514, 0xCE5A, 0xD6BB, 0xDEFB, 0xD6D9, 0xB531, 0x9C4B, 0x9C6F, 0xA511, 0x9CCF, 0x9CCF, 0x9CCF, 0xA48F, 0xA48F, 0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xA46F, 0xA46F,
  0xAC90, 0xAC90, 0xAC90, 0xA4B0, 0x9C91, 0x9C71, 0x9472, 0x9494, 0x83F1, 0xADB9, 0xB63C, 0xB63C, 0xBE1A, 0xBE1A, 0xC659, 0xC6D8, 0xD73A, 0xD73A, 0xB635, 0x946C, 0xA511, 0x9CCF, 0x9CCF, 0xA4D0, 0x9CCF, 0xA48F, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xAC90, 0xA4B0, 0xA4B0, 0x9C91, 0x9C71, 0x94B2, 0x8C52, 0xA59C, 0x955A, 0x955A, 0x955A, 0x955A, 0xADB9, 0xADB9, 0xB656, 0xB656, 0xB656, 0xBED7, 0xBED7, 0x94CD, 0x9550, 0x8D2E, 0x8D2E, 0x8D2E, 0x9CCF, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xAC90, 0xA4B0, 0xA4B0, 0x9C91, 0xA4D2, 0x9450, 0xADB9, 0x8D19, 0x749A, 0x643B, 0x4B5B, 0x3ADA, 0x539A, 0x84F9, 0xB635, 0xA633, 0xA633, 0xA633, 0xA675, 0xAEB5, 0x7D4D, 0x6DAD, 0x7D4D, 0x8D2E, 0x9CCF, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xAC90, 0xA4B0, 0xA4B0, 0x9C91, 0x9C91, 0xAD35, 0x9D36, 0x84B7, 0x643B, 0x2A1A, 0x223B, 0x231C, 0x231C, 0x335D, 0x9DD1, 0x9631, 0x9631, 0x8E71, 0x764F, 0x6ECF, 0x5EAD, 0x6DAD, 0x8D2E, 0x9CCF, 0x9CCF, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xAC90, 0xA4B0, 0xA4B0, 0xA4D2, 0x9C6F, 0xA554, 0x94B2, 0x8495, 0x6419, 0x22DC, 0x2BBD, 0x34BE, 0x34DE, 0x5C79, 0x5611, 0x4DD0, 0x55AD, 0x454A, 0x3C67, 0x464B, 0x6ECF, 0x7D4D, 0x9550, 0x9CCF, 0xA4D0, 0xA4D0, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xACD1, 0xACD0, 0x9CD0, 0x9CD0, 0x8CD3, 0x53B9, 0x2BBD, 0x34DE, 0x35BE, 0x2E3F, 0x6C74, 0x6EF5, 0x4E74, 0x3D8F, 0x3CEA, 0x3CA8, 0x4E8B, 0x75CE, 0x8D6F, 0x9550, 0x9CCF, 0xA4D0, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xAC90, 0xACAF, 0xAC90, 0xACAE, 0xB510, 0xACAD, 0xACAD, 0x9CD0, 0x333B, 0x2BBD, 0x3C5A, 0x257E, 0x2EBF, 0x952F, 0x8F38, 0x6E95, 0x4E53, 0x3D4C, 0x454A, 0x6E2C, 0x7D4D, 0x8D6F, 0x952F, 0x9CD0, 0xA4D0, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xACAF, 0xACAF, 0xACCF, 0xB4CD, 0xA48A, 0xACA9, 0xB4A4, 0xB505, 0xB4A7, 0xD4E4, 0xDDA9, 0xCD68, 0x842A, 0x96B3, 0x977B, 0x7ED7, 0x4E74, 0x3D8F, 0x55A8, 0x6D68, 0x850C, 0x848A, 0x952F, 0xA4D0, 0xA4D0, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xACAF, 0xACAF, 0xACCF, 0xACCE, 0xB50C, 0x9360, 0xAC20, 0xC521, 0xD522, 0xDCA1, 0xE58A, 0xF672, 0xFF7A, 0xEE8B, 0x956B, 0x7D8C, 0x6DD0, 0x4D6D, 0x44A8, 0x6DA7, 0x7CC5, 0x73E3, 0x7320, 0xA511, 0xA4D0, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xACAF, 0xACAF, 0xACCE, 0xACCE, 0xB50C, 0xA3E0, 0xB481, 0xD581, 0xD4C3, 0xD481, 0xE547, 0xEE0F, 0xFED4, 0x9D6A, 0xB7FF, 0x9F1B, 0x6637, 0x4E5A, 0x45DB, 0x9D6A, 0x8C86, 0x83C4, 0x8362, 0xA511, 0xA4D0, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xACAF, 0xACCF, 0xACCE, 0xB4CD, 0xB52F, 0xB441, 0xB481, 0xD581, 0xCC22, 0xD422, 0xDCE3, 0xE58A, 0xEDAC, 0x8633, 0x977F, 0x7F5F, 0x4F3F, 0x2EBF, 0x6637, 0x9DAD, 0x94C9, 0x83C4, 0x93E5, 0xAD32, 0xA4B0, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xACAF, 0xACCF, 0xACCE, 0xB4CD, 0xB52F, 0xB4A4, 0xC4E0, 0xD562, 0xC3C2, 0xCC61, 0xD481, 0xD4C3, 0xC505, 0x5EBB, 0x5F3F, 0x4F3F, 0x2EBF, 0x1DFF, 0x8E76, 0x9DAE, 0x94C9, 0x8C04, 0x9C4A, 0xAD32, 0xA4B0, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xACAE, 0xACCE, 0xB4CD, 0xB50C, 0xC54B, 0xC4E0, 0xD581, 0xE621, 0xEEA2, 0xEE82, 0xE603, 0xBDC7, 0x1E7F, 0x2EBF, 0x2EBF, 0x1DFF, 0x257E, 0x96B4, 0x9DAE, 0x94C9, 0x9425, 0xAD32, 0x9CD1, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xACAF, 0xACAE, 0xACAD, 0xB4CB, 0xBD2B, 0xC505, 0xD581, 0xDDC3, 0xE603, 0xE625, 0xE665, 0xCEC9, 0x8E74, 0x45DB, 0x35DC, 0x45DB, 0x6679, 0x96B4, 0x9DAE, 0x94C9, 0x9CCF, 0xAD32, 0xA4D0, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xACAF, 0xACAE, 0xACAD, 0xB4CB, 0xB4CB, 0xBD6C, 0xB4A4, 0xD562, 0xDDC3, 0xE625, 0xDE66, 0xD6EA, 0xC72F, 0xB772, 0x9F54, 0x9715, 0x9715, 0x96B3, 0x9DAD, 0x94CD, 0xA554, 0x9CD1, 0xA4B0, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xAC90, 0xACAE, 0xACAD, 0xB4CD, 0xB4CD, 0xB4CD, 0xB52F, 0xB4A7, 0xD562, 0xE625, 0xDEA6, 0xDF09, 0xCF4C, 0xBF4E, 0xB751, 0xAF11, 0xA6B1, 0x9631, 0x8572, 0x9D95, 0x9CD1, 0x9CD1, 0xA4B0, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xAC90, 0xACAF, 0xACAE, 0xACAE, 0xACCE, 0xACCE, 0xACCE, 0xB531, 0xACCE, 0xB4A7, 0xC5C5, 0xD727, 0xD727, 0xCF69, 0xBF0C, 0xA60C, 0x8D6F, 0x8D95, 0x95B6, 0x9512, 0x9CD1, 0x9CD1, 0xA4B0, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90,
  0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xACAF, 0xACAF, 0xACAF, 0xACCF, 0xACAF, 0xACD0, 0xAD32, 0xA4D2, 0x9CD0, 0x9CD0, 0x9CD0, 0x9512, 0x9575, 0x9575, 0x9512, 0x9512, 0x9512, 0x9CD1, 0xA4D0, 0xA4B0, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90, 0xAC90
};

GUI_CONST_STORAGE GUI_BITMAP bmbuttonuser2 = {
  31, // xSize
  20, // ySize
  62, // BytesPerLine
  16, // BitsPerPixel
  (unsigned char *)_acbutton2,  // Pointer to picture data
  NULL,  // Pointer to palette
  GUI_DRAW_BMP565
};





/*************************** End of file ****************************/