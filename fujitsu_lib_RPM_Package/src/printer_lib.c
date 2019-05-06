#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <usb.h>

#include "printer_lib.h"
#include "printer_bmp.h"


///////////// Global variables /////////////

//#define ENABLE_PRINT

#define PRINTER_VENDOR_ID   0x13EA    //!< Vendor ID of printer
#define PRINTER_PROD_ID 0x0639  //!< Product ID of printer
#define OUR_TIMEOUT     10    	//!< Timeout between receiving data chunks
#define OUR_TIMEOUT2    10       //!< Timeout for status
#define TIMEOUT_INIT    750     //!< Timeout for initialization
#define SPEED_MM_S      0x50    //!< Speed for printing
#define BUFFER_SIZE     2048    //!< Max size of buffer chunk
#define HEADER_SIZE_MAX     4
#define RECEIVE_SIZE_MAX    14
#define MAX_READ_ATTEMPT    100  //!< Max attempts to check receiving of data
#define PRI_NB_BYTE_PER_LINE    80  //!< Number of bytes per line
#define INIT_SLEEP      200 * 1000  //!< Sleep in milliseconds during open and init calls
#define CONN_MONITOR_SLEEP  5   //!< Sleep duration in seconds of connection monitoring thread
#define INIT_CONNECT_SLEEP  3   //!< Sleep duration in seconds for init connect function
#define UNSIGNED_32_BIT_VAL 4294967295  //!< Maximum value of 32 bit unsigned integer
#define CUT_WAIT_DUR    	2 //!< Waiting for cut operation
#define PRINT_WAIT_DUR    	2 //!< Waiting for print operation
#define ESCQ_RESPONSE    	50 //!< Waiting for esc q response 

#define PRI_PRINT_LINE_BMP  "\x1B\xCD\x50\x08"  //!< Print line bitmap
#define PRI_REPLACE_LAST_BYTE_STR(cmdStr, data) \
        *(cmdStr + (strlen(cmdStr) - 1)) = data //!< Replace last byte in string
#define PRI_MAX_LINE_FEED_CMD 0x7F  //!< Max line feed command
#define PRI_LINE_FEED "\x1D\x7F"    //!< Line feed

#define DEV_FILE_PRINTER    "/dev/printer"  //!< File for printer device

//Changed the mount path to rootfs_data to roofs_logs to data_morpho in SONIC
#ifdef BOARD_SONIC
#define PRINT_CTR_FILE  "/data_morpho/print_counter"    //!< File for maintaining print counter
#define PRINTER_PARAMS_FILE_PATH "/data_morpho/etc/config/printer_params.txt"   //! File for printer parameters
#else
#define PRINT_CTR_FILE  "/rootfs_data/print_counter"    //!< File for maintaining print counter
#define PRINTER_PARAMS_FILE_PATH "/usr/mgp/config/printer_params.txt"   //! File for printer parameters
#endif

#define PARAMS_LEN 101  //!< Parameter length read form printer_params.txt file
#define READ_LINE_LEN 128  //!< Length of line read form printer_params.txt file

#define THREAD_CONNECTION	0x02
#define THREAD_STATUS		0x04
static uint8_t g_printer_open = 0;		// for open / close

static pthread_mutex_t g_sem_write;        //!< Mutex to protect write operations
static pthread_mutex_t g_sem_read;         //!< Mutex to protect read operations
static pthread_mutex_t g_sem_send;         //!< Mutex to protect sending commands
static pthread_mutex_t g_sem_close;        //!< Mutex to protect close/disconnection operations
static pthread_mutex_t g_sem_status;       //!< Mutex to protect printer status flag
static pthread_mutex_t g_sem_printstatus;  //!< Mutex to protect global error structure

static pthread_t status_thread ;        //!< Thread to monitor the status of the printer
static pthread_t connection_thread;    //!< Thread to monitor connection status of the printer

static int32_t g_status = PRINTER_NOT_PLUGGED;     //!< Global to store the current printer status
static int32_t g_printing_size = 80;    //!< Global to store the current printing size

static status_callback g_app_status_callback;  //!< Global to store application callback function pointer
static status_callback g_status_callback_function;    //!< Global for storing internal callback function pointer

static uint8_t* g_send_buffer = NULL;    //!< Global pointer to store address of data to send
static uint8_t  g_receive_buffer[RECEIVE_SIZE_MAX]; //!< Global array to store data to receive
static uint8_t  g_header[HEADER_SIZE_MAX];        //!< Global array to store header to check after receiving data

static uint8_t g_timeout = 0;          //!< Global flag to store current status of timeout
static uint8_t g_printer_plugged = 0;        //!< Global flag to store current connection status
static uint8_t g_bootmode = 0;         //!< Global flag to store boot mode value
static uint8_t g_bitmap_id = 0;        //!< Global to store bitmap identifier

static T_PRI_LAST_ERROR  g_last_error;         //!< Global structure to store previous error

static int32_t g_header_size = 0;      //!< Global to store entete size
static int32_t g_send_size = 0;        //!< Global to store size of data to be sent
static int32_t g_data_to_send = 0;     //!< Global to store data to be sent
static int32_t g_close_threads = 0;            //!< Global to store status of disconnection
static int32_t g_data_to_receive = 0;  //!< Global to store data to be received
static int32_t g_receive_size = 0;     //!< Global to store size of data to be received
static int32_t g_data_send_status = 0; //!< Global to store status of sent data
static int32_t g_wait_printend = 0;     //!< Global flag to indicate wait for end of effective printing
static int32_t g_wait_cutend = 0;     //!< Global flag to indicate wait for end of paper cut
static uint32_t g_print_counter = 0;   //!< Global for holding the print counter
static int32_t com_port = 0;   //!< USB serial port of the printer
static uint32_t g_last_bitmap_width = 0;

static uint8_t set_page_ok = 0;    //!< Used in firmware upgrade
static uint8_t page_data_ok = 0;   //!< Used in firmware upgrade
static uint8_t boot_loader_mode = 0; //!< Used in firmware upgrade
static uint8_t download_ok = 0; //!< Used in firmware upgrade

static uint8_t need_printer_init = 0; //!< Used in firmware upgrade

static uint8_t g_print_density = 0;  //!< Used in printer parameters read from file
static uint8_t g_current_limit = 0;  //!< Used in printer parameters read from file
static uint8_t g_max_printer_speed = 0;  //!< Used in printer parameters read from file

///////////// Internal function declarations /////////////

static void start_status_thread(status_callback internal_status_callback);
static void stop_status_thread(void);
static void* status_thread_routine(void *arg);
static int32_t printer_reboot_to_normal_mode(void);
static int32_t printer_switch_to_bootloader_mode(void);
static int32_t printer_download_firmware(uint8_t* firmware_data,int32_t data_size);
static int32_t printer_connection(void);
static void* printer_connection_thread_routine(void *arg);
static int32_t printer_init(int32_t print_size);
static int32_t printer_open(status_callback internal_status_callback);
static int32_t printer_statuscb(int32_t status);

static int32_t printer_send_custom_data(uint8_t* data_buf, int32_t size, int32_t receive, int32_t rec_size);
static int32_t convert_status(uint8_t input_status,int32_t* output_status);
static uint64_t convert_bmp_to_raw(uint8_t * bitmap_data,uint8_t** raw_data,uint32_t*  output_width,uint32_t* output_height);
static int32_t white_compression(uint8_t* origin,uint8_t** compress,uint32_t width);

static void update_boolean(int32_t i);
static int32_t set_page(int32_t page);
static int32_t page_data(int16_t add, int8_t* data_buf);
static int32_t update_program_checksum(void);
static int32_t printer_dev_reset();
static uint32_t read_print_counter();
static void write_print_counter();
static int32_t file_read_params();

static int32_t printer_send_data(int32_t i_com_port, uint8_t* i_send_buffer, int32_t i_send_size, int32_t* io_pint32_data_send_status, int32_t i_data_to_send);

///////////// Printer Library exposed functions /////////////

