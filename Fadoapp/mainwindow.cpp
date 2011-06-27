#include "mainwindow.h"
#include "core/core.h"

MainWindow::MainWindow() : QMainWindow() {
	core = new Core(this);

	resize(800, 600);
	setWindowTitle("Fado");
	setWindowIcon(QIcon(":/001_08.png"));

	// *** tabs ***

	tabs = new QTabWidget(this);
	tabs->setTabPosition(QTabWidget::West);

	route = new Route(this);
	pattern  = new Pattern(this);
	track = new Track(this);
	playback = new Playback(this);

	tabs->addTab(route,    QIcon(":/machines.png"), tr("Machines"));
	tabs->addTab(pattern,  QIcon(":/patterns.png"), tr("Patterns"));
	tabs->addTab(track,    QIcon(":/sequencer.png"), tr("Sequencer"));
	tabs->addTab(playback, QIcon(":/playback.png"), tr("Playback"));

	setCentralWidget(tabs);

	// *** menu ***

	QMenuBar *menu = menuBar();

	QMenu *menuFile = menu->addMenu("&File");

	QAction *menuFileNew = new QAction(QIcon(":/layout_add.png"), "&New", this);
	menuFileNew->setShortcut(QKeySequence::New);
	connect(menuFileNew, SIGNAL(triggered()), this, SLOT(menuFileNewSlot()));
	menuFile->addAction(menuFileNew);

	QAction *menuFileOpen = new QAction(QIcon(":/folder.png"), "&Open", this);
	menuFileOpen->setShortcut(QKeySequence::Open);
	connect(menuFileOpen, SIGNAL(triggered()), this, SLOT(menuFileOpenSlot()));
	menuFile->addAction(menuFileOpen);

	QAction *menuFileSave = new QAction(QIcon(":/disk.png"), "&Save", this);
	menuFileSave->setShortcut(QKeySequence::Save);
	connect(menuFileSave, SIGNAL(triggered()), this, SLOT(menuFileSaveSlot()));
	menuFile->addAction(menuFileSave);

	QAction *menuFileSaveAs = new QAction(QIcon(":/disk.png"), "Save &as...", this);
	menuFileSave->setShortcut(QKeySequence::SaveAs);
	connect(menuFileSaveAs, SIGNAL(triggered()), this, SLOT(menuFileSaveAsSlot()));
	menuFile->addAction(menuFileSaveAs);

	QAction *menuFileQuit = new QAction(QIcon(":/001_05.png"), "&Quit", this);
	menuFileQuit->setShortcut(QKeySequence::Quit);
	connect(menuFileQuit, SIGNAL(triggered()), this, SLOT(close()));
	menuFile->addAction(menuFileQuit);

	menu->addMenu("&Settings");

	QMenu *debugMenu = menu->addMenu("&Debug");
	QAction *menuDebugDump = new QAction("Machine dump", this);
	connect(menuDebugDump, SIGNAL(triggered()), this, SLOT(menuDebugDumpSlot()));
	debugMenu->addAction(menuDebugDump);

	QMenu *menuHelp = menu->addMenu("&Help");

	QAction *menuHelpUpdates = new QAction(QIcon(":/001_10.png"), "Check for updates", this);
	connect(menuHelpUpdates, SIGNAL(triggered()), this, SLOT(menuHelpUpdatesSlot()));
	menuHelp->addAction(menuHelpUpdates);

	QAction *menuHelpAbout = new QAction(QIcon(":/001_10.png"), "About Fado", this);
	connect(menuHelpAbout, SIGNAL(triggered()), this, SLOT(menuHelpAboutSlot()));
	menuHelp->addAction(menuHelpAbout);

	menuFileNewSlot();
	
	status = statusBar();
}


void MainWindow::menuFileCloseSlot() {
	foreach (Machine *m, core->store->machines.values()) route->delMachine(m);
	core->store->machines.clear();
}

