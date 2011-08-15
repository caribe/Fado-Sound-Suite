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

#include "highpassfilter.h"

Highpassfilter::Highpassfilter() {
	author = "Caribe";
	name = "High Pass Filter";
	description = "A simple High Pass Filter";
	type = Machine::MachineEffect;

	addParam("fc", "Cut off", "The cut off frequency", Param::ParamFloat, 50, 20000);
}

Machine *Highpassfilter::factory() {
	return new Highpassfilter();
}

void Highpassfilter::reconfig(const int sampling_rate) {
	float deltat = 1.0 / sampling_rate;
	float fc = 1 / (2 * M_PI * paramsMap["fc"]->floatValue);
	alfa = fc / (fc + deltat);

	last_rxo = last_lxo = 0;
	last_rxi = last_lxi = 0;
}

void Highpassfilter::process(int framesStart, int framesLength) {
	lx[0] = alfa * (last_lxo + li[0] - last_lxi);
	rx[0] = alfa * (last_rxo + ri[0] - last_rxi);

	for (int i = framesStart; i < framesLength; i++) {
		lx[i] = alfa * (lx[i-1] + li[i] - li[i-1]);
		rx[i] = alfa * (rx[i-1] + ri[i] - ri[i-1]);
	}

	last_lxo = lx[framesLength-1];
	last_rxo = rx[framesLength-1];
	last_lxi = li[framesLength-1];
	last_rxi = ri[framesLength-1];
}

Q_EXPORT_PLUGIN2(highpassfilter, Highpassfilter)
