#ifndef CORE_MASTER_H
#define CORE_MASTER_H

#include <jack/jack.h>
#include <stdlib.h>

class Master;

#include "core/core.h"
#include "../Fadolib/machine.h"

int jack_process(jack_nframes_t nframes, void *arg);
void jack_shutdown(void *arg);

class Master : public Machine {
	public:
		Master();

		FILE *file;
		short buffer[4096];

		int period_per_beat;
		int period_counter, beat_counter, pattern_counter;
		int track_first, track_last;
		Core *core;
		jack_client_t *client;
		jack_port_t **input_port;
		jack_port_t **output_port;

		// Master(Store *store);
		int init(Store *store);
		void reconfig();
		int go(jack_client_t *client, jack_port_t **input_port, jack_port_t **output_port, bool record);
		int stop();
		int jack_process(jack_nframes_t nframes);
		int process(jack_nframes_t nframes);
};

#endif
