#include <stdio.h>

#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

#include "packets.h"
#include "udp.h"
#include "udp_streams.h"

#define PORT 4242



int main()
{
	uint8_t data[400];
	uint8_t *p_data = &(data[0]);

	ReadStream readStream = ReadStream(p_data, sizeof(data));

	printf("Init client done...\n");


	int size_in=readStream.RecvAny(PORT/*, 3000*/);

	assert(size_in >= 0);

	bool running=true;
	while( (size_in>0) && running && (readStream.atTheEnd()==false)){

		Packet packet_in;
		assert(packet_in.PacketType(readStream));

		switch(packet_in.packetType) {
		case PACKET_A:
			printf("Buffer A in\n");
			assert(packet_in.a.Serialize(readStream));
			printf("A: %d,%d,%d \n",packet_in.a.x,packet_in.a.y,packet_in.a.z);
			break;
		case PACKET_B:
			assert(packet_in.b.Serialize(readStream));
			printf("Buffer B in\n");
			printf("B: %d->%d,%d\n",packet_in.b.numElements,packet_in.b.elements[0],packet_in.b.elements[1]);

			break;
		case PACKET_C:
			assert(packet_in.c.Serialize(readStream));
			printf("Buffer C in\n");
			printf("C: %d,%d,%d\n",packet_in.c.x,(signed short)packet_in.c.y,packet_in.c.z);

			break;
		case PACKET_END:
			printf("EndOfTransmission\n");
			running = false;
			break;
		default:
			printf("packet type unknown: %d\n",packet_in.packetType);
			printf("ABORT\n");
			return 1;
			break;
		}
	}

	printf("Client Finished!\n");


	return 0;
}
