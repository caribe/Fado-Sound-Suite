#ifndef CORE_ENCODER_H
#define CORE_ENCODER_H

#include <QObject>

#include <sndfile.h>

class Encoder : public QObject
{
	Q_OBJECT

	public:
		static int encode(const QString &srcname, const QString &dstname, int samplerate, int channels, int format);
};

#endif
