/********************************************************************
 * Copyright (C) 2009 by Ensimag and Verimag                        *
 * Initial author: Matthieu Moy                                     *
 ********************************************************************/

#include "generator.h"

using namespace std;
#define add_memmory     0x10000000
#define add_video       0x100010A0
#define add_LCD         0x10015400
#define IMG_SIZE        320*240

void Generator::thread(void) {


	for (int i = 0; i < IMG_SIZE ; i++) {
		initiator.write (( add_video + i * 0x2),0xFFFFFFFF); //on decalle de 8 bit
	}

}

Generator::Generator(sc_core::sc_module_name Generator, size_t mem_size):
	sc_core::sc_module(Generator), mem_size(mem_size),
	memory(new ensitlm::data_t[mem_size / 4])
{
	srand(std::time(0));
	SC_THREAD(thread);
}

Generator::~Generator() {
	delete[] memory;
}

int Generator::check_status(tlm::tlm_response_status& status) {


	switch (status) {
		case tlm::TLM_OK_RESPONSE :
			return 0;
			cout << "transaction status : OK\n";
			break;
		case tlm::TLM_INCOMPLETE_RESPONSE :
			cout << "transaction status : INCOMPLETE RESPONSE\n";
			return 1;
			break;
		case tlm::TLM_GENERIC_ERROR_RESPONSE :
			cout << "transaction status : GENERIC ERROR \n";
			return 1;
			break;
		case tlm::TLM_ADDRESS_ERROR_RESPONSE :
			cout << "transaction status : ADDRESS ERROR\n";
			return 1;
			break;
		case tlm::TLM_COMMAND_ERROR_RESPONSE :
			cout << "transaction status : COMMAND ERROR\n";
			return 1;
			break;
		case tlm::TLM_BURST_ERROR_RESPONSE :
			cout << "transaction status : BURST ERROR\n";
			return 1;
			break;
		case tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE :
			cout << "transaction status : BYTE ENABLE ERROR\n";
			return 1;
			break;
		default :
			cout << "transaction status : NO STATUS\n";
			return 1;
			break;
	}
}

void Generator::get_user_value(ensitlm::data_t& val) {
	cout << "Entrer un nombre" << endl;
	cin >> val;
}

void Generator::get_user_addr(const ensitlm::addr_t& base_addr,
		ensitlm::addr_t& addr) {
	cout << "Entrer une addresse " << endl;
	cin >> addr;
	addr += base_addr;
}
