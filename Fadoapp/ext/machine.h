#ifndef LIBRARY_MACHINE_HPP
#define LIBRARY_MACHINE_HPP

#include <jack/jack.h>

#include <QHash>
#include <QtPlugin>

#include "param.h"

#define PI_2 6.28318530718

class Machine {

	public:
		virtual int process(jack_nframes_t nframes)=0;
		virtual void reconfig()=0;
		virtual void init() {}
		virtual void finish() {}

		enum types { t_output, t_input, t_generator, t_effect };
		jack_default_audio_sample_t lx[2048], rx[2048];
		jack_default_audio_sample_t li[2048], ri[2048];
		int id, x, y;

		QString type, name, description, author, version;

		QHash <QString, Param *> params;
		QList <QString> keys;
		QHash<int, QHash<int, QHash<QString, QString> > > patterns;

		QHash<int, int> track;

		QString validate(QString param, QString value);
		int preprocess(jack_nframes_t nframes);
		int preprocess(jack_nframes_t nframes, int process);

		void setName(const QString &name);
		void setDescription(const QString &description);
		void addParam(Param *param);
		Param *getParam(QString name);

};

Q_DECLARE_INTERFACE(Machine, "org.altervista.saitfainder.fado.Machine/1.0")

#endif
