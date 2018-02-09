/*
 * udp_streams.cpp
 *
 *  Created on: 8 févr. 2018
 *      Author: jerome
 */

#include "udp_streams.h"

WriteStream::WriteStream( uint8_t * Buffer, int bytes ) : m_writer( Buffer, bytes ) {}



bool WriteStream::Send(char * ip, uint32_t port){
	bool ret;
	this->m_writer.FlushBits();
	ret = this->m_writer.Send(ip, port);
	assert(ret);
	return true;
}


ReadStream::ReadStream( const uint8_t * Buffer, int bytes ) : m_reader( Buffer, bytes ) {}



bool ReadStream::atTheEnd(){
	bool ret = this->m_reader.nbr_of_bit_left()==0;
	return ret;
}

int32_t ReadStream::RecvAny(uint32_t port_in){
	return m_reader.RecvAny(port_in);
}
int32_t ReadStream::RecvAny(uint32_t port_in, uint32_t time_out_ms){
	return m_reader.RecvAny(port_in, time_out_ms);
}

uint32_t bits_required(int64_t min, int64_t max){
	uint64_t range = max-min; /* all numbers in [min:max] */

	uint32_t bits_nbr=0;
	while (range!=0){
		range>>=1;
		bits_nbr++;
	}

	return bits_nbr;
}

