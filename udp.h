/*
 * udp.h
 *
 *  Created on: 4 févr. 2018
 *      Author: jerome
 */

#ifndef UDP_H_
#define UDP_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>

bool udpSend(const uint8_t *msg,unsigned int sz, char *ip_adr, uint32_t port_out);

int udpRecvAny(const uint8_t *msg, uint32_t sz, uint32_t port_in);

int udpRecvAny(const uint8_t *msg, uint32_t sz, uint32_t port_in, uint32_t time_out_ms);

#endif /* UDP_H_ */
