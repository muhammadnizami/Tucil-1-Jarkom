//SELURUH OPERASI PENULISAN TIDAK THREAD SAFE
//PEMBACAAN INSYAALLAH THREAD SAFE

#ifndef RECEIVER_BUFFER_H
#define RECEIVER_BUFFER_H

#define BUFFER_MAXEL 20
#define BUFFER_MINUPPERLIM 15
#define BUFFER_MAXLOWERLIM 5

#include <stdbool.h>

bool buffer_isEmpty();

bool buffer_isFull();

//prekondisi: !isEmpty();
char buffer_pop();

//prekondisi: !isFull();
void buffer_push(char X);

int buffer_numElements();

void buffer_print();

#endif
