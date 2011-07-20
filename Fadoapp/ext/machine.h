#ifndef LIBRARY_MACHINE_HPP
#define LIBRARY_MACHINE_HPP

#include <jack/jack.h>

#include <QHash>
#include <QtPlugin>
#include <QVariantList>

#include "param.h"

#define PI_2 6.28318530718

class Volume {

public:
	int rx, lx;

	Volume() {
		rx = lx = 100;
	}
};

class MachinePattern {
public:
	QString name;
	QHash<int, QHash<int, QString> > params;
	enum PatternType { StandardPattern, MutePattern, BreakPattern };
	PatternType type;

	MachinePattern() {
		type = StandardPattern;
	}

	MachinePattern(PatternType type) {
		this->type = type;
	}
};

class Machine {

public:

	// Interface
	virtual Machine *factory()=0;
	virtual int process(jack_nframes_t nframes)=0;
	virtual void reconfig(const int sampling_rate)=0;
	virtual void init() {}
	virtual void finish() {}

	// Jack
	enum MachineType { MachineMaster, MachineInput, MachineGenerator, MachineEffect };

	jack_default_audio_sample_t lx[2048], rx[2048];
	jack_default_audio_sample_t li[2048], ri[2048];
	int x, y;

	MachineType type;
	QString name, description, author, alias;
	int version;

	QList<Param *> params;
	QList<MachinePattern *> patterns;

	// Connections
	QHash<Machine *, Volume *> connectionSrc;
	QHash<Machine *, Volume *> connectionDst;

	QHash<int, MachinePattern *> track;

	QString validate(int index, QString value);
	int preprocess(jack_nframes_t nframes);
	int preprocess(jack_nframes_t nframes, int process);

	void setName(const QString &name);
	void setDescription(const QString &description);
	QString getName();

	MachinePattern *addPattern(const QString &name);

	Param *getParam(int index);
	void addParam(Param *param);
	void addParam(QString name, QString description, Param::types type, int min, int max)
	{
		Param *param = new Param();
		param->name = name;
		param->description = description;
		param->type = type;
		param->min = min;
		param->max = max;
		params.append(param);
	}
};

Q_DECLARE_INTERFACE(Machine, "org.altervista.saitfainder.fado.Machine/1.0")

#endif
