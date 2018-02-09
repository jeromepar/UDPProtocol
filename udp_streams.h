/*
 * udp_streams.h
 *
 *  Created on: 8 févr. 2018
 *      Author: jerome
 */

#ifndef UDP_STREAMS_H_
#define UDP_STREAMS_H_

#include "rw_packets.h"

#define assert(cond) {if ( !(cond)){printf("assert failed %20s:%3d [%s] \n",__FILE__,__LINE__,#cond); return false;}}

class WriteStream
{
public:

    enum { IsWriting = 1 };
    enum { IsReading = 0 };

    WriteStream( uint8_t * Buffer, int bytes );

    template <typename inType>  bool serialize_bits( inType value, int64_t min, int64_t max );
    bool Send(char * ip, uint32_t port);

private:

    BufferUDP m_writer;
};

class ReadStream
{
public:

    enum { IsWriting = 0 };
    enum { IsReading = 1 };

    ReadStream( const uint8_t * Buffer, int bytes );

    template <typename outType>  bool serialize_bits( outType & value, int64_t min, int64_t max );
    bool atTheEnd();
    int32_t RecvAny(uint32_t port_in);
    int32_t RecvAny(uint32_t port_in, uint32_t time_out_ms);

private:

    BufferUDP m_reader;
};

uint32_t bits_required(int64_t min, int64_t max);




/* templates instantiations */
template <typename outType>  bool ReadStream::serialize_bits( outType & value, int64_t min, int64_t max )
{
	assert( min < max );

	const uint32_t bits = bits_required( min, max );
	assert( m_reader.nbr_of_bit_left() >= bits);

	uint32_t unsigned_value = m_reader.ReadBits( bits );
	value = (outType)unsigned_value + min;
	return true;
}

template <typename inType>  bool WriteStream::serialize_bits( inType value, int64_t min, int64_t max )
{
	assert( min < max );
	assert( value >= min );
	assert( value <= max );
	const uint32_t bits = bits_required( min, max );
	uint32_t unsigned_value = value - min;
	printf("range %d, bits %d\n",max-min,bits);

	m_writer.WriteBits( unsigned_value, bits );
	return true;
}

#endif /* UDP_STREAMS_H_ */
