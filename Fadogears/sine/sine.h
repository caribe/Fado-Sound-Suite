#ifndef SINE_H
#define SINE_H

#include "../../Fadoapp/ext/machine.h"

#include <cmath>

class Sine : public QObject, public Machine {

Q_OBJECT
Q_INTERFACES(Machine)

public:
	float rotor, rotor_step;

	Sine();
	void reconfig();
	int process(jack_nframes_t nframes);
};

#endif // SINE_H
