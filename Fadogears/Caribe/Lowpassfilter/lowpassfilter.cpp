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

#include "lowpassfilter.h"

Lowpassfilter::Lowpassfilter() {
	author = "Caribe";
	name = "Low Pass Filter";
	description = "A simple Low Pass Filter";
	type = Machine::MachineEffect;

	addParam("fc", "Cut off", "The cut off frequency", Param::ParamFloat, 50, 20000);
}

Machine *Lowpassfilter::factory() {
	return new Lowpassfilter();
}

void Lowpassfilter::reconfig(const int sampling_rate) {
	float deltat = 1.0 / sampling_rate;
	float fc = 1 / (2 * M_PI * paramsMap["fc"]->floatValue);
	alfa = deltat / (fc + deltat);

	last_rx = last_lx = 0;
}

void Lowpassfilter::process(unsigned long nframes) {
	lx[0] = last_lx + alfa * (li[0] - last_lx);
	rx[0] = last_rx + alfa * (li[0] - last_rx);

	for (unsigned int i = 1; i < nframes; i++) {
		lx[i] = lx[i-1] + alfa * (li[i] - lx[i-1]);
		rx[i] = rx[i-1] + alfa * (li[i] - lx[i-1]);
	}

	last_lx = lx[nframes-1];
	last_rx = rx[nframes-1];
}

Q_EXPORT_PLUGIN2(lowpassfilter, Lowpassfilter)
