/********************************************************************
 * Copyright (C) 2009 by Ensimag and Verimag			    *
 * Initial author: Matthieu Moy					    *
 ********************************************************************/

#include <cstdlib>
#include <ctime>
#include "ensitlm.h"
#include "bus.h"



struct Generator : sc_core::sc_module {
    
	ensitlm::initiator_socket<Generator> initiator;
	sc_core::sc_in<bool> interrupt_display;
	void thread(void);


	SC_HAS_PROCESS(Generator);
	Generator(sc_core::sc_module_name Generator, size_t mem_size);
	~Generator();
	int check_status(tlm::tlm_response_status& status);
	void get_user_value(ensitlm::data_t& val);
	void get_user_addr(const ensitlm::addr_t& base_addr, ensitlm::addr_t& addr);
	size_t mem_size;
	ensitlm::data_t* memory;
};
