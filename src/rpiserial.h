#ifndef RPISERIAL_H
#define RPISERIAL_H

struct readbuffer {
    char buffer[500];
    unsigned int readptr;
    unsigned int writeptr;
    int done;
    int error;
};

//Unix specific code
#ifdef __unix__

#include <unistd.h>     //Used for UART
#include <fcntl.h>      //Used for UART
#include <termios.h>    //Used for UART

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
struct rpiserial {
    char device[200];
    unsigned int baud;
    unsigned int databits;
    unsigned int stopbits;
    char parity[5];
    char flowcontrol[5];
};

#elif



#endif


#endif