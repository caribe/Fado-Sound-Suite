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

#ifndef OVERDRIVE_H
#define OVERDRIVE_H

#include "../../../Fadoapp/ext/machine.h"

class Overdrive : public QObject, public Machine {

Q_OBJECT
Q_INTERFACES(Machine)

private:

public:
	Overdrive();
	void reconfig(const int sampling_rate);
	Machine *factory();
	void process(jack_nframes_t nframes);

};

#endif // OVERDRIVE_H
