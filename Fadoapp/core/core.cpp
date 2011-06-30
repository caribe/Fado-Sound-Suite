#include "core/core.h"

Core::Core(QObject *parent) : QObject(parent)
{
	master = 0;
	pluginsPath = "../Fadogears/";

	beat_per_pattern = 16;
	total_patterns = 16;
	gearsTree = new QStandardItemModel();
}



void Core::loadPlugins()
{
	QStandardItem *item;

	// Standard gears

	item = new QStandardItem(QIcon(":/machine"), "Line Input");
	item->setEditable(false);
	item->setData(-2);
	gearsTree->appendRow(item);

	item = new QStandardItem(QIcon(":/machine"), "File Input");
	item->setEditable(false);
	item->setData(-3);
	gearsTree->appendRow(item);

	// Plugins

	QDir libs(pluginsPath);
	QStringList dirList = libs.entryList();

	qDebug() << "Loading machines...";

	foreach (QString name, dirList) {

		QFileInfo info(pluginsPath + name);

		if (info.isDir() && name != "." && name != "..") {

			qDebug() << "Trying " << pluginsPath + name + "/" + name + ".so";

			QPluginLoader *lib = new QPluginLoader(pluginsPath + name + "/lib" + name + ".so");

			if (lib->load()) {
				int id = gears.length();

				Machine *machine = qobject_cast<Machine *>(lib->instance());
				gears.append(machine);

				qDebug() << "Loaded" << machine->name << id;

				item = new QStandardItem(QIcon(":/machine"), machine->name);
				item->setEditable(false);
				item->setData(id);
				gearsTree->appendRow(item);

			} else {
				qDebug() << "Error" << lib->errorString();
			}
		}
	}
}


int Core::jack_init()
{
		qDebug() << "Starting Jack..." << endl;

	// try to become a client of the JACK server
	jack_status_t jack_status;
	if ((client = jack_client_open("fado", JackUseExactName, &jack_status, "default")) == 0) {
		emit messageCritical(tr("Jack server not running"), "Error " + QString::number(jack_status));
		return 1;
	}

		qDebug() << "Define callback..." << endl;

	// tell the JACK server to call `process()' whenever there is work to be done.
	jack_set_process_callback(client, ::jack_process, (void *)this);

		qDebug() << "Define shutdown..." << endl;

	// tell the JACK server to call `jack_shutdown()' if it ever shuts down, either entirely, or if it just decides to stop calling us.
	jack_on_shutdown(client, ::jack_shutdown, (void *)this);

		qDebug() << "Get sample rate..." << endl;

	// display the current sample rate. 
	sampling_rate = jack_get_sample_rate(client);
	buffer_size = jack_get_buffer_size(client);

	qDebug() << "Engine sample rate: " << jack_get_sample_rate(client) << endl;

	// create four ports
	input_port[0] = jack_port_register(client, "input_lx", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	input_port[1] = jack_port_register(client, "input_rx", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

	output_port[0] = jack_port_register(client, "output_lx", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	output_port[1] = jack_port_register(client, "output_rx", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

	return 0;
}


int Core::load(QString filename)
{
	this->filename = filename;
	qDebug() << "Loading config... (" << filename << ")" << endl;

	int config = 0;
	config = Config::load(filename, this);

	if (config != 0) {
		emit messageCritical(tr("Error"), "Cannot load config (" + filename + ")\nERROR CODE " + config);
		return config;
	}

	switch (orderMachines()) {
		case 1:
			emit messageCritical(tr("Error"), "Cannot establish this connection because it would create a loop...");
			return 1;
	}

	return 0;
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
	master->stop();
	Encoder::encode("/tmp/record.raw", "./record.ogg", 48000, 2, 0);
	return 0;
}



int Core::checkUpdates()
{
	if (!updates) updates = new Updates(this);
	updates->check();
}


int jack_process(jack_nframes_t nframes, void *arg)
{
	Core *core = (Core *)arg;
	if (core->master) core->master->process(nframes);
	return 0;
}


void jack_shutdown(void *arg) {}




int Core::orderMachines()
{
	QHash<Machine *, QList<Machine *> > iconns;
	QHash<Machine *, QList<Machine *> > oconns;
	QList<Machine *> order;

	// oconn e iconn initialization
	foreach (Machine *machine, machines) {
		qDebug() << machine->name;
		iconns[machine] << machine->connectionDst.keys();
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
	if (watchdog < 0) return 1;

	this->order = order;

	return 0;
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
