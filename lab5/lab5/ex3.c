/*************************************
 * Lab 5 Ex3
 * Name: Yang Chenglong
 * Student No: A0167107H
 * Lab Group: 03
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"

size_t my_fwrite(const void *ptr, size_t size, size_t nmemb, MY_FILE *stream) {
    // if 'r'
    if (stream->flag == O_RDONLY) {
        return -1;
    }
    // "w" or "a"

    // if last action is fread
    if (stream->last == 'r') {
        return -1;
    }
    size_t towrite = size*nmemb;
    size_t havewritten = 0;

    // fill up the buffer first
    if (4096-stream->head) {
        // fill up empty space in buffer or write all to buffer
        // whichever is smaller
        size_t canfill = (towrite > 4096-stream->head ? 4096-stream->head : towrite);
        memcpy(stream->base + stream->head, ptr, canfill);
        stream->head += canfill;
        towrite -= canfill;
        havewritten += canfill;
        // if buffer is full
        if (stream->head == 4096) {
            ssize_t tmp = write(stream->fd, stream->base, 4096);
            if (tmp == -1) {
                return -1;
            }
            stream->head = 0;
        }
        if (!towrite) goto finished;
    }

    // write blks of 4096 bytes straightly into file
    // instead of fill up the buffer and write to file
    int blks = towrite/4096;
    if (blks) {
        ssize_t tmp = write(stream->fd, ptr+havewritten, 4096*blks);
        if (tmp == -1) {
            return -1;
        }
        // I don't think I have any reasons to check if tmp == 4096*blks
        // if not, disk full? then what can I do?
        havewritten += tmp;
        towrite -= tmp;
    }

    // fill to buffer
    memcpy(stream->base, ptr+havewritten, towrite);
    havewritten += towrite;
    stream->head += towrite;
    towrite -= towrite;

    finished:
        stream->last = 'w';
	    return havewritten/size;
}
