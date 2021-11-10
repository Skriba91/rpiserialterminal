#ifndef STDIO_CONTROL_H
#define STDIOCONTROL_H

#define STDBUFFSIZE 512 /*!< Size of the async buffer */

/**
 * @brief A buffer FIFO struct with seperate read and write pointers.
 * This struct can be passed to a new thread which can write it. The calling
 * thread can detect if the read pointer is not equal to the write pointer
 * which means there is new data in the buffer.
 */
struct stdio_asyncbuff {
    char buffer[STDBUFFSIZE];    /*!< The character buffer buffer to use. Size must be a power of 2. */
    unsigned int readptr;       /*!< Index of the first byte to read from the buffer. */
    unsigned int writeptr;      /*!< Index of the position to write the next byte. */
    int error;                  /*!< When set it indicates that the write pointer reached the read pointer which means the data consistency is broken. */
};

/**
 * @brief The struct to pass to the new I/O thread.
 * This struct contains the parameters to pass to the new I/O thread.
 */
struct stdio_threadparams {
    int terminate;              /*!< When the condition is set it indicates that the thread should terminate. */
    struct stdio_asyncbuff *rx; /*!< The buffer FIFO struct to use. */
    int error;                  /*!< When set it indicates that there was an error while using the I/O device */
};

/**
 * @brief Writes to the stdout in a new thread.
 * This function writes to the stdout in a seperate thread.
 * @param arg Shall be stdio_threadparams struct.
 */
void *writestdo(void *arg); //TODO: Implement

/**
 * @brief 
 * 
 * @param arg Shall be stdio_threadparams struct.
 */
void *readstdi(void *arg); //TODO: Implement


#endif