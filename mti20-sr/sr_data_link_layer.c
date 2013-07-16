/*-----------------------------------------------------------------------------
 * file:  sr_data_linK_layer.c
 * date:  Tue Jul 16 MMXIII
 * Author: Matthew Ireland <mti20@cam.ac.uk>
 *
 * Description:
 *
 * Methods to implement stripping/adding ethernet headers
 *
 * This is where most of the functionality of the data-link layer is
 * implemented
 *
 *---------------------------------------------------------------------------*/

int sr_strip_datagram_hdr (struct sr_ethernet_datagram* new_datagram /* pointer to unallocated member, which will contain the new datagram */,
                           unsigned int len /* length of raw frame */,
                           uint8_t* raw_frame /* frame as received, ready to be processed */ ) {

  // allocate memory for datagram
  new_datagram = (struct sr_ethernet_datagram*)malloc(sizeof(sr_ethernet_datagram));
  if (!new_datagram) {
    perror("malloc failed on allocating space for datagram");
    return 1;
  }

  // allocate and populate header
  new_datagram->header = (sr_ethernet_header*)malloc(sizeof(sr_ethernet_hdr));
  if (! new_datagram->header) {
    perror("malloc failed on allocating ethernet header");
    return 2;
  }
  memcpy(new_datagram->header, raw_frame, 22);

  // allocate and populate payload
  int payload_length = len-22;
  new_datagram->payload = (uint8_t*)malloc(payload_length*sizeof(uint8_t));
  if (! new_datagram->payload) {
    perror("malloc failed on allocating ethernet payload");
    return 3;
  }
  memcpy(new_datagram->payload, raw_frame[23], payload_length);

  // debugging - print IP address from which packet was received (layering violation -- yes -- remove later)
  printf("%d.%d.%d.%d",new_datagram->payload[13],new_datagram->payload[14],new_datagram->payload[15],new_datagram->payload[16]);


}

nt sr_add_datagram_hdr (struct sr_ethernet_datagram* orig_datagram /* pointer to the separate header and payload ready to combine for the physical layer */,
			uint8_t* raw_frame   /* unallocated memory, ready to contain the combined datagram */) {

  // TODO

}

