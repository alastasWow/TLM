/********************************************************************
 * Copyright (C) 2009 by Ensimag and Verimag			    *
 * Initial author: Matthieu Moy					    *
 ********************************************************************/

#include "ensitlm.h"
#include "generator.h"
#include "memory.h"
#include "bus.h"
#include "LCDC.h"

int sc_main (int argc, char ** argv) {
	(void)argc;
	(void)argv;
	/*
	  +---------+	 +-------------+    +--------+
	  |	   +++	+-+	      +++  +++	     |
	  | Alice  | +--+ |  bus   | +--+ |	Bob  |
	  |	   +++	+-+	      +++  +++	     |
	  +---------+	 +-------------+    +--------+
	*/

	size_t mem_size = 87039;
        Memory memory("video memory", mem_size);
	Generator generator1("Generator1", mem_size);
	//Generator generator1("Generator1", 255);
	sc_core::sc_signal<bool> stock_interrupt;
	Bus bus("Bus");
	LCDC ecran("ecran", sc_core::sc_time(1.0/25, sc_core::SC_SEC));

	/* Bob is mapped at addresses [0, 100[, i.e. ... */
	//bus.map(memory.target, 0x10000000, 87040);
	//bus.map(memory.target, 0x10000000, 255);
	//bus.map(ecran.target_socket,0x10015400,0x0C);
	bus.map(ecran.target_socket,0,11);
	bus.map(memory.target,12,mem_size);

	/* connect components to the bus */
	generator1.initiator.bind(bus.target);
	bus.initiator.bind(memory.target);
	//bus.initiator.bind(ecran.initiator_socket);
	ecran.initiator_socket.bind(bus.target);
	bus.initiator.bind(ecran.target_socket);
	ecran.display_int(stock_interrupt);
	generator1.interrupt_display(stock_interrupt);

	/* and start simulation */
	sc_core::sc_start();
	return 0;
}
