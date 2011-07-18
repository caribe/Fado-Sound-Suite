#ifndef SINE_H
#define SINE_H

#include "../../Fadoapp/ext/machine.h"

#include <cmath>

#define PI_2 6.28318530718

class Sine : public QObject, public Machine {

Q_OBJECT
Q_INTERFACES(Machine)

private:
	float rotor, rotor_step;

public:
	Sine();
	void reconfig(const int sampling_rate);
	Machine *factory();
	int process(jack_nframes_t nframes);

};

#endif // SINE_H
