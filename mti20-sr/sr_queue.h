/*-----------------------------------------------------------------------------
* Filename: sr_queue.h
* Purpose: a thread-safe queue, designed for storing packets waiting to be
           processed
*-----------------------------------------------------------------------------*/

#ifndef SR_QUEUE_H_
#define SR_QUEUE_H_

#include<stdint.h>
#include<pthread.h>

/*-----------------------------------------------------------------------------
 * Data structure: sr_queue_item
 * Purpose: internal data structure designed to hold packet length and pointers
 * to their data
 *-----------------------------------------------------------------------------*/
struct sr_queue_item {
  uint8_t* packet_data;
  unsigned int packet_length;
};
typedef struct sr_queue_item sr_queue_item;

/*-----------------------------------------------------------------------------
 * Data structure: sr_packet_queue
 * Purpose: The actual queue data structure.
 *          Contains an array of queue items, the maximum allowable size of
 *          the queue and the current number of items in the queue
 *          Also contains a mutex so that only one thread can enqueue/dequeue
 *          at a time and a condition variable so that worker threads don't have
 *          to busy-wait for packets
 *-----------------------------------------------------------------------------*/
struct sr_packet_queue {
  unsigned int max_length;
  unsigned int length;
  struct sr_queue_item* items;
  pthread_mutex_t mutex;
  pthread_cond_t nonempty;
};
typedef struct sr_packet_queue sr_packet_queue;

/*-----------------------------------------------------------------------------
* Method: initqueue (...)
* Purpose: Allocate memory for the queue, with a given maximum size
* Returns: 0 if successful
*          1 if out of memory and can't initialise it
*          2 if size is zero, which is almost certainly not what the caller
*                                                                      intended
*          something else otherwise
*-----------------------------------------------------------------------------*/
int initqueue( sr_packet_queue* queue   /* pointer to the queue to initialise */,
               unsigned int maxsize      /* maximum allowable size that the queue can grow to */ );

/*-----------------------------------------------------------------------------
* Method: destroyqueue (...)
* Purpose: Frees up the memory allocated to the queue
* Returns: 0 if successful
           something else otherwise
*-----------------------------------------------------------------------------*/
int destroyqueue( sr_packet_queue* queue   /* pointer to the queue to destroy */ );

/*-----------------------------------------------------------------------------
* Method: enqueue (...)
* Purpose: Add a packet to the queue
*          Data is borrowed, its lifetime on the queue will be equal to its
*          current lifetime
* Returns: 0 if successful
*          1 if the queue was full
*          2 if out of memory and can't add the item
*          something else otherwise
*-----------------------------------------------------------------------------*/
int enqueue( sr_packet_queue* queue     /* queue to add item to */,
             uint8_t* packet_data       /* raw packet data to add to the queue */, 
             unsigned int packet_length /* length of packet being added to the queue */ ); 

/*-----------------------------------------------------------------------------
* Method: dequeue (...)
* Purpose: Remove a packet from the queue
*          If there are no packets on the queue, it blocks until there are
* Returns: length of dequeued packet if successful
*          0 if queue was destroyed while blocked
*          negative number if failed for some other reason
*-----------------------------------------------------------------------------*/
int dequeue( sr_packet_queue* queue     /* queue to dequeue the item from */,
             uint8_t* packet_data       /* pointer to the location that packet data will be written to */ );

#endif /* SR_QUEUE_H_ */
