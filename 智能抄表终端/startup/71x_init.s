;********************************************************************************
;* File Name          : 71x_init.s
;* Description        : This is the first code executed after RESET.
;*                      This code used to initialize system stacks
;*                      and critical peripherals before entering the C code.
;*******************************************************************************/

  DATA
      ORG 0X4000F000
FONT  
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x38,0xFC,0xFC,0x38,0x00,0x00
 DCB  0x00,0x00,0x00,0x0D,0x0D,0x00,0x00,0x00
 DCB  0x00,0x0E,0x1E,0x00,0x00,0x1E,0x0E,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x20,0xF8,0xF8,0x20,0xF8,0xF8,0x20,0x00
 DCB  0x02,0x0F,0x0F,0x02,0x0F,0x0F,0x02,0x00
 DCB  0x38,0x7C,0x44,0x47,0x47,0xCC,0x98,0x00
 DCB  0x03,0x06,0x04,0x1C,0x1C,0x07,0x03,0x00
 DCB  0x30,0x30,0x00,0x80,0xC0,0x60,0x30,0x00
 DCB  0x0C,0x06,0x03,0x01,0x00,0x0C,0x0C,0x00
 DCB  0x80,0xD8,0x7C,0xE4,0xBC,0xD8,0x40,0x00
 DCB  0x07,0x0F,0x08,0x08,0x07,0x0F,0x08,0x00
 DCB  0x00,0x10,0x1E,0x0E,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0xF0,0xF8,0x0C,0x04,0x00,0x00
 DCB  0x00,0x00,0x03,0x07,0x0C,0x08,0x00,0x00
 DCB  0x00,0x00,0x04,0x0C,0xF8,0xF0,0x00,0x00
 DCB  0x00,0x00,0x08,0x0C,0x07,0x03,0x00,0x00
 DCB  0x80,0xA0,0xE0,0xC0,0xC0,0xE0,0xA0,0x80
 DCB  0x00,0x02,0x03,0x01,0x01,0x03,0x02,0x00
 DCB  0x00,0x80,0x80,0xE0,0xE0,0x80,0x80,0x00
 DCB  0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x10,0x1E,0x0E,0x00,0x00,0x00
 DCB  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x0C,0x0C,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x80,0xC0,0x60,0x30,0x00
 DCB  0x0C,0x06,0x03,0x01,0x00,0x00,0x00,0x00
 DCB  0xF8,0xFC,0x04,0xC4,0x24,0xFC,0xF8,0x00
 DCB  0x07,0x0F,0x09,0x08,0x08,0x0F,0x07,0x00
 DCB  0x00,0x10,0x18,0xFC,0xFC,0x00,0x00,0x00
 DCB  0x00,0x08,0x08,0x0F,0x0F,0x08,0x08,0x00
 DCB  0x08,0x0C,0x84,0xC4,0x64,0x3C,0x18,0x00
 DCB  0x0E,0x0F,0x09,0x08,0x08,0x0C,0x0C,0x00
 DCB  0x08,0x0C,0x44,0x44,0x44,0xFC,0xB8,0x00
 DCB  0x04,0x0C,0x08,0x08,0x08,0x0F,0x07,0x00
 DCB  0xC0,0xE0,0xB0,0x98,0xFC,0xFC,0x80,0x00
 DCB  0x00,0x00,0x00,0x08,0x0F,0x0F,0x08,0x00
 DCB  0x7C,0x7C,0x44,0x44,0xC4,0xC4,0x84,0x00
 DCB  0x04,0x0C,0x08,0x08,0x08,0x0F,0x07,0x00
 DCB  0xF0,0xF8,0x4C,0x44,0x44,0xC0,0x80,0x00
 DCB  0x07,0x0F,0x08,0x08,0x08,0x0F,0x07,0x00
 DCB  0x0C,0x0C,0x04,0x84,0xC4,0x7C,0x3C,0x00
 DCB  0x00,0x00,0x0F,0x0F,0x00,0x00,0x00,0x00
 DCB  0xB8,0xFC,0x44,0x44,0x44,0xFC,0xB8,0x00
 DCB  0x07,0x0F,0x08,0x08,0x08,0x0F,0x07,0x00
 DCB  0x38,0x7C,0x44,0x44,0x44,0xFC,0xF8,0x00
 DCB  0x00,0x08,0x08,0x08,0x0C,0x07,0x03,0x00
 DCB  0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x06,0x06,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x30,0x30,0x00,0x00,0x00
 DCB  0x00,0x00,0x08,0x0E,0x06,0x00,0x00,0x00
 DCB  0x00,0x80,0xC0,0x60,0x30,0x18,0x08,0x00
 DCB  0x00,0x00,0x01,0x03,0x06,0x0C,0x08,0x00
 DCB  0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00
 DCB  0x02,0x02,0x02,0x02,0x02,0x02,0x02,0x00
 DCB  0x00,0x08,0x18,0x30,0x60,0xC0,0x80,0x00
 DCB  0x00,0x08,0x0C,0x06,0x03,0x01,0x00,0x00
 DCB  0x18,0x1C,0x04,0xC4,0xE4,0x3C,0x18,0x00
 DCB  0x00,0x00,0x00,0x0D,0x0D,0x00,0x00,0x00
 DCB  0xF0,0xF8,0x08,0xC8,0xC8,0xF8,0xF0,0x00
 DCB  0x07,0x0F,0x08,0x0B,0x0B,0x0B,0x01,0x00
 DCB  0xE0,0xF0,0x98,0x8C,0x98,0xF0,0xE0,0x00
 DCB  0x0F,0x0F,0x00,0x00,0x00,0x0F,0x0F,0x00
 DCB  0x04,0xFC,0xFC,0x44,0x44,0xFC,0xB8,0x00
 DCB  0x08,0x0F,0x0F,0x08,0x08,0x0F,0x07,0x00
 DCB  0xF0,0xF8,0x0C,0x04,0x04,0x0C,0x18,0x00
 DCB  0x03,0x07,0x0C,0x08,0x08,0x0C,0x06,0x00
 DCB  0x04,0xFC,0xFC,0x04,0x0C,0xF8,0xF0,0x00
 DCB  0x08,0x0F,0x0F,0x08,0x0C,0x07,0x03,0x00
 DCB  0x04,0xFC,0xFC,0x44,0xE4,0x0C,0x1C,0x00
 DCB  0x08,0x0F,0x0F,0x08,0x08,0x0C,0x0E,0x00
 DCB  0x04,0xFC,0xFC,0x44,0xE4,0x0C,0x1C,0x00
 DCB  0x08,0x0F,0x0F,0x08,0x00,0x00,0x00,0x00
 DCB  0xF0,0xF8,0x0C,0x84,0x84,0x8C,0x98,0x00
 DCB  0x03,0x07,0x0C,0x08,0x08,0x07,0x0F,0x00
 DCB  0xFC,0xFC,0x40,0x40,0x40,0xFC,0xFC,0x00
 DCB  0x0F,0x0F,0x00,0x00,0x00,0x0F,0x0F,0x00
 DCB  0x00,0x00,0x04,0xFC,0xFC,0x04,0x00,0x00
 DCB  0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00
 DCB  0x00,0x00,0x00,0x04,0xFC,0xFC,0x04,0x00
 DCB  0x07,0x0F,0x08,0x08,0x0F,0x07,0x00,0x00
 DCB  0x04,0xFC,0xFC,0xC0,0xF0,0x3C,0x0C,0x00
 DCB  0x08,0x0F,0x0F,0x00,0x01,0x0F,0x0E,0x00
 DCB  0x04,0xFC,0xFC,0x04,0x00,0x00,0x00,0x00
 DCB  0x08,0x0F,0x0F,0x08,0x08,0x0C,0x0E,0x00
 DCB  0xFC,0xFC,0x38,0x70,0x38,0xFC,0xFC,0x00
 DCB  0x0F,0x0F,0x00,0x00,0x00,0x0F,0x0F,0x00
 DCB  0xFC,0xFC,0x38,0x70,0xE0,0xFC,0xFC,0x00
 DCB  0x0F,0x0F,0x00,0x00,0x00,0x0F,0x0F,0x00
 DCB  0xF0,0xF8,0x0C,0x04,0x0C,0xF8,0xF0,0x00
 DCB  0x03,0x07,0x0C,0x08,0x0C,0x07,0x03,0x00
 DCB  0x04,0xFC,0xFC,0x44,0x44,0x7C,0x38,0x00
 DCB  0x08,0x0F,0x0F,0x08,0x00,0x00,0x00,0x00
 DCB  0xF8,0xFC,0x04,0x04,0x04,0xFC,0xF8,0x00
 DCB  0x07,0x0F,0x08,0x0E,0x3C,0x3F,0x27,0x00
 DCB  0x04,0xFC,0xFC,0x44,0xC4,0xFC,0x38,0x00
 DCB  0x08,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00
 DCB  0x18,0x3C,0x64,0x44,0xC4,0x9C,0x18,0x00
 DCB  0x06,0x0E,0x08,0x08,0x08,0x0F,0x07,0x00
 DCB  0x00,0x1C,0x0C,0xFC,0xFC,0x0C,0x1C,0x00
 DCB  0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00
 DCB  0xFC,0xFC,0x00,0x00,0x00,0xFC,0xFC,0x00
 DCB  0x07,0x0F,0x08,0x08,0x08,0x0F,0x07,0x00
 DCB  0xFC,0xFC,0x00,0x00,0x00,0xFC,0xFC,0x00
 DCB  0x01,0x03,0x06,0x0C,0x06,0x03,0x01,0x00
 DCB  0xFC,0xFC,0x00,0x80,0x00,0xFC,0xFC,0x00
 DCB  0x03,0x0F,0x0E,0x03,0x0E,0x0F,0x03,0x00
 DCB  0x0C,0x3C,0xF0,0xC0,0xF0,0x3C,0x0C,0x00
 DCB  0x0C,0x0F,0x03,0x00,0x03,0x0F,0x0C,0x00
 DCB  0x00,0x3C,0x7C,0xC0,0xC0,0x7C,0x3C,0x00
 DCB  0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00
 DCB  0x1C,0x0C,0x84,0xC4,0x64,0x3C,0x1C,0x00
 DCB  0x0E,0x0F,0x09,0x08,0x08,0x0C,0x0E,0x00
 DCB  0x00,0x00,0xFC,0xFC,0x04,0x04,0x00,0x00
 DCB  0x00,0x00,0x0F,0x0F,0x08,0x08,0x00,0x00
 DCB  0x38,0x70,0xE0,0xC0,0x80,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x01,0x03,0x07,0x0E,0x00
 DCB  0x00,0x00,0x04,0x04,0xFC,0xFC,0x00,0x00
 DCB  0x00,0x00,0x08,0x08,0x0F,0x0F,0x00,0x00
 DCB  0x08,0x0C,0x06,0x03,0x06,0x0C,0x08,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20
 DCB  0x00,0x00,0x03,0x07,0x04,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0xA0,0xA0,0xA0,0xE0,0xC0,0x00,0x00
 DCB  0x07,0x0F,0x08,0x08,0x07,0x0F,0x08,0x00
 DCB  0x04,0xFC,0xFC,0x20,0x60,0xC0,0x80,0x00
 DCB  0x08,0x0F,0x07,0x08,0x08,0x0F,0x07,0x00
 DCB  0xC0,0xE0,0x20,0x20,0x20,0x60,0x40,0x00
 DCB  0x07,0x0F,0x08,0x08,0x08,0x0C,0x04,0x00
 DCB  0x80,0xC0,0x60,0x24,0xFC,0xFC,0x00,0x00
 DCB  0x07,0x0F,0x08,0x08,0x07,0x0F,0x08,0x00
 DCB  0xC0,0xE0,0xA0,0xA0,0xA0,0xE0,0xC0,0x00
 DCB  0x07,0x0F,0x08,0x08,0x08,0x0C,0x04,0x00
 DCB  0x40,0xF8,0xFC,0x44,0x0C,0x18,0x00,0x00
 DCB  0x08,0x0F,0x0F,0x08,0x00,0x00,0x00,0x00
 DCB  0xC0,0xE0,0x20,0x20,0xC0,0xE0,0x20,0x00
 DCB  0x27,0x6F,0x48,0x48,0x7F,0x3F,0x00,0x00
 DCB  0x04,0xFC,0xFC,0x40,0x20,0xE0,0xC0,0x00
 DCB  0x08,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00
 DCB  0x00,0x00,0x20,0xEC,0xEC,0x00,0x00,0x00
 DCB  0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x20,0xEC,0xEC,0x00
 DCB  0x00,0x30,0x70,0x40,0x40,0x7F,0x3F,0x00
 DCB  0x04,0xFC,0xFC,0x80,0xC0,0x60,0x20,0x00
 DCB  0x08,0x0F,0x0F,0x01,0x03,0x0E,0x0C,0x00
 DCB  0x00,0x00,0x04,0xFC,0xFC,0x00,0x00,0x00
 DCB  0x00,0x00,0x08,0x0F,0x0F,0x08,0x00,0x00
 DCB  0xE0,0xE0,0x60,0xC0,0x60,0xE0,0xC0,0x00
 DCB  0x0F,0x0F,0x00,0x0F,0x00,0x0F,0x0F,0x00
 DCB  0x20,0xE0,0xC0,0x20,0x20,0xE0,0xC0,0x00
 DCB  0x00,0x0F,0x0F,0x00,0x00,0x0F,0x0F,0x00
 DCB  0xC0,0xE0,0x20,0x20,0x20,0xE0,0xC0,0x00
 DCB  0x07,0x0F,0x08,0x08,0x08,0x0F,0x07,0x00
 DCB  0x20,0xE0,0xC0,0x20,0x20,0xE0,0xC0,0x00
 DCB  0x40,0x7F,0x7F,0x48,0x08,0x0F,0x07,0x00
 DCB  0xC0,0xE0,0x20,0x20,0xC0,0xE0,0x20,0x00
 DCB  0x07,0x0F,0x08,0x48,0x7F,0x7F,0x40,0x00
 DCB  0x20,0xE0,0xC0,0x60,0x20,0x60,0xC0,0x00
 DCB  0x08,0x0F,0x0F,0x08,0x00,0x00,0x00,0x00
 DCB  0x40,0xE0,0xA0,0x20,0x20,0x60,0x40,0x00
 DCB  0x04,0x0C,0x09,0x09,0x0B,0x0E,0x04,0x00
 DCB  0x20,0x20,0xF8,0xFC,0x20,0x20,0x00,0x00
 DCB  0x00,0x00,0x07,0x0F,0x08,0x0C,0x04,0x00
 DCB  0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00,0x00
 DCB  0x07,0x0F,0x08,0x08,0x07,0x0F,0x08,0x00
 DCB  0x00,0xE0,0xE0,0x00,0x00,0xE0,0xE0,0x00
 DCB  0x00,0x03,0x07,0x0C,0x0C,0x07,0x03,0x00
 DCB  0xE0,0xE0,0x00,0x00,0x00,0xE0,0xE0,0x00
 DCB  0x07,0x0F,0x0C,0x07,0x0C,0x0F,0x07,0x00
 DCB  0x20,0x60,0xC0,0x80,0xC0,0x60,0x20,0x00
 DCB  0x08,0x0C,0x07,0x03,0x07,0x0C,0x08,0x00
 DCB  0xE0,0xE0,0x00,0x00,0x00,0xE0,0xE0,0x00
 DCB  0x47,0x4F,0x48,0x48,0x68,0x3F,0x1F,0x00
 DCB  0x60,0x60,0x20,0xA0,0xE0,0x60,0x20,0x00
 DCB  0x0C,0x0E,0x0B,0x09,0x08,0x0C,0x0C,0x00
 DCB  0x00,0x40,0x40,0xF8,0xBC,0x04,0x04,0x00
 DCB  0x00,0x00,0x00,0x07,0x0F,0x08,0x08,0x00
 DCB  0x00,0x00,0x00,0xBC,0xBC,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x0F,0x0F,0x00,0x00,0x00
 DCB  0x00,0x04,0x04,0xBC,0xF8,0x40,0x40,0x00
 DCB  0x00,0x08,0x08,0x0F,0x07,0x00,0x00,0x00
 DCB  0x08,0x0C,0x04,0x0C,0x08,0x0C,0x04,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x80,0xC0,0x60,0x30,0x60,0xC0,0x80,0x00
 DCB  0x07,0x07,0x04,0x04,0x04,0x07,0x07,0x00;//old last line
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x24
 DCB  0xC8,0x00,0x74,0x54,0x7C,0x54,0x7C,0x54,0x74,0x04,0x00,0x88,0x88,0xE8,0xA8,0xA8
 DCB  0xAC,0xA8,0xA8,0xE8,0x88,0x88,0x00,0x10,0xF0,0x1C,0xF0,0x10,0x10,0x10,0xFC,0x10
 DCB  0x10,0x10,0x00,0x10,0xF0,0x1C,0xF0,0x00,0xF8,0xA8,0xAC,0xA8,0xF8,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x40,0x40,0x80,0x80
 DCB  0x40,0x40,0x40,0x40,0xC0,0x40,0x40,0x40,0x40,0x40,0x80,0x80,0x00,0x80,0x80,0x80
 DCB  0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02
 DCB  0x1F,0x00,0x14,0x0D,0x05,0x15,0x1D,0x05,0x0D,0x14,0x00,0x11,0x10,0x08,0x06,0x02
 DCB  0x02,0x02,0x1E,0x10,0x10,0x1D,0x00,0x13,0x0A,0x04,0x0B,0x00,0x1F,0x11,0x11,0x11
 DCB  0x11,0x1F,0x00,0x10,0x0B,0x04,0x0B,0x00,0x1F,0x10,0x0B,0x04,0x0A,0x11,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x80,0x60,0x10,0x8C,0x82,0x41,0xA0,0x60,0x18,0x80,0x44,0x22
 DCB  0x22,0x44,0x80,0x01,0x84,0x42,0x22,0x24,0x40,0x8C,0x30,0x42,0x81,0x80,0x00,0x00
 DCB  0x00,0x07,0x78,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10
 DCB  0x91,0x55,0xF5,0x5D,0x57,0x55,0x55,0xD5,0x11,0x10,0x00,0x11,0x16,0xF0,0x04,0x14
 DCB  0x64,0x04,0x04,0xFF,0x04,0x04,0x00,0x20,0x22,0x2A,0xB2,0x62,0xFF,0x62,0xB2,0x2A
 DCB  0x22,0x20,0x00,0x00,0x1C,0x7E,0x7E,0x1C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x01,0x01,0xF1,0x0C,0x02,0x01,0xE0,0x10,0x08,0x09,0x12,0x94
 DCB  0x97,0x2B,0x29,0x28,0x21,0x12,0x14,0x17,0x23,0xC1,0xE0,0x10,0x00,0x00,0x01,0x03
 DCB  0x0D,0xF2,0x02,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01
 DCB  0x00,0x00,0x07,0x05,0x05,0x05,0x05,0x07,0x00,0x80,0x80,0x84,0x02,0x01,0x02,0x84
 DCB  0x84,0x85,0x85,0x05,0x84,0x84,0x80,0x02,0x02,0x01,0x80,0x80,0x87,0x80,0x00,0x01
 DCB  0x02,0x02,0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x84,0xC8,0xD0,0x55,0x2A,0x34,0x24,0x49
 DCB  0x49,0x90,0x90,0x93,0x93,0x48,0x48,0x64,0x53,0x40,0x20,0x21,0x50,0xD0,0xC8,0x8C
 DCB  0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x21,0x4C,0x82,0x00,0x01,0x03,0x01,0x00
 DCB  0x80,0xC0,0x7F,0x21,0x4C,0x82,0x00,0x01,0x03,0x01,0x00,0x80,0xC0,0x61,0x3F,0x1E
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x38,0x3C,0x3E,0x37,0x23,0x00,0x07,0x08,0x17,0x2D,0x0D,0x07
 DCB  0x03,0x01,0x03,0x03,0x01,0x03,0x07,0x0D,0x0D,0x07,0x20,0x10,0x08,0x07,0x00,0x23
 DCB  0x37,0x3E,0x3C,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x06,0x03
 DCB  0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x06,0x03,0x01,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
 DCB  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

  
; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_USR           EQU   0x10
Mode_FIQ           EQU   0x11
Mode_IRQ           EQU   0x12
Mode_SVC           EQU   0x13
Mode_ABT           EQU   0x17
Mode_UNDEF         EQU   0x1B
Mode_SYS           EQU   0x1F ; available on ARM Arch 4 and later

I_Bit              EQU   0x80 ; when I bit is set, IRQ is disabled
F_Bit              EQU   0x40 ; when F bit is set, FIQ is disabled



EIC_Base_addr      EQU   0xFFFFF800; EIC base address
ICR_off_addr       EQU   0x00      ; Interrupt Control register offset
CIPR_off_addr      EQU   0x08      ; Current Interrupt Priority Register offset
IVR_off_addr       EQU   0x18      ; Interrupt Vector Register offset
FIR_off_addr       EQU   0x1C      ; Fast Interrupt Register offset
IER_off_addr       EQU   0x20      ; Interrupt Enable Register offset
IPR_off_addr       EQU   0x40      ; Interrupt Pending Bit Register offset
SIR0_off_addr      EQU   0x60      ; Source Interrupt Register 0

EMI_Base_addr      EQU   0x6C000000; EMI base address
BCON0_off_addr     EQU   0x00      ; Bank 0 configuration register offset
BCON1_off_addr     EQU   0x04      ; Bank 1 configuration register offset
BCON2_off_addr     EQU   0x08      ; Bank 2 configuration register offset
BCON3_off_addr     EQU   0x0C      ; Bank 3 configuration register offset

