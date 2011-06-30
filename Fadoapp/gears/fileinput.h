#ifndef CORE_FILEINPUT_H
#define CORE_FILEINPUT_H

#include <sndfile.h>


#include "ext/machine.h"


class FileInput : public Machine {

	public:
		FileInput();
		void reconfig();
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
