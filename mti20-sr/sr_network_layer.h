/*-----------------------------------------------------------------------------
* Filename: sr_network_layer.h
* Purpose: routines and data structures for dealing with the network layer
*          i.e. stripping off/extracting information from IP headers, putting
*          them back on again, checking validity of the packet and checking/
*          computing the checksum
*-----------------------------------------------------------------------------*/

#ifndef SR_ETHERNET_H_
#define SR_ETHERNET_H_

#include "sr_protocol_headers.h"

struct sr_ip_packet {
  struct sr_ip_hdr* hdr;      // ip header as defined in sr_protocol_headers.h
  uint8_t* payload;           // length of payload is packet_length-20
} __attribute__ ((packed));

/*-----------------------------------------------------------------------------
* Method: sr_process_ip_packet (...) {
* Purpose: First method to be called after taking a packet off the input queue
*          Splits packet into header and payload, checks validity, checks
*          checksum and, if everything's OK, then passes packet to forwarding
*          routines
* Returns: 0 if successful
*          1 if dropped because header checksum didn't check out
*          something else otherwise
*-----------------------------------------------------------------------------*/
int sr_process_ip_packet(uint8_t* raw_packet /* pointer to the raw data containing the entire packet */,
                         unsigned int len /* length of the packet */ );

/*-----------------------------------------------------------------------------
* Method: sr_strip_ip_hdr (...) {
* Purpose: Called when processing the payload of an ethernet datagram passed up
*          from the data-link layer
*          Decomposes the packet into its header and payload, ready for
*          further processing or to pass up to the transport layer
* Returns: 0 if successful, something else otherwise
*-----------------------------------------------------------------------------*/
int sr_strip_ip_hdr (struct sr_ip_packet* new_packet /* pointer to unallocated memory, which will contain the new packet */,
                           unsigned int len /* length of the entire packet (input data) */,
                           uint8_t* raw_frame /* packet as received from the data-link layer, ready to be processed */ );

/*-----------------------------------------------------------------------------
* Method: sr_add_ip_hdr (...) {
* Purpose: Called when processing a packet that needs to be sent out
*          Puts the header and payload together into a uint8_t array, ready
*          to send down to the data-link layer to be the payload of a datagram
* Returns: length of combined packet, -1 if failed
*-----------------------------------------------------------------------------*/
int sr_add_ip_hdr (uint8_t* payload /* pointer to the payload, ready to copy into the packet */,
                   struct in_addr to /* to address */,
                   struct in_addr from /* from address */,
                   uint8_t* raw_frame /* packet ready to be sent down to the data-link layer */ );

#endif /* SR_ETHERNET_H_ */

