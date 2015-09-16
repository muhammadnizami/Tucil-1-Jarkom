#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFLEN 512
#define NPACK 100
#define PORT 9930

void diep(char *s)
{
	perror(s);
	exit(1);
}

const char XON_x = 17;
const char XOFF_x = 19;

int main(void)
{
	struct sockaddr_in si_me, si_other;
	int s, i, slen=sizeof(si_other);
	char buf[BUFLEN];

	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
		diep("socket");

	memset((char *) &si_me, 0, sizeof(si_me));
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(s, (struct sockaddr *) &si_me, sizeof(si_me))==-1)
			diep("bind");

	for (;;) {
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) &si_other, &slen)==-1)
			diep("recvfrom()");
		printf("Received packet from %s:%d\nData: %s\n\n", 
					 inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
		if (sendto(s, &XOFF_x, 1, 0, (struct sockaddr*) &si_other, slen) == -1)
		        diep("sendto()");
		if (sendto(s, &XON_x, 1, 0, (struct sockaddr*) &si_other, slen) == -1)
		        diep("sendto()");
	}

 close(s);
 return 0;
}