int32_t printer_init_connect(status_callback status_func_callback, printing_width_size printing_size)
{
    int ret = 0;

    // Initializing printing size
    g_printing_size = printing_size;

    if (( g_printing_size != 0) &&( g_printing_size != 72) &&( g_printing_size != 80))
    {
        g_printing_size =80;
    }

    if(	(g_printer_open & (THREAD_CONNECTION | THREAD_STATUS)) != 0)
    {
        printer_disconnect();
        usleep(INIT_SLEEP);
    }

    // Perform USB reset for printer
    printer_dev_reset();

    // Initializing semaphores
    pthread_mutex_init(&g_sem_write,NULL);
    pthread_mutex_init(&g_sem_read,NULL);
    pthread_mutex_init(&g_sem_send,NULL);
    pthread_mutex_init(&g_sem_close,NULL);
    pthread_mutex_init(&g_sem_status,NULL);
    pthread_mutex_init(&g_sem_printstatus,NULL);

    // Initializing global close, status & variable
    g_close_threads = 0;

    // Initializing status callback mechanism
    pthread_mutex_lock(&g_sem_status);
    g_status = PRINTER_NOT_PLUGGED;
    g_app_status_callback = status_func_callback;
    pthread_mutex_unlock(&g_sem_status);

    // Read current value of print counter and initialize counter
    g_print_counter = read_print_counter();

    // Read printer parameters from the configuration fileb or default
    ret = file_read_params();

    if(ret != PRINTER_NOERROR)
    {
#ifdef ENABLE_PRINT
        printf("file_read_params error = 0x%X\n", ret);
#endif
    }

    // start dialog thread to printer  FW
    printer_connection();

    // Clear Last error
    memset(&g_last_error ,0, sizeof( T_PRI_LAST_ERROR ));

    sleep(INIT_CONNECT_SLEEP);

    return PRINTER_NOERROR;
}

int32_t printer_disconnect(void)
{
    if ((g_printer_open &(THREAD_CONNECTION | THREAD_STATUS)) == 0)
    {
        return PRINTER_NOT_PLUGGED;
    }

    stop_status_thread();

    if((g_printer_open & THREAD_CONNECTION) && ((int)NULL == (pthread_join(connection_thread, NULL))))
    {
        g_printer_open = (g_printer_open & (~ THREAD_CONNECTION )) ;

        if((g_printer_open & THREAD_STATUS) && ((int)NULL == (pthread_join(status_thread, NULL))))
        {
            g_printer_open = (g_printer_open & (~THREAD_STATUS)) ;
        }
    }

    return PRINTER_NOERROR;
}

int32_t printer_get_status(int32_t* printer_status)
{
    if(g_bootmode == 1)
    {
        return PRINTER_ERROR;
    }

    pthread_mutex_lock(&g_sem_status);

    (*printer_status) = g_status;

    pthread_mutex_unlock(&g_sem_status);

    return PRINTER_NOERROR;
}

int printer_get_serial_number(uint8_t* serial_no)
{
    uint8_t serial[2] = {0};
    int32_t status = 0, ret = 0;

    if(g_bootmode == 1)
    {
        return PRINTER_ERROR;
    }

    printer_get_status(&status);

    if ((status == PRINTER_ERROR) || (status == PRINTER_NOT_PLUGGED) || (status == PRINTER_SEND_ERR))
    {
        return status;
    }

    serial[0]=0x1B;
    serial[1]=0x7A;

    g_header[0]=0x1B;
    g_header[1]=0x7A;
    g_header_size = 2;

    ret = printer_send_custom_data(serial,2,1,14);

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    pthread_mutex_lock(&g_sem_send); // parasoft-suppress BD-TRS-DLOCK "Mutex being unlocked from another thread"
    memcpy(serial_no,g_receive_buffer+2,12);
    pthread_mutex_unlock(&g_sem_send);

    if (g_timeout == 1)
    {
        g_timeout = 0;
        return PRINTER_ERROR;
    }

    return PRINTER_NOERROR;
} // parasoft-suppress BD-TRS-LOCK "Mutex being unlocked from another thread"

int printer_write_serial_number(uint8_t* serial_num)
{
    uint8_t serial[15];
    int32_t status = 0, ret = 0, i = 0;

    if(g_bootmode == 1)
    {
        return PRINTER_ERROR;
    }

    printer_get_status(&status);

    if ((status == PRINTER_ERROR) || (status == PRINTER_NOT_PLUGGED) || (status == PRINTER_SEND_ERR))
    {
        return status;
    }

    serial[0]=0x1B;
    serial[1]=0x79;
    serial[2]=0x0C;

    if (strlen(serial_num) > 12)
    {
        return PRINTER_BAD_SERIAL;
    }

    for(i=0; i<12; i++)
    {
        serial[i+3] = serial_num[i];
    }

    ret = printer_send_custom_data(serial,15,0,0);

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    return PRINTER_NOERROR;
} // parasoft-suppress BD-TRS-LOCK "Mutex being unlocked from another thread"

int32_t printer_print_bitmap(uint8_t* receipt_bitmap, uint64_t bitmap_size)
{
    uint32_t bitmap_height = 0 ,bitmap_width = 0;
    uint64_t size = 0, size_aux = 0 ,offset = 0;
    uint8_t* raw_data = NULL;
    uint8_t* buf = NULL;
    uint8_t* compress = NULL;
    uint8_t* buf2 =  NULL;
    int32_t leng = 0, pt_buf2 = 0, size2 = 0, ret = 0;
    int32_t status = 0;
    uint8_t comp[5];


    if(g_bootmode == 1)
    {
        return PRINTER_ERROR;
    }

    printer_get_status(&status);

    if ((status != PRINTER_NOERROR) && (status != PRINTER_PAPER_LOW))
    {
        return status;
    }


    offset = convert_bmp_to_raw(receipt_bitmap,&raw_data,&bitmap_width,&bitmap_height);
#ifdef ENABLE_PRINT
    printf("convert_bmp_to_raw : %d\n", offset);
#endif

    if ( bitmap_size != ( offset + (bitmap_width * bitmap_height)))
    {
        if (raw_data != NULL )
        {
            free(raw_data);
            raw_data = NULL ;
        }

        return PRINTER_BAD_FILE_FORMAT;
    }

    size = bitmap_width * bitmap_height;
    buf = (uint8_t*)malloc(bitmap_width);

    if (buf == NULL)
    {
        if (raw_data != NULL )
        {
            free(raw_data);
            raw_data = NULL ;
        }

        return PRINTER_MEMORY_ERR;
    }

    buf2 = (uint8_t*)malloc(size+(bitmap_height*5));

    if (buf2 == NULL)
    {
        if (raw_data != NULL)
        {
            free(raw_data);
            raw_data = NULL;
        }

        free(buf);
        buf = NULL;

        return PRINTER_MEMORY_ERR;
    }

    memset(buf2,0x00,size+(bitmap_height*5));
    size_aux = 0;
    pt_buf2 = 0;
    size2 = 0;

    if(raw_data == NULL)
    {
        free(buf2);
        buf2 = NULL;

        free(buf);
        buf = NULL;

        return PRINTER_MEMORY_ERR;
    }


    if(( g_printing_size == 0)&&(g_last_bitmap_width != bitmap_width))
    {
        // set the printer size
        comp[0]=0x1B;
        comp[1]=0xCD;
        comp[2]=0x01;
        comp[3]=0x43;
        comp[4]=bitmap_width;
        ret = printer_send_custom_data(comp,5,0,0);
        g_last_bitmap_width = bitmap_width ;
#ifdef ENABLE_PRINT
        printf( "==> Bitmap width = %d\n" ,bitmap_width );
#endif
    }

    while(size_aux < size)
    {
        memset(buf,0x00,bitmap_width);
        memcpy(buf,raw_data+size_aux,bitmap_width);

        leng = white_compression(buf,&compress, bitmap_width);

        if(leng == PRINTER_MEMORY_ERR)
        {
            return PRINTER_MEMORY_ERR;
        }

        size2+=leng;

        memcpy(buf2+pt_buf2,compress,leng);

        free(compress);
        compress = NULL;

        size_aux+=bitmap_width;
        pt_buf2+=leng;
    }

    if (raw_data != NULL)
    {
        free(raw_data);
        raw_data = NULL;
    }


#ifdef ENABLE_PRINT
    printf("printer_send_custom_data : %d\n",size2 );
#endif

    ret = printer_send_custom_data(buf2,size2,0x80,0);
    {
        pthread_mutex_lock(&g_sem_printstatus);
        // End of printing detection
        //  2 - Next Ticket number set to firmware
        g_bitmap_id++;
        g_last_error.bmp_height = bitmap_width;	//must interrested width information
        g_last_error.in_counter = bitmap_height;
        g_last_error.out_counter =0;
        g_last_error.bitmap_id = (g_bitmap_id & 0x7F) ;
        g_wait_printend = 1 ;  // wait for end printing
        pthread_mutex_unlock(&g_sem_printstatus);
    }

    if (buf2 != NULL)
    {
        free(buf2);
        buf2 = NULL;
    }

    if (buf != NULL)
    {
        free(buf);
        buf = NULL;
    }

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    // Increment print counter
    if(g_print_counter < UNSIGNED_32_BIT_VAL)
    {
        ++g_print_counter;

        write_print_counter();
    }

    return PRINTER_NOERROR;
}

