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

#include "sinefm.h"

Sinefm::Sinefm() {

	author = "Caribe";
	name = "Sine FM";
	description = "A sine signal with frequency modulation";
	type = Machine::MachineGenerator;

	addParam("freqC", "Carrier Frequency", "The carrier frequency", Param::ParamFloat, 50, 20000);
	addParam("freqM", "Modulator Frequency", "The modulator frequency", Param::ParamFloat, 1, 1000);
	addParam("amplM", "Modulator Amplitude", "The modulated ampliture", Param::ParamFloat, 0, 100);

	rotorC = rotorM = 0;
}

Machine *Sinefm::factory() {
	return new Sinefm();
}

void Sinefm::reconfig(const int sampling_rate) {

	rotorStepC = PI_2 / (sampling_rate / paramsMap["freqC"]->floatValue);
	rotorStepM = PI_2 / (sampling_rate / paramsMap["freqM"]->floatValue);
	amplM = paramsMap["amplM"]->floatValue;

}

void Sinefm::process(jack_nframes_t nframes) {

	for (unsigned int i = 0; i < nframes; i++) {
		rotorC += rotorStepC;
		rotorM += rotorStepM;

		lx[i] = rx[i] = std::cos(rotorC + std::cos(rotorM) * amplM);

		if (rotorC > PI_2) rotorC -= PI_2;
		if (rotorM > PI_2) rotorM -= PI_2;
	}
}

Q_EXPORT_PLUGIN2(sinefm, Sinefm)
