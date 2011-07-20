#include "gears/fileinput.h"

FileInput::FileInput()
{
	type = Machine::MachineInput;

	author = "Core";
	name = "File Input";
	description = "Input";

	addParam(new Param("file", "File name path", Param::ParamString, 0, 0));
	addParam(new Param("seek", "Position inside file", Param::ParamInt, 0, 1000000));
}


Machine *FileInput::factory() {
	return new FileInput();
}


void FileInput::init()
{
	SNDFILE *file;

	foreach (MachinePattern *pattern, patterns) {
		foreach (int row, pattern->params.keys()) {
			if (pattern->params[row].contains(0)) {
				info.format = 0;
				file = sf_open(pattern->params[row][0].toLatin1().data(), SFM_READ, &info);
				if (file == 0) {
					qDebug() << "Cannot open file" << pattern->params[row][0];
				} else if (sf_error(file)) {
					qDebug() << "Audio file error" << sf_strerror(file);
				} else {
					qDebug() << "Samplarate:" << info.samplerate << "Channels:" << info.channels << "Frames:" << info.frames;

					float *buf = (float *)malloc(sizeof(float) * info.frames * info.channels);
					int i = sf_readf_float(file, buf, info.frames);
					sf_close(file);
					
					if (i != info.frames) {
						qDebug() << "Loaded only bytes" << i;
						free(buf);
					} else {
						filebuffer[pattern->params[row][0]] = buf;
					}
				}
			}
		}
	}

}



void FileInput::finish()
{
	foreach (QString s, filebuffer.keys()) {
		// free(filebuffer[s]);
	}
}



void FileInput::reconfig(const int sampling_rate)
{
	seek = 0;
	ratio = (float)info.samplerate / (float)sampling_rate;
}



int FileInput::process(jack_nframes_t nframes) {
	// seek2 = seek * info.samplerate / sampling_rate
	unsigned int i, seek2;

	if (info.channels == 2) {
		for (i = 0; i < nframes; i++) {
			if (++seek > info.frames) seek = 0;
			seek2 = seek * ratio;
			lx[i] = filebuffer[params[0]->getString()][seek2*2];
			rx[i] = filebuffer[params[0]->getString()][seek2*2+1];
		}
	} else if (info.channels == 1) {
		for (i = 0; i < nframes; i++) {
			if (++seek > info.frames) seek = 0;
			seek2 = seek * ratio;
			lx[i] = rx[i] = filebuffer[params[0]->getString()][seek2];
		}
	}

	return 0;
}
