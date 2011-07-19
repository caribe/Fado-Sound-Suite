#ifndef SQUARE_H
#define SQUARE_H

#include "../../../Fadoapp/ext/machine.h"

#include <cmath>

#define PI_2 6.28318530718

class Square : public QObject, public Machine {

Q_OBJECT
Q_INTERFACES(Machine)

private:
	float rotor, rotor_step;

public:
	Square();
	void reconfig(const int sampling_rate);
	Machine *factory();
	int process(jack_nframes_t nframes);

};

#endif // SQUARE_H
