#ifdef __unix__

#include <stdio.h>

#include <unistd.h>     //Used for UART
#include <fcntl.h>      //Used for UART
#include <termios.h>    //Used for UART

#include <pthread.h>    //Multithreading

#include "rpiserial.h"

/**
 * @brief Canculates the data size between the read and the wqrite pointer
 * Calculates the data size which which sould be read from the buffer.
 * 
 * @param rx The FIFO buffer
 * @return unsigned int The size of the data which should be read.
 */
unsigned int getreadsize(struct uart_asyncbuff *rx) {
    //Reading the read and write ponter values from the struct
    unsigned int wrprt = rx->writeptr;
    unsigned int rptr = rx->readptr; 
    //Calculating the absolute value of the difference between the read and write pointer
    if(wrprt > rptr) {
        //If the write pointer is bigger than the read pointer, the size is the difference
        return wrprt - rptr;
    } else {
        //If the write pointer is smaller than the read pointer, the size is the modulo RXBUFFSIZE of the sum of the read and write pointer
        return (wrprt + rptr)%RXBUFFSIZE;
    }
}

void writebuffer(struct uart_asyncbuff *rx, unsigned char *buffer, int size) {
    //The caller function is responsible to check if the buffers are not NULL and the size is not 0
    int i;
    for(i = 0; i < size; i++) {
        //Reading data from the buffer
        rx->buffer[(i+rx->writeptr)%RXBUFFSIZE] = buffer[i];
    }
    rx->writeptr = (i+rx->writeptr)%RXBUFFSIZE;
    //After writing data the write pointer reached the read pointer, data loss is possible
    if(rx->writeptr == rx->readptr) {
        rx->error = 1;
    }
}

int readbuffer(struct uart_asyncbuff *rx, unsigned char *buffer) {
    unsigned int i;
    unsigned int size = getreadsize(rx);    //Calculating the numbers of bytes which should be read
    for(i = 0; i < size; i++) {
        buffer[i] = rx->buffer[(i+rx->readptr)%RXBUFFSIZE];
    }
    //Setting new value for the read pointer
    rx->readptr = (i+rx->readptr)%RXBUFFSIZE;
    return i;
}

void *readuart_async(void *arg) {
    //Reading parameters form function argument
    struct uart_threadparams *param = (struct uart_threadparams *)arg;
    //Filestream, buffer to store in, number of bytes to read (max)
    unsigned char rx_buffer[256];
    //Thread runs until the thread is terminated with setting the thread parameter to 1
    while(param->terminate != 1) {
        //TODO: Implement polling with 1 second timeout
        int rx_length = read(param->uart_fs, (void*)rx_buffer, 256);		
        if (rx_length < 0)
        {
            //printf("No bytes received: %d\n", rx_length);
        }
        else if (rx_length == 0)
        {
            //No data waiting
        }
        else
        {
            //Bytes received writing to FIFO buffer
            writebuffer(param->rx, rx_buffer, rx_length);
        }
    }
    //Terminating thread
    pthread_exit(NULL);
}

int writeuart_sync(int fd, char *buffer, int len) {
    //Writes data with len length to the UART from buffer
    int count = write(fd, &buffer, len);
    //Returns the number of bytes written, if count not equal to len, that indicates error
    return count;
}

speed_t getBaudrate(unsigned int baudrate) {
    speed_t brate;   //Variable for return value
    switch(baudrate) {
        case 4800: brate = B4800; break;
        case 9600: brate = B9600; break;
        case 19200: brate = B19200; break;
        case 38400: brate = B38400; break;
        case 57600: brate = B57600; break;
        case 115200: brate = B115200; break;
        //TODO: Add more baudrates
        default: brate = B9600; break;
    }
    return brate;
}

int getUartstream(struct rpiserial_conf *conf) {
    //---- SETUP UART ----
	int uart0_filestream = -1;
    struct termios options;
    speed_t baudrate;
	//Open in non blocking read/write mode
	uart0_filestream = open(conf->device, O_RDWR | O_NOCTTY | O_NDELAY);
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT, Ensure it is not in use by another application
		//TODO: Implement error handling and error logging
        conf->error = 1;
        return uart0_filestream;
	}
    baudrate = getBaudrate(conf->baud);
    //TODO: Implement start bits, stop bits, parity, flow control
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = baudrate | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
    return uart0_filestream;
}

struct uart_threadparams startserial(struct rpiserial_conf *conf) {
    struct uart_threadparams uartreadparams;
    int uart0_filestream = getUartstream(conf);
    uartreadparams.uart_fs = uart0_filestream;  //Setting the UART filestream to the thread parameter
    uartreadparams.terminate = 0;
    return uartreadparams;
}


#else




#endif