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

#include "triangle.h"

Triangle::Triangle() {

	author = "Caribe";
	name = "Triangle";
	description = "A simple triangle wave";
	type = Machine::MachineGenerator;

	addParam("freq", "Frequency", "The wave frequency", Param::ParamFloat, 50, 20000);

	rotor = 0;
}

Machine *Triangle::factory() {
	return new Triangle();
}

void Triangle::reconfig(const int sampling_rate) {

	rotor_step = PI_2 / (sampling_rate / paramsMap["freq"]->floatValue); // Sample rate / Frequency / (2*PI)

}

void Triangle::process(int framesStart, int framesEnd) {

	for (int i = framesStart; i < framesEnd; i++, rotor += rotor_step) {
		if (rotor > 1) rotor -= 1;

		if (rotor <= 0.25) {
			lx[i] = rx[i] = rotor * 4;
		} else if (rotor <= 0.75) {
			lx[i] = rx[i] = 2 - rotor * 4;
		} else {
			lx[i] = rx[i] = -4 + rotor * 4;
		}
	}
}

Q_EXPORT_PLUGIN2(triangle, Triangle)