EMI_ENABLE         EQU   0x8000
EMI_SIZE_16        EQU   0x0001

GPIO2_Base_addr    EQU   0xE0005000; GPIO2 base address
PC0_off_addr       EQU   0x00      ; Port Configuration Register 0 offset
PC1_off_addr       EQU   0x04      ; Port Configuration Register 1 offset
PC2_off_addr       EQU   0x08      ; Port Configuration Register 2 offset
PD_off_addr        EQU   0x0C      ; Port Data Register offset

CPM_Base_addr      EQU   0xA0000040; CPM Base Address
BOOTCR_off_addr    EQU   0x10      ; CPM - Boot Configuration Register
FLASH_mask         EQU   0x0000    ; to remap FLASH at 0x0
RAM_mask           EQU   0x0002    ; to remap RAM at 0x0

;|----------------------------------------------------------------------------------|
;| - APB Bridge  (System Peripheral)                                               |
;|----------------------------------------------------------------------------------|
APB1_base_addr     EQU   0xC0000000          ; APB Bridge1 Base Address
APB2_base_addr     EQU   0xE0000000          ; APB Bridge2 Base Address
CKDIS_off_addr     EQU   0x10                ; APB Bridge1 - Clock Disable  Register
SWRES_off_addr     EQU   0x14                ; APB Bridge1 - Software Reset Register
CKDIS1_config_all  EQU   0x27FB              ; To enable/disable clock of all APB1's peripherals
SWRES1_config_all  EQU   0x27FB              ; To reset all APB1's peripherals
CKDIS2_config_all  EQU   0x7FDD              ; To enable/disable clock of all APB2's peripherals
SWRES2_config_all  EQU   0x7FDD              ; To reset all APB2's peripherals