int32_t printer_cut_paper(void)
{
    uint8_t cut[2] ;
    int32_t ret = 0;
    int32_t status = 0;

    if(g_bootmode == 1)
    {
        return PRINTER_ERROR;
    }

    printer_get_status(&status);

    if ((status & (PRINTER_NOT_PLUGGED | PRINTER_COVER_OPEN | PRINTER_PAPER_OUT )) != 0)
    {
        return status;
    }

    cut[0]=0x1B;
    cut[1]=0x69;

    ret = printer_send_custom_data(cut,2,0x80,0);

    {
        pthread_mutex_lock(&g_sem_printstatus);
        // 3 - Next Ticket index for cut end
        g_bitmap_id ++;
        g_last_error.bitmap_id = (g_bitmap_id & 0x7F) ;
        g_wait_cutend = 1 ;
        pthread_mutex_unlock(&g_sem_printstatus);
    }

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    return PRINTER_NOERROR;

} // parasoft-suppress BD-TRS-LOCK "Mutex being unlocked from another thread"

int32_t printer_line_feed(int32_t no_of_lines)
{
    int32_t count = 0, size = 0;
    int8_t cmd_data[10] = {0};
    int8_t *p_data = NULL, *p_idx = NULL;
    int32_t status = 0, ret = 0;

    if(g_bootmode == 1)
    {
        return PRINTER_ERROR;
    }

    printer_get_status(&status);

    if ((status != PRINTER_NOERROR) && (status != PRINTER_PAPER_LOW))
    {
        return status;
    }

    /*In order to send one block of data to the printer, determinate the size
      of the data block. The maximum number of line that can be feed in one
      command is 0x7F. Compute the number of command to send to reache the
      required number of line to feed.*/
    size = (no_of_lines / PRI_MAX_LINE_FEED_CMD) * strlen(PRI_LINE_FEED);

    if (no_of_lines % PRI_MAX_LINE_FEED_CMD)
    {
        size += strlen(PRI_LINE_FEED);
    }

    /*Allocation of the data.*/
    p_idx = p_data = (int8_t *)malloc(size);

    if (p_idx == NULL)
    {
        return PRINTER_MEMORY_ERR;
    }

    /*Fill the full line feed.*/
    strcpy(cmd_data, PRI_LINE_FEED);

    for (count = 0; count < (no_of_lines / PRI_MAX_LINE_FEED_CMD); count++)
    {
        memcpy(p_idx, cmd_data, strlen(cmd_data));  // parasoft-suppress PB-34 "Length of cmd_data will only be used as it is processed before memcpy"
        p_idx += strlen(cmd_data);
    }

    /*Fill the ultimate line feed.*/
    if (no_of_lines % PRI_MAX_LINE_FEED_CMD)
    {
        strcpy(cmd_data, PRI_LINE_FEED);
        PRI_REPLACE_LAST_BYTE_STR(cmd_data, no_of_lines % PRI_MAX_LINE_FEED_CMD);
        memcpy(p_idx, cmd_data, strlen(cmd_data));  // parasoft-suppress PB-34 "Length of cmd_data will only be used as it is processed before memcpy"
    }

    ret = printer_send_custom_data((uint8_t*)p_data,size,0x80,0);

    /*Free the memory allocated.*/
    if (p_data)
    {
        free(p_data);
    }

    return ret;
}

int32_t printer_get_firmware_version(uint8_t *firmware_version)
{
    uint8_t ini[1] = {0};
    int32_t status = 0, ret = 0;
    uint8_t len = 8 ;

    if(g_bootmode == 1)
    {
        return PRINTER_ERROR;
    }

    printer_get_status(&status);

    if (status == PRINTER_NOT_PLUGGED || status == PRINTER_SEND_ERR)
    {
        return status;
    }

    ini[0] = 0x17;
    g_header[0]=0x17;
    g_header_size = 1;

    ret = printer_send_custom_data(ini,1,1,10);

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    pthread_mutex_lock(&g_sem_send); // parasoft-suppress BD-TRS-DLOCK "Mutex being unlocked from another thread"

    if((g_receive_buffer[9] > 0x2F )&&(g_receive_buffer[9] < 0x7B) )
    {
        len = 9 ; // extra caractere
    }

    memcpy(firmware_version,g_receive_buffer+1,len);
    pthread_mutex_unlock(&g_sem_send);

    if (g_timeout == 1)
    {
        g_timeout = 0;
        return PRINTER_ERROR;
    }

    return PRINTER_NOERROR;
} // parasoft-suppress BD-TRS-LOCK "Mutex being unlocked from another thread"

int32_t printer_upgrade_firmware(uint8_t* firmware_data,int32_t firmware_data_size)
{
    int32_t ret = PRINTER_NOERROR;
    int32_t status = 0;

    printer_get_status(&status);

    if (status == PRINTER_NOT_PLUGGED || status == PRINTER_SEND_ERR)
    {
        return status;
    }

    // Switching printer to bootloader mode for upgrade
    ret = printer_switch_to_bootloader_mode();

    if(PRINTER_NOERROR != ret)
    {
        return PRINTER_ERROR;
    }

    usleep(50* 1000);
#ifdef ENABLE_PRINT
    printf("Error in printer_switch_to_bootloader_mode: %d\n", ret);
#endif

    // Calling firmware download API
    ret = printer_download_firmware(firmware_data, firmware_data_size);

#ifdef ENABLE_PRINT
    printf("printer_download_firmware: %d\n", ret);
#endif
    usleep(1000* 1000);
    // Switching printer back to normal mode
    ret = printer_reboot_to_normal_mode();
    usleep(1000* 1000);
#ifdef ENABLE_PRINT
    printf("printer_reboot_to_normal_mode: %d\n", ret);

    printf("Disconnecting printer\n");
#endif

    printer_disconnect();
    usleep(4000* 1000);
#ifdef ENABLE_PRINT
    printf("Connecting printer\n");
#endif

    printer_init_connect(g_app_status_callback, g_printing_size);

    return PRINTER_NOERROR;
} // parasoft-suppress BD-TRS-LOCK "Mutex being unlocked from another thread"

int32_t printer_get_counter_value(uint32_t* print_counter)
{
    *print_counter = g_print_counter;

    return PRINTER_NOERROR;
}

int32_t printer_get_last_error( T_PRI_LAST_ERROR *last_error )
{
    pthread_mutex_lock(&g_sem_printstatus);
    last_error->bmp_height = g_last_error.bmp_height;
    last_error->error_status = (g_last_error.error_status & ~(PRINTER_PRINT_END | PRINTER_CUT_END) ) ;

    if(last_error->error_status == 0 )
    {
        last_error->error_status = g_status ;
    }

    last_error->in_counter = g_last_error.in_counter;
    last_error->out_counter = g_last_error.out_counter;
    last_error->bitmap_id = g_last_error.bitmap_id ;
    pthread_mutex_unlock(&g_sem_printstatus);

    return PRINTER_NOERROR;
}

int32_t printer_discard_error(void)
{
    uint8_t ini[1] = {0};
    int32_t status = 0;

    ini[0] = 0x16;

    printer_get_status(&status);

    if (status == PRINTER_NOT_PLUGGED || status == PRINTER_SEND_ERR)
    {
        return status;
    }

    return PRINTER_NOERROR;
}

///////////// Printer Library internal functions /////////////

static int32_t file_read_params()
{
    unsigned int params_cnt = 0;
    unsigned int params = 0;
    int pos = 0;
    char param_name[PARAMS_LEN] = "\0";
    char param_value[PARAMS_LEN] = "\0";
    char line[READ_LINE_LEN] = "\0";
    char temp[PARAMS_LEN] = "\0";
    char file[] = PRINTER_PARAMS_FILE_PATH;
    // default values
    g_print_density = 0x15 ;
    g_current_limit = 0x00 ;
    g_max_printer_speed =SPEED_MM_S;

    FILE *fp = fopen(file, "r");

    if (fp != NULL)
    {
        while (fgets(line, sizeof line, fp) != NULL)
        {
            /********************check the line content************************/
            char ch = ':';
            char *ret_val = NULL;
            ret_val = strchr(line, ch);

            if(ret_val != NULL)
            {
                pos = ret_val-line;

                if(line[pos-1] != ' ')
                {
                    int i = 0;

                    while(*ret_val != '\0')
                    {
                        temp[i++] = *ret_val++;
                    }

                    temp[i] = '\0';
                    line[pos] = ' ';
                    line[pos + 1] = '\0';

                    strcat(line,temp);
                }
            }

            memset(temp, '\0', PARAMS_LEN);
            /******************************************************************/
            sscanf(line, "%100s : %100s", param_name, param_value);
            params = strtoul(param_value, NULL, 16);

            if(strcmp(param_value, "") == 0 && strcmp(param_name, "") != 0)
            {
#ifdef ENABLE_PRINT
                printf("LIB: No value for parameter(%X) in printer_params.txt file..\n", params);
#endif
                fclose(fp);
                return PRINTER_ERROR;
            }

            if(strcmp(param_name, "Print_density") == 0)
            {
                g_print_density = params;
                ++params_cnt;
#ifdef ENABLE_PRINT
                printf("LIB: Print density = %X\n", g_print_density);
#endif
            }

            else if(strcmp(param_name, "Current_Limit") == 0)
            {
                g_current_limit = params;
                ++params_cnt;
#ifdef ENABLE_PRINT
                printf("LIB: Current Limit = %X\n", g_current_limit);
#endif
            }

            else if(strcmp(param_name, "Maximum_printer_speed") == 0)
            {
                g_max_printer_speed = params;
                ++params_cnt;
#ifdef ENABLE_PRINT
                printf("printer speed = %X\n", g_max_printer_speed);
#endif
            }

            else
            {
#ifdef ENABLE_PRINT
                printf("LIB: Wrong parameter name(%s) in printer_params.txt file..\n", param_name);
#endif
                fclose(fp);
                return PRINTER_ERROR;
            }

            memset(line, 0 , sizeof line);
            memset(param_name, 0, PARAMS_LEN);
            memset(param_value, 0, PARAMS_LEN);
            params = 0;
        }//while

        fclose(fp);

        if(params_cnt != 3)
        {
#ifdef ENABLE_PRINT
            printf("LIB: Missing some parameters in file..!%d\n", params_cnt);
#endif
            return PRINTER_ERROR;
        }
    }//if
    else
    {
#ifdef ENABLE_PRINT
        printf("LIB: printer config(%s) file not found..\n", file);
#endif
        return PRINTER_ERROR;
    }

    return PRINTER_NOERROR;
}

