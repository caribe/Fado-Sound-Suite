#ifndef CORE_ENCODER_H
#define CORE_ENCODER_H

#include <sndfile.h>

class Encoder
{
	public:
		static int encode(char *srcname, char *dstname, int samplerate, int channels, int format);
};

#endif
