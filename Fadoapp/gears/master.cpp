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
 * aunsigned long with Fado Sound Suite.  If not, see <http://www.gnu.org/licenses/>.
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



int Master::go(PaStream *client, bool record)
{
	QSettings settings;

	QString filename = settings.value("settings/tempFolder", "/tmp").toString()+"/fado.raw";
	file = fopen(filename.toUtf8().data(), "w");
	if (!file) QMessageBox::critical(0, QObject::tr("Cannot open temporary file"), QObject::tr("Sorry, but I cannot open '%1' as temporary file for recording").arg(filename));

	PaError err = Pa_StartStream(client);
	if( err != paNoError ) return 1;

	this->client = client;

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
	Pa_StopStream(this->client);

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

void Master::process(unsigned long nframes) {}

void Master::process(unsigned long nframes, const void *input, void *output)
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

		//qDebug() << "Beat" << beat_counter << "/" << core->beat_per_pattern;
		//qDebug() << "Pattern" << pattern_counter << "/" << core->total_patterns;

		foreach (Machine *machine, core->order) {
			//qDebug() << machine->name;
			if (machine->track.contains(pattern_counter) and machine->track[pattern_counter]->params.contains(beat_counter)) {
				//qDebug() << "Reconfig: " << machine->name;

				QHash<int, QString> params = machine->track[pattern_counter]->params[beat_counter];

				foreach (int key, params.keys()) {
					//qDebug() << key << " => " << params[key];
					machine->params[key]->set(params[key]);
				}

				machine->reconfig(core->sampling_rate);
			}
		}
	}

	// Get input buffers
	float **inputPtr = (float **)input;
	float *lxi = inputPtr[0];
	float *rxi = inputPtr[1];

	// Calls all machines in the right order
	foreach (Machine *machine, core->order) {
		if (machine->type == Machine::MachineMaster) {
			machine->preprocess(nframes, 0);
		} else if (machine->type == Machine::MachineInput and machine->name == "Line Input") {
			memcpy(machine->lx, lxi, sizeof(float) * nframes);
			memcpy(machine->rx, rxi, sizeof(float) * nframes);
		} else {
			machine->preprocess(nframes);
		}
	}

	float **outputPtr = (float **)output;

	// Output copy
	memcpy(outputPtr[0], core->machines[0]->li, sizeof(float) * nframes);
	memcpy(outputPtr[1], core->machines[0]->ri, sizeof(float) * nframes);

	// If recording writes to file
	if (file) {
		for (unsigned int i = 0; i < nframes; i++) {
			buffer[i*2] = std::floor(outputPtr[0][i] * 32767);
			buffer[i*2+1] = std::floor(outputPtr[1][i] * 32767);
		}
		fwrite(buffer, nframes * 2, 2, file);
	}

}