static int32_t printer_send_data(int32_t i_com_port, uint8_t* i_send_buffer, int32_t i_send_size, int32_t* io_pint32_data_send_status, int32_t i_data_to_send)
{
    int32_t remain;
    int32_t total;
    int32_t written;
    int32_t timeout;
    struct timeval time1, time2;
    int32_t ret_val1 = 0;
    int32_t ret_val2 = 0;
    uint8_t rec_status[4] = {0};
    uint8_t discard[1] = {0};

    int32_t chunk_size= BUFFER_SIZE;//  BUFFER_SIZE lines write
    int32_t write_length;

    remain = i_send_size;
    total = 0;
    written = 0;
    timeout = 0;
    discard[0]=0x16;

    gettimeofday(&time1, NULL);

    while((remain > 0) &&  (timeout == 0))
    {
        // Flushing the input buffer
        tcflush(i_com_port, TCIFLUSH);
        usleep(100);
        ret_val1 = read(i_com_port, rec_status, 4);

        if (((rec_status[0] & 0x80) == 0x80) && (rec_status[0] & 0x6E) && (i_data_to_send & 0x80))
        {
            convert_status(rec_status[0],io_pint32_data_send_status);

            if ((rec_status[0] & 0xC0) == 0xC0)
            {
                write(i_com_port, discard,1);
            }

#ifdef ENABLE_PRINT
            printf(" Send RStatus = %X",rec_status[0] );
#endif
            ret_val2 = -1;
            break;

        }

        //FR quick test
        if(remain > chunk_size)
        {
            write_length = chunk_size;
        }
        else
        {
            write_length = remain;
        }

        written = write(i_com_port, i_send_buffer+total, write_length);

        if (written > 0)
        {
            gettimeofday (&time1, NULL);
        }

        remain=remain-written;
        total+=written;
        gettimeofday (&time2, NULL);

        if((time2.tv_sec - time1.tv_sec) > OUR_TIMEOUT2)
        {
            timeout = 1;
            ret_val2 = -1;
        }
    }

    if ((timeout == 1) && (i_data_to_send & 0x80))
    {
        *io_pint32_data_send_status = ( *io_pint32_data_send_status | PRINTER_SEND_TIMEOUT );
    }

    return ret_val2;
}

static void printer_receive_data(int32_t i_com_port, uint8_t* i_header, int32_t i_header_size, uint8_t* io_receive_buffer, int32_t i_receive_size, uint8_t* io_timeout)
{
    int32_t counter = 0;
    struct timeval time1, time2;
    uint8_t rec_buf[25] = {0};
    int32_t receive = 0;
    int32_t ret = 0;
    int32_t i = 0;

    counter = 0;
    gettimeofday (&time1, NULL);

    do
    {
        ret = read(i_com_port, rec_buf,4);

        if (ret > 0)
        {
            gettimeofday (&time1, NULL);

            for(i=0; i<i_header_size; i++)
            {
                if (rec_buf[i] == i_header[i])
                {
                    counter = 0;

                    if(i == (i_header_size-1))
                    {
                        receive = 1;
                    }
                }
                else
                {
                    counter++ ;
                    break;
                }
            }
        }

        gettimeofday (&time2, NULL);

        if((time2.tv_sec - time1.tv_sec) > OUR_TIMEOUT)
        {
            *io_timeout = 1;
        }
    }
    while((receive == 0) && (*io_timeout == 0) && (counter < MAX_READ_ATTEMPT));

    if ( counter >= MAX_READ_ATTEMPT )
    {
#ifdef ENABLE_PRINT
        printf("Timeout occured\n");
#endif
        //reset
        memset(io_receive_buffer , 0 , RECEIVE_SIZE_MAX);
    }
    else if(*io_timeout == 0)
    {
        if (i_receive_size-4 > 0)
        {
            read(i_com_port,rec_buf+4,i_receive_size-4);
        }

        memcpy(io_receive_buffer,rec_buf,i_receive_size);
    }
}

static int32_t printer_init_synchronous(int32_t print_size)
{
    //int32_t status = 0;
    uint8_t init[1] = {0};
    uint8_t density[2] = {0};
    uint8_t peak[2] = {0};
    uint8_t size[5] = {0};
    uint8_t speed[5] = {0};
    //uint8_t count[2] = {0};
    int32_t ret = 0;

    int32_t send_status = 0;

#ifdef ENABLE_PRINT
    printf("In printer_init_synchronous\n");
#endif

    usleep(50*1000);  // parasoft-suppress BD-TRS-TSHL "This function is developed by Morpho to solve bug [TC4_SW-196]."

    // Initialize the printer
#ifdef ENABLE_PRINT
    printf("Initialize the printer\n");
#endif
    init[0]=0x16;
    ret = printer_send_data(com_port, init, 1, &send_status, 0x80);

    if ((ret != PRINTER_NOERROR) || (send_status != 0))
    {
        return -1;
    }

    usleep(50*1000);

    // Initialize the print density
#ifdef ENABLE_PRINT
    printf("Initialize the print density\n");
#endif
    density[0]=0x1E;
    density[1]=g_print_density;
    ret = printer_send_data(com_port, density, 2, &send_status, 0x80);

    if ((ret != PRINTER_NOERROR) || (send_status != 0))
    {
        return -1;
    }

    usleep(50*1000);

    // Initialize the current peak limit
#ifdef ENABLE_PRINT
    printf("Initialize the current peak limit\n");
#endif
    peak[0]=0x1C;
    peak[1]=g_current_limit;
    ret = printer_send_data(com_port, peak, 2, &send_status, 0x80);

    if ((ret != PRINTER_NOERROR) || (send_status != 0))
    {
        return -1;
    }

    usleep(50*1000);

    // Initialize the printer size
#ifdef ENABLE_PRINT
    printf("Initialize the printer size\n");
#endif
    g_last_bitmap_width = ((print_size != 0) ? print_size : 80);
    size[0]=0x1B;
    size[1]=0xCD;
    size[2]=0x01;
    size[3]=0x43;
    size[4]= g_last_bitmap_width;		// default
    ret = printer_send_data(com_port, size, 5, &send_status, 0x80);

    if ((ret != PRINTER_NOERROR) || (send_status != 0))
    {
        g_last_bitmap_width =0;
    }

    usleep(50*1000);

    // Initialize maximum speed of printer
#ifdef ENABLE_PRINT
    printf("Initialize maximum speed of printer\n");
#endif
    speed[0]=0x1B;
    speed[1]=0xCD;
    speed[2]=0x01;
    speed[3]=0x42;
    speed[4]= g_max_printer_speed;
    ret = printer_send_data(com_port, speed, 5, &send_status, 0x80);

    if ((ret != PRINTER_NOERROR) || (send_status != 0))
    {
        return -1;
    }

    usleep(50*1000);
    /*
        //Reset counter
        count[0]=0x1B;
        count[1]=0x51;		// ESC + 'Q'
        ret = printer_send_data(com_port, count, 2, &send_status, 0x80);

        usleep(50*1000);
        //  1 - Current Ticket number set to firmware
        size[0]=0x1B;
        size[1]=0xCD;
        size[2]=0x01;
        size[3]=0x6B;
        size[4]= g_bitmap_id;
        ret = printer_send_data(com_port, size, 5, &send_status, 0x80);
        // Clear Last error
         usleep(50*1000);
    */    memset(&g_last_error ,0, sizeof( T_PRI_LAST_ERROR ));
    return PRINTER_NOERROR;
}

