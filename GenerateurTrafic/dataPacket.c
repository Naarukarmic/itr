#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "dataPacket.h"

void free_packet(packetData *packet){
	free( packet->data);
	free(packet);
}

uint32_t get_packet_size(const packetData *packet){
	const uint32_t packet_size_no_data = sizeof(unsigned long) + sizeof(double)+ sizeof(int);
	const uint32_t packet_size_data = ((packet->data_size + 1) * sizeof(char) );
	return packet_size_data+packet_size_no_data;
}

char *packet_to_buffer(const packetData *packet){
	const uint32_t packet_size_no_data = sizeof(unsigned long) + sizeof(double)+ sizeof(int);
	const uint32_t packet_size_data = (packet->data_size * sizeof(char) ) +1;
	char *buffer = malloc((packet_size_no_data+packet_size_data) * sizeof(char));
	memcpy(buffer, (void *) packet, packet_size_no_data);
	memcpy(buffer + packet_size_no_data, packet->data, packet_size_data);
	return buffer;
}

packetData *buffer_to_packet(const char* buffer){
	const uint32_t packet_size_no_data = sizeof(unsigned long) + sizeof(double)+ sizeof(int);
	packetData *packet = malloc(sizeof(packetData));
	memcpy(packet, buffer, packet_size_no_data);
	const uint32_t packet_size_data = (sizeof(char) * packet->data_size)+1;
	packet->data = malloc(packet_size_data * sizeof(char));
	memcpy(packet->data, buffer+packet_size_no_data,packet_size_data);
	return packet;
}

void print_packet(const packetData *packet){
	printf("ID: %lu\nTime (sec): %f\nData size: %d\nData: %s\n",
		packet->id,packet->time_sec,packet->data_size,packet->data );
}