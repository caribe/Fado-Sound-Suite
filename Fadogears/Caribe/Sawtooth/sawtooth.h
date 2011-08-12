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

#ifndef SAWTOOTH_H
#define SAWTOOTH_H

#include "../../../Fadoapp/ext/machine.h"

class Sawtooth : public QObject, public Machine {

Q_OBJECT
Q_INTERFACES(Machine)

private:
	float rotor, rotor_step;

public:
	Sawtooth();
	void reconfig(const int sampling_rate);
	Machine *factory();
	void process(unsigned long nframes);
};

#endif // SAWTOOTH_H
