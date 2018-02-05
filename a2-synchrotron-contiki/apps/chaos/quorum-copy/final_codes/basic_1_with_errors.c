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
//#define FLAGS_LEN CHAOS_NODES

#define LAST_FLAGS  ((1 << (((chaos_node_count - 1) & 7) + 1)) - 1)


#define FLAG_SUM    (((FLAGS_LEN - 1) << 8) - (FLAGS_LEN - 1) + LAST_FLAGS)


#define NETSTACK_CONF_WITH_CHAOS_NODE_DYNAMIC 0

#if NETSTACK_CONF_WITH_CHAOS_NODE_DYNAMIC
#define FLAGS_ESTIMATE FLAGS_LEN_X(MAX_NODE_COUNT)

#warning "APP: due to packet size limitation: maximum network size = MAX_NODE_COUNT"

#else
#define FLAGS_ESTIMATE FLAGS_LEN_X(CHAOS_NODES)
//#define FLAGS_ESTIMATE (CHAOS_NODES)
#endif

#define FAILURE 1




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
  uint8_t flags[FLAGS_ESTIMATE];

} quorum_t_local;



static int complete, tx = 0 , i, txrx_count, done = 0, j = 0, local_round, t, a = 0, flip_coin, intiator_id, set, a1, flag_arry[30], turn = 0;

static uint16_t flag_sum = 0, flag_sum_1 = 0;

static quorum_t_local quorum_local; /* used only for house keeping and reporting */

static uint8_t* quorum_flags, quorum_flags1;

static chaos_state_t




process(uint16_t round_count, uint16_t slot_count, chaos_state_t current_state, int chaos_txrx_success, size_t payload_length, uint8_t* rx_payload, uint8_t* tx_payload, uint8_t** app_flags)

