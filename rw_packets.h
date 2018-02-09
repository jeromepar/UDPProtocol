/*
 * rw_packets.h
 *
 *  Created on: 3 févr. 2018
 *      Author: jerome
 */

#ifndef RW_PACKETS_H_
#define RW_PACKETS_H_

#include "stdint.h"
#include "udp.h"
#include "errors.h"


#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

#define MASK_UPTO_BIT(a) (((uint64_t)1 << a) - 1)
#define READ_VALUE(var,bits) var=*(typeof var *)ReadBits( BufferUDP, (bits) )



class BufferUDP
{
public:
	BufferUDP( const uint8_t * Buffer, int bytes );
	uint32_t nbr_of_bit_left();
	uint32_t ReadBits( uint16_t nbr);
	void WriteBits(uint32_t value, uint16_t nbr);
    void FlushBits( );
    bool Send(char * ip, uint32_t port);
    int32_t RecvAny(uint32_t port_in);
    int32_t RecvAny(uint32_t port_in, uint32_t time_out_ms);
private:

    const uint8_t * data;     // pointer to BufferUDP data
	int size;           // size of BufferUDP data (bytes)
	uint64_t scratch_value;	//used to aggregate datas
	uint16_t pos_in_scratch_value; // position in scratch_value, from 0 to 63
	int index;          // index of next byte to be read/written

	void WriteInt32( uint32_t value );
	void WriteInt16( uint16_t value );
	void WriteByte(  uint8_t value );

	uint32_t ReadInt32();
	uint16_t ReadInt16();
	uint8_t  ReadByte();
};

uint32_t bits_required(int64_t min, int64_t max);

#endif /* RW_PACKETS_H_ */
