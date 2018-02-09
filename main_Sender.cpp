#include <stdio.h>


#include <fcntl.h>
#include <stdlib.h>

#include "packets.h"
#include "udp_streams.h"

/*
 *
 sudo nc -lvu  127.0.0.1 -p 4242
 *
 */

#define HOSTNAME "127.0.0.1"
#define PORT 4242


int main()
{
	char ip[]=HOSTNAME;

	uint8_t data[400];
	uint8_t *p_data = &(data[0]);

	WriteStream writeStream = WriteStream(p_data, sizeof(data));

	printf("Init server done...\n");

	Packet packet;

	packet.packetType=PACKET_A;
	packet.a={255,116,0b101};
	assert(packet.PacketType(writeStream));
	assert(packet.a.Serialize(writeStream));

	packet.packetType=PACKET_B;
	packet.b={2,{-5,301}};
	assert(packet.PacketType(writeStream));
	assert(packet.b.Serialize(writeStream));

	packet.packetType=PACKET_C;
	packet.c={true,-99,11};
	assert(packet.PacketType(writeStream));
	assert(packet.c.Serialize(writeStream));

	assert(packet.EndTransmission(writeStream));
	assert(writeStream.Send(ip,PORT));

	printf("Server Finished!\n");


	return 0;
}
