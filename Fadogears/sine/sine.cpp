#include "../../Fadoapp/ext/machine.h"

#include <cmath>

class Sine : public QObject, public Machine {

Q_OBJECT
Q_INTERFACES(Machine)

public:
	float rotor, rotor_step;

	Sine() {
		addParam(new Param("freq", "The sine frequency", Param::float_t, 50, 20000));
		addParam(new Param("vol", "The sine volume", Param::float_t, 0, 1));

		params["freq"]->set(1000.0);
		params["vol"]->set(1.0);

		rotor = 0;
	}

	void reconfig() {
		rotor_step = PI_2 / (store->sampling_rate / params["freq"]->getFloat()); // Sample rate / Frequency / (2*PI)
	}

	int process(jack_nframes_t nframes) {
		float vol = params["vol"]->getFloat();
		for (unsigned int i = 0; i < nframes; i++, rotor += rotor_step) {
			lx[i] = rx[i] = std::cos(rotor) * vol;
			if (rotor > PI_2) rotor -= PI_2;
		}
	}
};

Q_EXPORT_PLUGIN2(sine, Sine)
