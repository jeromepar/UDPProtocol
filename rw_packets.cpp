/*
 * rw_packets.cpp
 *
 *  Created on: 3 févr. 2018
 *      Author: jerome
 */

#include "rw_packets.h"

void BufferUDP::WriteInt32(uint32_t value )
{
	ASSERT( index + 4 <= size, __FILE__, __LINE__ );
#ifndef __LITTLE_ENDIAN
	*((uint32_t*)(data+index)) = bswap( value );
#else // #ifdef BIG_ENDIAN
	*((uint32_t*)(data+index)) = value;
#endif // #ifdef BIG_ENDIAN

	index += 4;
}

void BufferUDP::WriteInt16(uint16_t value ){
	ASSERT(index + 2 <= size, __FILE__, __LINE__ );

#ifndef __LITTLE_ENDIAN
	*((uint16_t*)(data+index)) = bswap( value );
#else // #ifdef BIG_ENDIAN
	*((uint16_t*)(data+index)) = value;
#endif // #ifdef BIG_ENDIAN

	index += 2;

}
void BufferUDP::WriteByte(uint8_t value ){
	ASSERT( index + 1 <= size, __FILE__, __LINE__ );

#ifndef __LITTLE_ENDIAN
	*((uint8_t*)(data+index)) = bswap( value );
#else // #ifdef BIG_ENDIAN
	*((uint8_t*)(data+index)) = value;
#endif // #ifdef BIG_ENDIAN

	index += 1;
}

uint32_t BufferUDP::ReadInt32(){
	uint32_t value;

	ASSERT( (index + 4) < size, __FILE__, __LINE__ );

#ifndef __LITTLE_ENDIAN
	value = bswap( *((uint32_t*)(data+index)) );
#else // #ifdef BIG_ENDIAN
	value = *((uint32_t*)(data+index));
#endif // #ifdef BIG_ENDIAN

	index += 4;

	return value;
}

uint16_t BufferUDP::ReadInt16(){
	uint16_t value;

	ASSERT( index + 2 < size, __FILE__, __LINE__ );

#ifndef __LITTLE_ENDIAN
	value = bswap( *((uint16_t*)(data+index)) );
#else // #ifdef BIG_ENDIAN
	value = *((uint16_t*)(data+index));
#endif // #ifdef BIG_ENDIAN

	index += 2;

	return value;

}
uint8_t BufferUDP::ReadByte(){
	uint8_t value;

	ASSERT( index + 1 < size, __FILE__, __LINE__ );

#ifndef __LITTLE_ENDIAN
	value = bswap( *((uint8_t*)(data+index)) );
#else // #ifdef BIG_ENDIAN
	value = *((uint8_t*)(data+index));
#endif // #ifdef BIG_ENDIAN

	index += 1;

	return value;
}




BufferUDP::BufferUDP(const uint8_t * Buffer, int bytes)
{
	this->data = Buffer;
	this->size=bytes;
	this->index=0;
	this->pos_in_scratch_value=0;
	this->scratch_value = 0;
}
uint32_t BufferUDP::nbr_of_bit_left()
{
	return( (size-index)*8-pos_in_scratch_value);
}
uint32_t BufferUDP::ReadBits( uint16_t nbr)
{
	uint32_t output;

	ASSERT( nbr <= 32, __FILE__, __LINE__ ); /* not asking to read more than 32 bits at a time */
	ASSERT( index + nbr/8 + 1 <= size, __FILE__, __LINE__ ); /* still enough bits to read */

	for (int i=0; (i<MIN(4,(size-index)) ) && (pos_in_scratch_value < 32); i++){
		uint64_t value_read = (uint64_t)ReadByte();
		uint64_t mask = ~MASK_UPTO_BIT(pos_in_scratch_value);
		scratch_value += ( (value_read<<pos_in_scratch_value) & mask );
		pos_in_scratch_value += 8;
	}


	output = scratch_value & MASK_UPTO_BIT(nbr); /* keep last nbr bits */

	pos_in_scratch_value-=nbr;
	scratch_value = (scratch_value>>nbr) & MASK_UPTO_BIT(pos_in_scratch_value) ;

	return output;
}
void BufferUDP::WriteBits(uint32_t value, uint16_t nbr)
{
	ASSERT( nbr <= 32, __FILE__, __LINE__ );
	ASSERT( index + nbr/8 + 1 <= size, __FILE__, __LINE__ );

	scratch_value += (uint64_t)value<<pos_in_scratch_value;
	pos_in_scratch_value += nbr;

	if (pos_in_scratch_value >= 32)
	{
		pos_in_scratch_value -= 32;
		WriteInt32(MASK_UPTO_BIT(32) & scratch_value);
		scratch_value = (scratch_value >> 32) & MASK_UPTO_BIT(32);
	}

}
void BufferUDP::FlushBits( ){
	if (pos_in_scratch_value > 0){
		WriteInt32(scratch_value & MASK_UPTO_BIT(32) );
	}
	scratch_value = 0;
	pos_in_scratch_value = 0;
}

bool BufferUDP::Send(char * ip, uint32_t port){
	udpSend(data, index, ip, port);
	return true;
}


int32_t BufferUDP::RecvAny(uint32_t port_in){
	return udpRecvAny(data, size,port_in);
}
int32_t BufferUDP::RecvAny(uint32_t port_in, uint32_t time_out_ms){
	return udpRecvAny(data, size, port_in, time_out_ms);
}
