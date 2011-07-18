#ifndef CORE_FILEINPUT_H
#define CORE_FILEINPUT_H

#include <QDebug>

#include <sndfile.h>

#include "ext/machine.h"


class FileInput : public Machine {

	public:
		FileInput();
		void reconfig(const int sampling_rate);
		int process(jack_nframes_t nframes);
		void init();
		void finish();
		Machine *factory();

		QHash<QString, float *> filebuffer;
		QString filename;
		SF_INFO info;
		int seek;
		float ratio;
};

#endif
