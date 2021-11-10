#ifndef BUFFERS_H
#define BUFFERS_H

/**
 * @brief A buffer FIFO struct with seperate read and write pointers.
 * This struct can be passed to a new thread which can write it. The calling
 * thread can detect if the read pointer is not equal to the write pointer
 * which means there is new data in the buffer.
 */
struct asyncbuff {
    char *buffer;               /*!< The character buffer buffer to use. Size must be a power of 2. */
    unsigned int size;          /*!< The size of the buffer. */
    unsigned int readptr;       /*!< Index of the first byte to read from the buffer. */
    unsigned int writeptr;      /*!< Index of the position to write the next byte. */
    int error;                  /*!< When set it indicates that the write pointer reached the read pointer which means the data consistency is broken. */
};

/**
 * @brief Initialise the buffer with given size.
 * This function allocates memory for the buffer and initialises the read and
 * write pointers.
 * @param buff The buffer to initialise.
 * @param size The size of the buffer. It must be a power of 2.
 */
void initbuffer(struct asyncbuff *buff, unsigned int size);

/**
 * @brief Free the memory allocated for the buffer.
 * This function frees the memory allocated for the buffer.
 * @param buff The buffer to free.
 */
void delletebuffer(struct asyncbuff *buff);

/**
 * @brief Canculates the data size between the read and the wqrite pointer
 * Calculates the data size which which sould be read from the buffer.
 * 
 * @param buff The FIFO buffer
 * @return unsigned int The size of the data which should be read.
 */
unsigned int getreadsize(struct asyncbuff *buff);

/**
 * @brief Function to write the async FIFO buffer.
 * This function can write the async FIFO buffer.
 * @param destbuff The FIFO buffer in which the data from buffer is written. The caller function is responsible to check if the buffer is not NULL.
 * @param sourcebuff The buffer which contains the new data to write in the FIFO. The caller function is responsible to check if the buffer is not NULL.
 * @param size The sizze of the new data to write. The caller function is responsible to check if the size is not 0.
 */
void writebuffer(struct asyncbuff *destbuff, unsigned char *sourcebuff, int size);

/**
 * @brief Function to read the async FIFO buffer.
 * This function reads the async FIFO buffer. The size of the data to read is determined by this function.
 * @param sourcebuff The FIFO buffer from which the data is read. The caller function is responsible to check if the buffer is not NULL.
 * @param destbuffer The buffer where the data is read to. The caller function is responsible to check if the buffer is not NULL.
 * @return int Returns the size of the data read.
 */
int readbuffer(struct asyncbuff *sourcebuff, unsigned  char *destbuffer);

#endif