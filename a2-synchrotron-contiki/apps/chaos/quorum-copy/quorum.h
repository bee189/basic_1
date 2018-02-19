#ifndef _MAX_H_
#define _MAX_H_

#include "chaos.h"
#include "testbed.h"
#include "chaos-config.h"


enum {
    CAN_WRITE = 0 ,WRITE_PROPOSE = 1, WRITE_DONE =2 ,CAN_READ =3 , READ_PROPOSE = 4, CAN_WRITE_QUERY = 5
};
#define PHASE_TO_STR(phase) (\
   (phase == CAN_WRITE) ? ("CAN_WRITE" ) : \
		(phase == CAN_READ) ? ("CAN_READ" ) : \
		   (phase == CAN_WRITE_QUERY) ? ("CAN_WRITE_QUERY" ) : \
				(phase == WRITE_PROPOSE) ? ("WRITE_PROPOSE" ) : \
					(phase == WRITE_DONE) ? ("WRITE_DONE" ) : \
						(phase == READ_PROPOSE) ? ("READ_PROPOSE" ) : \
 )

enum {
    WRITE_HEADER = 0, READ_HEADER = 1, WRITE_QUERY_HEADER = 2
};
#define HEADER_TO_STR(msg_type) (\
   (msg_type == READ_HEADER) ? ("READ_HEADER" ) : \
		(msg_type == WRITE_HEADER) ? ("WRITE_HEADER" ) : \   
 )



#define MAX_SLOT_LEN          (4*(RTIMER_SECOND/1000)+0*(RTIMER_SECOND/1000)/4)    //1 rtimer tick == 2*31.52 us

#ifndef MAX_ROUND_MAX_SLOTS
#warning "define MAX_ROUND_MAX_SLOTS"
#define MAX_ROUND_MAX_SLOTS   (255)
#endif

#define MAX_SLOT_LEN_DCO      (MAX_SLOT_LEN*CLOCK_PHI)    //TODO needs calibration

int quorum_round_begin(const uint16_t round_count, const uint8_t app_id, uint16_t* value, uint8_t** final_flags, uint8_t* phase ,uint16_t* time_stamp, uint8_t* header );

int quorum_is_pending(const uint16_t round_count);

int quorum_get_flags_length(void);

uint16_t quorum_get_off_slot();

#endif /* _MAX_H_ */
