#ifndef TARGET_H
#define TARGET_H

#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include "adder_payload.h"

using namespace sc_core;
using namespace tlm;
using namespace std;

struct Target : sc_module {
	tlm_utils::simple_target_socket<Target> socket;

	SC_CTOR(Target){
		socket.register_b_transport(this, &Target::b_transport);
	}
	void b_transport(tlm_generic_payload& trans, sc_time& delay){
		AdderPayload* payload = reinterpret_cast<AdderPayload*>(trans.get_data_ptr());
		int a = payload->a;
		int b = payload->b;
		payload->sum = a+b;
		cout << "[Target] Received : " << a << " + " << b 
			<< " = " << payload->sum << " at time " << sc_time_stamp() << endl;
		trans.set_response_status(TLM_OK_RESPONSE);
		delay += sc_time(5,SC_NS);
	}
};

#endif
