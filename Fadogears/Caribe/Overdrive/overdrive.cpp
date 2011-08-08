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

#include "overdrive.h"

Overdrive::Overdrive() {

	author = "Caribe";
	name = "Overdrive";
	description = "Overdrive description";
	type = Machine::MachineEffect;

	addParam("gain", "Gain", "The gain factor", Param::ParamFloat, 1, 1000);
	addParam("cut", "Cut", "The cut value", Param::ParamFloat, 0, 1);

	// params["gain"]->set(2);
	// params["cut"]->set(0.95);
}


Machine *Overdrive::factory() {
	return new Overdrive();
}

void Overdrive::reconfig(const int sampling_rate) {}

void Overdrive::process(jack_nframes_t nframes) {

	float gain = paramsMap["gain"]->floatValue;
	float cut = paramsMap["cut"]->floatValue;

	for (unsigned int i = 0; i < nframes; i++) {
		lx[i] = li[i] * gain;
		if (lx[i] > cut) lx[i] = cut;
		else if (lx[i] < -cut) lx[i] = -cut;

		rx[i] = ri[i] * gain;
		if (rx[i] > cut) rx[i] = cut;
		else if (rx[i] < -cut) rx[i] = -cut;
	}
}

Q_EXPORT_PLUGIN2(overdrive, Overdrive)
