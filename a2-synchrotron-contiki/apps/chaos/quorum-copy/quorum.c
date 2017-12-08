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



#ifndef MAX_NODE_COUNT
 
#define MAX_NODE_COUNT 254

#endif
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

  uint16_t write_value;
  uint16_t seq_nr;
  uint8_t msg_type;

  uint8_t flags[];

} quorum_t;

typedef struct __attribute__((packed)) quorum_t_local_struct {

  quorum_t quorum_value;

  uint8_t phase;
  uint16_t local_seq_nr;  
  uint8_t flags[];

} quorum_t_local;



static int complete, rx = 1, tx = 0, flag_sum , i;

static quorum_t_local quorum_local; /* used only for house keeping and reporting */

static uint8_t* quorum_flags;

static chaos_state_t




process(uint16_t round_count, uint16_t slot_count, chaos_state_t current_state, int chaos_txrx_success, size_t payload_length, uint8_t* rx_payload, uint8_t* tx_payload, uint8_t** app_flags)

{

  quorum_t* tx_quorum = (quorum_t*)tx_payload;

  quorum_t* rx_quorum = (quorum_t*)rx_payload;

  unsigned int array_index = chaos_node_index / 8;

  unsigned int array_offset = chaos_node_index % 8;
// chaos_state_t next_state = CHAOS_RX;

   chaos_state_t next_state;  
 

  if (IS_INITIATOR() && current_state == CHAOS_INIT){
    quorum_local.quorum_value.write_value = 2 ;
    next_state = CHAOS_TX;
 rx =0;
     tx = 1;
     quorum_local.phase = WRITE_PROPOSE;
     quorum_local.local_seq_nr = quorum_local.local_seq_nr+1;
    quorum_local.quorum_value.flags[array_index] |= 1 << (array_offset);
        }
  else {

        next_state = CHAOS_RX;

        }


     if ( current_state == CHAOS_TX && !chaos_txrx_success ){
        next_state == CHAOS_TX;
        }
      else if ( current_state == CHAOS_RX && chaos_txrx_success ){

        if (rx_quorum->seq_nr > quorum_local.local_seq_nr ){
        quorum_local.local_seq_nr = rx_quorum->seq_nr; 
        quorum_local.quorum_value.write_value = rx_quorum->write_value;

          }
         
         else {        
         for( i = 0; i < FLAGS_LEN; i++){
        COOJA_DEBUG_STR("f");
        //tx |= (rx_quorum->flags[i] != tx_quorum->flags[i]);
        tx_quorum->flags[i] |= rx_quorum->flags[i];
        flag_sum += tx_quorum->flags[i];
        }
        if(flag_sum > FLAGS_LEN){
        complete = 1;
        }
        else {
        tx =1;
        }
        }
        }
        
if (tx ==1){
                next_state = CHAOS_TX;
                tx_quorum->write_value =  quorum_local.quorum_value.write_value;
                 tx_quorum->seq_nr = quorum_local.local_seq_nr;
                }

        if(complete)
        {
          next_state = CHAOS_OFF;
          tx = 0;
        }

    *app_flags = tx_quorum->flags;
   return next_state;

}



 
 int quorum_round_begin(const uint16_t round_number, const uint8_t app_id, uint16_t* value, uint8_t** final_flags, uint8_t* phase ,uint16_t* time_stamp )

{


COOJA_DEBUG_STR("value");


quorum_local.quorum_value.write_value = *value;

unsigned int array_index = chaos_node_index / 8;

unsigned int array_offset = chaos_node_index % 8;

chaos_round(round_number, app_id, (const uint8_t const*)&quorum_local.quorum_value.write_value, sizeof(quorum_t) + quorum_get_flags_length(), MAX_SLOT_LEN_DCO, MAX_ROUND_MAX_SLOTS, quorum_get_flags_length(), process);

*value = quorum_local.quorum_value.write_value;

*time_stamp = quorum_local.local_seq_nr;
}

int quorum_is_pending(const uint16_t round_count){
return 1;

}


int quorum_get_flags_length() {

  return FLAGS_LEN;

}
