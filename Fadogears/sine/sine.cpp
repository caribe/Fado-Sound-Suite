#include "sine.h"

Sine::Sine() {

	name = "My Sine";
/*
	addParam(new Param("freq", "The sine frequency", Param::float_t, 50, 20000));
	addParam(new Param("vol", "The sine volume", Param::float_t, 0, 1));

	params["freq"]->set(1000.0);
	params["vol"]->set(1.0);
*/
	rotor = 0;

}

void Sine::reconfig() {

	// rotor_step = PI_2 / (core->sampling_rate / params["freq"]->getFloat()); // Sample rate / Frequency / (2*PI)

}

int Sine::process(jack_nframes_t nframes) {

	float vol = params["vol"]->getFloat();
	for (unsigned int i = 0; i < nframes; i++, rotor += rotor_step) {
		lx[i] = rx[i] = std::cos(rotor) * vol;
		if (rotor > PI_2) rotor -= PI_2;
	}

	return 0;
}

Q_EXPORT_PLUGIN2(sine, Sine)
