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

#include "basskick.h"

Basskick::Basskick() {

	author = "Caribe";
	name = "Bass kick";
	description = "Bass Kick";
	type = Machine::MachineGenerator;

	addParam("duration", "Duration", "The duration", Param::ParamFloat, 0, 1);

	max = RAND_MAX / 2.0;
}


Machine *Basskick::factory() {
	return new Basskick();
}


void Basskick::reconfig(const int sampling_rate)
{
	duration = paramsMap["duration"]->floatValue * sampling_rate;
	rotor = 0;
}



void Basskick::process(unsigned long nframes) {
	for (unsigned int i = 0; i < nframes; i++) {
		if (rotor++ < duration) {
			lx[i] = rand() / max - 1;
			rx[i] = rand() / max - 1;
		} else {
			lx[i] = rx[i] = 0;
		}
	}
}

Q_EXPORT_PLUGIN2(basskick, Basskick)