static void* status_thread_routine(void *arg)  // parasoft-suppress METRICS-35 "Function is easy to understand and so there is no need to further divide into more functions."
{
    int32_t ret = 0;
    int8_t connected = -1;
    int32_t status = 0;
    int32_t status_prev = -1;
    uint8_t init = 0;
    uint8_t rec_status[4] = {0};
    uint8_t rec_buf[25] = {0};
    int32_t counter = 0;
    uint8_t discard[1] = {0};
    uint8_t esc_q[2] = {0};
    uint8_t print_id[5] = {0};
    int32_t ret_val = 0;
    int32_t ret_val_1 = 0;
    uint32_t elapse = 0 ;

    struct timeval time1, time2,timetrf;
    struct termios term_options;
    struct timeval temp_time;

    /* The format will be <ESC><'Q'><MSB in><LSB in><MSB out><LSB out> for firmware version greater than 79 */
    esc_q[0]=0x1B;
    esc_q[1]=0x51;		// ESC + 'Q'

    discard[0]=0x16;
    g_data_send_status = 0 ;

    print_id[0]=0x1B;
    print_id[1]=0xCD;
    print_id[2]=0x01;
    print_id[3]=0x6B;
    print_id[4]= g_bitmap_id;

    com_port = open(DEV_FILE_PRINTER, O_RDWR);

    if(-1 == com_port)
    {
#ifdef ENABLE_PRINT
        printf("Error in opening printer device FD. Line:%d\n",__LINE__);
#endif
        connected = 0;
        g_printer_open = (g_printer_open & (~THREAD_STATUS));
        return NULL;
    }

#ifdef ENABLE_PRINT
    printf("LIB: chunk_size = %04d\n", BUFFER_SIZE);
#endif
    g_timeout = 0;
    gettimeofday (&time1, NULL);
    gettimeofday (&temp_time, NULL);

    /* Get control parameters of UART */
    ret_val = tcgetattr(com_port, &term_options);

    if(0 > ret_val)
    {
#ifdef ENABLE_PRINT
        printf("UsbOpen: Unable to get control parameters of UART.\n");
#endif
        g_printer_open = (g_printer_open & (~THREAD_STATUS));
        return NULL;
    }

    /* Configure as RAW mode */
    cfmakeraw(&term_options);

    term_options.c_cc[VEOF] =1 ;

    /*  Set control parameters of UART */
    ret_val = tcsetattr(com_port, TCSANOW, &term_options);

    if(0 > ret_val)
    {
#ifdef ENABLE_PRINT
        printf("UsbOpen: Unable to set control parameters of UART.\n");
#endif
        g_printer_open = (g_printer_open & (~THREAD_STATUS));
        return NULL;
    }

    if(cfsetispeed(&term_options, B115200) < 0 || cfsetospeed(&term_options, B115200) < 0)
    {
#ifdef ENABLE_PRINT
        printf("UsbOpen: Unable to set speed of UART.\n");
#endif
        g_printer_open = (g_printer_open & (~THREAD_STATUS));
        return NULL;
    }

    tcflush(com_port, TCIOFLUSH);

    g_status_callback_function(PRINTER_NOERROR);

    pthread_mutex_lock(&g_sem_status);

    g_status = PRINTER_NOERROR;

    pthread_mutex_unlock(&g_sem_status);

    while(1)
    {
        pthread_mutex_lock(&g_sem_write);

        if(need_printer_init)
        {
            ret_val = printer_init_synchronous(g_printing_size);

            if(0 > ret_val)
            {
#ifdef ENABLE_PRINT
                printf("FW Not Ready\n");
#endif
                usleep(50*1000);
            }
            else
            {
                need_printer_init = 0;	// Done
            }
        }

        if ((g_data_to_send & 0x7f) == 1)
        {
            gettimeofday (&timetrf, NULL);  //trace temps de tranfert
            printer_send_data(com_port,g_send_buffer,g_send_size,&g_data_send_status,g_data_to_send);

            free(g_send_buffer);
            g_data_to_send = 0;

            if((g_wait_printend == 1)||(g_wait_cutend == 1))
            {
                // send command update bitmap_id for end of print or cut
                usleep(50*1000);
                print_id[4]= g_last_error.bitmap_id;
                ret_val_1 = write(com_port, print_id,5);
            }

            if( (g_data_send_status & PRINTER_SEND_TIMEOUT) == PRINTER_SEND_TIMEOUT )
            {
                g_data_to_receive = 0;
            }

            if (g_data_to_receive == 1)
            {
                printer_receive_data(com_port,g_header,g_header_size,g_receive_buffer,g_receive_size,&g_timeout);
                g_data_to_receive = 0;
                pthread_mutex_unlock(&g_sem_send);
            }

            pthread_mutex_unlock(&g_sem_read);
        }

        pthread_mutex_unlock(&g_sem_write);

        // For printing end detection
        if(g_wait_printend == 1)
        {
            counter =1 ;
            tcflush(com_port, TCIFLUSH);
            usleep(1*100);
            gettimeofday (&time1, NULL);

            do
            {
                counter++;
                ret_val_1 = read(com_port, rec_status,4);

                if (rec_status[3] == g_last_error.bitmap_id) // this only to know when firmware finish printing
                {
                    if ((rec_status[0] & 0x80) == 0x80)
                    {
                        convert_status(rec_status[0],&status);

                        if ((status & PRINTER_BUFFER_FULL) == PRINTER_BUFFER_FULL)
                        {
                            ret_val_1 = write(com_port, discard,1);
                        }
                    }

                    counter =0;
                    break;
                }

                gettimeofday (&temp_time, NULL);

            }
            while((temp_time.tv_sec - time1.tv_sec) < PRINT_WAIT_DUR);

            //print end
            pthread_mutex_lock(&g_sem_printstatus);
            g_data_send_status |= (PRINTER_PRINT_END | status) ;
            g_last_error.error_status = g_data_send_status ;
            g_wait_printend = 0;
            pthread_mutex_unlock(&g_sem_printstatus);

#ifdef ENABLE_PRINT
            printf(" PRINT t=%d count=%d Id=%X (%X)\n",(int )(temp_time.tv_sec - time1.tv_sec) ,counter ,rec_status[3],g_last_error.bitmap_id);
#endif
            g_status_callback_function(g_data_send_status);
            status_prev =( g_data_send_status & 0x7F);
            g_data_send_status = 0;
#ifdef ENABLE_PRINT
            elapse = (((temp_time.tv_sec - timetrf.tv_sec)*1000) +   ((temp_time.tv_usec - timetrf.tv_usec)/1000)) ;
            printf("==> Tranfert %dms %dlignes [Vap= %dmm/s   param= %dmm/s] \n",elapse,g_last_error.in_counter, ((g_last_error.in_counter*1000/8)/elapse) ,g_max_printer_speed);
#endif
        }        // For cut end detection
        else if(g_wait_cutend == 1)
        {
            counter =0 ;
            tcflush(com_port, TCIFLUSH);
            usleep(1*1000);
            gettimeofday (&time1, NULL);

            do
            {
                counter++;
                ret_val_1 = read(com_port, rec_status,4);

                if (rec_status[3] == g_last_error.bitmap_id) // this only to know when firmware finish printing
                {
                    if ((rec_status[0] & 0x80) == 0x80)
                    {
                        convert_status(rec_status[0],&status);

                        if ((status & PRINTER_BUFFER_FULL) == PRINTER_BUFFER_FULL)
                        {
                            ret_val_1 = write(com_port, discard,1);
                        }
                    }

                    counter =0;
                    break;
                }

                gettimeofday (&temp_time, NULL);

            }
            while((temp_time.tv_sec - time1.tv_sec) < CUT_WAIT_DUR);

#ifdef ENABLE_PRINT
            printf(" CUT 0 t=%d count=%d Id=%X (%X)\n",(int )(temp_time.tv_sec - time1.tv_sec) ,(uint16_t)counter ,rec_status[3],g_last_error.bitmap_id);
#endif
            gettimeofday (&time1, NULL);
            counter =0;
            tcflush(com_port, TCIOFLUSH);
            usleep(1*1000);
            write(com_port, esc_q,2);

            do
            {
                counter++ ;
                ret = read(com_port, rec_buf,6);

                if ((rec_buf[0] == esc_q[0]) && (rec_buf[1] == esc_q[1]))
                {
                    //counter = 0;
                    break;
                }

                gettimeofday (&temp_time, NULL);
            }
            while( counter < ESCQ_RESPONSE );

            pthread_mutex_lock(&g_sem_printstatus);
            g_data_send_status |= (PRINTER_CUT_END | status) ;
            g_last_error.error_status = g_data_send_status ;

            if(rec_buf[0] == esc_q[0])
            {
                g_last_error.in_counter = (((rec_buf[2] & 0xFF) << 8) | (rec_buf[3]&0xFF) );
                g_last_error.out_counter =(((rec_buf[4] & 0xFF) << 8) | (rec_buf[5]&0xFF) );
            }

            g_wait_cutend = 0;
            pthread_mutex_unlock(&g_sem_printstatus);
#ifdef ENABLE_PRINT
            printf(" CUT 1 t=%d count= %d s=%X\ in=%X\n",(int )(temp_time.tv_sec - time1.tv_sec) , (uint16_t)counter, rec_buf[0],g_last_error.in_counter );
#endif
            g_status_callback_function(g_data_send_status);
            status_prev =( g_data_send_status & 0x7F );
            g_data_send_status = 0;
        }

        if(1)
        {
            ret_val_1 =tcflush(com_port, TCIFLUSH);

            if((ret_val_1 < 0 ) && (errno == EIO ))
            {
#ifdef ENABLE_PRINT
                printf("tcflush return =%d errno =%d \n" ,ret_val_1,errno);
#endif
                printf("Printer is Disconnected ! \n");
                stop_status_thread();
            }

            usleep(1*1000);

            if (connected != 1)
            {
                g_status_callback_function(PRINTER_CONNECTED);
                g_printer_plugged = 1;
                connected = 1;
            }

            ret_val_1 = read(com_port, rec_status,4);

            if(ret_val_1 < 0 )
            {
#ifdef ENABLE_PRINT
                printf("Before stop_status_thread \n");
#endif
//               stop_status_thread();
            }

            if ((rec_status[0] & 0x80) == 0x80)
            {

                convert_status(rec_status[0],&status);

                if ((status & PRINTER_BUFFER_FULL) == PRINTER_BUFFER_FULL)
                {
                    ret_val_1 = write(com_port, discard,1);

                }

                if ((status_prev != -1) &&
                        ((status_prev & PRINTER_COVER_OPEN) == PRINTER_COVER_OPEN) &&
                        ((status & PRINTER_COVER_OPEN) != PRINTER_COVER_OPEN))
                {
                    status = PRINTER_COVER_CLOSE | PRINTER_INITIALIZATION | (status & ~PRINTER_PAPER_OUT);

                    init = 1;

                    gettimeofday (&time1, NULL);
                    time2 = time1;
                }

                gettimeofday (&time2, NULL);

                if(((status & PRINTER_PAPER_OUT) == PRINTER_PAPER_OUT) && (init == 1))
                {
                    if( (time2.tv_sec - time1.tv_sec) >= TIMEOUT_INIT )
                    {
                        init = 0;
                    }
                    else
                    {
                        status = PRINTER_INITIALIZATION | (status & ~PRINTER_PAPER_OUT);
                    }
                }

                if ((init == 1) && ((status & PRINTER_INITIALIZATION) != PRINTER_INITIALIZATION))
                {
                    init = 0;
                }

                pthread_mutex_lock(&g_sem_status);
                g_status = status;
                pthread_mutex_unlock(&g_sem_status);

                if (status_prev != status)
                {
                    g_status_callback_function(status);
                    status_prev = status;

                }
            }

            /*            else
                        {
            		printf("Status= %x %x %x %x \n",rec_status[0],rec_status[1],rec_status[2],rec_status[3]);
            //		tcflush(com_port, TCIOFLUSH);
            //		memset(rec_status,0,4);
                        }
            */
        }
        else
        {
            if (connected != 0)
            {
                pthread_mutex_lock(&g_sem_status);
                g_status = PRINTER_NOT_PLUGGED;
                pthread_mutex_unlock(&g_sem_status);
                g_printer_plugged = 0;
                connected = 0;
                stop_status_thread();
#ifdef ENABLE_PRINT
                printf("Printer Disconnected");
#endif
            }

            status_prev = -1;
        }

        if (g_close_threads == 1)
        {
#ifdef ENABLE_PRINT
            printf("Thread Exit\n");
#endif
            g_status_callback_function(PRINTER_NOT_PLUGGED);
            pthread_mutex_lock(&g_sem_status);
            g_status = PRINTER_NOT_PLUGGED;
            pthread_mutex_unlock(&g_sem_status);
            connected = 0;
            g_printer_plugged = 0;
            close(com_port);
#ifdef ENABLE_PRINT
            printf("Thread Exit completed\n");
#endif
            break;
        }

        usleep(1 * 100);
    }

    g_printer_open = (g_printer_open & (~THREAD_STATUS));
    return NULL;
}

