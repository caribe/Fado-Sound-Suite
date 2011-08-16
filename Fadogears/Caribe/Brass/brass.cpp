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

#include "brass.h"


Brass::Brass() {

	author = "Caribe";
	name = "Brass";
	description = "Brass";
	type = Machine::MachineGenerator;

	addParam("freq", "Frequency", "The square frequency", Param::ParamFloat, 50, 20000);
}


Machine *Brass::factory() {
	return new Brass();
}

void Brass::init() {
	rotor = 0;
	contour = -1;
	flag = 0;
}



void Brass::reconfig(const int sampling_rate)
{
	rotor_step = 1 / (sampling_rate / paramsMap["freq"]->floatValue); // Sample rate / Frequency / (2*PI)

	flag = 1;
	contour = 0;
	contour_step = 1.0 / (sampling_rate * 2);
	contour_attack = 0.05;
	contour_attack_val = 1;
	contour_decay = 0.05;
	contour_sustain = 0.80;
	contour_sustain_val = 0.50;
	contour_release = 0.10;

	tremolo = 0;
	tremolo_step = 6 * PI_2 / sampling_rate;

	vibrato = 0;
	vibrato_step = 5 * PI_2 / sampling_rate;

	filter_dt = 1.0 / sampling_rate;
	filter_last = 0;
}



void Brass::process(int framesStart, int framesEnd)
{
	for (int i = framesStart; i < framesEnd; i++, rotor += rotor_step) {
		if (rotor > 1) rotor -= 1;

		contour_val = 0;
		if (flag) {
			if (contour < contour_attack) {
				// contour_attack_val : contour_attack = X : contour
				contour_val = contour_attack_val * contour / contour_attack;
			} else if (contour < contour_attack + contour_decay) {
				// contour_attack_val - contour_sustain_val : contour_decay = X : contour - contour_attack
				contour_val = contour_attack_val - ((contour_attack_val - contour_sustain_val) * (contour - contour_attack) / contour_decay);
			} else if (contour < contour_attack + contour_decay + contour_sustain) {
				contour_val = contour_sustain_val;
				// 1 : contour_attack + contour_decay + contour_sustain = X : contour - (contour_attack + contour_decay)
				contour_val += 0.05 * cos(tremolo) * ((contour - (contour_attack + contour_decay)) / (contour_attack + contour_decay + contour_sustain));
				tremolo += tremolo_step;
				if (tremolo > PI_2) tremolo -= PI_2;
			} else if (contour < contour_attack + contour_decay + contour_sustain + contour_decay) {
				// contour_sustain_val : contour_release = X : contour - contour_attack - contour_decay - contour_sustain
				contour_val = contour_sustain_val - (contour_sustain_val * (contour - contour_attack - contour_decay - contour_sustain) / contour_release);
			} else {
				flag = 0;
			}
			contour += contour_step;
		}

		vibrato_val = 0.1 * cos(vibrato) * contour;
		value = 1.0 - (rotor + vibrato_val) * 2; // Square wave
		vibrato += vibrato_step;
		if (vibrato > PI_2) vibrato -= PI_2;
		value *= contour_val;

		// Dynamic low-pass filter
		filter_fc = 1.0 / (PI_2 * (150 + 600 * contour_val));
		filter_alfa = filter_dt / (filter_fc + filter_dt);
		filter_value = value;
		value = filter_last + filter_alfa * (filter_value - filter_last);
		filter_last = value;

		lx[i] = rx[i] = value;
	}
}

Q_EXPORT_PLUGIN2(brass, Brass)
