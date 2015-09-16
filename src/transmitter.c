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

const int EOF_var = EOF;

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

//kontrol
#define XON 17
#define XOFF 19


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
	while (fscanf(f, "%c", &etc) != EOF) {
		while (ch==XOFF) usleep(10000);
		if (sendto(s,&etc, 1,0, (struct sockaddr*) &si_other, slen)==-1)
		        diep("sendto()");
	}
	if (sendto(s,&EOF_var, 1,0, (struct sockaddr*) &si_other, slen)==-1)
		        diep("sendto()");
}