{

  quorum_t* tx_quorum = (quorum_t*)tx_payload;

  quorum_t* rx_quorum = (quorum_t*)rx_payload;

  unsigned int array_index = chaos_node_index / 8;

  unsigned int array_offset = chaos_node_index % 8;
// chaos_state_t next_state = CHAOS_RX;

   chaos_state_t next_state;

//&& quorum_local.phase == CAN_WRITE
/* int a = bin_count(*p); 
p = &flag_sum; */

/*  intiator_id = IS_INITIATOR; */

if (FAILURE == 1 && node_id==1 && turn == 1){
	 next_state = CHAOS_OFF;
	 printf("node_1 failure");
	
	
}

   else if (IS_INITIATOR()){
		
		// current_state == CHAOS_INIT &&
    if (current_state == CHAOS_INIT && quorum_local.phase == CAN_WRITE ){
		if(!complete)
		{
			printf("->write_%u failed ", quorum_local.quorum_value.seq_nr);
		}
		memset(tx_quorum->flags, 0, quorum_get_flags_length());
		tx_quorum->msg_type = WRITE_HEADER;
		tx_quorum->write_value = quorum_local.quorum_value.write_value + 1 ; 
		quorum_local.phase = WRITE_PROPOSE;
		tx_quorum->seq_nr = quorum_local.quorum_value.seq_nr+1;
		complete = 0;
	
		 unsigned int array_index = chaos_node_index / 8;
		 unsigned int array_offset = chaos_node_index % 8;
		 tx_quorum->flags[array_index] |= 1 << (array_offset);
		 next_state = CHAOS_TX;


        }
		
	else if (current_state == CHAOS_INIT && quorum_local.phase == CAN_READ ){
		
		    memset(tx_quorum->flags, 0, quorum_get_flags_length());
			tx_quorum->msg_type = READ_HEADER;
			tx_quorum->write_value = quorum_local.quorum_value.write_value;
			quorum_local.phase = READ_PROPOSE;
			tx_quorum->seq_nr = quorum_local.quorum_value.seq_nr;
			unsigned int array_index = chaos_node_index / 8;
			unsigned int array_offset = chaos_node_index % 8;
			tx_quorum->flags[array_index] |= 1 << (array_offset);
			
			next_state = CHAOS_TX;
		 
	 }

    else if(current_state == CHAOS_TX && chaos_txrx_success ){

        next_state = CHAOS_RX;
        }
    else if ( current_state == CHAOS_TX && !chaos_txrx_success){
        next_state = CHAOS_TX;

        }
        else if ( current_state == CHAOS_RX && !chaos_txrx_success ){


        next_state = CHAOS_TX;
        }

      else if ( current_state == CHAOS_RX && chaos_txrx_success ){
		  
		  /* if(rx_quorum->msg_type == WRITE_HEADER && quorum_local.phase == READ_PROPOSE ){
			  
				tx_quorum->msg_type = WRITE_HEADER;
				quorum_local.phase = WRITE_PROPOSE;
				memset(tx_quorum->flags, 0, quorum_get_flags_length());
				memcpy(tx_quorum->flags, rx_quorum->flags, quorum_get_flags_length());
				unsigned int array_index = chaos_node_index / 8;
				unsigned int array_offset = chaos_node_index % 8;
				tx_quorum->flags[array_index] |= 1 << (array_offset);
				tx_quorum->seq_nr = rx_quorum->seq_nr;
				tx_quorum->write_value = rx_quorum->write_value;
			  
		   }
		  
		  else if(rx_quorum->msg_type == WRITE_HEADER && quorum_local.phase == WRITE_PROPOSE){
			   
			   
			   
		   } */
		 

            if (rx_quorum->seq_nr == tx_quorum->seq_nr && rx_quorum->msg_type == tx_quorum->msg_type){

            flag_sum = 0;
            //int bit_count = 0;
			tx = 0;
			a = 0;
            for( i = 0; i < FLAGS_LEN; i++){ 
            tx |= (tx_quorum->flags[i] != rx_quorum->flags[i]);
            tx_quorum->flags[i] |= rx_quorum->flags[i];
            flag_sum += tx_quorum->flags[i];
            a1 = bin_count(flag_sum);
				 a = a1+a;
				 flag_sum = 0;
           
                    }
                    
            
           
            if(a > (CHAOS_NODES/2)){           
             //quorum_local.phase = CAN_WRITE;
             next_state = CHAOS_OFF;
			 complete = 1;

              }
			  else{
         next_state = CHAOS_TX;

				}
            }

            }
            else {
            next_state = CHAOS_TX;;
                 }
		/* if(tx){
			flag_stack(flag_sum);
			
		} */
		
					
					

    }

    else {
   //&&  quorum_local.phase != READ_PROPOSE
	
	if ((current_state == CHAOS_INIT) &&(quorum_local.phase ==  CAN_READ || quorum_local.phase == CAN_WRITE )){
		

			txrx_count = 0;
			 done = 0;
		    memset(tx_quorum->flags, 0, quorum_get_flags_length());
			unsigned int array_index = chaos_node_index / 8;
			unsigned int array_offset = chaos_node_index % 8;
			tx_quorum->flags[array_index] |= 1 << (array_offset);
			tx_quorum->msg_type = READ_HEADER;
			tx_quorum->write_value = quorum_local.quorum_value.write_value;
			quorum_local.phase = READ_PROPOSE;
			tx_quorum->seq_nr = quorum_local.quorum_value.seq_nr;
			next_state = CHAOS_TX;
	 }
	
	
	

       else if(current_state == CHAOS_RX && chaos_txrx_success){
			
			//tx_quorum->msg_type = rx_quorum->msg_type ;
			if(rx_quorum->msg_type != tx_quorum->msg_type ){
				
				/* next_state = CHAOS_TX; */
			   if(rx_quorum->msg_type == WRITE_HEADER){
				   
				memset(tx_quorum->flags, 0, quorum_get_flags_length());
				txrx_count = 0;
				done = 0;
				tx_quorum->msg_type = rx_quorum->msg_type;
				quorum_local.phase = WRITE_PROPOSE;
				/* memcpy(tx_quorum->flags, rx_quorum->flags, quorum_get_flags_length()); */
				unsigned int array_index = chaos_node_index / 8;
				unsigned int array_offset = chaos_node_index % 8;
				tx_quorum->flags[array_index] |= 1 << (array_offset);
				tx_quorum->seq_nr = rx_quorum->seq_nr;
				tx_quorum->write_value = rx_quorum->write_value;
				flag_sum = 0;
				tx = 0;	
				a = 0;
					
					 for( i = 0; i < FLAGS_LEN; i++){
						tx |= (tx_quorum->flags[i] != rx_quorum->flags[i]);
						tx_quorum->flags[i] |= rx_quorum->flags[i];
						flag_sum += tx_quorum->flags[i];
						a1 = bin_count(flag_sum);
						 a = a1+a;
						 flag_sum = 0;
						}
			  
					}
				}
				
		    else if(tx_quorum->msg_type == rx_quorum->msg_type){
     
				if (rx_quorum->seq_nr > tx_quorum->seq_nr ){
					
					memset(tx_quorum->flags, 0, quorum_get_flags_length());
					txrx_count = 0;
					 done = 0;
				/* memcpy(tx_quorum->flags, rx_quorum->flags, quorum_get_flags_length()); */
					unsigned int array_index = chaos_node_index / 8;
					unsigned int array_offset = chaos_node_index % 8;
					tx_quorum->flags[array_index] |= 1 << (array_offset);
					tx_quorum->seq_nr = rx_quorum->seq_nr;
					tx_quorum->write_value = rx_quorum->write_value;
					flag_sum = 0;
					tx = 0;
					a = 0;
					 for( i = 0; i < FLAGS_LEN; i++){
						//COOJA_DEBUG_STR("f");
						tx |= (tx_quorum->flags[i] != rx_quorum->flags[i]);
						tx_quorum->flags[i] |= rx_quorum->flags[i];
						//tx_quorum->flags[i] = rx_quorum->flags[i];
						flag_sum += tx_quorum->flags[i];
						a1 = bin_count(flag_sum);
						 a = a1+a;
						 flag_sum = 0;
					 }

					}
			  else if (rx_quorum->seq_nr == tx_quorum->seq_nr ){
				 
				 if(set != 1){
				unsigned int array_index = chaos_node_index / 8;
				unsigned int array_offset = chaos_node_index % 8;
				tx_quorum->flags[array_index] |= 1 << (array_offset);
				 set=1;
				 }
			  flag_sum = 0;
			  tx = 0;
			  a = 0;
			 for( i = 0; i < FLAGS_LEN; i++){
				tx |= (tx_quorum->flags[i] != rx_quorum->flags[i]);
				tx_quorum->flags[i] |= rx_quorum->flags[i];
				flag_sum += tx_quorum->flags[i];
				 a1 = bin_count(flag_sum);
				 a = a1+a;
				 flag_sum = 0;

			 }
			 }
			}

        
		 done = 0;
		
         if(a > (CHAOS_NODES/2)){			  
         tx = 1;
         done = 1;
         //LEDS_ON(LEDS_RED);

         }

         if(tx){
         if(done == 1){
         txrx_count++;
         }
         next_state = CHAOS_TX;

         }


        }

        else if ( current_state == CHAOS_TX && !chaos_txrx_success){

        next_state = CHAOS_TX;

        }

        else if ( current_state == CHAOS_RX && !chaos_txrx_success){
         
		 
        if(done == 1){
         txrx_count++;
		 next_state = CHAOS_TX;
		 
         }
         else{
         next_state = CHAOS_RX;

        }
        }

        else{
         next_state = CHAOS_RX;

        }		

        }
		

		//current_state == CHAOS_TX && txrx_count || slot_count >= MAX_ROUND_MAX_SLOTS - 1
		
     if(current_state == CHAOS_TX && txrx_count > 9){
		 
		printf(":Round%u. set bits %u. ", round_count, a);
        next_state = CHAOS_OFF;
        txrx_count = 0;

        }


    if((next_state == CHAOS_TX)|| (current_state == CHAOS_RX && chaos_txrx_success)||(next_state == CHAOS_OFF) ){
		flag_sum_1 = 0;
        for( i = 0; i < FLAGS_LEN; i++){
			flag_sum_1 = 0;
            flag_sum_1 += tx_quorum->flags[i];

                    }
			flag_stack(flag_sum_1);
	
			}
	
    
	
	
	
	int end = (next_state == CHAOS_OFF) || (slot_count >= MAX_ROUND_MAX_SLOTS - 2);
	if(end) {
		quorum_local.quorum_value.write_value = tx_quorum->write_value;
        quorum_local.quorum_value.seq_nr = tx_quorum->seq_nr;
		quorum_flags = tx_quorum->flags;
		quorum_local.quorum_value.msg_type = tx_quorum->msg_type;
		
	}
	
   *app_flags = tx_quorum->flags;
   return next_state;

}




 int quorum_round_begin(const uint16_t round_number, const uint8_t app_id, uint16_t* value, uint8_t** final_flags, uint8_t* phase ,uint16_t* time_stamp, uint8_t* header )

