#define test
#ifdef test

#include <stdio.h>

#include <unistd.h>     //Used for UART
#include <fcntl.h>      //Used for UART
#include <termios.h>    //Used for UART

#include "rpiserial.h"

void *readuart(void *arg) {
    struct uartrxparams *param = (struct uartrxparams *)arg;
    while(param->terminate != 1) {
        if (param->uart_fs != -1)
        {

            //Filestream, buffer to store in, number of bytes to read (max)
            int rx_length = read(param->uart_fs, (void*)param->buffer, 500);		
            if (rx_length < 0)
            {
                printf("No bytes received\n");
            }
            else if (rx_length == 0)
            {
                //No data waiting
            }
            else
            {
                //Bytes received
                rx_buffer[rx_length] = '\0';
                printf("%i bytes read : %s\n", rx_length, rx_buffer);
                if(savetofile) writeLog(rx_buffer);
                fflush(stdin);
                getchar();
            }
        }
        else
        {
            param->error = -1;
            return NULL;
        }
    }
    return NULL;
}

void *readstdin(void *arg);

void writestdout(char *buffer, int size);

void writeuart(int fd, char *buffer, int len);

speed_t getBaudrate(unsigned int baudrate);

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
    baudrate = getBaudrate(conf->baudrate);
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = baudrate | CS8 | CLOCAL | CREAD;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);
    return uart0_filestream;
}

unsigned int startsrtial(struct rpiserial_conf *conf) {
    struct uartrxparams uartreadparams;
    int uart0_filestream = getUartstream(conf);
    uartreadparams.uart0_fs = uart0_filestream;
    uartreadparams.readptr = 0;
    uartreadparams.writeptr = 0;
    uartreadparams.done = 1;
    uartreadparams.error = 0;
    uartreadparams.terminate = 0;
    pthread_t readuart_thread;
    pthread_t readstdin_thread;
    pthread_create(&readuart_thread, NULL, readuart, (void *)uartreadparams);
    pthread_create(&readstdin_thread, NULL, readstdin, NULL);
    return 0;
}


#else




#endif