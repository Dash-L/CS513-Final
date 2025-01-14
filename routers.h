#pragma once
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "vec.h"
#include "queue.h"

#define MAX_NODES 256

extern pthread_mutex_t printMutex;

struct ROUTER_INFO;

enum ROUTER_MESSAGE_TYPE {
  ROUTER_MESSAGE_DEBUG,
  ROUTER_MESSAGE_PACKET,
  ROUTER_MESSAGE_EDGE_ADD,
  //EDGE_REMOVE,
  //EDGE_UPDATE,
  ROUTER_MESSAGE_WEIGHTS_UPDATE
};

enum ROUTER_EDGE_ADD_REPLY_TYPE {
  ROUTER_EDGE_ADD_DO_NOT_REPLY,
  ROUTER_EDGE_ADD_REPLY_IN_KIND,
  ROUTER_EDGE_ADD_REPLY_SEND_DIST_VEC,
};

typedef union {
  struct {
    double *distVec;
    struct ROUTER_INFO *source;
  } distanceVectorMessage;

  struct {
    double weight;
    struct ROUTER_INFO *other;
    enum ROUTER_EDGE_ADD_REPLY_TYPE repRequired;
  } edgeAdditionMessage;

  struct {
    char message;
    unsigned char destination;
  } packetMessage;
} ROUTER_MESSAGE_CONTENTS;

typedef struct ROUTER_MESSAGE {
  enum ROUTER_MESSAGE_TYPE msgType;
  ROUTER_MESSAGE_CONTENTS contents;
} ROUTER_MESSAGE;

typedef struct ROUTER_EDGE_RECORD {
  struct ROUTER_INFO *end;
  double cost;
  double distanceVector[MAX_NODES];
} ROUTER_EDGE_RECORD;

typedef struct {
  unsigned char name;
  double cost;

} ROUTER_DIST_ENTRY;

VEC(ROUTER_MESSAGE);

QUEUE(ROUTER_MESSAGE);

VEC(ROUTER_EDGE_RECORD);

// For describing the thread
typedef struct ROUTER_INFO {
  ROUTER_MESSAGE_QUEUE *incomingMessageQueue;
  unsigned char name;
  ROUTER_EDGE_RECORD_VEC *edges;
} ROUTER_INFO;

ROUTER_INFO *ROUTER_INFO_create(unsigned char n);

void *router(void *arg);

typedef struct ROUTER_MANAGER {
  ROUTER_INFO *allRouters[MAX_NODES];
} ROUTER_MANAGER;

void ROUTER_MANAGER_add_edge(ROUTER_MANAGER *manager, unsigned char a, unsigned char b, double weight);
void ROUTER_MANAGER_remove_edge(ROUTER_MANAGER *manager, unsigned char a, unsigned char b);
void ROUTER_MANAGER_print_distance_vec(ROUTER_MANAGER *manager, unsigned char name);
void ROUTER_MANAGER_route_packet(ROUTER_MANAGER *manager, unsigned char a, unsigned char b, char message);
ROUTER_MANAGER *ROUTER_MANAGER_create();
