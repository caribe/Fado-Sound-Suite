#include "mainwindow.h"
#include "core/core.h"

MainWindow::MainWindow() : QMainWindow() {

	setWindowTitle(tr("Fado"));
	setWindowIcon(QIcon(":/001_08.png"));

	settingsLoad();

	core = new Core();
	connect(core, SIGNAL(messageCritical(QString,QString)), SLOT(messageCritical(QString,QString)));
	core->loadPlugins();
	// core->jack_init();

	// *** tabs ***

	tabs = new QTabWidget(this);
	tabs->setTabPosition(QTabWidget::West);

	route = new Route(core);
	pattern  = new Pattern(core);
	track = new Track(core);
	playback = new Playback(core);

	tabs->addTab(route,    QIcon(":/machines"), tr("Machines"));
	tabs->addTab(pattern,  QIcon(":/patterns"), tr("Patterns"));
	tabs->addTab(track,    QIcon(":/sequencer"), tr("Sequencer"));
	tabs->addTab(playback, QIcon(":/playback"), tr("Playback"));

	setCentralWidget(tabs);

	status = statusBar();

	// *** Actions ***

	QAction *actionTabRoute = new QAction(QIcon(":/machines"), tr("Machines"), this);
	actionTabRoute->setShortcut(Qt::Key_F1);
	actionTabRoute->setObjectName("actionTabRoute");
	connect(actionTabRoute, SIGNAL(triggered()), SLOT(setTabByAction()));

	QAction *actionTabPattern = new QAction(QIcon(":/patterns"), tr("Patterns"), this);
	actionTabPattern->setShortcut(Qt::Key_F2);
	actionTabPattern->setObjectName("actionTabPattern");
	connect(actionTabPattern, SIGNAL(triggered()), SLOT(setTabByAction()));

	QAction *actionTabTrack = new QAction(QIcon(":/sequencer"), tr("Sequencer"), this);
	actionTabTrack->setShortcut(Qt::Key_F3);
	actionTabTrack->setObjectName("actionTabTrack");
	connect(actionTabTrack, SIGNAL(triggered()), SLOT(setTabByAction()));

	QAction *actionTabPlayback = new QAction(QIcon(":/playback"), tr("Playback"), this);
	actionTabPlayback->setShortcut(Qt::Key_F4);
	actionTabPlayback->setObjectName("actionTabPlayback");
	connect(actionTabPlayback, SIGNAL(triggered()), SLOT(setTabByAction()));

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

	QMenu *tabs = menu->addMenu("&Tabs");
	tabs->addAction(actionTabRoute);
	tabs->addAction(actionTabPattern);
	tabs->addAction(actionTabTrack);
	tabs->addAction(actionTabPlayback);

	menu->addMenu("&Settings");

	QMenu *menuHelp = menu->addMenu("&Help");

	QAction *menuHelpUpdates = new QAction(QIcon(":/001_10.png"), "Check for updates", this);
	connect(menuHelpUpdates, SIGNAL(triggered()), this, SLOT(menuHelpUpdatesSlot()));
	menuHelp->addAction(menuHelpUpdates);

	QAction *menuHelpAbout = new QAction(QIcon(":/001_10.png"), "About Fado", this);
	connect(menuHelpAbout, SIGNAL(triggered()), this, SLOT(menuHelpAboutSlot()));
	menuHelp->addAction(menuHelpAbout);

	// Resetting everything
	menuFileNewSlot();
}


void MainWindow::menuFileCloseSlot() {

	foreach (Machine *m, core->machines) route->delMachine(m);
	core->machines.clear();

}

void MainWindow::menuFileNewSlot() {

	menuFileCloseSlot();

	core->total_patterns = 20;
	core->beat_per_pattern = 16;

	Master *tx = new Master();
	tx->x = 100;
	tx->y = 100;
	tx->track_first = 0;
	tx->track_last = core->total_patterns - 1;

	core->machines.append(tx);

	route->addMachine((Machine *)tx);
	core->orderMachines();
	// refreshMachines();

	setWindowTitle(tr("Fado"));

}


void MainWindow::menuFileOpenSlot() {
	/*
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
	*/
}



void MainWindow::menuFileSaveSlot()
{
	core->save();
}



void MainWindow::menuFileSaveAsSlot() {
	QString filename = QFileDialog::getSaveFileName(this, "Save Project", "", "Fado project files (*.xml)");

	if (filename == "") return;

	if (core->save(filename) == 0) {
		setWindowTitle("Fado - "+filename);
	}
}



void MainWindow::menuHelpAboutSlot()
{
	QMessageBox::about(this, "Fado", "Synthetic music generator and sound processor");
}


void MainWindow::menuHelpUpdatesSlot()
{
	core->checkUpdates();
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


void MainWindow::setTabByAction()
{
	QString sender = QObject::sender()->objectName();

	if (sender == "actionTabRoute") {
		tabs->setCurrentIndex(0);
	} else if (sender == "actionTabPattern") {
		tabs->setCurrentIndex(1);
	} else if (sender == "actionTabTrack") {
		tabs->setCurrentIndex(2);
	} else if (sender == "actionTabPlayback") {
		tabs->setCurrentIndex(3);
	}
}


void MainWindow::closeEvent(QCloseEvent *event)
{
	this->settingsSave();
}


void MainWindow::critical(const QString & s)
{
	QMessageBox::critical(this, tr("Errore"), s);
}


void MainWindow::settingsSave()
{
	QSettings settings;

	if (isMaximized()) {
		settings.setValue("settings/maximized", true);
	} else {
		settings.setValue("settings/maximized", false);
		settings.setValue("settings/left", x());
		settings.setValue("settings/top", y());
		settings.setValue("settings/width", width());
		settings.setValue("settings/height", height());
	}
}


void MainWindow::settingsLoad()
{
	QSettings settings;

	if (settings.value("settings/maximized", false).toBool()) {
		this->showMaximized();
	} else {
		move(settings.value("settings/left", 100).toInt(), settings.value("settings/top", 100).toInt());
		resize(settings.value("settings/width", 800).toInt(), settings.value("settings/height", 600).toInt());
	}
}



void MainWindow::messageCritical(const QString &title, const QString &text) {
	QMessageBox::critical(this, title, text);
}
