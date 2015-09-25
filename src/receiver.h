#ifndef RECEIVER_H
#define RECEIVER_H

#include "dcomm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define DELAY 100
#define RXQSIZE 8
#define BUF_SIZE 500
#define MIN_UPPERLIMIT 4
#define MAX_LOWERLIMIT 1

Byte rxbuf[RXQSIZE];
QTYPE rcvq = { 0, 0, 0, RXQSIZE, rxbuf };
QTYPE *rxq = &rcvq;
Byte sent_xonxoff;
bool send_xon, send_xoff;
/* Socket */
int sockfd; // listen on sock_fd
/* Functions declaration */
static Byte *rcvchar(int sockfd, QTYPE *queue);
static Byte *q_get(QTYPE *, Byte *);
#endif
