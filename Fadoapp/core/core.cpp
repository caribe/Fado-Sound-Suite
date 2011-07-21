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

#include "core/core.h"

Core::Core(QObject *parent) : QObject(parent)
{
	master = 0;
	pluginsPath = "../Fadogears/";

	beat_per_pattern = 16;
	total_patterns = 16;
	gearsTree = new QStandardItemModel();

	mutePattern = new MachinePattern();
	mutePattern->type = MachinePattern::MutePattern;
	mutePattern->name = "Mute";

	breakPattern = new MachinePattern();
	breakPattern->type = MachinePattern::BreakPattern;
	breakPattern->name = "Break";
}



void Core::loadPlugins()
{
	QStandardItem *item;

	// Base folders

	QStandardItem *coreFolder = new QStandardItem(tr("Core"));
	coreFolder->setEditable(false);
	coreFolder->setData(-100);
	gearsTree->appendRow(coreFolder);

	QStandardItem *generatorsFolder = new QStandardItem(tr("Generators"));
	generatorsFolder->setEditable(false);
	generatorsFolder->setData(-100);
	gearsTree->appendRow(generatorsFolder);

	QStandardItem *effectsFolder = new QStandardItem(tr("Effects"));
	effectsFolder->setEditable(false);
	effectsFolder->setData(-100);
	gearsTree->appendRow(effectsFolder);

	// Buffers

	QHash<QString, QStandardItem *> generatorsBuffer;
	QHash<QString, QStandardItem *> effectsBuffer;

	// Standard gears

	item = new QStandardItem(QIcon(":line-input"), "Line Input");
	item->setEditable(false);
	item->setData(-2);
	coreFolder->appendRow(item);

	item = new QStandardItem(QIcon(":file-input"), "File Input");
	item->setEditable(false);
	item->setData(-3);
	coreFolder->appendRow(item);

	// Plugins

	QDir dir(pluginsPath);
	loadPluginsFolder(dir, generatorsBuffer, effectsBuffer);

	foreach (QStandardItem *item, generatorsBuffer.values()) {
		generatorsFolder->appendRow(item);
	}

	foreach (QStandardItem *item, effectsBuffer.values()) {
		effectsFolder->appendRow(item);
	}
}



void Core::loadPluginsFolder(QDir &dir, QHash<QString, QStandardItem *> &generatorsBuffer, QHash<QString, QStandardItem *> &effectsBuffer)
{
	qDebug() << "Loading machines..." << dir.path();

	QStringList dirList = dir.entryList();

	foreach (QString name, dirList) {

		if (name.startsWith(".")) continue;

		QFileInfo info(dir.path() + "/" + name);

		if (info.isDir()) {
			QDir dir(info.filePath());
			loadPluginsFolder(dir, generatorsBuffer, effectsBuffer);
		} else if (name.endsWith(".so")) {

			QPluginLoader *lib = new QPluginLoader(info.filePath());

			if (lib->load()) {
				int id = gears.length();

				Machine *machine = qobject_cast<Machine *>(lib->instance());
				gears.append(machine);

				qDebug() << "Loaded" << machine->name << id;

				QStandardItem *item;

				if (machine->type == Machine::MachineGenerator) {
					if (generatorsBuffer.contains(machine->author) == false) {
						QStandardItem *userItem = new QStandardItem(QIcon(":user"), machine->author);
						userItem->setEditable(false);
						userItem->setData(-100);
						generatorsBuffer[machine->author] = userItem;
					}
					item = new QStandardItem(QIcon(":generator"), machine->name);
					generatorsBuffer[machine->author]->appendRow(item);
				} else if (machine->type == Machine::MachineEffect) {
					if (effectsBuffer.contains(machine->author) == false) {
						QStandardItem *userItem = new QStandardItem(QIcon(":user"), machine->author);
						userItem->setEditable(false);
						userItem->setData(-100);
						effectsBuffer[machine->author] = userItem;
					}
					item = new QStandardItem(QIcon(":effect"), machine->name);
					effectsBuffer[machine->author]->appendRow(item);
				}

				item->setToolTip(machine->description);
				item->setEditable(false);
				item->setData(id);

			} else {
				qDebug() << "Error" << lib->errorString();
			}
		}
	}
}



