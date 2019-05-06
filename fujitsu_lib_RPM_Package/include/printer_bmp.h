/**
* @section LICENSE
* Copyright &copy; 2014 Safran Morpho - Safran Morpho confidential - all rights reserved
*
* @section DESCRIPTION
* This file contains the function definitions of printer library.
*
* @file printer_bmp.h
*/

#ifndef PRINTER_BMP_H_
#define PRINTER_BMP_H_

#ifdef __cplusplus
extern "C" {
#endif /*  __cplusplus */

#include <stdint.h>
/**
 * \brief File information header to provide general information about the file
 */
typedef struct __attribute__ (( packed ))
{
    uint8_t bfType[2];
    int32_t bfSize;
    int16_t bfReserved1;
    int16_t bfReserved2;
    int32_t bfOffBits;
}
BITMAP_FILEHEADER;    // parasoft-suppress OPT-13 "Cannot change suboptimal memory order as the structure is received from printer"

/**
 * \brief Bitmap information header to provide information specific to the image data
 */
typedef struct __attribute__ (( packed ))
{
    int32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    int16_t biPlanes;
    int16_t biBitCount;
    int32_t biCompression;
    int32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    int32_t biClrUsed;
    int32_t biClrImportant;
}
BITMAP_INFOHEADER;    // parasoft-suppress OPT-13 "Cannot change suboptimal memory order as the structure is received from printer"

/**
 * \brief Colour palette
 */
typedef struct __attribute__ (( packed ))
{
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
    uint8_t rgbReserved;
}
RGBQUAD;

/**
 * \brief Bitmap file
 */
typedef struct __attribute__ (( packed ))
{
    BITMAP_FILEHEADER m_x_Fheader;
    BITMAP_INFOHEADER m_x_Iheader;
    RGBQUAD			 m_x_Irgb;
}
BITMAP,*PT_BITMAP;

#ifdef __cplusplus
}
#endif /*   __cplusplus */


#endif /* PRINTER_BMP_H_ */
