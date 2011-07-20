#ifndef ECHO_H
#define ECHO_H

#include "../../../Fadoapp/ext/machine.h"

class Echo : public QObject, public Machine {

Q_OBJECT
Q_INTERFACES(Machine)

private:
	jack_default_audio_sample_t *buffer_lx, *buffer_rx;
	float alfa;
	int samples, cursor;

public:
	Echo();
	void reconfig(const int sampling_rate);
	Machine *factory();
	int process(jack_nframes_t nframes);

};

#endif // ECHO_H
