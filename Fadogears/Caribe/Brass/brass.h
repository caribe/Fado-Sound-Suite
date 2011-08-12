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

#ifndef BRASS_H
#define BRASS_H

#include <cmath>

#include "../../../Fadoapp/ext/machine.h"

class Brass : public QObject, public Machine {

Q_OBJECT
Q_INTERFACES(Machine)

private:
	float rotor, rotor_step, volume, value;
	float contour, contour_step, contour_attack, contour_decay, contour_sustain, contour_release, contour_attack_val, contour_sustain_val, contour_val;
	float tremolo, tremolo_step;
	float vibrato, vibrato_step, vibrato_val;
	float filter_dt, filter_fc, filter_alfa, filter_last, filter_value;
	int flag;

public:
	Brass();
	void reconfig(const int sampling_rate);
	void init();
	Machine *factory();
	void process(unsigned long nframes);

};

#endif // BRASS_H
