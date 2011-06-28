#include "core/core.h"

Core::Core(MainWindow *mainWindow)
{
	this->mainWindow = mainWindow;
	master = 0;
	store = 0;
	pluginsPath = "../Fadogears/";

	loadPlugins();

	settings = new Settings(this);
	settings->load();

	// jack_init();
}



void Core::loadPlugins()
{
	// TODO Memory leaks!
	if (store != 0) delete store;
	store = new Store();

	QDir libs(pluginsPath);
	QStringList dirList = libs.entryList();

	qDebug() << "Loading machines...";

	// Caricamento makers per tutte le librerie
	foreach (QString name, dirList) {

		QFileInfo info(pluginsPath + name);

		if (info.isDir() && name != "." && name != "..") {

			qDebug() << "Trying " << pluginsPath + name + "/" + name + ".so";

			QPluginLoader *lib = new QPluginLoader(pluginsPath + name + "/lib" + name + ".so");

			if (lib->load()) {
				int id = store->gears.length();
				store->gears.append(lib);

				Machine *machine = qobject_cast<Machine *>(lib->instance());

				qDebug() << "Loaded" << machine->name;

				QStandardItem *item = new QStandardItem(QIcon(":/machine"), machine->name);
				item->data() = id;

// 				delete machine;

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
		mainWindow->critical("Jack server not running. Error "+QString::number(jack_status));
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
	store->sampling_rate = jack_get_sample_rate(client);
	store->buffer_size = jack_get_buffer_size(client);

	qDebug() << "Engine sample rate: " << jack_get_sample_rate(client) << endl;

	// create four ports
	input_port[0] = jack_port_register(client, "input_lx", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);
	input_port[1] = jack_port_register(client, "input_rx", JACK_DEFAULT_AUDIO_TYPE, JackPortIsInput, 0);

	output_port[0] = jack_port_register(client, "output_lx", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	output_port[1] = jack_port_register(client, "output_rx", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
}


int Core::load(QString filename)
{
	errstr = "";
	this->filename = filename;
	qDebug() << "Loading config... (" << filename << ")" << endl;

	int config = 0;
	config = Config::load(filename, store);

	if (config != 0) {
		errstr = "Cannot load config (" + filename + ")\nERROR CODE " + config;
		return config;
	}

	switch (store->orderMachines()) {
		case 1:
			errstr = "Cannot establish this connection because it would create a loop...";
			return 1;
	}

	return 0;
}


int Core::save()
{
	return Config::save(filename, store);
}


int Core::save(QString filename)
{
	this->filename = filename;
	return save();
}


int Core::start(bool record)
{
	errstr = "";
	master = (Master *)store->machines.value(0);
	if (master) {
		if (master->init(store) == 0) {
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


int Core::close()
{
	settings->save();
}


int Core::optimizeMachines()
{
	QHash<int, int> changes;
	store->optimizeMachines(changes);

	/*
		route->machines[i] = route->machines[order[i]];
		route->machines.remove(order[i]);
*/

	store->orderMachines();

	mainWindow->refreshMachines();
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