int Core::jack_init()
{
		qDebug() << "Starting Jack...";

	// try to become a client of the JACK server
	jack_status_t jack_status;
	if ((client = jack_client_open("fado", JackUseExactName, &jack_status, "default")) == 0) {
		emit messageCritical(tr("Jack server not running"), "Error " + QString::number(jack_status));
		return 1;
	}

		qDebug() << "Define callback...";

	// tell the JACK server to call `process()' whenever there is work to be done.
	jack_set_process_callback(client, ::jack_process, (void *)this);

		qDebug() << "Define shutdown...";

	// tell the JACK server to call `jack_shutdown()' if it ever shuts down, either entirely, or if it just decides to stop calling us.
	jack_on_shutdown(client, ::jack_shutdown, (void *)this);

		qDebug() << "Get sample rate...";

	// display the current sample rate. 
	sampling_rate = jack_get_sample_rate(client);
	buffer_size = jack_get_buffer_size(client);

	qDebug() << "Engine sample rate: " << jack_get_sample_rate(client);

	// create four ports
	input_port[0] = jack_port_register(client, "input_lx", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	input_port[1] = jack_port_register(client, "input_rx", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

	output_port[0] = jack_port_register(client, "output_lx", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	output_port[1] = jack_port_register(client, "output_rx", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	return 0;
}


bool Core::load(QString filename)
{
	this->filename = filename;
	qDebug() << "Loading config..." << filename;

	int config = 0;
	config = Config::load(filename, this);

	if (config != 0) {
		emit messageCritical(tr("Error"), tr("Cannot load config (%1)\nError code %2").arg(filename).arg(config));
		return false;
	}

	if (orderMachines()) {
		return true;
	} else {
		emit messageCritical(tr("Error"), tr("Cannot establish this connection because it would create a loop..."));
		return false;
	}
}


int Core::save()
{
	return Config::save(filename, this);
}


int Core::save(QString filename)
{
	this->filename = filename;
	return save();
}


int Core::start(bool record)
{
	this->record = record;
	master = (Master *)machines.value(0);
	if (master) {
		if (master->init(this) == 0) {
			master->go(client, input_port, output_port, record);
		}
		return 0;
	} else {
		emit messageCritical(tr("Error"), "No master?");
		return 1;
	}
}


int Core::stop()
{
	QSettings settings;

	master->stop();

	if (record) Encoder::encode(settings.value("settings/tmpfile", "/tmp/record.raw").toString(), "./record.ogg", 48000, 2, 0);

	return 0;
}



int Core::checkUpdates()
{
	if (!updates) updates = new Updates(this);
	updates->check();
	return 0;
}


int jack_process(jack_nframes_t nframes, void *arg)
{
	Core *core = (Core *)arg;
	if (core->master) core->master->process(nframes);
	return 0;
}


void jack_shutdown(void *arg) {}




bool Core::orderMachines()
{
	QHash<Machine *, QList<Machine *> > iconns;
	QHash<Machine *, QList<Machine *> > oconns;
	QList<Machine *> order;

	// oconn e iconn initialization
	foreach (Machine *machine, machines) {
		iconns[machine] << machine->connectionSrc.keys();
		oconns[machine] << machine->connectionDst.keys();
	}

	// *** Ordering machines to elaboration

	int watchdog = machines.length();

	while (iconns.size() > 0 and --watchdog >= 0) { // While there are machines to check

		QList<Machine *> machines = iconns.keys();
		foreach (Machine *machine, machines) {
			if (iconns[machine].length() == 0) { // Search machine with no pending inputs

				foreach (Machine *dst, oconns[machine]) { // Removes the machine from the inputs of the connected machines
					iconns[dst].removeOne(machine);
				}

				order << machine;
				iconns.remove(machine);
				oconns.remove(machine);
			}
		}

	}

	// Loop
	if (watchdog < 0) return false;

	this->order = order;

	return true;
}



int Core::toggleConnection(Machine *m1, Machine *m2)
{
	if (m1->connectionDst.contains(m2)) {
		Volume *volume = m1->connectionDst[m2];
		m1->connectionDst.remove(m2);
		m2->connectionSrc.remove(m1);
		delete volume;
		orderMachines();
		return 1;
	}

	if (m1->connectionSrc.contains(m2)) {
		Volume *volume = m1->connectionSrc[m2];
		m1->connectionSrc.contains(m2);
		m2->connectionDst.contains(m1);
		delete volume;
		orderMachines();
		return 1;
	}

	if (m1->type == Machine::MachineMaster) return 2; // Master
	if (m2->type == Machine::MachineGenerator or m2->type == Machine::MachineInput) return 2; // Generators and inputs

	Volume *volume = new Volume();
	m1->connectionDst[m2] = volume;
	m2->connectionSrc[m1] = volume;

	orderMachines();
	return 0;
}