void MainWindow::menuFileNewSlot() {
	menuFileCloseSlot();

	Master *tx = new Master();
	tx->id = 0;
	tx->x = 100;
	tx->y = 100;
	tx->store = getStore();
	core->store->machines[tx->id] = tx;
	tx->patterns[0];
	core->store->total_patterns = 20;
	core->store->beat_per_pattern = 16;
	tx->track_first = 0;
	tx->track_last = core->store->total_patterns - 1;

	route->addMachine((Machine *)tx);
	refreshMachines();

	setWindowTitle("Fado");
}


void MainWindow::menuFileOpenSlot() {
	QString filename = QFileDialog::getOpenFileName(this, "Open Project", "", "Fado project files (*.xml)");
	if (filename == "") return;

	menuFileCloseSlot();

	if (core->load(filename) != 0) {
		QMessageBox::critical(this, "Open failed", core->errstr);
	} else {
		setWindowTitle("Fado - "+filename);
		foreach (int i, core->store->machines.keys()) route->addMachine(core->store->machines[i]);
		foreach (int i, core->store->connections.keys()) {
			foreach (int j, core->store->connections[i].keys()) {
				route->addConnection(i, j);
			}
		}
	}

	// Toglie eventuali buchi
	core->optimizeMachines();
}



void MainWindow::menuFileSaveSlot()
{
	core->save();
}



void MainWindow::menuFileSaveAsSlot() {
	QString filename = QFileDialog::getSaveFileName(this, "Save Project", "", "Fado project files (*.xml)");

	if (filename == "") return;

	if (core->save(filename) != 0) {
		QMessageBox::critical(this, "Save failed", core->errstr);
	} else {
		setWindowTitle("Fado - "+filename);
	}
}



void MainWindow::menuHelpAboutSlot()
{
	// QMessageBox::About(this, "Fado", "Synthetic music generator and sound processor\n<a href='http://saitfainder.altervista.org/fado/'>CheckqDebug() our site</a>");
	new About(this);
}


void MainWindow::menuHelpUpdatesSlot()
{
	core->checkUpdates();
}


Store *MainWindow::getStore() {
	return core->store;
}


void MainWindow::refreshMachines()
{
	pattern->refreshMachines();
	track->refreshMachines();
}


void MainWindow::refreshPatterns()
{
	pattern->refreshPatterns();
	track->refreshPatterns();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key_F2:
			tabs->setCurrentIndex(0);
			break;
		case Qt::Key_F3:
			tabs->setCurrentIndex(1);
			break;
		case Qt::Key_F4:
			tabs->setCurrentIndex(2);
			break;
		case Qt::Key_F5:
			tabs->setCurrentIndex(3);
			break;
		default:
			QMainWindow::keyPressEvent(event);
	}
}


void MainWindow::closeEvent(QCloseEvent *event)
{
	core->close();
}

void MainWindow::menuDebugDumpSlot()
{
	qDebug() << "*** Machines";
	foreach (int i, core->store->machines.keys()) {
		qDebug() << i << "\t" << core->store->machines[i]->type << "\t" << core->store->machines[i]->author << "\t" << core->store->machines[i]->name << "\t" << core->store->machines[i]->id << "\t" << core->store->machines[i]->x << "\t" << core->store->machines[i]->y << "\n";
	}

	qDebug() << "*** Connections\n";
	foreach (int i, core->store->connections.keys()) {
		foreach (int j, core->store->connections[i].keys()) {
			qDebug() << i << " => " << j << " (" << core->store->connections[i][j] << ")\n";
		}
	}

	qDebug() << "*** In Connections\n";
	foreach (int i, core->store->iconn.uniqueKeys()) {
		foreach (int j, core->store->iconn[i]) {
			qDebug() << i << " => " << j << "\n";
		}
	}

	qDebug() << "*** qDebug() Connections\n";
	foreach (int i, core->store->oconn.uniqueKeys()) {
		foreach (int j, core->store->oconn[i]) {
			qDebug() << i << " => " << j << "\n";
		}
	}

	qDebug() << "*** Order\n";
	foreach (int i, core->store->order) {
		qDebug() << i << "\n";
	}
}


void MainWindow::critical(const QString & s)
{
	
}
