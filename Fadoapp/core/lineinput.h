#ifndef CORE_LINEINPUT_H
#define CORE_LINEINPUT_H

#include "ext/machine.h"

class LineInput : public Machine {

	public:
		LineInput();
		void reconfig();
		int process(jack_nframes_t nframes);
};

#endif
