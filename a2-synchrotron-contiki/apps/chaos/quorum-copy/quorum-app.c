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
static uint8_t header= 0;
static uint16_t off_slot;
static uint16_t time_stamp =0;
static uint16_t round_count_local;
static void round_begin(const uint16_t round_count, const uint8_t id);

CHAOS_APP(chaos_quorum_app, MAX_SLOT_LEN, MAX_ROUND_MAX_SLOTS, 1, quorum_is_pending, round_begin);
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

				if(header == READ_HEADER){
					printf("reading ");
					
				}
				else if(header == WRITE_HEADER){
					printf("writing ");
				}
				else if(header == WRITE_QUERY_HEADER){
					printf("write query ");
				}
                 printf("Round:%u[value:%u Seq.nr:%u flags:" ,round_count_local, value, time_stamp);
                 int i;
                   
                    for( i=0; i<quorum_get_flags_length(); i++ ){
						
                         printf("%02x", flags[i]);

                         }  
						 printf("]");
				
}

}



PROCESS_END();
}

static void round_begin(const uint16_t round_count, const uint8_t id){
 // value = 1;
   complete = quorum_round_begin(round_count, id, &value, &flags, &phase, &time_stamp, &header);
   round_count_local = round_count;
 // off_slot = quorum_get_off_slot();
  process_poll(&chaos_quorum_app_process);
}