static void start_status_thread(status_callback internal_status_callback)
{
    g_status_callback_function = internal_status_callback;

    if(0 != pthread_create(&status_thread,NULL,status_thread_routine,NULL))
    {
#ifdef ENABLE_PRINT
        printf("Error in creating status thread. Line:%d\n",__LINE__);
#endif
    }
    else
    {
        g_printer_open = (g_printer_open | THREAD_STATUS) ;

    }
}

static void stop_status_thread(void)
{
    pthread_mutex_lock(&g_sem_close);
    g_close_threads = 1;
    pthread_mutex_unlock(&g_sem_close);
}

static int32_t printer_send_custom_data(uint8_t* data_buf, int32_t size, int32_t receive, int32_t rec_size)
{
    if (g_printer_plugged != 1)
    {
        return PRINTER_NOT_PLUGGED;
    }

    pthread_mutex_lock(&g_sem_read); // parasoft-suppress BD-TRS-DLOCK "Mutex being unlocked from another thread"

    pthread_mutex_lock(&g_sem_write);

    g_send_buffer = (uint8_t*)malloc(size); // parasoft-suppress BD-RES-LEAKS "g_send_buffer is freed in other thread."

    if(NULL != g_send_buffer)
    {
        memset(g_send_buffer,0x00,size);
        memcpy(g_send_buffer,data_buf,size);
        g_send_size = size;
        g_data_to_send = (receive | 1);

        if ((receive & 0x7f) == 1)
        {
            g_data_to_receive = 1;
            g_receive_size = rec_size;
            pthread_mutex_lock(&g_sem_send);
        }
    }

    pthread_mutex_unlock(&g_sem_write);
    return PRINTER_NOERROR;
}

static int32_t convert_status(uint8_t input_status,int32_t* output_status)
{
    //initialisation
    (*output_status) = PRINTER_NOERROR;

    if (input_status == 0x80)
    {
        return 0;
    }
    else
    {
        if ((input_status & 0x81) == 0x81)
        {
            (*output_status) = (*output_status) | PRINTER_PAPER_LOW;
        }

        if ((input_status & 0x82) == 0x82)
        {
            (*output_status) = (*output_status) | PRINTER_PAPER_OUT;
        }

        if ((input_status & 0x84) == 0x84)
        {
            (*output_status) = (*output_status) | PRINTER_TEMP_TOOHOT;
        }

        if ((input_status & 0x88) == 0x88)
        {
            (*output_status) = (*output_status) | PRINTER_COVER_OPEN;
        }

        if ((input_status & 0x90) == 0x90)
        {
            (*output_status) = (*output_status) | PRINTER_CUTTER_ERR;
        }

        if ((input_status & 0xC0) == 0xC0)
        {
            (*output_status) = (*output_status) | PRINTER_BUFFER_FULL;
        }

        return 0;
    }
}

static uint64_t convert_bmp_to_raw(uint8_t* bitmap_data, uint8_t** raw_data, uint32_t*  output_width, uint32_t* output_height)
{
    BITMAP bitmap_str;
    uint8_t* buf = NULL, *buf_read = NULL;
    uint32_t offset = 0, size = 0;
    int32_t i = 0, k = 0;

    memcpy(&bitmap_str,bitmap_data,sizeof(BITMAP));

    offset = bitmap_str.m_x_Fheader.bfOffBits;
    buf =bitmap_data+offset;

    (*output_width) = (bitmap_str.m_x_Iheader.biWidth)/8;
    (*output_height) = bitmap_str.m_x_Iheader.biHeight;
    size = (*output_width)*(*output_height);

    if (bitmap_str.m_x_Fheader.bfSize != (size + offset) )
    {
        return PRINTER_BAD_FILE_FORMAT;
    }

    // test bitmap Width must be multiple of 8 bits or g_printing_size
    if(bitmap_str.m_x_Iheader.biWidth % 8)
    {
        return PRINTER_BAD_FILE_FORMAT ;
    }

    //Min and max printing data size 24 to 640 dots
    if ((g_printing_size ==0 )&& (((bitmap_str.m_x_Iheader.biWidth /8) < 3 )||((bitmap_str.m_x_Iheader.biWidth /8) > 80 )))
    {
        return PRINTER_BAD_FILE_FORMAT;
    }

    //or must be printing data size 640 or 756
    if ((g_printing_size !=0 )&& ((bitmap_str.m_x_Iheader.biWidth /8) != g_printing_size ))
    {
        return PRINTER_BAD_FILE_FORMAT;
    }

    (*raw_data) = (uint8_t*)malloc(size);

    if ((*raw_data) == NULL)
    {
        return PRINTER_MEMORY_ERR;
    }

    k=0;

    for( i=((*output_height)-1) ; i>=0; i--)
    {
        buf_read = buf + (*output_width)*i;
        memcpy(((*raw_data)+(*output_width)*k),buf_read,(*output_width));
        k++;
    }

    return offset;
}

