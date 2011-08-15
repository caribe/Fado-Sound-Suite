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

#include "echo.h"

Echo::Echo() {

	author = "Caribe";
	name = "Echo";
	description = "A simple echo effect";
	type = Machine::MachineEffect;

	addParam("time", "Time", "Seconds of echo", Param::ParamFloat, 0.1, 1);
	addParam("vol", "Volume", "Echo's volume", Param::ParamFloat, 0, 1);

	cursor = 0;
	samples = 0;
	buffer_lx = buffer_rx = 0;
}

Machine *Echo::factory() {
	return new Echo();
}


void Echo::reconfig(const int sampling_rate)
{
	int s = sampling_rate * paramsMap["time"]->floatValue;

	if (samples < s) {
		if (buffer_lx != 0) delete buffer_lx;
		buffer_lx = new float[s];
		if (buffer_rx != 0) delete buffer_rx;
		buffer_rx = new float[s];

		for (int i = samples; i < s; i++) buffer_lx[i] = buffer_rx[i] = 0;
	}
	samples = s;
}


void Echo::process(int framesStart, int framesLength)
{
	int j;
	float e1 = paramsMap["vol"]->floatValue;
	float e2 = 1 - e1;

	if (samples == 0) return; // Avoid exception

	for (int i = framesStart; i < framesLength; i++) {
		j = (cursor + i) % samples;
		lx[i] = buffer_lx[j] = li[i] * e1 + buffer_lx[j] * e2;
		rx[i] = buffer_rx[j] = ri[i] * e1 + buffer_rx[j] * e2;
	}

	cursor = j;
}

Q_EXPORT_PLUGIN2(echo, Echo)
