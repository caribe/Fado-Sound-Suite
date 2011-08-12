/*
 * Fado Sound Suite - Modular synthetic music generator and sound processor
 *
 * Copyright (C) 2011 Vincenzo Buttazzo <vbuttazzo@yahoo.com>
 *
 * This file is part of Fado Sound Suite.
 *
 * Fado Sound Suite is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fado Sound Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * aunsigned long with Fado Sound Suite.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "core/encoder.h"

int Encoder::encode(const QString &srcname, const QString &dstname, int samplerate, int channels, int format) {
	short *buffer = new short[channels*1024];
	int frames;

	SF_INFO sfinfo_src;
	SF_INFO sfinfo_dst;
	SNDFILE* src;
	SNDFILE* dst;

	if (format != 0) return -1;

	// Input file

	sfinfo_src.samplerate = samplerate;
	sfinfo_src.channels = channels;
	sfinfo_src.format = SF_FORMAT_RAW | SF_FORMAT_PCM_16;

	if (sf_format_check(&sfinfo_src) == 0) return 0x10;
	if ((src = sf_open(srcname.toLatin1().data(), SFM_READ, &sfinfo_src)) == 0) return 0x11;

	// Output file

	sfinfo_dst.format = SF_FORMAT_OGG | SF_FORMAT_VORBIS;
	sfinfo_dst.samplerate = samplerate;
	sfinfo_dst.channels = channels;

	if (sf_format_check(&sfinfo_dst) == 0) return 0x20;
	if ((dst = sf_open(dstname.toLatin1().data(), SFM_WRITE, &sfinfo_dst)) == 0) return 0x21;

	// Conversion

	while ((frames = sf_readf_short(src, buffer, 1024)) > 0) sf_writef_short(dst, buffer, frames);

	// Bye

	sf_close(dst);
	sf_close(src);

	return 0;
}
