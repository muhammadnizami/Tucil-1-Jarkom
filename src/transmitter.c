/*Tugas kecil 1 Jaringan Komputer
pembuat:
13512501 Muhammad Nizami
13513039 Ivan Andrianto
*/

#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include "dcomm.h"


#define TRANSMIT_DELAY 0 //delay dalam microsecond
#define WAIT_DELAY 100000

const int EOF_var = Endfile;

void diep(char *s)
{
	perror(s);
	exit(1);
}

void pthread_diep(char *s)
{
	perror(s);
	pthread_exit(NULL);
}

//SIGNAL READER
char ch = XON;
struct signalReaderArg_type{
	int sockfd;
	struct sockaddr* si_other;
};
void readOneSignal(int sockfd, struct sockaddr* si_other){
	int slen;
	if (recvfrom(sockfd, &ch, 1, 0, si_other, &slen)==-1)
		pthread_diep("recvfrom()");
	else{
		printf("Received signal %s\n",ch==XON?"XON":"XOFF");
		fflush(stdout);
	}
}
void signalReader(struct signalReaderArg_type* a){
	printf("Signal Reader online\n");
	while (1){
		readOneSignal(a->sockfd,a->si_other);
	}
}

int main(int argc, char ** argv){
	int portno;
	const char* rcv_ip;
	const char* pathtofiletosend;

	if (argc != 4){
		printf("usage:  ./transmitter <path-to-file> <IP> <PORT NUMBER>\n");
		exit(1);
	}

	pathtofiletosend = argv[1];
	rcv_ip = argv[2];
	portno = atoi(argv[3]);

	struct sockaddr_in si_other;
	int s, i, slen=sizeof(si_other);

	printf("Membuat socket untuk koneksi(?) ke %s:%d ...\n",rcv_ip,portno);
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep("socket");

	memset((char *) &si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(portno);
	if (inet_aton(rcv_ip, &si_other.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	pthread_t signalReaderThread;
	struct signalReaderArg_type signalReaderArg;
	signalReaderArg.sockfd = s;
	signalReaderArg.si_other = (struct sockaddr*)&si_other;
	pthread_create( &signalReaderThread, NULL, signalReader, (void*) &signalReaderArg);

	char etc;
	//membuka file dan mengirimkan per karakter
	FILE * f = fopen(pathtofiletosend,"r");	
	if (f==NULL) {printf("error opening file\n");exit(1);}
	int bytecount = 1;
	while (fscanf(f, "%c", &etc) != EOF) {
		while (ch==XOFF) {printf("Menunggu XON...\n");fflush(stdout);usleep(WAIT_DELAY);}
		switch(etc){
			case LF: printf("mengirim byte ke-%d: LF\n",++i);fflush(stdout); break;
			case CR: printf("mengirim byte ke-%d: CR\n",++i);fflush(stdout); break;
			default: printf("mengirim byte ke-%d: '%c'\n",++i,etc);fflush(stdout); break;
		}

		if (sendto(s,&etc, 1,0, (struct sockaddr*) &si_other, slen)==-1)
		        diep("sendto()");
		usleep(TRANSMIT_DELAY);
	}
	if (sendto(s,&EOF_var, 1,0, (struct sockaddr*) &si_other, slen)==-1)
		        diep("sendto()");
	pthread_kill(signalReaderThread,SIGKILL);
}
