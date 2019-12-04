/*************************************
 * Lab 5 Ex2
 * Name: Yang Chenglong
 * Student No: A0167107H
 * Lab Group: 03
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include "my_stdio.h"
#include <stdio.h>

size_t my_fread(void * ptr, size_t size, size_t nmemb, MY_FILE * stream) {
    // if "w" or "a"
    if ((stream->flag) & O_WRONLY) {
        return -1;
    }
    // "r" or "+"

    // if last action is fwrite
    if (stream->last == 'w') {
        return -1;
    }

    size_t toread = size*nmemb;
    size_t haveread = 0;

    // feed what is left in the buffer
    if (stream->cnt) {
        // read what is left in buffer or all required, whichever is smaller
        size_t canread = (toread > stream->cnt ? stream->cnt : toread);
        memcpy(ptr+haveread, stream->base + stream->head, canread);
        //printf("\tfilled from buffer with %ld bytes\n", canread);
        stream->head += canread;
        stream->cnt -= canread;
        toread -= canread;
        haveread += canread;
        if (!toread) goto finished;
    }

    // read blks of 4096 bytes straightly into ptr
    // instead of reading into buffer and memcpying to ptr
    int blks = toread / 4096;
    //printf("\tneed to read %d blks of 4096\n", blks);
    if (blks) {
        ssize_t tmp = read(stream->fd, ptr+haveread, 4096*blks);
        if (tmp == -1) {
            return -1;
        // eof of the file
        } else if (tmp < 4096*blks) {
            haveread += tmp;
            //printf("\tstraightly read into ptr %ld bytes and reached eof\n", tmp);
            goto finished;
        } else {
            haveread += tmp;
            toread -= tmp;
            //printf("\tstraightly read into ptr %ld bytes\n", tmp);
        }
    }
    //printf("\tdirect read finished\n");

    // read one block of 4096 bytes into buffer
    // and memcpy required bytes to ptr
    // toread is guaranteed to be no more than 4096
    ssize_t tmp = read(stream->fd, stream->base, 4096);
    if (tmp == -1) {
        return -1;
    }

    stream->cnt = tmp;
    stream->head = 0;
    size_t canread = (toread > stream->cnt ? stream->cnt : toread);
    //printf("\tcan fread %ld bytes from buffer\n", canread);
    //printf("\thaveread = %d, canread = %d\n", haveread, canread);
    memcpy(ptr + haveread, stream->base, canread);
    stream->head += canread;
    stream->cnt -= canread;
    haveread += canread;

    finished:
        stream->last = 'r';
        return haveread / size;
}

