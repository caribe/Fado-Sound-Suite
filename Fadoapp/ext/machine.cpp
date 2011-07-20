#include "machine.h"


int Machine::preprocess(jack_nframes_t nframes, int process)
{
	for (unsigned int j = 0; j < nframes; j++) li[j] = ri[j] = 0;

	float value = 100.0 * this->connectionSrc.size();

	foreach (Machine *src, this->connectionSrc.keys()) {
		jack_default_audio_sample_t *lx = src->lx;
		jack_default_audio_sample_t *rx = src->rx;

		Volume *volume = this->connectionSrc[src];

		for (unsigned int j = 0; j < nframes; j++) {
			li[j] += lx[j] * volume->lx / value;
			ri[j] += rx[j] * volume->rx / value;
		}
	}

	if (process > 0) this->process(nframes);
	return 0;
}


int Machine::preprocess(jack_nframes_t nframes)
{
	return preprocess(nframes, 1);
}


void Machine::setName(const QString &name)
{
	this->name = name;
}


void Machine::setDescription(const QString &description)
{
	this->description = description;
}


void Machine::addParam(Param *param)
{
	params.append(param);
}



Param *Machine::getParam(int index)
{
	return params[index];
}


QString Machine::validate(int param, QString value)
{
	if (value == "") return "";

	if (params[param]->type == Param::ParamInt or params[param]->type == Param::ParamFreq) {
		int intValue = value.toInt();
		if (intValue < params[param]->min) intValue = params[param]->min;
		if (intValue > params[param]->max) intValue = params[param]->max;
		return QString::number(intValue);
	}

	if (params[param]->type == Param::ParamFloat) {
		float floatVal = value.toFloat();
		if (floatVal < params[param]->min) floatVal = params[param]->min;
		if (floatVal > params[param]->max) floatVal = params[param]->max;
		return QString::number(floatVal);
	}

	if (params[param]->type == Param::ParamNote) {
		QChar note, octave;
		bool mid;
		int pos = 0;

		note = value[0];
		if (value[1] == '#') {
			mid = true;
			octave = value[2];
		} else {
			mid = false;
			octave = value[1];
		}
		
		if (note == 'C') {
			pos = mid ? 0 : 1;
		} else if (note == 'D') {
			pos = mid ? 2 : 3;
		} else if (note == 'E') {
			pos = 4;
		} else if (note == 'F') {
			pos = mid ? 5 : 6;
		} else if (note == 'G') {
			pos = mid ? 7 : 8;
		} else if (note == 'A') {
			pos = mid ? 9 : 10;
		} else if (note == 'B') {
			pos = 11;
		} else {
			return "";
		}

		if (octave == '3') {
			pos += 12;
		} else if (octave == '4') {
			pos += 12 * 2;
		} else if (octave == '5') {
			pos += 12 * 3;
		} else if (octave == '6') {
			pos += 12 * 4;
		} else if (octave == '7') {
			pos += 12 * 5;
		} else {
			return "";
		}

		return value;
	}

	return value;
}


MachinePattern *Machine::addPattern(const QString &name)
{
	MachinePattern *pattern = new MachinePattern();
	pattern->name = name;
	patterns.append(pattern);
	return pattern;
}


QString Machine::getName()
{
	if (alias.isNull()) {
		return name;
	} else {
		return alias;
	}
}
