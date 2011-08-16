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

#ifndef CORE_CORE_H
#define CORE_CORE_H

#include <QPluginLoader>
#include <QDebug>
#include <QDir>
#include <QMessageBox>
#include <QStringList>
#include <QHash>
#include <QList>
#include <QStandardItemModel>
#include <QSettings>

#include <portaudio.h>

class Core;

#include "config.h"
#include "ext/machine.h"
#include "gears/master.h"
#include "gears/lineinput.h"
#include "gears/fileinput.h"

class Core : public QObject {

Q_OBJECT

public:
	explicit Core(QObject *parent = 0);

	// Funtionalities

	void loadPlugins();
	void loadPluginsFolder(QDir &dir, QHash<QString, QStandardItem *> &generatorsBuffer, QHash<QString, QStandardItem *> &effectsBuffer);

	Master *master;
	QString filename;

	// Playback control

	bool init();
	bool load(QString filename);
	int start(bool record);
	int stop();
	int save();
	int save(QString filename);

	PaStream *client;
	PaStreamParameters outputParameters;
	PaStreamParameters inputParameters;

	// *** Core

	int beat_per_pattern, total_patterns, track_first, track_last, baseBpm;
	int sampling_rate, buffer_size;
	bool record;

	// Gears
	QStandardItemModel *gearsTree;
	QList<Machine *> gears;

	// Gears instances (machines)
	QList<Machine *> machines;

	// Machine render order
	QList<Machine *> order;

	// Patterns

	MachinePattern *mutePattern;
	MachinePattern *breakPattern;

	// Methods

	bool orderMachines();
	int toggleConnection(Machine *from, Machine *to);

public slots:
	void baseBpmChanged(int);

signals:
	void messageCritical(const QString &title, const QString &text);
};

#endif
