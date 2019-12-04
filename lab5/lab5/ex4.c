/*************************************
 * Lab 5 Ex4
 * Name: Yang Chenglong
 * Student No: A0167107H
 * Lab Group: 03
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

int my_fflush(MY_FILE *stream) {
    // used to be input stream
    if (stream->last == 'r') {
        // set back file descriptor position
        int tmp = lseek(stream->fd, -(stream->cnt), SEEK_CUR);
        if (tmp == -1) {
            return -1;
        }
        stream->cnt = 0;
        stream->head = 0;
    // used to be output stream
    } else if (stream->last == 'w') {
        int tmp = write(stream->fd, stream->base, stream->head);
        if (tmp == -1) {
            return -1;
        }
        stream->cnt = 0;
        stream->head = 0;
    }
    stream->last = 'f';
	return 0;
}

int my_fseek(MY_FILE *stream, long offset, int whence) {
    int tmp = my_fflush(stream);
    if (tmp == -1) {
        return -1;
    }
    stream->last = 's';
    return lseek(stream->fd, offset, whence);
}
