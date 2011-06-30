#include "gears/master.h"

#include <cmath>

Master::Master()
{
	type = Machine::MachineMaster;
	name = "output";
	author = "Core";
	description = "Master Output";

	addParam(new Param("bpm", "Beats per minute", Param::int_t, 50, 1000));
}


Machine *Master::factory() {
	return new Master();
}


int Master::init(Core *core)
{
	/*
	this->store = store;

	for (int i = 0; i < store->order.length(); i++) {
		if (store->machines.contains(store->order[i]) == false) continue;
		Machine *m = store->machines[store->order[i]];
		qDebug() <<"Init " << m->name << endl;
		m->init();
	}
*/
	return 0;
}



int Master::go(jack_client_t *client, jack_port_t **input_port, jack_port_t **output_port, bool record)
{
	const char **ports;
	this->client = client;
	this->input_port = input_port;
	this->output_port = output_port;

	// tell the JACK server that we are ready to roll
	if (jack_activate(client)) {
		qDebug() << "Cannot activate client" << endl;
		return 1;
	}

	// connect the ports. Note: you can't do this before the client is activated, because we can't allow connections to be made to clients that aren't running.
	if ((ports = jack_get_ports(client, 0, 0, JackPortIsPhysical|JackPortIsOutput)) == 0) {
		qDebug() << "Cannot find any physical capture ports" << endl;
		return 2;
	}

	if (jack_connect(client, ports[0], jack_port_name(input_port[0]))) qDebug() << "Cannot connect input ports LX" << endl;
	if (jack_connect(client, ports[1], jack_port_name(input_port[1]))) qDebug() << "Cannot connect input ports RX" << endl;

	free(ports);

	if ((ports = jack_get_ports(client, 0, 0, JackPortIsPhysical|JackPortIsInput)) == 0) {
		qDebug() << "Cannot find any physical playback ports" << endl;
		return 3;
	}

	for (int i = 0; ports[i] != 0; i++) {
		qDebug() <<"Connecting port #" << i << endl;
		if (jack_connect(client, jack_port_name(output_port[i & 1]), ports[i])) qDebug() << "Cannot connect output ports #" << i << endl;
	}

	free(ports);

	if (record == true) {
		file = fopen("/tmp/record.raw", "w");
		if (!file) QMessageBox::critical(0, "Cannot open temporary file", "Sorry, but I cannot open a temporary file for recording");
	} else {
		file = 0;
	}

	period_per_beat = 100;

	period_counter = -1;
	beat_counter = -1;
	pattern_counter = track_last;

	return 0;
}


void Master::reconfig() {
	/*
// 	qDebug() <<params["bpm"]->getInt() << "\t" << store->buffer_size << "\t" << store->sampling_rate << "\n";
	period_per_beat = (int)((60.0 / params["bpm"]->getInt()) / ((float)store->buffer_size / (float)store->sampling_rate));
	*/
}


int Master::stop()
{
	jack_deactivate(client);
/*
	for (int i = 0; i < store->order.length(); i++) {
		if (store->machines.contains(store->order[i]) == false) continue;
		Machine *m = store->machines[store->order[i]];
		qDebug() <<"Finish " << m->name << endl;
		m->finish();
	}
	
	if (file) fclose(file);
*/
	return 0;
}

int Master::process(jack_nframes_t nframes)
{
	// qDebug() <<"Period: " << period_counter << " / " << period_per_beat << endl;
/*
	// It's time to play a new beat
	if (period_counter == -1 or ++period_counter >= period_per_beat) {
		period_counter = 0;

		if (beat_counter == -1 or ++beat_counter >= store->beat_per_pattern) {
			beat_counter = 0;
			// It's time to play a new pattern!
			if (++pattern_counter > track_last) {
				pattern_counter = track_first;
			}
		}

		qDebug() <<"Beat: " << beat_counter << " / " << store->beat_per_pattern << endl;
		qDebug() <<"Pattern: " << pattern_counter << " / " << store->total_patterns << endl;

		for (int i = 0; i < store->order.length(); i++) {
			if (store->machines.contains(store->order[i]) == false) {
				qDebug() << "Cannot find machine #" << store->order[i] << endl;
				continue;
			}

			Machine *m = store->machines[store->order[i]];
			qDebug() <<"\t" << m->name << endl;
			if (m->track.contains(pattern_counter) and m->patterns.contains(m->track[pattern_counter]) and m->patterns[m->track[pattern_counter]].contains(beat_counter)) {
				qDebug() <<"Reconfig: " << m->name << endl;
				QHashIterator<QString, QString> j(m->patterns[m->track[pattern_counter]][beat_counter]);
				while (j.hasNext()) {
					j.next();
					qDebug() <<"\t" << j.key() << " => " << j.value() << endl;
					m->params[j.key()]->set(j.value());
				}
				m->reconfig();
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
	for (int i = 0; i < store->order.length(); i++) {
		if (store->machines.contains(store->order[i]) == false) continue;

		Machine *m = store->machines[store->order[i]];

		if (m->author == "Core") {
			if (m->name == "lineinput") {
				memcpy(m->lx, lxi, sizeof(jack_default_audio_sample_t) * nframes);
				memcpy(m->rx, rxi, sizeof(jack_default_audio_sample_t) * nframes);
			} else if (m->name == "fileinput") {
				m->preprocess(nframes);
			} else {
				m->preprocess(nframes, 0);
			}
		} else {
			m->preprocess(nframes);
		}
	}

	// Output copy
	memcpy(lxo, store->machines[0]->li, sizeof(jack_default_audio_sample_t) * nframes);
	memcpy(rxo, store->machines[0]->ri, sizeof(jack_default_audio_sample_t) * nframes);

	// If recording writes to file
	if (file) {
		for (int i = 0; i < nframes; i++) {
			buffer[i*2] = std::floor(lxo[i] * 32000);
			buffer[i*2+1] = std::floor(rxo[i] * 32000);
		}
		fwrite(buffer, nframes * 2, 2, file);
	}
*/
	return 0;
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
