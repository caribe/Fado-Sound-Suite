#include "core/lineinput.h"

LineInput::LineInput()
{
	type = Machine::t_input;

	type = "generator";
	author = "Core";
	name = "lineinput";
	description = "Input";
}

void LineInput::reconfig() {};


int LineInput::process(jack_nframes_t nframes) {};
