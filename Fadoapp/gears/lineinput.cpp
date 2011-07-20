#include "gears/lineinput.h"

LineInput::LineInput()
{
	type = Machine::MachineInput;

	author = "Core";
	name = "Line Input";
	description = "Input";
}

Machine *LineInput::factory() {
	return new LineInput();
}

void LineInput::reconfig(const int sampling_rate) {}


int LineInput::process(jack_nframes_t nframes) { return 0; }