static int32_t white_compression(uint8_t* origin,uint8_t** compress, uint32_t width)
{
    int8_t aux[100] = {0};
    int32_t ind_tot = 0, ind = 0, compt = 0;
    int32_t count = 0;

    ind_tot=1;
    ind=0;
    compt=0;

    for (count = 0; count < width; count++)
    {
        compt++;

        if(origin[count]==0x00)
        {
            ind++;

            if (compt == width)
            {
                aux[ind_tot]=0;
                aux[ind_tot+1]=ind;
                ind_tot+=2;
                ind=0;
            }
        }
        else
        {
            if(ind >= 1)
            {
                aux[ind_tot]=0;
                aux[ind_tot+1]=ind;
                ind_tot+=2;
                ind=0;
            }

            aux[ind_tot]=origin[count];
            ind_tot+=1;
            ind=0;
        }
    }

    aux[0]=256-(ind_tot-1);

    if (ind_tot <= (width +1))
    {
        (*compress) = (uint8_t*)malloc((ind_tot)*sizeof(uint8_t));

        if (*compress!=NULL)
        {
            memcpy((*compress),aux,ind_tot);
            return (ind_tot);
        }
        else
        {
            return PRINTER_MEMORY_ERR;
        }
    }
    else
    {
        (*compress) = (uint8_t*)malloc((width + strlen(PRI_PRINT_LINE_BMP))*sizeof(uint8_t));

        if (*compress!=NULL)
        {
            // uncompressed line  with linefeed "\x1B\xCD\xXX\x08"
            (*compress)[0] = 0x1B ;
            (*compress)[1] = 0xCD ;
            (*compress)[2] = width ;
            (*compress)[3] = 0x08 ;

            memcpy((*compress)+strlen(PRI_PRINT_LINE_BMP),origin,width);
            return (width + strlen(PRI_PRINT_LINE_BMP));
        }
        else
        {
            return PRINTER_MEMORY_ERR;
        }
    }

}

static void update_boolean(int32_t i)
{
    if(i==0)
    {
        set_page_ok=0;
    }

    if(i==1)
    {
        page_data_ok=0;
    }
}

static int32_t set_page(int32_t page)
{
    uint8_t ini[5] = {0};
    int8_t i = 0x00,j = 0x00;
    int32_t ret = 0;

    while(page>0xFF)
    {
        page = page - 0x100;
        i = i + 0x01;
    }

    while(page>0)
    {
        page = page - 0x01;
        j = j + 0x01;
    }

    ini[0] = 0x1B;
    ini[1] = 0x4C;
    ini[2] = 0x50;
    ini[3] = j;
    ini[4] = i;

    g_header[0]=0x1B;
    g_header[1]=0x50;
    g_header_size = 2;

    /*Send directly the data to the printer.*/
    ret = printer_send_custom_data(ini,5,1,2);

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    pthread_mutex_lock(&g_sem_send);    // parasoft-suppress BD-TRS-DLOCK "Mutex being unlocked from another thread"
    pthread_mutex_unlock(&g_sem_send);

    if (g_timeout == 1)
    {
        g_timeout = 0;
        return PRINTER_ERROR;
    }
    else
    {
        update_boolean(0);
    }

    return PRINTER_NOERROR;
} // parasoft-suppress BD-TRS-LOCK "Mutex being unlocked from another thread"

static int32_t page_data(int16_t add, int8_t* data_buf)
{
    uint8_t* p_malloc = NULL;
    int32_t count = 0, ret = 0;

    p_malloc = malloc(add+5);

    if(p_malloc == NULL)
    {
        return PRINTER_MEMORY_ERR;
    }

    p_malloc[0] = 0x1B;
    p_malloc[1] = 0x4C;
    p_malloc[2] = 0x3A;
    p_malloc[3]=(char)(add>>16);
    p_malloc[4]=(char)add;

    g_header[0]=0x1B;
    g_header[1]=0x3A;
    g_header_size = 2;

    for(count = 5; count < 5 + add; count++)
    {
        p_malloc[count] = data_buf[count - 5];
    }

    /*Send directly the data to the printer.*/
    ret = printer_send_custom_data(p_malloc,133,1,2);

    if (ret != PRINTER_NOERROR)
    {
        free(p_malloc);

        return ret;
    }

    pthread_mutex_lock(&g_sem_send);    // parasoft-suppress BD-TRS-DLOCK "Mutex being unlocked from another thread"
    pthread_mutex_unlock(&g_sem_send);

    free(p_malloc);

    if (g_timeout == 1)
    {
        g_timeout = 0;
        return PRINTER_ERROR;
    }
    else
    {
        update_boolean(1);
    }

    return PRINTER_NOERROR;
} // parasoft-suppress BD-TRS-LOCK "Mutex being unlocked from another thread"