;---------------------------------------------------------------
; ?program_start
;---------------------------------------------------------------
		MODULE	?program_start
		RSEG	IRQ_STACK:DATA(2)
		RSEG	FIQ_STACK:DATA(2)
		RSEG	UNDEF_STACK:DATA(2)
		RSEG	ABT_STACK:DATA(2)		
		RSEG	SVC_STACK:DATA(2)
		RSEG	CSTACK:DATA(2)
		RSEG	ICODE:CODE(2)
		PUBLIC	__program_start
		EXTERN	?main
                CODE32


;*******************************************************************************
;*******                         -- MACROS --                            *******
;*******************************************************************************
;*******************************************************************************
;* Macro Name     : EMI_INIT
;* Description    : This macro Initialize EMI bank 1: 16-bit 7 wait state
;* Input          : None.
;* Output         : None.
;*******************************************************************************
EMI_INIT  MACRO 
        LDR     r0, =GPIO2_Base_addr      ; Configure P2.0 -> 3 in AF_PP mode
        LDR     r2, [r0, #PC0_off_addr]
        ORR     r2, r2,#0x0000000F
        STR     r2, [r0, #PC0_off_addr]
        LDR     r2, [r0, #PC1_off_addr]
        ORR     r2, r2,#0x0000000F
        STR     r2, [r0, #PC1_off_addr]
        LDR     r2, [r0, #PC2_off_addr]
        ORR     r2, r2,#0x0000000F
        STR     r2, [r0, #PC2_off_addr]
        LDR     r0, =EMI_Base_addr
        LDR     r1, =0x18:OR:EMI_ENABLE:OR:EMI_SIZE_16
        STR     r1, [r0, #BCON1_off_addr] ; Enable bank 1 16-bit 7 wait state
        ENDM
;*******************************************************************************
;* Macro Name     : EIC_INIT
;* Description    : This macro Initialize the EIC as following :
;                 - IRQ disabled
;                 - FIQ disabled
;                 - IVR contain the load PC opcode (0xE59FFXXX)
;                 - Current priority level equal to 0
;                 - All channels are disabled
;                 - All channels priority equal to 0
;                 - All SIR registers contain offset to the related IRQ
;                   table entry
;* Input          : None.
;* Output         : None.
;*******************************************************************************
EIC_INIT   MACRO

        LDR     r3, =EIC_Base_addr
        LDR     r4, =0xE59F0000
        STR     r4, [r3, #IVR_off_addr]; Write the LDR pc,[pc,#offset]
                                       ; instruction code in IVR[31:16]
        LDR     r2, =32                ; 32 Channel to initialize
        LDR     r0, =T0TIMI_Addr       ; Read the address of the IRQs
                                       ; address table
        LDR     r1, =0x00000FFF
        AND     r0,r0,r1
        LDR     r5, =SIR0_off_addr     ; Read SIR0 address
        SUB     r4,r0,#8               ; subtract 8 for prefetch
        LDR     r1, =0xF7E8            ; Add the offset from IVR to 0x00000000
                                       ; address(IVR address + 7E8 = 0x00000000)
                                       ; 0xF7E8 used to complete the
                                       ; LDR pc,[pc,#offset] opcode (0xE59FFXXX)
        ADD     r1,r4,r1               ; Compute the jump offset from IVR to the
                                       ; IRQ table entry.
EIC_INI MOV     r4, r1, LSL #16        ; Left shift the result
        STR     r4, [r3, r5]           ; Store the result in SIRx register
        ADD     r1, r1, #4             ; Next IRQ address
        ADD     r5, r5, #4             ; Next SIR
        SUBS    r2, r2, #1             ; Decrement the number of SIR registers
                                       ; to initialize
        BNE     EIC_INI                ; If more then continue
        ENDM
;*******************************************************************************
;* Macro Name     : PERIPHERAL_INIT
;* Description    : This macro reset all device peripherals.
;* Input          : None.
;* Output         : None.
;*******************************************************************************
PERIPHERAL_INIT MACRO

        LDR     r1, =APB1_base_addr      ; r0= APB1 base address
        LDR     r2, =APB2_base_addr      ; r0= APB2 base address
        LDR     r0, =CKDIS1_config_all
        STRH    r0, [r1, #CKDIS_off_addr]; Clock Disabling for all APB1 peripherals
        LDR     r0, =CKDIS2_config_all
        STRH    r0, [r2, #CKDIS_off_addr]; Clock Disabling for all APB2 peripherals
        LDR     r0, =SWRES1_config_all
        STRH    r0, [r1, #SWRES_off_addr]; Keep all APB1 peripherals under reset
        LDR     r0, =SWRES2_config_all
        STRH    r0, [r2, #SWRES_off_addr]; Keep all APB2 peripherals under reset
        MOV     r7, #10                  ; Wait that the selected macrocells exit from reset
loop1   SUBS    r7, r7, #1
        BNE     loop1
        MOV     r0, #0
        STRH    r0, [r1, #SWRES_off_addr]; Enable all all APB1 peripherals
        STRH    r0, [r2, #SWRES_off_addr]; Enable all all APB2 peripherals
        STRH    r0, [r1, #CKDIS_off_addr]; Clock Enabling for all APB1 peripherals
        STRH    r0, [r2, #CKDIS_off_addr]; Clock Enabling for all APB2 peripherals
        MOV     r7, #10                  ; Wait that the selected macrocells exit from reset
loop2   SUBS    r7, r7, #1
        BNE     loop2
        ENDM
;********************************************************************************************

; define remapping
; If you need to remap memory before entring the main program
; uncomment next ligne
;            #define   remapping

; Then define which memory to remap to address 0x00000000
;  Uncomment next line if you want to remap RAM
;         #define  remap_ram

;  Uncomment next line if you want to remap FLASH
;         #define remap_flash


        IMPORT  T0TIMI_Addr
__program_start
         LDR     pc, =NextInst
NextInst
		NOP		; Wait for OSC stabilization
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP
		NOP

        MSR     CPSR_c, #Mode_ABT|F_Bit|I_Bit
        ldr      sp,=SFE(ABT_STACK) & 0xFFFFFFF8     ; End of ABT_STACK

        MSR     CPSR_c, #Mode_UNDEF|F_Bit|I_Bit
        ldr      sp,=SFE(UNDEF_STACK) & 0xFFFFFFF8     ; End of UNDEF_STACK

        MSR     CPSR_c, #Mode_SVC|F_Bit|I_Bit
       ldr      sp,=SFE(SVC_STACK) & 0xFFFFFFF8     ; End of SVC_STACK
       
       
       
; Uncomment next ligne if you need to reset all device pripherals
;       PERIPHERAL_INIT           ; Reset all device peripherals

; Uncomment next ligne if you need to enable the EMI Bank 1
;       EMI_INIT                  ; Initialize EIM Bank 1

;Uncomment next ligne if you need to initialize the EIC
        EIC_INIT                  ; Initialize EIC

;******************************************************************************
;REMAPPING
;Description  : Remapping  memory whether RAM,FLASH
;               at Address 0x0 after the application has started executing.
;               Remapping is generally done to allow RAM  to replace FLASH
;               at 0x0.
;               the remapping of RAM allow copying of vector table into RAM
;               To enable the memory remapping uncomment: (see above)
;               #define  remapping to enable memory remapping
;                  AND
;               #define  remap_ram to remap RAM
;                  OR
;               #define  remap_flash to remap FLASH
;******************************************************************************
#ifdef remapping
    #ifdef remap_flash
        MOV     r0, #FLASH_mask
    #endif
    #ifdef remap_ram
        MOV     r0, #RAM_mask
    #endif

        LDR     r1, =CPM_Base_addr
        LDRH    r2, [r1, #BOOTCR_off_addr]; Read BOOTCR Register
        BIC     r2, r2, #0x03             ; Reset the two LSB bits of BOOTCR
        ORR     r2, r2, r0                ; change the two LSB bits of BOOTCR
        STRH    r2, [r1, #BOOTCR_off_addr]; Write BOOTCR Register
#endif
       	MSR     CPSR_c, #Mode_FIQ|I_Bit; Change to FIQ mode
        ldr      sp,=SFE(FIQ_STACK) & 0xFFFFFFF8     ; End of FIQ_STACK

       	MSR     CPSR_c, #Mode_IRQ|I_Bit; Change to IRQ mode
        ldr      sp,=SFE(IRQ_STACK) & 0xFFFFFFF8     ; End of IRQ_STACK

        MSR     CPSR_c, #Mode_USR         ; Change to User mode, Enable IRQ and FIQ
       ldr     sp,=SFE(CSTACK) & 0xFFFFFFF8        ; End of CSTACK(user)


; --- Now branches to a C lib function that copies RO data from their
;     load region to their execute region, create the RW and ZI regions
;     then jumps to user C main program.

        b ?main   ; Note : use B not BL, because an application will
                         ; never return this way

        LTORG




        END
