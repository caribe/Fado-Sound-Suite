#ifndef LIBRARY_MACHINE_HPP
#define LIBRARY_MACHINE_HPP

#include "store.h"
#include "param.h"

#include <QHash>

#include <jack/jack.h>

#define MACHINE(CLASS, TYPE, GROUP, NAME, DESCRIPTION, VERSION) \
extern "C" QStringList info() { \
	QStringList info; \
	info << TYPE << GROUP << NAME << DESCRIPTION << VERSION; \
	return info; \
} \
\
extern "C" Machine *maker() { \
	Machine *m = new CLASS(); \
	QStringList i = info(); \
	m->type = i[0]; \
	m->author = i[1]; \
	m->name = i[2]; \
	m->description = i[3]; \
	m->version = i[4]; \
	return m; \
} \

class Store;

using namespace std;

class Machine
{
	public:
		enum types { t_output, t_input, t_generator, t_effect };
		jack_default_audio_sample_t lx[2048], rx[2048];
		jack_default_audio_sample_t li[2048], ri[2048];
		Store *store;
		int id, x, y;

		QString type, name, description, author, version;

		QHash <QString, Param *> params;
		QList <QString> keys;
		QHash<int, QHash<int, QHash<QString, QString> > > patterns;

		QHash<int, int> track;

		virtual int process(jack_nframes_t nframes)=0;
		virtual void reconfig()=0;
		virtual void init() {}
		virtual void finish() {}

		QString validate(QString param, QString value);
		int preprocess(jack_nframes_t nframes);
		int preprocess(jack_nframes_t nframes, int process);

		void setName(const QString &name);
		void setDescription(const QString &description);
		void addParam(Param *param);
		Param *getParam(QString name);
};

#endif
