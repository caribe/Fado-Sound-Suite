#include "mainwindow.h"
#include "core/core.h"

MainWindow::MainWindow() : QMainWindow() {

	setWindowTitle(tr("Fado"));
	setWindowIcon(QIcon(":/logo"));

	settingsLoad();

	core = new Core();
	connect(core, SIGNAL(messageCritical(QString,QString)), SLOT(messageCritical(QString,QString)));
	core->loadPlugins();
	core->jack_init();

	// *** tabs ***

	tabs = new QTabWidget(this);
	tabs->setTabPosition(QTabWidget::West);

	route = new Route(this, core);
	pattern  = new Pattern(this, core);
	track = new Tracks(this, core);
	playback = new Playback(this, core);

	tabs->addTab(route,    QIcon(":/machines"), tr("Machines"));
	tabs->addTab(pattern,  QIcon(":/patterns"), tr("Patterns"));
	tabs->addTab(track,    QIcon(":/sequencer"), tr("Sequencer"));
	tabs->addTab(playback, QIcon(":/playback"), tr("Playback"));

	setCentralWidget(tabs);

	status = statusBar();

	connect(route, SIGNAL(signalDisplayStatus(QString)), status, SLOT(showMessage(QString)));
	connect(route, SIGNAL(signalClearStatus()), status, SLOT(clearMessage()));
	connect(route, SIGNAL(signalDisplayPatterns(Machine*)), SLOT(slotDisplayPatterns(Machine*)));

	connect(tabs, SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));

	connect(route, SIGNAL(machinesChanged()), SLOT(refreshMachines()));

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

	QAction *menuFileNew = new QAction(QIcon(":/document.png"), tr("&New"), this);
	menuFileNew->setShortcut(QKeySequence::New);
	connect(menuFileNew, SIGNAL(triggered()), this, SLOT(menuFileNewSlot()));
	menuFile->addAction(menuFileNew);

	QAction *menuFileOpen = new QAction(QIcon(":/folder-open-document.png"), tr("&Open"), this);
	menuFileOpen->setShortcut(QKeySequence::Open);
	connect(menuFileOpen, SIGNAL(triggered()), this, SLOT(menuFileOpenSlot()));
	menuFile->addAction(menuFileOpen);

	QAction *menuFileSave = new QAction(QIcon(":/disk.png"), tr("&Save"), this);
	menuFileSave->setShortcut(QKeySequence::Save);
	connect(menuFileSave, SIGNAL(triggered()), this, SLOT(menuFileSaveSlot()));
	menuFile->addAction(menuFileSave);

	QAction *menuFileSaveAs = new QAction(QIcon(":/disk--pencil.png"), tr("Save &as..."), this);
	menuFileSaveAs->setShortcut(QKeySequence::SaveAs);
	connect(menuFileSaveAs, SIGNAL(triggered()), this, SLOT(menuFileSaveAsSlot()));
	menuFile->addAction(menuFileSaveAs);

	QAction *menuFileQuit = new QAction(QIcon(":/cross-button.png"), tr("&Quit"), this);
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

	QAction *menuHelpUpdates = new QAction(QIcon(":/001_10.png"), tr("Check for updates"), this);
	connect(menuHelpUpdates, SIGNAL(triggered()), this, SLOT(menuHelpUpdatesSlot()));
	menuHelp->addAction(menuHelpUpdates);

	QAction *menuHelpAbout = new QAction(QIcon(":/question.png"), tr("About Fado"), this);
	connect(menuHelpAbout, SIGNAL(triggered()), this, SLOT(menuHelpAboutSlot()));
	menuHelp->addAction(menuHelpAbout);


	// Toolbars

	QToolBar *toolbarPatterns = new QToolBar(tr("Patterns Tools"));
	toolbarPatterns->addAction(QIcon(":/table--plus.png"), tr("Add Pattern"), pattern, SLOT(addPattern()));
	toolbarPatterns->addAction(QIcon(":/table--minus.png"), tr("Delete Pattern"), pattern, SLOT(delPattern()));
	toolbarPatterns->addAction(QIcon(":/table--pencil.png"), tr("Rename Pattern"), pattern, SLOT(renPattern()));

	QToolBar *toolbarTracks = new QToolBar(tr("Sequences Tools"));
	toolbarTracks->addAction(QIcon(":/plus.png"), tr("Add Row"), track, SLOT(addButtonSlot()));
	toolbarTracks->addAction(QIcon(":/minus.png"), tr("Del Row"), track, SLOT(delButtonSlot()));
	toolbarTracks->addAction(QIcon(":/control-stop.png"), tr("First Row"), track, SLOT(frsButtonSlot()));
	toolbarTracks->addAction(QIcon(":/control-stop-180.png"), tr("Last Row"), track, SLOT(lstButtonSlot()));
	toolbarTracks->addAction(QIcon(":/cross.png"), tr("Delete"), track, SLOT(deleteButtonSlot()));
	toolbarTracks->addAction(QIcon(":/slash.png"), tr("Mute"), track, SLOT(muteButtonSlot()));
	toolbarTracks->addAction(QIcon(":/control-stop-square.png"), tr("Break"), track, SLOT(breakButtonSlot()));

	QToolBar *toolbarPlayback = new QToolBar(tr("Playback Tools"));
	toolbarPlayback->addAction(QIcon(":/control.png"), tr("Play"), playback, SLOT(buttonPlay()));
	toolbarPlayback->addAction(QIcon(":/control-record.png"), tr("Record"), playback, SLOT(buttonRec()));
	toolbarPlayback->addAction(QIcon(":/control-stop-square.png"), tr("Stop"), playback, SLOT(buttonStop()));
	toolbarPlayback->addAction(QIcon(":/system-monitor.png"), tr("View"), playback, SLOT(buttonView()));

	addToolBar(toolbarPatterns);
	addToolBar(toolbarTracks);
	addToolBar(toolbarPlayback);

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

	core->machines.append(tx);

	route->addMachine((Machine *)tx);
	core->orderMachines();
	// refreshMachines();

	setWindowTitle(tr("Fado"));

}


void MainWindow::menuFileOpenSlot() {
	QSettings settings;
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Project"), settings.value("state/cwd", "").toString(), tr("Fado project files (*.fado)"));
	if (filename.isNull()) return;

	menuFileCloseSlot();
	if (core->load(filename)) {
		setWindowTitle(tr("%1 - Fado").arg(filename));
		foreach (Machine *machine, core->machines) route->addMachine(machine);
		foreach (Machine *m1, core->machines) {
			foreach (Machine *m2, m1->connectionDst.keys()) {
				route->addConnection(m1, m2);
			}
		}
		pattern->refreshMachines();
		track->refreshMachines();
	}
}



void MainWindow::menuFileSaveSlot()
{
	core->save();
}



void MainWindow::menuFileSaveAsSlot() {
	QSettings settings;
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Project"), settings.value("state/cwd", "").toString(), tr("Fado project files (*.fado)"));
	if (filename.isNull()) return;

	if (core->save(filename) == 0) {
		setWindowTitle("Fado - "+filename);
	}
}



void MainWindow::menuHelpAboutSlot()
{
	QMessageBox::about(this, tr("Fado"), tr("Synthetic music generator and sound processor"));
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
	event->accept();
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



void MainWindow::tabChanged(int index)
{
	if (index == 1) {
		pattern->refreshPatterns();
	}
}


void MainWindow::slotDisplayPatterns(Machine *m)
{
	pattern->displayPatterns(m);
	tabs->setCurrentIndex(1);
}
