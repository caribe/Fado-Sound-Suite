#include "square.h"

Square::Square() {

	author = "Caribe";
	name = "My Square";
	description = "A simple square signal";
	type = Machine::MachineGenerator;

	addParam("freq", "The square frequency", Param::ParamFloat, 50, 20000);

	rotor = 0;
}


Machine *Square::factory() {
	return new Square();
}


void Square::reconfig(const int sampling_rate) {
	rotor_step = PI_2 / (sampling_rate / params[0]->floatValue); // Sample rate / Frequency / (2*PI)
}


int Square::process(jack_nframes_t nframes) {

	float d, j;
	for (unsigned int i = 0; i < nframes; i++, rotor += rotor_step) {
		d = modff(rotor, &j);
		if (d < 0.5) {
			lx[i] = rx[i] = 1;
		} else {
			lx[i] = rx[i] = -1;
		}
		if (rotor > 1) rotor -= 1;
	}

	return 0;
}

Q_EXPORT_PLUGIN2(square, Square)
