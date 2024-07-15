/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        emWin Font Converter (ST) version 5.26                      *
*        Compiled Aug 14 2014, 10:10:08                              *
*                                                                    *
*        (c) 1998 - 2014 Segger Microcontroller GmbH & Co. KG        *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: 2t.c                                                  *
* Font:        ΢���ź�                                              *
* Height:      19                                                    *
*                                                                    *
**********************************************************************
*                                                                    *
* Initial font height:  19                                           *
* Range disabled:       0000 - FFFF                                  *
* Read pattern file: C:\Users\ray_bao\Desktop\1.txt                  *
* Range disabled:       0000 - FFFF                                  *
* Read pattern file: C:\Users\ray_bao\Desktop\1.txt                  *
*                                                                    *
**********************************************************************
*/

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/* The following line needs to be included in any file selecting the
   font.
*/
extern GUI_CONST_STORAGE GUI_FONT GUI_Font2t;

/* Start of unicode area <Basic Latin> */
GUI_CONST_STORAGE unsigned char acGUI_Font2t_0032[ 76] = { /* code 0032, DIGIT TWO */
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x01, 0xCF, 0xFC, 0x10,
  0x1B, 0x40, 0x0A, 0xC1,
  0x00, 0x00, 0x05, 0xE2,
  0x00, 0x00, 0x05, 0xC1,
  0x00, 0x00, 0x1C, 0x80,
  0x00, 0x00, 0xAC, 0x10,
  0x00, 0x0A, 0xC1, 0x00,
  0x00, 0xAC, 0x10, 0x00,
  0x0A, 0xA0, 0x00, 0x00,
  0x5F, 0xFF, 0xFF, 0xE2,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00
};

/* Start of unicode area <CJK Unified Ideographs> */
GUI_CONST_STORAGE unsigned char acGUI_Font2t_6B21[133] = { /* code 6B21 */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x04, 0xD2, 0x00, 0x00, 0x00,
  0x5A, 0x00, 0x08, 0x80, 0x00, 0x00, 0x00,
  0x0A, 0xA0, 0x1C, 0xFF, 0xFF, 0xFF, 0x40,
  0x00, 0xA4, 0x5A, 0x00, 0x00, 0x2C, 0x20,
  0x00, 0x02, 0xC2, 0x08, 0x80, 0x5A, 0x00,
  0x00, 0x55, 0x00, 0x0A, 0x80, 0xA5, 0x00,
  0x01, 0xB4, 0x00, 0x1C, 0xC1, 0x00, 0x00,
  0x05, 0xA0, 0x00, 0x4A, 0xA4, 0x00, 0x00,
  0x1A, 0x20, 0x01, 0xB4, 0x4B, 0x10, 0x00,
  0x58, 0x00, 0x1C, 0x80, 0x08, 0x80, 0x00,
  0x92, 0x04, 0xF5, 0x00, 0x00, 0xAA, 0x00,
  0x00, 0x8E, 0x20, 0x00, 0x00, 0x0A, 0xA0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

GUI_CONST_STORAGE GUI_CHARINFO GUI_Font2t_CharInfo[2] = {
   {   8,   8,  4, acGUI_Font2t_0032 } /* code 0032 */
  ,{  13,  13,  7, acGUI_Font2t_6B21 } /* code 6B21 */
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font2t_Prop2 = {
   0x6B21 /* first character */
  ,0x6B21 /* last character  */
  ,&GUI_Font2t_CharInfo[  1] /* address of first character */
  ,(GUI_CONST_STORAGE GUI_FONT_PROP *)0 /* pointer to next GUI_FONT_PROP */
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_Font2t_Prop1 = {
   0x0032 /* first character */
  ,0x0032 /* last character  */
  ,&GUI_Font2t_CharInfo[  0] /* address of first character */
  ,&GUI_Font2t_Prop2 /* pointer to next GUI_FONT_PROP */
};

GUI_CONST_STORAGE GUI_FONT GUI_Font2t = {
   GUI_FONTTYPE_PROP_AA4 /* type of font    */
  ,19 /* height of font  */
  ,19 /* space of font y */
  ,1 /* magnification x */
  ,1 /* magnification y */
  ,{&GUI_Font2t_Prop1}
  ,15 /* Baseline */
  ,7 /* Height of lowercase characters */
  ,10 /* Height of capital characters */
};

