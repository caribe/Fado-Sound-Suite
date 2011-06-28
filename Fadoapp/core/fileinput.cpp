#include "core/fileinput.h"

FileInput::FileInput()
{
	/*
	type = Machine::t_input;

	type = "generator";
	author = "Core";
	name = "fileinput";
	description = "Input";

	addParam(new Param("file", "File name path", Param::string_t, 0, 0));
	addParam(new Param("seek", "Position inside file", Param::int_t, 0, 1000000));
	*/
}


void FileInput::init()
{
	SNDFILE *file;
/*
	foreach (int pattern, patterns.keys()) {
		foreach (int row, patterns[pattern].keys()) {
			if (patterns[pattern][row].contains("file")) {
				info.format = 0;
				file = sf_open(patterns[pattern][row]["file"].toUtf8().data(), SFM_READ, &info);
				if (file == 0) {
					printf("Cannot open file %s\n", patterns[pattern][row]["file"].toUtf8().data());
				} else if (sf_error(file)) {
					printf("Audio file error: %s\n", sf_strerror(file));
				} else {
					printf("Samplarate: %d Channels: %d Frames: %d\n", info.samplerate, info.channels, info.frames);

					float *buf = (float *)malloc(sizeof(float) * info.frames * info.channels);
					int i = sf_readf_float(file, buf, info.frames);
					sf_close(file);
					
					if (i != info.frames) {
						printf("Loaded only %d bytes...\n", i);
						free(buf);
					} else {
						filebuffer[patterns[pattern][row]["file"]] = buf;
					}
				}
			}
		}
	}
*/
}


void FileInput::finish() {
	foreach (QString s, filebuffer.keys()) {
		free(filebuffer[s]);
	}
}

void FileInput::reconfig() {
	/*
	seek = 0;
	ratio = (float)info.samplerate / (float)store->sampling_rate;
	*/
}

// seek2 = seek * info.samplerate / sampling_rate

int FileInput::process(jack_nframes_t nframes) {
	/*
	int i, seek2;

	if (info.channels == 2) {
		for (i = 0; i < nframes; i++) {
			if (++seek > info.frames) seek = 0;
			seek2 = seek * ratio;
 			lx[i] = filebuffer[params["file"]->getString()][seek2*2];
 			rx[i] = filebuffer[params["file"]->getString()][seek2*2+1];
		}
	} else if (info.channels == 1) {
		for (i = 0; i < nframes; i++) {
			if (++seek > info.frames) seek = 0;
			seek2 = seek * ratio;
			lx[i] = rx[i] = filebuffer[params["file"]->getString()][seek2];
		}
	}
*/
	return 0;
}
