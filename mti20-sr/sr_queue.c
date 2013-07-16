/*-----------------------------------------------------------------------------
* file: sr_queue.c
* date: Tue Jul 16 MMXIII
* Author: Matthew Ireland <mti20@cam.ac.uk>
*
* Description:
* TODO
*
*---------------------------------------------------------------------------*/

#include<stdlib.h>
#include<stdio.h>
#include "sr_queue.h"

int initqueue( sr_packet_queue* queue,   /* pointer to the queue to initialise */
               unsigned int maxsize      /* maximum allowable size that the queue can grow to */ ) {
  // allocate memory
  queue = (sr_packet_queue*)malloc(sizeof(sr_packet_queue));
  queue->items = (sr_queue_item*)malloc(maxsize*sizeof(sr_queue_item));

  if ( (!queue) || (!queue->items)) {
    perror("malloc failed - could not allocate memory for queue");
    return 1;
  }
  
  // initialise variables and state
  // if maxsize is zero, throw an error, since the condition test in enqueue won't work (also it's silly)
  if (maxsize == 0) {
    perror("Couldn't initialise queue: size must be non-zero");
    return 2;
  }
  queue->max_length = maxsize;
  queue->length = 0;
  pthread_mutex_init(&(queue->mutex), 0);
  pthread_cond_init(&(queue->nonempty), 0);

  // --- debugging stuff ---
  printf("--Initialised packet input queue. Maxsize: %d",queue->max_length);
  // --- end debugging stuff ---
}

int enqueue( sr_packet_queue* queue     /* queue to add item to */,
             uint8_t* packet_data       /* raw packet data to add to the queue */, 
             unsigned int packet_length /* length of packet being added to the queue */ ) {

  // acquire lock before adding item to queue
  // (block if someone else has it)
  pthread_mutex_lock(&(queue->mutex));

  // if queue length is zero, signal so that if someone is blocked on dequeue, they can wake up
  if (queue->length == 0) {
    pthread_cond_signal(&(queue->nonempty));
  } else if (queue->length == queue->max_length) {     // if queue is full, signal an error (packet most likely needs to be dropped)
    perror("Queue is full: could not add packet");
    pthread_mutex_unlock(&(queue->mutex));
    return 1;
  }

  // populate the item
  sr_queue_item new_item = {packet_data, packet_length};

  // finally, add the item to the queue
  queue->items[queue->length] = new_item;
  queue->length++;

  // --- debugging stuff ---
  printf("--Added packet to queue. Queue length now %d",queue->length);
  // --- end debugging stuff ---
  
  // clean up
  pthread_mutex_unlock(&(queue->mutex));
}


int dequeue( sr_packet_queue* queue     /* queue to dequeue the item from */,
             uint8_t* packet_data       /* pointer to the location that packet data will be written to */ ) {
  // acquire lock before remove item from queue
  // (block if someone else has it)
  pthread_mutex_lock(&(queue->mutex));

  // if queue length is non-zero, release the lock and block until we can do something
  if (queue->length == 0) {
    pthread_cond_wait(&queue->nonempty, &queue->mutex);
    if (queue->length == 0) {     // something's gone wrong (e.g. queue destroyed) if it's still zero
      pthread_mutex_unlock(&(queue->mutex));
      return 0;
    }
  }

  // retrieve length of packet and extract data
  int packet_length = (queue->items[queue->length]).packet_length;
  packet_data = (queue->items[queue->length]).packet_data;

  // remove item from queue
  queue->length--;

 // --- debugging stuff ---
  printf("--Removed packet from queue. Queue length now %d",queue->length);
  // --- end debugging stuff ---

  // clean up
  pthread_mutex_unlock(&(queue->mutex));  

  return packet_length;
}

int destroyqueue( sr_packet_queue* queue   /* pointer to the queue to destroy */ ) {
  // acquire the lock to make sure that noone else is currently using the queue
  pthread_mutex_lock(&(queue->mutex));

  // free up everything still in the queue (since references will be lost upon destroying
  // the queue anyway
  int queue_position;
  for (queue_position=queue->length; queue_position>0; queue_position--) {
    sr_queue_item current_item = queue->items[queue_position];
    free(current_item.packet_data);
  }

  // wake up anyone still waiting on the queue - they'll have to handle the error condition
  pthread_cond_broadcast(&queue->nonempty);

  // release the lock (fine, because if someone wakes up, they'll release there's an error and exit)
  pthread_mutex_unlock(&(queue->mutex));  

  // destroy the lock and condition variable
  pthread_mutex_destroy(&queue->mutex);
  pthread_cond_destroy(&queue->nonempty);

  // ... and finally free up the memory for the queue structure itself
  free(queue);
}
