/********************************************************************
 * Copyright (C) 2009 by Ensimag and Verimag			    *
 * Initial author: Matthieu Moy					    *
 ********************************************************************/

#include "ensitlm.h"
#include "generator.h"
#include "memory.h"
#include "bus.h"
#include "LCDC.h"
#include "ROM.h"

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
	size_t rom_size = 38399;
        Memory memory("video memory", mem_size);
	Generator generator1("Generator1", mem_size);
	sc_core::sc_signal<bool,sc_core::SC_MANY_WRITERS> irq_signal("IRQ");
	Bus bus("Bus");
	LCDC ecran("ecran", sc_core::sc_time(1.0/25, sc_core::SC_SEC));
	ROM rom("rom");


	bus.map(ecran.target_socket,0,11);
	bus.map(memory.target,12,mem_size);
	bus.map(rom.socket,mem_size+13,rom_size);

	/* connect components to the bus */
	generator1.initiator.bind(bus.target);
	bus.initiator.bind(memory.target);
	bus.initiator.bind(rom.socket);

	ecran.initiator_socket.bind(bus.target);
	bus.initiator.bind(ecran.target_socket);

	ecran.display_int(irq_signal);
	generator1.interrupt_display(irq_signal);

	/* and start simulation */
	sc_core::sc_start();
	return 0;
}
