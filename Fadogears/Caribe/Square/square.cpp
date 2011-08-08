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

#include "square.h"

Square::Square() {

	author = "Caribe";
	name = "Square";
	description = "A simple square signal";
	type = Machine::MachineGenerator;

	addParam("freq", "Frequency", "The square frequency", Param::ParamFloat, 50, 20000);

	rotor = 0;
}


Machine *Square::factory() {
	return new Square();
}


void Square::reconfig(const int sampling_rate) {
	rotor_step = PI_2 / (sampling_rate / paramsMap["freq"]->floatValue); // Sample rate / Frequency / (2*PI)
}


void Square::process(jack_nframes_t nframes) {

	for (unsigned int i = 0; i < nframes; i++, rotor += rotor_step) {
		if (rotor < PI2) {
			lx[i] = rx[i] = 1;
		} else {
			lx[i] = rx[i] = -1;
		}
		if (rotor > PI_2) rotor -= PI_2;
	}
}

Q_EXPORT_PLUGIN2(square, Square)
