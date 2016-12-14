/********************************************************************
 * Copyright (C) 2009 by Ensimag and Verimag                        *
 * Initial author: Matthieu Moy                                     *
 ********************************************************************/

#include "generator.h"

using namespace std;
#define memmory_add     0x0C
#define video_add       0x10AC
#define LCD_add         0x0
#define rom_add         0x1540c
#define IMG_SIZE        320*240

void Generator::thread(void) {
	int j = 239;
	uint32_t increment=0xFF;
	while(true){
		uint32_t pixel_Word;
		uint32_t masque_image = (increment << 24) + (increment << 16) +
			(increment << 8) + increment;
		ensitlm::data_t d;
		tlm::tlm_response_status statut;
		uint32_t buffer[4];
		/*for (int i = 0; i < IMG_SIZE ; i++) { version sans ROM
			initiator.write (( video_add + i * 0x2),0xFFFFFFFF); //on decalle de 8 bit
		}*/
		for (int i = 0; i<IMG_SIZE/4; i+=2){
			statut = initiator.read(rom_add + (i*2),d);
			buffer[0] = (d & 0xF0000000);
			buffer[1] = (d & 0x0F000000) >> 4;
			buffer[2] = (d & 0x00F00000) >> 8;
			buffer[3] = (d & 0x000F0000) >> 12;
			pixel_Word = buffer[0] + buffer[1] + buffer[2] + buffer[3];
			pixel_Word = pixel_Word & masque_image;
			statut = initiator.write(video_add + ((( i * 4) +
			          (j * 320 )) % (IMG_SIZE )) , pixel_Word);
			buffer[0] = (d & 0x0000F000) << 16;
			buffer[1] = (d & 0x00000F00) << 12;
			buffer[2] = (d & 0x000000F0) << 8;
			buffer[3] = (d & 0x0000000F) << 4;
			pixel_Word = buffer[0] + buffer[1] + buffer[2] + buffer[3];
			pixel_Word = pixel_Word & masque_image;
			statut = initiator.write(video_add +( ((( i + 1 ) * 4) +
				(j * 320 )) % (IMG_SIZE)) , pixel_Word);
		}

		initiator.write(LCDC_ADDR_REG , video_add);
		initiator.write(LCDC_START_REG , 0x00000001);
		sc_core::wait(end_irq);
		cout << "Validation de l'interruption, on reprends le thread"<< endl;
		sc_core::wait(10000000, sc_core::SC_NS);
		initiator.write(LCDC_INT_REG, 0x00000000);
		if(j - 1 < 0){
			j = 239 ;
		}else{
			j += 1;
		}
		if(increment > 0){
			increment --;
		}
		else{
			increment = 0xFF;
		}
	}
}

void Generator::method(void) {
	end_irq.notify();
}

Generator::Generator(sc_core::sc_module_name Generator, size_t mem_size):
	sc_core::sc_module(Generator), mem_size(mem_size),
	memory(new ensitlm::data_t[mem_size / 4])
{
	srand(std::time(0));
	SC_THREAD(thread);
	SC_METHOD(method);
	sensitive << interrupt_display;
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
