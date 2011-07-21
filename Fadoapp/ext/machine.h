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
 * along with Fado Sound Suite.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
	QHash<QString, Param *> paramsMap;
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
	void addParam(QString code, QString name, QString description, Param::types type, int min = 0, int max = 0)
	{
		Param *param = new Param();
		param->name = name;
		param->description = description;
		param->type = type;
		param->min = min;
		param->max = max;
		params << param;
		paramsMap[code] = param;
	}
};

Q_DECLARE_INTERFACE(Machine, "org.altervista.saitfainder.fado.Machine/1.0")

#endif
