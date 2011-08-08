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

#include "gears/master.h"

#include <cmath>

Master::Master()
{
	type = Machine::MachineMaster;
	name = "Master";
	author = "Core";
	description = "Master Output";

	addParam("bpm", "BPM", "Beats per minute", Param::ParamInt, 50, 1000);
}


Machine *Master::factory() {
	return new Master();
}


int Master::init(Core *core)
{

	this->core = core;
	beat_counter = pattern_counter = 0;

	foreach (Machine *machine, core->order) {
		qDebug() << "Init " << machine->name;
		machine->init();
	}

	return 0;
}



int Master::go(jack_client_t *client, jack_port_t **input_port, jack_port_t **output_port, bool record)
{
	QSettings settings;

	const char **ports;
	this->client = client;
	this->input_port = input_port;
	this->output_port = output_port;

	// tell the JACK server that we are ready to roll
	if (jack_activate(client)) {
		qDebug() << "Cannot activate client";
		return 1;
	}

	// connect the ports. Note: you can't do this before the client is activated, because we can't allow connections to be made to clients that aren't running.
	if ((ports = jack_get_ports(client, 0, 0, JackPortIsPhysical|JackPortIsOutput)) == 0) {
		qDebug() << "Cannot find any physical capture ports";
		return 2;
	}

	if (jack_connect(client, ports[0], jack_port_name(input_port[0]))) qDebug() << "Cannot connect input ports LX";
	if (jack_connect(client, ports[1], jack_port_name(input_port[1]))) qDebug() << "Cannot connect input ports RX";

	free(ports);

	if ((ports = jack_get_ports(client, 0, 0, JackPortIsPhysical|JackPortIsInput)) == 0) {
		qDebug() << "Cannot find any physical playback ports";
		return 3;
	}

	for (int i = 0; ports[i] != 0; i++) {
		qDebug() <<"Connecting port #" << i;
		if (jack_connect(client, jack_port_name(output_port[i & 1]), ports[i])) qDebug() << "Cannot connect output ports #" << i;
	}

	free(ports);

	QString filename = settings.value("settings/tempFolder", "/tmp").toString()+"/fado.raw";
	file = fopen(filename.toUtf8().data(), "w");
	if (!file) QMessageBox::critical(0, QObject::tr("Cannot open temporary file"), QObject::tr("Sorry, but I cannot open '%1' as temporary file for recording").arg(filename));

	period_per_beat = 100;

	period_counter = -1;
	beat_counter = -1;
	// pattern_counter = track_last;

	return 0;
}


void Master::reconfig(const int sampling_rate) {

	period_per_beat = (int)((60.0 / paramsMap["bpm"]->getInt()) / ((float)core->buffer_size / (float)core->sampling_rate));

}


int Master::stop()
{
	jack_deactivate(client);

	foreach (Machine *machine, core->order) {
		qDebug() << "Finish " << machine->name;
		machine->finish();
	}
	
	if (file) {
		fclose(file);
		file = 0;
	}

	return 0;
}


void Master::process(jack_nframes_t nframes)
{
	// qDebug() <<"Period: " << period_counter << " / " << period_per_beat;

	// It's time to play a new beat
	if (period_counter == -1 or ++period_counter >= period_per_beat) {
		period_counter = 0;

		if (beat_counter == -1 or ++beat_counter >= core->beat_per_pattern) {
			beat_counter = 0;
			// It's time to play a new pattern!
			if (++pattern_counter > core->track_last) {
				pattern_counter = core->track_first;
			}
		}

		qDebug() << "Beat" << beat_counter << "/" << core->beat_per_pattern;
		qDebug() << "Pattern" << pattern_counter << "/" << core->total_patterns;

		foreach (Machine *machine, core->order) {
			qDebug() << machine->name;
			if (machine->track.contains(pattern_counter) and machine->track[pattern_counter]->params.contains(beat_counter)) {
				qDebug() << "Reconfig: " << machine->name;

				QHash<int, QString> params = machine->track[pattern_counter]->params[beat_counter];

				foreach (int key, params.keys()) {
					qDebug() << key << " => " << params[key];
					machine->params[key]->set(params[key]);
				}

				machine->reconfig(core->sampling_rate);
			}
		}
	}

	// Get input buffers
	jack_default_audio_sample_t *lxi = (jack_default_audio_sample_t *)jack_port_get_buffer(input_port[0], nframes);
	jack_default_audio_sample_t *rxi = (jack_default_audio_sample_t *)jack_port_get_buffer(input_port[1], nframes);

	// Get output buffers
	jack_default_audio_sample_t *lxo = (jack_default_audio_sample_t *)jack_port_get_buffer(output_port[0], nframes);
	jack_default_audio_sample_t *rxo = (jack_default_audio_sample_t *)jack_port_get_buffer(output_port[1], nframes);

	// Calls all machines in the right order
	foreach (Machine *machine, core->order) {
		if (machine->type == Machine::MachineMaster) {
			machine->preprocess(nframes, 0);
		} else if (machine->type == Machine::MachineInput and machine->name == "Line Input") {
			memcpy(machine->lx, lxi, sizeof(jack_default_audio_sample_t) * nframes);
			memcpy(machine->rx, rxi, sizeof(jack_default_audio_sample_t) * nframes);
		} else {
			machine->preprocess(nframes);
		}
	}

	// Output copy
	memcpy(lxo, core->machines[0]->li, sizeof(jack_default_audio_sample_t) * nframes);
	memcpy(rxo, core->machines[0]->ri, sizeof(jack_default_audio_sample_t) * nframes);

	// If recording writes to file
	if (file) {
		for (unsigned int i = 0; i < nframes; i++) {
			buffer[i*2] = std::floor(lxo[i] * 32767);
			buffer[i*2+1] = std::floor(rxo[i] * 32767);
		}
		fwrite(buffer, nframes * 2, 2, file);
	}
}


/*

* Overall operation failed.
JackFailure = 0x01,

* The operation contained an invalid or unsupported option.
JackInvalidOption = 0x02,

* The desired client name was not unique.  With the @ref
* JackUseExactName option this situation is fatal.  Otherwise,
* the name was modified by appending a dash and a two-digit
* number in the range "-01" to "-99".  The
* jack_get_client_name() function will return the exact string
* that was used.  If the specified @a client_name plus these
* extra characters would be too long, the open fails instead.
JackNameNotUnique = 0x04,

* The JACK server was started as a result of this operation.
* Otherwise, it was running already.  In either case the caller
* is now connected to jackd, so there is no race condition.
* When the server shuts down, the client will find out.
JackServerStarted = 0x08,

* Unable to connect to the JACK server.
JackServerFailed = 0x10,

* Communication error with the JACK server.
JackServerError = 0x20,

* Requested client does not exist.
JackNoSuchClient = 0x40,

* Unable to load internal client
JackLoadFailure = 0x80,

* Unable to initialize client
JackInitFailure = 0x100,

* Unable to access shared memory
JackShmFailure = 0x200,

* Client's protocol version does not match
JackVersionError = 0x400,

* BackendError
JackBackendError = 0x800,

* Client is being shutdown against its will
JackClientZombie = 0x1000

*/
