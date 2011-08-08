/*
 * Fado Sound Suite - Modular synthetic music generator and sound processor
 *
 * Copyright (C) 2011 Vincenzo Buttazzo <vbuttazzo@yahoo.com>
 *
 * This file is part of Fado Sound Suite.
 *
 * Fado Sound Suite is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fado Sound Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fado Sound Suite.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef CORE_MASTER_H
#define CORE_MASTER_H

#include <QObject>
#include <QSettings>
#include <QFile>
#include <jack/jack.h>

class Master;

#include "ext/machine.h"
#include "core/core.h"

int jack_process(jack_nframes_t nframes, void *arg);
void jack_shutdown(void *arg);

class Master : public Machine {
	public:
		Master();

		FILE *file;
		short buffer[4096];

		int period_per_beat;
		int period_counter, beat_counter, pattern_counter;
		Core *core;
		jack_client_t *client;
		jack_port_t **input_port;
		jack_port_t **output_port;

		int init(Core *core);
		void reconfig(const int sampling_rate);
		int go(jack_client_t *client, jack_port_t **input_port, jack_port_t **output_port, bool record);
		int stop();
		int jack_process(jack_nframes_t nframes);
		void process(jack_nframes_t nframes);
		Machine *factory();
};

#endif
