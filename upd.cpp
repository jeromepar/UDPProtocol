/*
 * upd.cpp
 *
 *  Created on: 4 févr. 2018
 *      Author: jerome
 */


#include "udp.h"

const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}


bool udpSend(const uint8_t *msg,unsigned int sz, char *ip_adr, uint32_t port_out){
	sockaddr_in servaddr;
	int fd = socket(AF_INET,SOCK_DGRAM,0);
	if(fd<0){
		perror("cannot open socket");
		return false;
	}

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(ip_adr);
	servaddr.sin_port = htons(port_out);
	if (sendto(fd, msg, sz, 0,
			(sockaddr*)&servaddr, sizeof(servaddr)) < 0){
		perror("cannot send message");
		close(fd);
		return false;
	}
	close(fd);
	return true;
}

int udpRecvAny(const uint8_t *msg, uint32_t sz, uint32_t port_in){
	int n;
	sockaddr_in servaddr = {};

	int fd = socket(AF_INET,SOCK_DGRAM,0);

	if(fd<0){
		perror("cannot open socket");
		return -1;
	}

	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port_in);

	if(bind (fd, (sockaddr*)&servaddr, sizeof(servaddr)) <0)
	{
		perror("bind()");
		return -1;
	}

	socklen_t adress_size = 0;

	if((n = recvfrom(fd, (void *)msg, sz - 1, 0, (sockaddr*)&servaddr, &adress_size)) < 0)
	{
		perror("recvfrom()");
		return -1;
	}


	for (int i =0; i< n; i++){
		printf("%08x-",msg[i]);
	}
	printf("\n");
	for (int i =0; i< n; i++){
		printf("%s-",byte_to_binary(msg[i]));
	}
	printf("\n");


	return n;
};

int udpRecvAny(const uint8_t *msg, uint32_t sz, uint32_t port_in, uint32_t time_out_ms){
	int n;
	fd_set readfs;


	sockaddr_in servaddr = {};
	int fd = socket(AF_INET,SOCK_DGRAM,0);
	if(fd<0){
		perror("cannot open socket");
		return -1;
	}


	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port_in);

	if(bind (fd, (sockaddr*)&servaddr, sizeof(servaddr)) <0)
	{
		perror("bind()");
		return -1;
	}

	FD_ZERO(&readfs);
	FD_SET(fd, &readfs);

	int result;
	timeval timeout={0,time_out_ms*1000};
	if( (result = select(fd + 1, &readfs, NULL, NULL, &timeout)) < 0)
	{
		perror("select()");
		return (-1);
	}


	if(result == 0)
	{
		/* ici le code si la temporisation (dernier argument) est écoulée (il faut bien évidemment avoir
		 * mis quelque chose en dernier argument).
		 */
		perror("timeout");
		return -1;
	}


	if(FD_ISSET(fd, &readfs))
	{
		/* des données sont disponibles sur le socket */
		/* traitement des données */
		socklen_t adress_size = 0;
		if((n = recvfrom(fd, (void *)msg, sz - 1, 0, (sockaddr*)&servaddr, &adress_size)) < 0)
		{
			perror("recvfrom()");
			return -1;
		}
	}




	return n;
}
