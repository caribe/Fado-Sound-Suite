#include "core/core.h"

Core::Core(QObject *parent) : QObject(parent)
{
	master = 0;
	pluginsPath = "../Fadogears/";

	beat_per_pattern = 16;
	total_patterns = 16;
	gearsTree = new QStandardItemModel();


	loadPlugins();

	// jack_init();
}



void Core::loadPlugins()
{
	QStandardItem *item;

	// Standard gears

	item = new QStandardItem(QIcon(":/machine"), "Line Input");
	item->setEditable(false);
	item->data() = -1;
	gearsTree->appendRow(item);

	item = new QStandardItem(QIcon(":/machine"), "File Input");
	item->setEditable(false);
	item->data() = -2;
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
				gears.append(lib);

				Machine *machine = qobject_cast<Machine *>(lib->instance());

				qDebug() << "Loaded" << machine->name;

				item = new QStandardItem(QIcon(":/machine"), machine->name);
				item->setEditable(false);
				item->data() = id;

				gearsTree->appendRow(item);

				// delete machine;

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
		// emit messageCritical("Jack server not running", "Error " + QString::number(jack_status));
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
	errstr = "";
	this->filename = filename;
	qDebug() << "Loading config... (" << filename << ")" << endl;

	int config = 0;
	config = Config::load(filename, this);

	if (config != 0) {
		errstr = "Cannot load config (" + filename + ")\nERROR CODE " + config;
		return config;
	}

	switch (orderMachines()) {
		case 1:
			errstr = "Cannot establish this connection because it would create a loop...";
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
	errstr = "";
	master = (Master *)machines.value(0);
	if (master) {
		if (master->init(this) == 0) {
			master->go(client, input_port, output_port, record);
		}
		return 0;
	} else {
		errstr = "No master?";
		return 1;
	}
}


int Core::stop()
{
	errstr = "";
	master->stop();
	Encoder::encode("/tmp/record.raw", "./record.ogg", 48000, 2, 0);
	return 0;
}



int Core::optimizeMachines()
{
	QHash<int, int> changes;
	optimizeMachines(changes);

	/*
		route->machines[i] = route->machines[order[i]];
		route->machines.remove(order[i]);
*/

	orderMachines();

	// mainWindow->refreshMachines();
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



int Core::optimizeMachines(QHash<int, int> &changes)
{
	/*
	int max = 0;
	int i;

	foreach (i, machines.values()) if (i > max) max = i;
	QList<int> order;

	for (i = 0; i <= max; i++) if (machines.contains(i)) order.append(i);

	for (i = 0; i < order.size(); i++) if (order[i] != i) {
		machines[i] = machines[order[i]];
		machines.remove(order[i]);
		machines[i]->id = i;

		changes[order[i]] = i;

		foreach (int j, connections.keys()) {
			if (j == order[i]) {
				connections[i] = connections[j];
				connections.remove(j);
			} else {
				foreach (int k, connections[j].keys()) if (k == order[i]) {
					connections[i][k] = connections[j][k];
					connections[j].remove(k);
				}
			}
		}
	}
*/
	return 0;
}


int Core::orderMachines()
{
	/*
	QHash<int, QList<int> > oconn;
	QHash<int, QList<int> > iconn;
	QList<int> order;

	// oconn and iconn keys must exists
	foreach (int i, machines.keys()) {
		if (oconn.contains(i) == false) oconn[i].clear();
		if (iconn.contains(i) == false) iconn[i].clear();
	}

	// oconn e iconn initialization
	foreach (int i, connections.keys()) {
		foreach (int j, connections[i].keys()) {
			oconn[i] << j;
			iconn[j] << i;
		}
	}

	this->iconn = iconn;
	this->oconn = oconn;

	int watchdog = machines.keys().length();

	// *** Ordening machines to elaboration

	while (iconn.size() > 0 and --watchdog >= 0) { // While there are machines to check
		QList<int> iconnKeys = iconn.keys();
		for (int i = 0; i < iconnKeys.length(); i++) { // Cycle on in-connections hash
			if (iconn[iconnKeys[i]].length() == 0) { // If there are unmanaged in-connections
				QList<int> oconnValues = oconn.value(iconnKeys[i]);
				for (int j = 0; j < oconnValues.length(); j++) { // Get out-connections of this machine
					QList<int> t;
					QList<int> iconnValues = iconn.value(oconnValues[j]);
					for (int k = 0; k < iconnValues.length(); k++) { // Cycle on in-connections of child machines
						if (iconnValues[k] != iconnKeys[i]) { // Recreates the set without the machine
							t.append(iconnValues[k]);
						}
					}
					iconn[oconnValues[j]] = t;
				}

				order << iconnKeys[i];
				iconn.remove(iconnKeys[i]);
				oconn.remove(iconnKeys[i]);
			}
		}
	}

	// Loop
	if (watchdog < 0) return 1;

	this->order = order;
*/
	return 0;
}



int Core::flipConnection(int from, int to)
{
	/*
	if (connections.contains(from) and connections[from].contains(to)) {
		connections[from].remove(to);
		if (connections[from].isEmpty()) connections.remove(from);
		orderMachines();
		return 1;
	} else if (connections.contains(to) and connections[to].contains(from)) {
		connections[to].remove(from);
		if (connections[to].isEmpty()) connections.remove(to);
		orderMachines();
		return 1;
	} else {
		Machine *f = machines[from];
		Machine *t = machines[to];

		if (f->author == "Core" and f->name == "output") return 2;
		if (t->type == "generator") return 2;

		connections[from][to] = 100;
		orderMachines();
		return 0;
	}
	*/
}
