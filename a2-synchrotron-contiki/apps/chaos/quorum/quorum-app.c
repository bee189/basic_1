#include "contiki.h"
#include <stdio.h> /* For printf() */
#include "net/netstack.h"

#include "chaos-control.h"
#include "node.h"
#include "quorum.h"

static uint16_t value = 0;
static uint8_t* flags;
static uint16_t complete = 0;
static uint8_t phase = 0;
static uint16_t off_slot;

static void round_begin(const uint16_t round_count, const uint8_t id);

CHAOS_APP(chaos_quorum_app, MAX_SLOT_LEN, MAX_ROUND_MAX_SLOTS, 1, max_is_pending, round_begin);
#if NETSTACK_CONF_WITH_CHAOS_NODE_DYNAMIC
#include "join.h"
CHAOS_APPS(&join, &chaos_quorum_app);
#else
CHAOS_APPS(&chaos_quorum_app);
#endif /* NETSTACK_CONF_WITH_CHAOS_NODE_DYNAMIC */

PROCESS(chaos_quorum_app_process, "Chaos quorum App Process");
AUTOSTART_PROCESSES(&chaos_quorum_app_process);

PROCESS_THREAD(chaos_quorum_app_process, ev, data)
{

PROCESS_BEGIN();
  printf("{boot} Quorum Test Application\n");
  NETSTACK_MAC.on();
  
while( 1 ){
		PROCESS_YIELD();
		if(chaos_has_node_index){
        

     
}
}

PROCESS_END();



}

static void round_begin(const uint16_t round_count, const uint8_t id){
  value = 1;
   complete = quorum_round_begin(id, &value, &flags, &phase);
  off_slot = max_get_off_slot();
  process_poll(&chaos_quorum_app_process);
}
