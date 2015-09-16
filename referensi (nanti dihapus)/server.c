/*UDP server for linux*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char**argv)
{
	int socketfd, newsockfd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;


	if (argc != 2)
	{
		printf("usage:  ./server <PORT NUMBER>\n");
		exit(1);
	}
	
	//buat socket
	socketfd = socket(AF_INET,SOCK_DGRAM,17);
	if (socketfd < 0)
		error("ERROR opening socket");

	//persiapan bind
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(po	rtno);

	//bind
	if (bind(socketfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) error ("ERROR on binding");	

	//listen
	listen(socketfd,5);

	clilen = sizeof(cli_addr);
	newsockfd = accept(socketfd, 
			  (struct sockaddr *) &cli_addr, 
			  &clilen);
	if (newsocketfd < 0) 
		error("ERROR on accept");
	bzero(buffer,256);
	n = read(newsocketfd,buffer,255);
	if (n < 0) error("ERROR reading from socket");
	printf("Here is the message: %s\n",buffer);
	n = write(newsocketfd,"I got your message",18);
	if (n < 0) error("ERROR writing to socket");
	close(newsocketfd);
	close(sockfd);
	return 0; 


}
