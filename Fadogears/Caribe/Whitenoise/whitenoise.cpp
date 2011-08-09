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

#include "whitenoise.h"

Whitenoise::Whitenoise() {
	author = "Caribe";
	name = "White Noise";
	description = "A simple white noise signal";
	type = Machine::MachineGenerator;
}

Machine *Whitenoise::factory() {
	return new Whitenoise();
}

void Whitenoise::init() {
	max = RAND_MAX / 2.0;
}

void Whitenoise::process(jack_nframes_t nframes) {
	for (unsigned int i = 0; i < nframes; i++) {
		lx[i] = qrand() / max - 1;
		rx[i] = qrand() / max - 1;
	}
}

Q_EXPORT_PLUGIN2(whitenoise, Whitenoise)
