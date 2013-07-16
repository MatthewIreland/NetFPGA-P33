/*-----------------------------------------------------------------------------
* file: sr_network_layer.c
* date: Tue Jul 16 MMXIII
* Author: Matthew Ireland <mti20@cam.ac.uk>
*
* Description:
* Methods to implement stripping/adding IP headers, checking validity of the
* packet and checking/computing the checksum
*
* This is where most of the elementary processing of network layer packets
* happens - they're then passed on to the routines in sr_forwarding.c, where
* the cool stuff happens :-)
*
*---------------------------------------------------------------------------*/

#include "sr_network_layer.h"
#include <stdint.h>
#include <stdio.h>

int sr_process_ip_packet(uint8_t* raw_packet /* pointer to the raw data containing the entire packet */,
                         unsigned int len /* length of the packet */ ) {
  // declare storage for decomposed packet
  struct sr_ip_packet* packet;

  // strip off the headers
  if ( sr_strip_ip_hdr(packet,len,raw_packet) != 0 ) {
    perror("Stripping headers failed");
    return -1;
  }

  // check header checksum, drop packet if doesn't check out
  // TODO

  // check ip version, send ICMP message and drop packet if unsupported
  // TODO

  // check there are no options; if there are, drop the packet
  // TODO

  // check the packet is not fragmented, if it is, drop it
  // TODO

  // check the higher layer protocol is TCP; if not, send ICMP message and drop packet
  // TODO

  // pass on to forwarding functions
  // TODO
}

int sr_strip_ip_hdr (struct sr_ip_packet* new_packet /* pointer to unallocated memory, which will contain the new packet */,
                     unsigned int len /* length of the entire packet (input data) */,
		     uint8_t* raw_frame /* packet as received from the data-link layer, ready to be processed */ ) {

  // split raw_frame (relying on the memory that it's already been allocated)
  new_packet = (struct sr_ip_packet*)raw_frame;

  /* ---  debugging stuff - print packet details --- */
  struct in_addr from = new_packet->hdr->ip_src;
  struct in_addr to = new_packet->hdr->ip_dst;
  uint16_t total_length = new_packet->hdr->ip_len;
  printf(   "-- Received IP packet. From: %s    To: %s    Total length (bytes): %d\n",
            inet_ntoa(from),
	    inet_ntoa(to),
	    total_length   );
  /* --- end debugging stuff --- */
}

int sr_add_ip_hdr (uint8_t* payload /* pointer to the payload, ready to copy into the packet */,
                   struct in_addr to /* to address */,
                   struct in_addr from /* from address */,
                   uint8_t* raw_frame /* packet ready to be sent down to the data-link layer */ ) {


  // TODO

  // finally, free the memory that was allocated to the payload

}
