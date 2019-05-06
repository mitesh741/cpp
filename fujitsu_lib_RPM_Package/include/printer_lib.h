/**
* @section LICENSE
* Copyright &copy; 2014 Safran Morpho - Safran Morpho confidential - all rights reserved
*
* @section DESCRIPTION
* This file contains the function definitions of printer library.
*
* @file printer_lib.h
*/

#ifndef PRINTER_LIB_H_
#define PRINTER_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif /*  __cplusplus */


#include <stdint.h>

#define PRINTER_NOERROR         0x00000000  //!< Success
#define PRINTER_PAPER_LOW       0x00000001  //!< Error for paper low
#define PRINTER_PAPER_OUT       0x00000002  //!< Error for paper out or no paper
#define PRINTER_TEMP_TOOHOT     0x00000004  //!< Error for printer temperature too hot
#define PRINTER_COVER_OPEN      0x00000008  //!< Error for printer cover open
#define PRINTER_CUTTER_ERR      0x00000010  //!< Error in cutter
#define PRINTER_BUFFER_FULL     0x00000020  //!< Error for printer buffer full
#define PRINTER_SEND_ERR        0x00000040  //!< Error for sending command to printer
#define PRINTER_NOT_PLUGGED     0x00000080  //!< Error for printer not plugged
#define PRINTER_BAD_SERIAL      0x00000100  //!< Error for bad serial number
#define PRINTER_DOWNLOAD_ERR    0x00000200  //!< Error in firmware download
#define PRINTER_ERROR           0x00000400  //!< Printer error
#define PRINTER_MEMORY_ERR      0x00000800  //!< Error in printer memory
#define PRINTER_CONNECTED       0x00001000  //!< Printer connected (used for signal handling)
#define PRINTER_BAD_FILE_FORMAT 0x00002000  //!< Error for bad image file format
#define PRINTER_INITIALIZATION  0x00004000  //!< Successful initialization
#define PRINTER_NOT_COMPLETE    0x00008000  //!< Error for printing not complete
#define PRINTER_SEND_TIMEOUT    0x00010000  //!< Error for timeout
#define PRINTER_COVER_CLOSE     0X00020000  //!< Signal for cover close
#define PRINTER_CUT_END    	0x01000000  //!< Printer status for printing BMP file
#define PRINTER_PRINT_END    	0x00100000  //!< Printer status for printing BMP file

typedef int32_t (*status_callback)(int32_t status); //!<function callback for printer status

/**
 * \brief Printer error status
 */
typedef struct
{
    uint64_t bmp_height;   //!< Height of the bitmap to print i.e. Number of lines sent to printer.
    int32_t error_status; //!< Cause which stopped the printing.
    uint16_t in_counter;    //!< Number of lines to print received since last successful cut action.
    uint16_t out_counter;   //!< Number of motor step done by the printer since last successful cut action.
    uint8_t bitmap_id;    //!< Index number of the bitmap to print, defined on one byte modulo 127 (from 1 to 127). It is incremented by one for each printing request.
} T_PRI_LAST_ERROR, *PT_PRI_LAST_ERROR;

/**
 * \brief Printing width size
 */
typedef enum
{
    width_all = 0,
    width_80_00_mm_paper = 72,  //!< 72 bytes for 80mm paper
    width_82_50_mm_paper =  80, //!< 80 bytes for 82.5mm paper
} printing_width_size;

/**
 * Establishes connection and initializtion of the printer.
 * Initialization consists of Print density = + 31, Peak limitation = 0, Max speed = 90mm/s
 *
 * \param[in] status_func_callback
 * - Callback function pointer of the function to be called signal/status handling.
 * \param[in] printing_size
 * - Width size for printing
 *
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_init_connect(status_callback status_func_callback, printing_width_size printing_size);

/**
 * Closes connection with the printer.
 *
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_disconnect(void);

/**
 * This function is called for launching a cut action on the printer.
 *
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_cut_paper(void);

/**
 * This function is called for printing a bitmap image file on the printer.
 * The bitmap shall be in black and white format.
 *
 * \param[in] receipt_bmp
 * - Bitmap image data
 * \param[in] bitmap_size
 * - Bitmap image data size
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_print_bitmap(uint8_t* receipt_bitmap,uint64_t bitmap_size);

/**
 * This function makes the paper advance of specified number of lines.
 *
 * \param[in] no_of_lines
 * - Number of empty lines to add
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_line_feed(int32_t no_of_lines);

/**
 * This function retrieves the current status of the printer.
 *
 * \param[in] printer_status
 * Pointer to a variable used to retrieve printer status. This variable shall be initialized by the application.
 *
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_get_status(int32_t* printer_status);

/**
 * This function upgrades the firmware on the printer.
 * Note: This function put the printer in bootloader mode, which is used to perform firmware upgrade.
 * Once the firmware is flashed, the printer is back in normal mode.
 * During 25 seconds, printer will be unavailable.
 * Communication will have to be initialized again to access the printer after upgrade
 *
 * \param[in] firmware_data
 * Pointer to the buffer containing the firmware to upgrade in the printer.
 *
 * \param[in] firmware_data_size
 * Length of the buffer.
 *
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_upgrade_firmware(uint8_t *firmware_data,int32_t firmware_data_size);

/**
 * This function retrieves the current firmware version of the printer.
 *
 * \param[in] firmware_version
 * Pointer of the buffer which will be used to retrieve the firmware version. This buffer shall be initialized by the application.
 *
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_get_firmware_version(uint8_t *firmware_version);

/**
 * This function retrieves the last error occured on the printer.
 *
 * \param[in] printer_last_error
 * Pointer to the structure containing information on the last print error. This structure shall be initialized by the application.
 *
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_get_last_error(T_PRI_LAST_ERROR *printer_last_error) ;

/**
 * This function clears the error buffer.
 *
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_discard_error(void);

/**
 * This function returns the current value of the print counter.
 *
 * \param[in/out] print_counter
 * Current value of the print counter holding number of prints.
 *
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_get_counter_value(uint32_t* print_counter);

/**
 * This function retrieves the serial number of the printer.
 *
 * \param[in] serial_no
 * Pointer of the buffer which will be used to retrieve the serial number. This buffer shall be initialized by the application.
 *
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_get_serial_number(uint8_t* serial_no);

/**
 * This function flashes the serial number of the printer in its internal memory.
 *
 * \param[in] serial_num
 * Pointer of the buffer which will be used to store printer serial number provided by the application to write.
 * This printer serial number shall be maximum 12 bytes long, without NULL charactor.
 *
 * \return
 * - PRINTER_NOERROR on success else error code
 */
int32_t printer_write_serial_number(uint8_t* serial_num);


#ifdef __cplusplus
}
#endif /*   __cplusplus */


#endif /* PRINTER_LIB_H_ */
