#include "gears/lineinput.h"

LineInput::LineInput()
{
	type = Machine::MachineInput;

	author = "Core";
	name = "lineinput";
	description = "Input";
}

Machine *LineInput::factory() {
	return new LineInput();
}

void LineInput::reconfig() {}


int LineInput::process(jack_nframes_t nframes) {}
