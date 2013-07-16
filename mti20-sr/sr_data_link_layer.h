/*-----------------------------------------------------------------------------
 *  Filename: sr_ethernet.h
 *  Purpose: routines and data structures for dealing with the data-link layer
 *           i.e. stripping off/extracting information from ethernet headers
 *           and putting them back on again
 *-----------------------------------------------------------------------------*/

#ifndef SR_ETHERNET_H_
#define SR_ETHERNET_H_

struct sr_ethernet_datagram {
  unsigned int datagram_length;
  struct sr_ethernet_header hdr;
  uint8_t payload[];
} __attribute__ ((packed));

/*-----------------------------------------------------------------------------
 * Method: sr_strip_datagram_hdr (...) {
 * Purpose: Called when processing a raw frame that has come off an interface.
 *          Decomposes the frame into its header and payload, ready to pass up
 *          to the network layer
 * Returns: 0 if successful, 1 otherwise
 *-----------------------------------------------------------------------------*/
int sr_strip_datagram_hdr (struct sr_ethernet_datagram* new_datagram /* pointer to unallocated member, which will contain the new datagram */,
                           struct uint8_t* raw_frame /* frame as received, ready to be processed */ );

/*-----------------------------------------------------------------------------
 * Method: sr_add_datagram_hdr (...) {
 * Purpose: Called when processing a datagram that needs to be sent out
 *          Puts the header and payload together into a uint8_t array, ready
 *          to send down to the physical layer
 * Returns: 0 if successful, 1 otherwise
 *-----------------------------------------------------------------------------*/
int sr_add_datagram_hdr ();


#endif
