#ifndef RPISERIAL_H
#define RPISERIAL_H

/**
 * @brief 
 * 
 */
struct uartrxparams {
    int uart_fs;
    char buffer[500];
    unsigned int readptr;
    unsigned int writeptr;
    int done;
    int error;
    int terminate;
};

/**
 * @brief Configure the UART
 * @param device file descriptor
 * @param baud bouderate
 * @param databits data bits
 * @param stopbits stop bits
 * @param parity parity, can be "odd", "even" or "none"
 * @param flowcontrol flow control, can be "none", "hardware" or "software"
 * 
 */
struct rpiserial_conf {
    char device[200];
    unsigned int baud;
    unsigned int databits;
    unsigned int stopbits;
    char parity[5];
    char flowcontrol[5];
};

//Unix specific code
#ifdef __unix__

#include <unistd.h>     //Used for UART
#include <fcntl.h>      //Used for UART
#include <termios.h>    //Used for UART

/**
 * @brief Reads constantl from UART rx port in a seperate thread
 * 
 * @param arg 
 * @return void* 
 */
void *readuart(void *arg);

/**
 * @brief Reads from stdin in a seperate thread
 * 
 * @param arg 
 * @return void* 
 */
void *readstdin(void *arg);

/**
 * @brief Writes to stdout
 * 
 * @param buffer //Pointer to the buffer to write
 * @param size      //Number of characters to write
 */
void writestdout(char *buffer, int size);

/**
 * @brief Writes to UART TX
 * 
 * @param fd 
 * @param buffer 
 * @param len 
 */
void writeuart(int fd, char *buffer, int len);

/**
 * @brief Get the Baudrate object
 * 
 * @param baudrate The baudrate which the user defined
 * @return speed_t The baud rate in a speed_t object
 */
speed_t getBaudrate(unsigned int baudrate);

/**
 * @brief Initialising the Uartstream object
 * 
 * @param conf The configuration parameters from config file or from the user
 * @return int 
 */
int getUartstream(struct rpiserial_conf *conf);

/**
 * @brief 
 * 
 * @param conf 
 * @return unsigned int 
 */
unsigned int startsrtial(struct rpiserial_conf *conf);



#else

void print_conf(struct rpiserial_conf *conf);

#endif


#endif