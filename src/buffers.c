#include <stdlib.h>
#include "buffers.h"


void initbuffer(struct asyncbuff *buff, unsigned int size) {
    //Check if the size is power of two and not 0. If not power of two or 0,
    //then return with error.
    if((size == 0) || (((size & (size -1)) != 0))) {
        buff->error = 2;
        return;
    }
    buff->size = size;
    //Allocating memory for buffer.
    buff->buffer = (char*)malloc(size*sizeof(char));
    //Check if memory allocation is successful.
    if(buff->buffer == NULL) {
        buff->error = 3;
        return;
    }
    buff->readptr = 0;
    buff->writeptr = 0;
}

void delletebuffer(struct asyncbuff *buff) {
    //Freeing the memory allocated for buffer.
    if(buff->buffer != NULL) {
        free(buff->buffer);
        buff->buffer = NULL;
    }
    //Setting other variable to an empty state.
    buff->size = 0;
    buff->readptr = 0;
    buff->writeptr = 0;
    buff->error = -1;
    return;
}


unsigned int getreadsize(struct asyncbuff *buff) {
    //Reading the read and write ponter values from the struct
    unsigned int wrprt = buff->writeptr;
    unsigned int rptr = buff->readptr; 
    unsigned int size = buff->size;
    //Calculating the absolute value of the difference between the read and write pointer
    if(wrprt > rptr) {
        //If the write pointer is bigger than the read pointer, the size is the difference
        return wrprt - rptr;
    } else {
        //If the write pointer is smaller than the read pointer, the size is the modulo RXBUFFSIZE of the sum of the read and write pointer
        return (wrprt + rptr)%size;
    }
}


void writebuffer(struct asyncbuff *destbuff, unsigned char *sourcebuff, int size) {
    //The caller function is responsible to check if the buffers are not NULL and the size is not 0
    int i;
    for(i = 0; i < size; i++) {
        //Reading data from the buffer
        destbuff->buffer[(i+destbuff->writeptr)%destbuff->size] = sourcebuff[i];
    }
    destbuff->writeptr = (i+destbuff->writeptr)%destbuff->size;
    //After writing data the write pointer reached the read pointer, data loss is possible
    if(destbuff->writeptr == destbuff->readptr) {
        destbuff->error = 1;
    }
}


int readbuffer(struct asyncbuff *sourcebuff, unsigned  char *destbuffer) {
    unsigned int i;
    //Calculating the numbers of bytes which should be read
    unsigned int size = getreadsize(sourcebuff);
    for(i = 0; i < size; i++) {
        destbuffer[i] = sourcebuff->buffer[(i+sourcebuff->readptr)%sourcebuff->size];
    }
    //Setting new value for the read pointer
    sourcebuff->readptr = (i+sourcebuff->readptr)%sourcebuff->size;
    return i;
}