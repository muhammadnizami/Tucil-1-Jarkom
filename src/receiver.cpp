/*
* File : receiver.cpp
Nama: Ivan Ausiri
NIM: 13513039
*/
#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dcomm.h"
#include "receiver.h"
using namespace std;

struct sockaddr_in addr;
 struct sockaddr_in  *sockaddr_ipv4;
socklen_t addrLength = sizeof(addr);
int co = 0; //count total byte which want to be read
int con = 0;
void *consume_byte(void *);
Byte c;
bool hasReceived = false;
char *port;

int main(int argc, char *argv[]){
	
	
	/*Insert code here to bind socket to the port number given in argv[1].*/
	if (argc != 2) {
		printf("usage: ./receiver <PORT NUMBER>\n");
		return 0;
	}
	/* Mengambil port dari parameter */
	port = argv[1];

	/* Membuat struktur address server */
	struct addrinfo hints; // Hints for address
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC; /* Allow IPv4 or IPv6 */
	hints.ai_flags = AI_PASSIVE; /* For wildcard IP address - accept on any address/port */
	hints.ai_protocol = IPPROTO_UDP; /* UDP socket */
	hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
	hints.ai_canonname = NULL;
	hints.ai_addr = NULL;
	hints.ai_next = NULL;
	
	struct addrinfo *serverAddresses; // List of server addresses
	
	int ret = getaddrinfo(NULL, port, &hints, &serverAddresses);
	//returns one or more addrinfo structures pada res
	if (ret != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
		printf("getaddrinfo failed");
		return 0;
	}
	
	/* create socket */
	sockfd = socket(serverAddresses->ai_family, serverAddresses->ai_socktype, serverAddresses->ai_protocol);
	if (sockfd < 0) {
		printf("socket failed");
		return 0;
	}
	/* Menge-bind ke local address */
	int bindResult = bind(sockfd, serverAddresses->ai_addr, serverAddresses->ai_addrlen);
	if(bindResult < 0){
		printf("bind failed");
		return 0;
	}

	/* Daftar free address */
	freeaddrinfo(serverAddresses);
	
	/* Initialize XON/XOFF flags */
	sent_xonxoff = XON;
	send_xon = false;
	send_xoff = false;

	/* Create child process */
	pthread_t t;

	/*** IF CHILD PROCESS ***/
	pthread_create(&t, NULL, consume_byte, NULL);
	
	/*** IF PARENT PROCESS ***/
	while (true) {
		c = *(rcvchar(sockfd, rxq));
		/* Quit on end of file */
		if (c == Endfile) {
			exit(0);
		}
	}

	pthread_join(t, NULL);

	return 0;
}


void *consume_byte(void *){
	while (true) {
		/* Call q_get */
		Byte *b = q_get(rxq,&c);

		if(b!=NULL){
			if (rxq->front > 0) {
				switch(rxq->data[rxq->front-1]){
					case Endfile: 
						printf("End of File accepted.\n");
						exit(0);
						break;
					case CR: printf("Mengkonsumsi byte ke-%i: CR\n",++con); break;
					case LF: printf("Mengkonsumsi byte ke-%i: LF\n",++con); break;
					default: printf("Mengkonsumsi byte ke-%i: '%c'\n",++con,rxq->data[rxq->front-1]); break;
				}
			} else {
				switch(rxq->data[RXQSIZE-1]){
					case Endfile: 
						printf("End of File accepted.\n");
						exit(0);
						break;
					case CR: printf("Mengkonsumsi byte ke-%i: CR\n",++con); break;
					case LF: printf("Mengkonsumsi byte ke-%i: LF\n",++con); break;
					default: printf("Mengkonsumsi byte ke-%i: '%c'\n",++con,rxq->data[RXQSIZE-1]); break;
				}
			}
		}

		/* Can introduce some delay here. */
		usleep(DELAY*1000);
	}
	return NULL;
}

static Byte *rcvchar(int sockfd, QTYPE *queue){
	/*Read a character from socket and put it to the receive buffer.
	If the number of characters in the receive buffer is above certain
	level, then send XOFF and set a flag (why?).
	Return a pointer to the buffer where data is put.*/

	Byte rcv_buffer;
	Byte byte[2];
	int putloc;
	if (!send_xoff) {
		//read from socket & push it to queue
		ssize_t receivedBytes = recvfrom(sockfd, byte, sizeof(byte), 0,
		(struct sockaddr *) &addr, &addrLength);
		
		if (receivedBytes < 0) {
			//if error
			printf("recvfrom() failed\n");
		} else {
			/* print IP address for the first time */
			if(!hasReceived){
				char *some_addr = inet_ntoa(addr.sin_addr);
				printf("Binding pada %s:%d\n", some_addr,atoi(port));
				hasReceived = true;
			}
			//fill the circular
			queue->data[queue->rear] = byte[0];
			putloc=queue->rear;
			queue->count++;
			if (queue->rear < RXQSIZE-1) {
				queue->rear++;
			} else {
				queue->rear = 0;
			}
			co++;
		}

		switch(byte[0]){
			case LF: printf("Menerima byte ke-%i: LF\n",co); break;
			case CR: printf("Menerima byte ke-%i: CR\n",co); break;
			case Endfile: break;
			default: printf("Menerima byte ke-%i: %c\n",co,byte[0]); break;
		}

		/* Jika ukuran rcv_buffer size melebihi minimum upperlimit */
		if (queue->count > MIN_UPPERLIMIT && sent_xonxoff == XON) {
			/* mengeset xon dan xoff */
			sent_xonxoff = XOFF;
			send_xon = false;
			send_xoff = true;
			
			char ch[2];
			ch[0] = XOFF;
			/* send XOFF to transmitter */
			printf("Send XOFF\n");
			ssize_t sentBytes = sendto(sockfd, ch, sizeof(ch), 0,
			(struct sockaddr *) &addr, sizeof(addr));
			if (sentBytes < 0){
				printf("sendto() failed)");
			}			
		}
		return &queue->data[0];
	} else {
		rcv_buffer = 0;
		return &rcv_buffer;
	}
}

/* q_get returns a pointer to the buffer where data is read or NULL if buffer is empty. */
static Byte *q_get(QTYPE *queue, Byte *data){
	Byte *current = new Byte[1];
	/* Nothing in the queue */
	if (!queue->count) return (NULL);

	/*else queue NOT NULL*/
	bool valid = false;

	/* Retrieve data from buffer, save it to "current" and "data" */
	while (!valid){
		if (queue->count > 0) {
			(*current) = queue->data[queue->front];
			queue->count--;
			if (queue->front < RXQSIZE-1) {
				queue->front++;
			} else {
				queue->front = 0;
			}
			valid = (*current >32 || *current ==CR || *current ==LF || *current == EOF || *current == Endfile);
		}

		if((queue->count <= 0) || (*current>=32) || (*current == LF))
			valid = true;
	}
	memcpy(data, current, sizeof(Byte));

	/* If the number of characters in the receive buffer is below certain level, then send XON. */
	if (queue->count < MAX_LOWERLIMIT && sent_xonxoff == XOFF) {
		printf("Send XON\n");

		/* mengeset xon dan xoff */
		sent_xonxoff = XON;
		send_xon = true;
		send_xoff = false;

		char SendBuf[1024];
		SendBuf[0] = XON;
		int bufLen = 1024;

		ssize_t sentBytes = sendto(sockfd, SendBuf, bufLen, 0, (struct sockaddr *) &addr, sizeof(addr));
		if (sentBytes < 0){
			printf("sendto() failed\n");
		}
	}
	delete current;
	return data;
}
