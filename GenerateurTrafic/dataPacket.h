#ifndef _DATAPACKET_H
#define _DATAPACKET_H

#include <stdint.h>

//Structure for storing packet messages to exchange
typedef struct _packetData{
  unsigned long id;
  double time_sec;
  int data_size;
  char *data;
} packetData;

char *packet_to_buffer(const packetData *packet);

packetData *buffer_to_packet(const char* buffer);

uint32_t get_packet_size(const packetData *packet);

void free_packet(packetData *packet);

void print_packet(const packetData *packet);

#endif //_DATAPACKET_H