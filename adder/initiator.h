#ifndef INITIATOR_H
#define INITIATOR_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include "adder_payload.h"

using namespace sc_core;
using namespace tlm;
using namespace std;

struct Initiator : sc_module {
	tlm_utils :: simple_initiator_socket<Initiator> socket ;

	SC_CTOR(Initiator) {
		SC_THREAD(process);
	}

	void process(){
		
		int testcases[5][2] = { {7,5}, {10,10}, {3,9}, {15,-4}, {100,200} } ;

		for(int i=0; i<5; i++){
			tlm_generic_payload trans;
			sc_time delay = SC_ZERO_TIME ;
			
			AdderPayload* payload = new AdderPayload;
			payload->a = testcases[i][0];
			payload->b = testcases[i][1];
			payload->sum = 0;

			trans.set_command(TLM_WRITE_COMMAND);
			trans.set_address(0);
			trans.set_data_ptr(reinterpret_cast<unsigned char*>(payload));

			trans.set_data_length(sizeof(AdderPayload));
			trans.set_streaming_width(sizeof(AdderPayload));
			trans.set_byte_enable_ptr(0);
			trans.set_dmi_allowed(false);
			trans.set_response_status(TLM_INCOMPLETE_RESPONSE);

			cout << "\n [Initiator] Sending a = " << payload->a << ", b = " << payload->b
				<< " at time " << sc_time_stamp() << endl;

			socket->b_transport(trans,delay);

			if(trans.is_response_error()){  
				cout << "[Initiator] Transaction Error: " << trans.get_response_string() << endl;
			}else{
				cout << "[Initiator] Transaction Ok : " << trans.get_response_string() <<
					", Result sum = " << payload->sum << endl;
			}
			wait(10, SC_NS);
			
		}
	}
};

#endif
