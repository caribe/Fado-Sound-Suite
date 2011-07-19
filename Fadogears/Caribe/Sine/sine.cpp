#include "sine.h"

Sine::Sine() {

	author = "Caribe";
	name = "My Sine";
	description = "A simple sine signal";
	type = Machine::MachineGenerator;

	addParam("freq", "The sine frequency", Param::ParamFloat, 50, 20000);
	addParam("offset", "The sine offset", Param::ParamInt, 0, 100);

	rotor = 0;
}

Machine *Sine::factory() {
	return new Sine();
}

void Sine::reconfig(const int sampling_rate) {

	rotor_step = PI_2 / (sampling_rate / params[0]->floatValue); // Sample rate / Frequency / (2*PI)

}

int Sine::process(jack_nframes_t nframes) {

	for (unsigned int i = 0; i < nframes; i++, rotor += rotor_step) {
		lx[i] = rx[i] = std::cos(rotor);
		if (rotor > PI_2) rotor -= PI_2;
	}

	return 0;
}

Q_EXPORT_PLUGIN2(sine, Sine)