static int32_t update_program_checksum(void)
{
    uint8_t ini[3] = {0};
    int32_t ret = 0;
    int8_t g_puc_chaine[50] = {0};

    ini[0] = 0x1B;
    ini[1] = 0x4C;
    ini[2] = 0x55;

    g_header[0]=1;
    g_header[1]=2;
    g_header[2]=3;
    g_header[3]=4;
    g_header_size = 4;

    /*Send directly the data to the printer.*/
    ret = printer_send_custom_data(ini,3,1,10);

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    pthread_mutex_lock(&g_sem_send);    // parasoft-suppress BD-TRS-DLOCK "Mutex being unlocked from another thread"

    if ((g_receive_buffer[0] == 0x01)) // parasoft-suppress BD-PB-CC "rep is being changed in another thread"
    {
        if ((g_receive_buffer[1] == 0x02))
        {
            if ((g_receive_buffer[2] == 0x03))
            {
                if ((g_receive_buffer[3] == 0x04))
                {
                    if ((g_receive_buffer[4] == 0x05))
                    {
                        if ((g_receive_buffer[5] == 0x06))
                        {
                            if ((g_receive_buffer[6] == 0x07))
                            {
                                if ((g_receive_buffer[7] == 0x08))
                                {
                                    if ((g_receive_buffer[8] == 0x09))
                                    {
                                        if ((g_receive_buffer[9] == 0x00))
                                        {
                                            download_ok = 1;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    pthread_mutex_unlock(&g_sem_send);
    g_receive_buffer[10]='\0';
    sprintf(g_puc_chaine,"%s\n",g_receive_buffer);

    if (g_timeout == 1)
    {
        g_timeout = 0;
        return PRINTER_ERROR;
    }

    return PRINTER_NOERROR;
} // parasoft-suppress BD-TRS-LOCK "Mutex being unlocked from another thread"

static int32_t printer_reboot_to_normal_mode(void)
{
    uint8_t ini[1] = {0};
    int32_t ret = 0;

    if(g_bootmode == 0)
    {
        return PRINTER_ERROR;
    }

    ini[0] = 0x1A;

    ret = printer_send_custom_data(ini,1,0,0);

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    g_bootmode = 0;
    return PRINTER_NOERROR;
} // parasoft-suppress BD-TRS-LOCK "Mutex being unlocked from another thread"

static int32_t printer_switch_to_bootloader_mode(void)
{
    uint8_t ini[3] = {0};
    int32_t status = 0,ret = 0;

    if(g_bootmode == 1)
    {
        return PRINTER_ERROR;
    }

    printer_get_status(&status);

    if (status == PRINTER_NOT_PLUGGED || status == PRINTER_SEND_ERR)
    {
        return status;
    }

    boot_loader_mode = 0;

    ini[0] = 0x1B;
    ini[1] = 0x4C;
    ini[2] = 0x4A;

    g_header[0] = 0x1B;
    g_header[1] = 0x4C;
    g_header[2] = 0x4A;
    g_header_size = 3;

    /*Send directly the data to the printer.*/
    ret = printer_send_custom_data(ini,3,1,5);

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    pthread_mutex_lock(&g_sem_send);    // parasoft-suppress BD-TRS-DLOCK "Mutex being unlocked from another thread"

    if ((g_receive_buffer[3] == 0x4F) && (g_receive_buffer[4] == 0x4B)) // parasoft-suppress BD-PB-CC "rep is being modified from another thread"
    {
        boot_loader_mode = 1;
    }
    else
    {
        return PRINTER_ERROR;
    }

    pthread_mutex_unlock(&g_sem_send);

    if (g_timeout == 1)
    {
        g_timeout = 0;
        return PRINTER_ERROR;
    }

    g_bootmode = 1;
    return PRINTER_NOERROR;
} // parasoft-suppress BD-TRS-LOCK "Mutex being unlocked from another thread"

static int32_t printer_download_firmware(uint8_t* firmware_data,int32_t data_size)
{
    int32_t i = 1, j = 0;
    int32_t end = 0;
    int8_t page[128] = {0};
    int32_t ret = 0;

    if(g_bootmode == 0)
    {
        return PRINTER_ERROR;
    }

    download_ok=0;

    end=data_size/128;

    if (end % 128)
    {
        end = end + 1;
    }

    set_page_ok=1;
    page_data_ok=1;

    while(end)
    {
        set_page(0x0000 + i -1);

        j=0;

        while(set_page_ok)
        {
            j++;

            if(j == 100)
            {
                return  PRINTER_DOWNLOAD_ERR;
            }
        }

        set_page_ok=1;

        memcpy(page,firmware_data+((i-1)*0x0080),128);
        page_data(0x80,page);
        j=0;

        while(page_data_ok)
        {
            j++;

            if(j == 100)
            {
                return  PRINTER_DOWNLOAD_ERR;
            }
        }

        page_data_ok=1;
        i++;
        end--;
    }

    ret = update_program_checksum();

    if (ret != PRINTER_NOERROR)
    {
        return PRINTER_DOWNLOAD_ERR;
    }

    i=0;

    if (download_ok)
    {
        download_ok=0;
        return PRINTER_NOERROR;
    }
    else
    {
        return PRINTER_DOWNLOAD_ERR;
    }
}

static void* printer_connection_thread_routine(void *arg)
{
    int32_t prev_status = PRINTER_NOERROR, status = PRINTER_NOERROR;
    int32_t res = 0;

    while(1)
    {
        printer_get_status(&status);

        if(prev_status != status)
        {
#ifdef ENABLE_PRINT
            printf("Printer status changed: Previous status: %x, Current status: %x\n", prev_status, status);
#endif
        }

        if(1 == g_close_threads)
        {
#ifdef ENABLE_PRINT
            printf("Disconnection requested\n");
#endif
            break;
        }

        if (PRINTER_NOT_PLUGGED == status)
        {
            res = printer_open(printer_statuscb);
#ifdef ENABLE_PRINT
            printf("printer_open Res = %d | Line = %d\n",res,__LINE__);
#endif
            usleep(INIT_SLEEP);

            res = printer_init(g_printing_size);
#ifdef ENABLE_PRINT
            printf("printer_init Res = %d | Line = %d\n",res,__LINE__);
#endif

            if(res == PRINTER_NOT_PLUGGED)
            {
                g_status_callback_function(PRINTER_NOT_PLUGGED);

                printer_disconnect();
            }

            usleep(INIT_SLEEP);
        }
        else
        {
            sleep(CONN_MONITOR_SLEEP);
        }
    }

    g_printer_open = (g_printer_open & (~ THREAD_CONNECTION )) ;
    return NULL;
}


static int32_t printer_connection(void)
{
    if(0 != pthread_create(&connection_thread,NULL,printer_connection_thread_routine,NULL))
    {
#ifdef ENABLE_PRINT
        printf("Error in creating Connection thread. Line:%d\n",__LINE__);
#endif
    }
    else
    {
        g_printer_open = (g_printer_open | THREAD_CONNECTION) ;
    }

    return PRINTER_NOERROR;
}

static int32_t printer_open(status_callback internal_status_callback)
{
    g_data_to_send = 0;
    g_data_to_receive = 0;
    g_close_threads = 0;
    g_wait_printend = 0;
    g_wait_cutend =0;

    start_status_thread(internal_status_callback);

    return PRINTER_NOERROR;
}

static int32_t printer_statuscb(int32_t status)
{
#ifdef ENABLE_PRINT
    printf("Lib: Printer status :%x\n",status);
#endif

    if((status & PRINTER_COVER_CLOSE) == PRINTER_COVER_CLOSE)
    {
        need_printer_init = 1;
#ifdef ENABLE_PRINT
        printf("----> SET need_printer_init = 1\n");
#endif
    }

    if(NULL != g_app_status_callback)
    {
        g_app_status_callback(status);
    }

    return 0;
}

static int32_t printer_init(int32_t print_size)
{
    int32_t status = 0;
    uint8_t init[1] = {0};
    uint8_t density[2] = {0};
    uint8_t peak[2] = {0};
    uint8_t size[5] = {0};
    uint8_t speed[5] = {0};
    uint8_t count[2] = {0};
    int32_t ret = 0;

    if(g_bootmode == 1)
    {
        return PRINTER_ERROR;
    }

    printer_get_status(&status);

    if ((status == PRINTER_ERROR) || (status == PRINTER_NOT_PLUGGED) || (status == PRINTER_SEND_ERR))
    {
        return status;
    }

    // Initialize the printer  discard error
    init[0]=0x16;
    ret = printer_send_custom_data(init,1,0,0);

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    // Initialize the print density
    density[0]=0x1E;
    density[1]=g_print_density;
    ret = printer_send_custom_data(density,2,0,0);

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    // Initialize the current peak limit
    peak[0]=0x1C;
    peak[1]=g_current_limit;

    ret = printer_send_custom_data(peak,2,0,0);

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    // Initialize the printer size
    g_last_bitmap_width = ((print_size != 0) ? print_size : 80);
    size[0]=0x1B;
    size[1]=0xCD;
    size[2]=0x01;
    size[3]=0x43;
    size[4]= g_last_bitmap_width;		// default
    ret = printer_send_custom_data(size,5,0,0);

    if (ret != PRINTER_NOERROR)
    {
        g_last_bitmap_width =0;
    }

    // Initialize maximum speed of printer
    speed[0]=0x1B;
    speed[1]=0xCD;
    speed[2]=0x01;
    speed[3]=0x42;
    speed[4]= g_max_printer_speed;

    ret = printer_send_custom_data(speed,5,0,0);

    if (ret != PRINTER_NOERROR)
    {
        return ret;
    }

    /*
        count[0]=0x1B;
        count[1]=0x51;		// ESC + 'Q'
        //Reset counter
     */
    ret = printer_send_custom_data(count,2,0,0);
    //  1 - Current Ticket number set to firmware
    size[0]=0x1B;
    size[1]=0xCD;
    size[2]=0x01;
    size[3]=0x6B;
    size[4]= g_bitmap_id;
    ret = printer_send_custom_data(size,5,0,0);
    // Clear Last error
    memset(&g_last_error ,0, sizeof( T_PRI_LAST_ERROR ));

    return PRINTER_NOERROR;
}

static int32_t printer_dev_reset()
{
    struct usb_bus *busses;
    struct usb_bus *bus;
    struct usb_device *deviceTemp;
    struct usb_dev_handle  *printer_handle = NULL;
    int32_t ret = 0;
    int32_t result = 0;

    usb_init();
    usb_find_busses();
    usb_find_devices();
    busses = usb_get_busses();

    for (bus = busses; bus; bus = bus->next)
    {
        for (deviceTemp = bus->devices; deviceTemp; deviceTemp = deviceTemp->next)
        {
            if ((deviceTemp->descriptor.idVendor == PRINTER_VENDOR_ID) && (deviceTemp->descriptor.idProduct == PRINTER_PROD_ID))
            {
                printer_handle = usb_open(deviceTemp);

                if (printer_handle)
                {
                    ret = usb_reset(printer_handle);

                    if (ret < 0)
                    {
                        result = -1;
                    }

                    /* usb_close is not required for printer_handle because device is reset and handle no longer exist */
                }
                else
                {
                    result = -1;
                }
            }
        }
    }

    return result;
}

static uint32_t read_print_counter()
{
    uint32_t counter = 0;
    FILE *fp = NULL;

    fp = fopen(PRINT_CTR_FILE, "rb");

    if(NULL == fp)
    {
#ifdef ENABLE_PRINT
        printf("No print counter file found\n");
#endif
        counter = 0;
    }
    else
    {
        // Read counter
        fread(&counter, sizeof(uint32_t), 1, fp);
        fclose(fp);
    }

    return counter;
}

static void write_print_counter()
{
    uint32_t counter = g_print_counter;
    FILE *fp = NULL;

    fp = fopen(PRINT_CTR_FILE, "wb");

    if(NULL == fp)
    {
#ifdef ENABLE_PRINT
        printf("No print counter file found\n");
#endif
    }
    else
    {
        // Write current value
        fwrite(&counter, sizeof(uint32_t), 1, fp);
        fclose(fp);
    }
}
