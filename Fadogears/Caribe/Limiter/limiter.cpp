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

#include "limiter.h"

Limiter::Limiter() {

	author = "Caribe";
	name = "Limiter";
	description = "A simple limiter effect";
	type = Machine::MachineEffect;

	addParam("vol", "Limit", "The limit level", Param::ParamFloat, 0.1, 1);

}

Machine *Limiter::factory() {
	return new Limiter();
}

void Limiter::process(jack_nframes_t nframes)
{
	float vol = paramsMap["vol"]->floatValue;

	for (unsigned int i = 0; i < nframes; i++) {
		if (li[i] > vol) lx[i] = vol; else if (li[i] < -vol) lx[i] = -vol; else lx[i] = li[i];
		if (ri[i] > vol) rx[i] = vol; else if (ri[i] < -vol) rx[i] = -vol; else rx[i] = ri[i];
	}

}

Q_EXPORT_PLUGIN2(limiter, Limiter)
