#ifndef RPISERIAL_H
#define RPISERIAL_H

#define RXBUFFSIZE 512 /*!< Size of the async buffer */

/**
 * @brief A buffer FIFO struct with seperate read and write pointers.
 * This struct can be passed to a new thread which can write it. The calling
 * thread can detect if the read pointer is not equal to the write pointer
 * which means there is new data in the buffer.
 */
struct uart_asyncbuff {
    char buffer[RXBUFFSIZE];    /*!< The character buffer buffer to use. Size must be a power of 2. */
    unsigned int readptr;       /*!< Index of the first byte to read from the buffer. */
    unsigned int writeptr;      /*!< Index of the position to write the next byte. */
    int error;                  /*!< When set it indicates that the write pointer reached the read pointer which means the data consistency is broken. */
};

/**
 * @brief The struct to pass to the new UART thread.
 * This struct contains the parameters to pass to the new UART thread.
 */
struct uart_threadparams {
    int terminate;              /*!< When the condition is set it indicates that the thread should terminate. */
    int uart_fs;                /*!< The file descriptor of the UART device. */
    struct uart_asyncbuff *rx;  /*!< The buffer FIFO struct to use. */
    int error;                  /*!< When set it indicates that there was an error while using the UART device */
};

/**
 * @brief The configuration data for the UART device.
 * 
 */
struct rpiserial_conf {
    char device[200];       /*!< The path to the file discriptor of the UART device. */
    unsigned int baud;      /*!< The baudrate to use. [Partially implemented] */
    unsigned int databits;  /*!< The number of data bits to use. [NOT IMPLEMENTED YET] */
    unsigned int stopbits;  /*!< The number of stop bits to use. [NOT IMPLEMENTED YET] */
    char parity[5];         /*!< The parity to use. [NOT IMPLEMENTED YET] */
    char flowcontrol[5];    /*!< The flow controll. [NOT IMPLEMENTED YET] */
    int error;              /*!< Indicates and error */
};

/**
 * @brief Function to write the async FIFO buffer.
 * This function can write the async FIFO buffer.
 * @param rx The FIFO buffer in which the data from buffer is written. The caller function is responsible to check if the buffer is not NULL.
 * @param buffer The buffer which contains the new data to write in the FIFO. The caller function is responsible to check if the buffer is not NULL.
 * @param size The sizze of the new data to write. The caller function is responsible to check if the size is not 0.
 */
void writebuffer(struct uart_asyncbuff *rx, unsigned char *buffer, int size);

/**
 * @brief Function to read the async FIFO buffer.
 * This function reads the async FIFO buffer. The size of the data to read is determined by this function.
 * @param rx The FIFO buffer from which the data is read. The caller function is responsible to check if the buffer is not NULL.
 * @param buffer The buffer where the data is read to. The caller function is responsible to check if the buffer is not NULL.
 * @return int Returns the size of the data read.
 */
int readbuffer(struct uart_asyncbuff *rx, unsigned  char *buffer);

//Unix specific code
#ifdef __unix__

#include <unistd.h>     //Used for UART
#include <fcntl.h>      //Used for UART
#include <termios.h>    //Used for UART

/**
 * @brief Reads the UART device in a new thread.
 * This function reads the UART device in a new thread thus not blocking the
 * program execution.
 * @param arg Shall be uart_threadparams struct.
 */
void *readuart_async(void *arg);

/**
 * @brief Reads the UART device.
 * This function reads the UART device in the same thread thus blocking the
 * program execution.
 * @param arg Shall be uart_threadparams struct.
 */
void readuart_sync(void *arg);

/**
 * @brief Writes the UART device in a new thread.
 * Writes the UART device in a new thread thus not blocking the program
 * execution.
 * @param fd File descriptor of the UART device.
 * @param buffer Buffer to write. Caller function is responsible to check if the buffer is not NULL.
 * @param len Length of data to write. Caller function is responsible to check if the length is not 0.
 */
void writeuart_async(int fd, char *buffer, int len); //TODO: Implement

/**
 * @brief Writes the UART device.
 * Writes the UART device in the same thread thus blocking the program
 * execution.
 * @param fd File descriptor of the UART device. The caller function is responsible to check if the file descriptor is not -1.
 * @param buffer Buffer to write. Caller function is responsible to check if the buffer is not NULL.
 * @param len Length of data to write. Caller function is responsible to check if the length is not 0.
 * @return int Returns the number of bytes written. If the return value is less than len it indicates an error.
 */
int writeuart_sync(int fd, char *buffer, int len);

/**
 * @brief Get the speed_t Baudrate object from user defined speed.
 * 
 * @param baudrate The baudrate which the user defined
 * @return speed_t The baud rate in a speed_t object
 */
speed_t getBaudrate(unsigned int baudrate);

/**
 * @brief Initialising the UART device and returns the file descriptor.
 * Initialising the UART device with the user given parameters and returns the
 * file descriptor.
 * @param conf The configuration parameters from config file or from the user
 * @return int The file descriptor of the UART device.
 */
int getUartstream(struct rpiserial_conf *conf);

/**
 * @brief Starts the UART stream with initalising the threadparams object.
 * Initialising the threadparams object and starts the UART stream.
 * @param conf Configuration parameters from config file or from the user.
 * @return struct threadparams The initialized threadparams object which contains the UART file descriptor.
 */
struct uart_threadparams startserial(struct rpiserial_conf *conf);



#else

void print_conf(struct rpiserial_conf *conf);

#endif


#endif