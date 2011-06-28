#ifndef CORE_FILEINPUT_H
#define CORE_FILEINPUT_H

#include "ext/machine.h"

#include <sndfile.h>
// #include <cstdlib>

class FileInput : public Machine {

	public:
		FileInput();
		void reconfig();
		int process(jack_nframes_t nframes);
		void init();
		void finish();

		QHash<QString, float *> filebuffer;
		QString filename;
		SF_INFO info;
		int seek;
		float ratio;
};

#endif
