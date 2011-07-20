#include "echo.h"

Echo::Echo() {

	author = "Caribe";
	name = "Echo";
	description = "A simple echo effect";
	type = Machine::MachineEffect;

	addParam("time", "Seconds of echo", Param::ParamFloat, 0.1, 1);
	addParam("vol", "Echo's volume", Param::ParamFloat, 0, 1);

	cursor = 0;
	samples = 0;
	buffer_lx = buffer_rx = 0;
}

Machine *Echo::factory() {
	return new Echo();
}


void Echo::reconfig(const int sampling_rate)
{
	int s = sampling_rate * params[0]->floatValue;

	if (samples < s) {
		if (buffer_lx != 0) delete buffer_lx;
		buffer_lx = new jack_default_audio_sample_t[s];
		if (buffer_rx != 0) delete buffer_rx;
		buffer_rx = new jack_default_audio_sample_t[s];

		for (int i = samples; i < s; i++) buffer_lx[i] = buffer_rx[i] = 0;
	}
	samples = s;
}


int Echo::process(jack_nframes_t nframes)
{
	unsigned int i;
	int j;
	float e1 = params[1]->floatValue;
	float e2 = 1 - e1;

	for (i = 0; i < nframes; i++) {
		j = (cursor + i) % samples;
		lx[i] = buffer_lx[j] = li[i] * e1 + buffer_lx[j] * e2;
		rx[i] = buffer_rx[j] = ri[i] * e1 + buffer_rx[j] * e2;
	}

	cursor = j;
	return 0;
}

Q_EXPORT_PLUGIN2(echo, Echo)
