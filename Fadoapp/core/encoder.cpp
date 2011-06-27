#include "core/encoder.h"

int Encoder::encode(char *srcname, char *dstname, int samplerate, int channels, int format) {
	short *buffer = new short[channels*1024];
	int frames;

	SF_INFO sfinfo_src;
	SF_INFO sfinfo_dst;
	SNDFILE* src;
	SNDFILE* dst;

	// Input file

	sfinfo_src.samplerate = samplerate;
	sfinfo_src.channels = channels;
	sfinfo_src.format = SF_FORMAT_RAW | SF_FORMAT_PCM_16;

	if (sf_format_check(&sfinfo_src) == 0) return 0x10;
	if ((src = sf_open(srcname, SFM_READ, &sfinfo_src)) == 0) return 0x11;

	// Output file

	sfinfo_dst.format = SF_FORMAT_OGG | SF_FORMAT_VORBIS;
	sfinfo_dst.samplerate = samplerate;
	sfinfo_dst.channels = channels;

	if (sf_format_check(&sfinfo_dst) == 0) return 0x20;
	if ((dst = sf_open(dstname, SFM_WRITE, &sfinfo_dst)) == 0) return 0x21;

	// Conversion

	while ((frames = sf_readf_short(src, buffer, 1024)) > 0) sf_writef_short(dst, buffer, frames);

	// Bye

	sf_close(dst);
	sf_close(src);

	return 0;
}
