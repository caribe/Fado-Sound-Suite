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

#include "gaussiannoise.h"

Gaussiannoise::Gaussiannoise() {
	author = "Caribe";
	name = "Gaussian Noise";
	description = "A simple gaussian noise signal";
	type = Machine::MachineGenerator;
}

Machine *Gaussiannoise::factory() {
	return new Gaussiannoise();
}

void Gaussiannoise::reconfig(const int sampling_rate) {}

void Gaussiannoise::process(jack_nframes_t nframes) {

	for (unsigned int i = 0; i < nframes; i++) {
		lx[i] = (qrand() % 1000) / 1000.0;
		rx[i] = (qrand() % 1000) / 1000.0;
	}
}

Q_EXPORT_PLUGIN2(gaussiannoise, Gaussiannoise)
