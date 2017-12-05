#include "contiki.h"

#include <string.h>



#include "chaos.h"

#include "chaos-random-generator.h"

#include "node.h"

#include "quorum.h"

#include "chaos-config.h"



#undef ENABLE_COOJA_DEBUG

#define ENABLE_COOJA_DEBUG COOJA

#include "dev/cooja-debug.h"



#define FLAGS_LEN_X(X)   (((X) >> 3) + (((X) & 7) ? 1 : 0))

#define FLAGS_LEN   (FLAGS_LEN_X(chaos_node_count))


#define LAST_FLAGS  ((1 << (((chaos_node_count - 1) & 7) + 1)) - 1)


#define FLAG_SUM    (((FLAGS_LEN - 1) << 8) - (FLAGS_LEN - 1) + LAST_FLAGS)


#define NETSTACK_CONF_WITH_CHAOS_NODE_DYNAMIC 0

#if NETSTACK_CONF_WITH_CHAOS_NODE_DYNAMIC
#define FLAGS_ESTIMATE FLAGS_LEN_X(MAX_NODE_COUNT)

#warning "APP: due to packet size limitation: maximum network size = MAX_NODE_COUNT"

#else

#define FLAGS_ESTIMATE FLAGS_LEN_X(CHAOS_NODES)

#endif





typedef struct __attribute__((packed)) quorum_t_struct {

  uint16_t quorum;

  uint8_t flags[];

} quorum_t;



typedef struct __attribute__((packed)) quorum_t_local_struct {

  quorum_t quorum;

  uint8_t flags[FLAGS_ESTIMATE];

} quorum_t_local;





static quorum_t_local quorum_local; /* used only for house keeping and reporting */

static uint8_t* quorum_flags;

static chaos_state_t

process(uint16_t round_count, uint16_t slot_count, chaos_state_t current_state, int chaos_txrx_success, size_t payload_length, uint8_t* rx_payload, uint8_t* tx_payload, uint8_t** app_flags)

{

  quorum_t* tx_quorum = (quorum_t*)tx_payload;

  quorum_t* rx_quorum = (quorum_t*)rx_payload;
  
 
 
 }
 
 int max_round_begin(const uint8_t app_id, uint16_t* value, uint8_t** final_flags)

{


COOJA_DEBUG_STR("value");


}















}





























