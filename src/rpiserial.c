#ifdef __unix__

#include <stdio.h>

#include <unistd.h>     //Used for UART
#include <fcntl.h>      //Used for UART
#include <termios.h>    //Used for UART

#include <pthread.h>    //Multithreading

#include "rpiserial.h"

unsigned int getreadsize(struct uart_asyncbuff *rx) {
    unsigned int wrprt = rx->writeptr;
    if(wrprt > rx->readptr) {
        return wrprt - rx->readptr;
    } else {
        return (wrprt +  rx->readptr)%RXBUFFSIZE;
    }
}

void writebuffer(struct uart_asyncbuff *rx, unsigned char *buffer, int size) {
    int i;
    //TODO: check if buffer is null
    //TODO: check if size is 0
    //TODO: handle write pointer overflow --> define buffer size
    for(i = 0; i < size; i++) {
        rx->buffer[(i+rx->writeptr)%RXBUFFSIZE] = buffer[i];
    }
    printf("New size: %d\n" ,(i+rx->writeptr)%RXBUFFSIZE);
    rx->writeptr = (i+rx->writeptr)%RXBUFFSIZE;
}

int readbuffer(struct uart_asyncbuff *rx, unsigned char *buffer) {
    unsigned int i;
    unsigned int size = getreadsize(rx);
    printf("Size: %d\n", size);
    for(i = 0; i < size; i++) {
        printf("point: %d, char: %d\n", (i+rx->readptr)%RXBUFFSIZE, rx->buffer[(i+rx->readptr)%RXBUFFSIZE]);
        buffer[i] = rx->buffer[(i+rx->readptr)%RXBUFFSIZE];
    }
    rx->readptr = (i+rx->readptr)%RXBUFFSIZE;
    return i;
}

void *readstdin(void *arg);

void writestdout(char *buffer, int size);

void *readuart(void *arg) {
    struct threadparams *param = (struct threadparams *)arg;
    while(param->terminate != 1) {
        if (param->uart_fs != -1)
        {

            //Filestream, buffer to store in, number of bytes to read (max)
            unsigned char rx_buffer[256];
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
                //Bytes received
                printf("Char: %d\n", rx_buffer[0]);
                printf("Bytes received: %d\n", rx_length);
                writebuffer(param->rx, rx_buffer, rx_length);
            }
        }
        else
        {
            param->error = -1;
            return NULL;
        }
    }
    pthread_exit(NULL);
}

void writeuart(int fd, char *buffer, int len) {
    if (fd != -1) {	//Filestream, bytes to write, number of bytes to write
        int count = write(fd, &buffer, len);
        if (count < 0)
        {
            printf("UART TX error\n");
        }
    }
    else {
        printf("UART TX error\n");
    }
}

speed_t getBaudrate(unsigned int baudrate) {
    switch(baudrate) {
        case 9600:
            return B9600;
        case 19200:
            return B19200;
        case 38400:
            return B38400;
        case 57600:
            return B57600;
        case 115200:
            return B115200;
        default:
            return B9600;
    }
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
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART."
				"Ensure it is not in use by another application\n");
	}
    baudrate = getBaudrate(conf->baud);
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = baudrate | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
    return uart0_filestream;
}

struct threadparams startserial(struct rpiserial_conf *conf) {
    struct threadparams uartreadparams;
    int uart0_filestream = getUartstream(conf);
    uartreadparams.uart_fs = uart0_filestream;
    uartreadparams.terminate = 0;
    return uartreadparams;
}


#else




#endif