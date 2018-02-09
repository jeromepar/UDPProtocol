/*
 * packets.h
 *
 *  Created on: 3 févr. 2018
 *      Author: jerome
 */

#ifndef PACKETS_H_
#define PACKETS_H_

#include "rw_packets.h"
#include "udp_streams.h"

enum PacketTypeEnum { PACKET_A, PACKET_B, PACKET_C, PACKET_END };

struct PacketA
{
	int x,y,z;

	template <typename Stream> bool Serialize( Stream & stream )
	{
		assert(stream.serialize_bits( x, 0,255 ));
		assert(stream.serialize_bits( y, 0,255 ));
		assert(stream.serialize_bits( z, 0,255 ));
		return true;
	}

};


struct PacketB
{
	int numElements;
	int elements[10];

	template <typename Stream> bool Serialize( Stream & stream )
	{
		assert(stream.serialize_bits( numElements, 0,100 ));
		for (int i=0; i<numElements; i++)
		{
			assert(stream.serialize_bits(elements[i], -500, +500 ));
		}
		return true;
	}
};
struct PacketC
{
	uint8_t x;
	int16_t y;
	int32_t z;

	template <typename Stream> bool Serialize( Stream & stream )
	{
		assert(stream.serialize_bits( x, 0,255 ));
		assert(stream.serialize_bits( y, -100,100 ));
		assert(stream.serialize_bits( z, -65000,65000 ));

		if ( Stream::IsReading ) {
			printf("IS READING C\n");
		}

		return true;
	}
};

struct Packet
{

	uint8_t packetType;

	union
	{
		struct PacketA a;
		struct PacketB b;
		struct PacketC c;
	};

	template <typename Stream> bool PacketType( Stream & stream )
	{
		assert( stream.serialize_bits( packetType, 0,PACKET_END ));
		return true;
	}

	template <typename Stream> bool EndTransmission( Stream & stream )
	{
		assert(stream.serialize_bits( PACKET_END, 0,PACKET_END ));
		return true;
	}

};

#endif /* PACKETS_H_ */