{


/* COOJA_DEBUG_STR("value"); */

memset(&quorum_local , 0, sizeof(quorum_local));
j = 0;
set = 0;
if(turn==0) turn=1;  else turn=0;
quorum_local.quorum_value.write_value = *value;
quorum_local.quorum_value.seq_nr = *time_stamp;


flip_coin = rand() %2;
 if(flip_coin == 0){
	quorum_local.phase = CAN_WRITE;
	
 }
 else if(flip_coin == 1){

	quorum_local.phase = CAN_READ;
	
 } 


chaos_round(round_number, app_id, (const uint8_t const*)&quorum_local.quorum_value, sizeof(quorum_t) + quorum_get_flags_length(), MAX_SLOT_LEN_DCO, MAX_ROUND_MAX_SLOTS, quorum_get_flags_length(), process);

printf(": set bits %u. ", a);
print_func(1);
memcpy(quorum_local.quorum_value.flags, quorum_flags, quorum_get_flags_length());
*value = quorum_local.quorum_value.write_value;
*time_stamp = quorum_local.quorum_value.seq_nr;
*final_flags = quorum_local.quorum_value.flags;
*header = quorum_local.quorum_value.msg_type;
return j;

//local_round = round_number;

}



int quorum_is_pending(const uint16_t round_count){
return 1;

}


int quorum_get_flags_length() {

  return FLAGS_LEN;

}

int bin_count(int n){
          int bit_count = 0 ;
           while(n){
           n &=n-1;
           bit_count++;
           }
           return bit_count;
           }

int flag_stack(int summ){
	int k = j-1;
	if (j==0){
		for(i = 0; i<30; i++){
		  flag_arry[j]= 0;
			     }
		flag_arry[j]= summ;
		j= j+1;
		
	}
				
	 else if(flag_arry[k]!= summ){
			
		
		flag_arry[j]= summ;	
			j = j+1;
		}
			if(j==30){
				
				j=0;
			}
			
		
		/* else {
		
		t = local_round;
		for(i= 0 ; i<= sizeof(flag_arry) ; i++){
		printf("%2x", flag_arry[i]);
		}
			
		} */
			
}
int print_func(int var){	
	printf("flags[");

	for(i = 0; i<j; i++){
		
		
		
		printf(".%02x", flag_arry[i]);
		
		
		
	}
	printf("] \n");
}




