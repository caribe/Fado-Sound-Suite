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

#include "mainwindow.h"
#include "core/core.h"

MainWindow::MainWindow() : QMainWindow() {

	setWindowTitle(tr("Fado"));
	setWindowIcon(QIcon(":fado-logo"));

	QSettings settings;
	settingsLoad();

	core = new Core();
	connect(core, SIGNAL(messageCritical(QString,QString)), SLOT(messageCritical(QString,QString)));
	core->loadPlugins();
	core->init();

	// *** tabs ***

	tabs = new QTabWidget(this);
	tabs->setTabPosition(QTabWidget::West);

	route = new Route(this, core);
	pattern  = new Pattern(this, core);
	track = new Tracks(this, core);
	analyze = new Analyze(this, core);

	tabs->addTab(route,    QIcon(":routes"), tr("Machines"));
	tabs->addTab(pattern,  QIcon(":patterns"), tr("Patterns"));
	tabs->addTab(track,    QIcon(":sequences"), tr("Sequences"));
	tabs->addTab(analyze, QIcon(":icons/system-monitor.png"), tr("Analyze"));

	setCentralWidget(tabs);

	status = statusBar();

	connect(route, SIGNAL(signalDisplayStatus(QString)), status, SLOT(showMessage(QString)));
	connect(route, SIGNAL(signalClearStatus()), status, SLOT(clearMessage()));
	connect(route, SIGNAL(signalDisplayPatterns(Machine*)), SLOT(slotDisplayPatterns(Machine*)));

	connect(tabs, SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));

	connect(route, SIGNAL(machinesChanged()), SLOT(refreshMachines()));

	// *** Actions ***

	QAction *actionTabRoute = new QAction(QIcon(":routes"), tr("Machines"), this);
	actionTabRoute->setShortcut(Qt::Key_F1);
	actionTabRoute->setObjectName("actionTabRoute");
	connect(actionTabRoute, SIGNAL(triggered()), SLOT(setTabByAction()));

	QAction *actionTabPattern = new QAction(QIcon(":patterns"), tr("Patterns"), this);
	actionTabPattern->setShortcut(Qt::Key_F2);
	actionTabPattern->setObjectName("actionTabPattern");
	connect(actionTabPattern, SIGNAL(triggered()), SLOT(setTabByAction()));

	QAction *actionTabTrack = new QAction(QIcon(":sequences"), tr("Sequencer"), this);
	actionTabTrack->setShortcut(Qt::Key_F3);
	actionTabTrack->setObjectName("actionTabTrack");
	connect(actionTabTrack, SIGNAL(triggered()), SLOT(setTabByAction()));

	QAction *actionTabAnalyze = new QAction(QIcon(":icons/system-monitor.png"), tr("Analyze"), this);
	actionTabAnalyze->setShortcut(Qt::Key_F4);
	actionTabAnalyze->setObjectName("actionTabAnalyze");
	connect(actionTabAnalyze, SIGNAL(triggered()), SLOT(setTabByAction()));

	// Toolbars

	toolbarPatterns = new QToolBar(tr("Patterns Tools"));
	QAction *patternsAdd = toolbarPatterns->addAction(QIcon(":pattern-add"), tr("Create machine pattern"), pattern, SLOT(addPattern()));
	QAction *patternsDel = toolbarPatterns->addAction(QIcon(":pattern-del"), tr("Delete machine pattern"), pattern, SLOT(delPattern()));
	QAction *patternsRen = toolbarPatterns->addAction(QIcon(":pattern-ren"), tr("Rename machine pattern"), pattern, SLOT(renPattern()));

	toolbarTracks = new QToolBar(tr("Sequences Tools"));
	QAction *sequencesClear = toolbarTracks->addAction(QIcon(":/icons/broom.png"), tr("Clear sequence cell"), track, SLOT(deleteButtonSlot()));
	// QAction *sequencesMute = toolbarTracks->addAction(QIcon(":pattern-mute"), tr("Set mute cell"), track, SLOT(muteButtonSlot()));
	// QAction *sequencesBreak = toolbarTracks->addAction(QIcon(":pattern-break"), tr("Set break cell"), track, SLOT(breakButtonSlot()));
	QAction *sequencesInsert = toolbarTracks->addAction(QIcon(":/icons/table-insert-row.png"), tr("Add Row to sequence"), track, SLOT(addButtonSlot()));
	QAction *sequencesRemove = toolbarTracks->addAction(QIcon(":/icons/table-delete-row.png"), tr("Del row from sequence"), track, SLOT(delButtonSlot()));
	QAction *sequencesFirst = toolbarTracks->addAction(QIcon(":/icons/control-stop.png"), tr("Set as first sequence row"), track, SLOT(frsButtonSlot()));
	QAction *sequencesLast = toolbarTracks->addAction(QIcon(":/icons/control-stop-180.png"), tr("Set ad last sequence row"), track, SLOT(lstButtonSlot()));

	toolbarPlayback = new QToolBar(tr("Playback Tools"));
	QAction *playbackPlay = toolbarPlayback->addAction(QIcon(":/icons/control.png"), tr("Play"), this, SLOT(playbackPlaySlot()));
	QAction *playbackRec = toolbarPlayback->addAction(QIcon(":/icons/control-record.png"), tr("Record"), this, SLOT(playbackRecSlot()));
	QAction *playbackStop = toolbarPlayback->addAction(QIcon(":/icons/control-stop-square.png"), tr("Stop"), this, SLOT(playbackStopSlot()));

	baseBpm = new QSpinBox();
	baseBpm->setMinimum(16);
	baseBpm->setMaximum(500);
	baseBpm->setToolTip(tr("Base Beat per Minute (BPM)"));
	baseBpm->setValue(126);
	connect(baseBpm, SIGNAL(valueChanged(int)), core, SLOT(baseBpmChanged(int)));
	toolbarPlayback->addWidget(baseBpm);

	toolbarAnalyze = new QToolBar(tr("Analyze Tools"));
	QAction *analyzeView = toolbarAnalyze->addAction(QIcon(":/icons/system-monitor.png"), tr("View"), this, SLOT(analyzeSlot()));

	addToolBar(toolbarPatterns);
	addToolBar(toolbarTracks);
	addToolBar(toolbarPlayback);
	addToolBar(toolbarAnalyze);

	// *** menu ***

	QMenuBar *menu = menuBar();

	QMenu *menuFile = menu->addMenu("&File");

	QAction *menuFileNew = new QAction(QIcon(":/icons/document.png"), tr("&New"), this);
	menuFileNew->setShortcut(QKeySequence::New);
	connect(menuFileNew, SIGNAL(triggered()), this, SLOT(menuFileNewSlot()));
	menuFile->addAction(menuFileNew);

	QAction *menuFileOpen = new QAction(QIcon(":/icons/folder-open-document.png"), tr("&Open"), this);
	menuFileOpen->setShortcut(QKeySequence::Open);
	connect(menuFileOpen, SIGNAL(triggered()), this, SLOT(menuFileOpenSlot()));
	menuFile->addAction(menuFileOpen);

	menuFileRecent = new QMenu(tr("&Recent"), this);
	rebuildRecent();

	menuFile->addMenu(menuFileRecent);

	menuFile->addSeparator();

	QAction *menuFileSave = new QAction(QIcon(":save"), tr("&Save"), this);
	menuFileSave->setShortcut(QKeySequence::Save);
	connect(menuFileSave, SIGNAL(triggered()), this, SLOT(menuFileSaveSlot()));
	menuFile->addAction(menuFileSave);

	QAction *menuFileSaveAs = new QAction(QIcon(":save-as"), tr("Save &as..."), this);
	menuFileSaveAs->setShortcut(QKeySequence::SaveAs);
	connect(menuFileSaveAs, SIGNAL(triggered()), this, SLOT(menuFileSaveAsSlot()));
	menuFile->addAction(menuFileSaveAs);

	menuFile->addSeparator();

	QAction *menuFileQuit = new QAction(QIcon(":quit"), tr("&Quit"), this);
	menuFileQuit->setShortcut(QKeySequence::Quit);
	connect(menuFileQuit, SIGNAL(triggered()), this, SLOT(close()));
	menuFile->addAction(menuFileQuit);

	QMenu *tabs = menu->addMenu(tr("&Tabs"));
	tabs->addAction(actionTabRoute);
	tabs->addAction(actionTabPattern);
	tabs->addAction(actionTabTrack);
	tabs->addAction(actionTabAnalyze);

	patternsMenu = menu->addMenu(tr("&Patterns"));
	patternsMenu->addAction(patternsAdd);
	patternsMenu->addAction(patternsDel);
	patternsMenu->addAction(patternsRen);

	sequencesMenu = menu->addMenu(tr("&Sequences"));
	sequencesMenu->addAction(sequencesClear);
	// sequencesMenu->addAction(sequencesMute);
	// sequencesMenu->addAction(sequencesBreak);
	sequencesMenu->addAction(sequencesInsert);
	sequencesMenu->addAction(sequencesRemove);
	sequencesMenu->addAction(sequencesFirst);
	sequencesMenu->addAction(sequencesLast);

	playbackMenu = menu->addMenu(tr("Pl&ayback"));
	playbackMenu->addAction(playbackPlay);
	playbackMenu->addAction(playbackRec);
	playbackMenu->addAction(playbackStop);

	analyzeMenu = menu->addMenu(tr("Anal&yze"));
	analyzeMenu->addAction(analyzeView);

	QMenu *settingsMenu = menu->addMenu(tr("Set&tings"));
	settingsMenu->addAction(QIcon(":/icons/wrench.png"), tr("Configure Fado..."), this, SLOT(menuSettingsConfigureSlot()));

	QMenu *menuHelp = menu->addMenu(tr("&Help"));

	QAction *menuHelpUpdates = new QAction(QIcon(":update"), tr("Check for updates"), this);
	connect(menuHelpUpdates, SIGNAL(triggered()), this, SLOT(menuHelpUpdatesSlot()));
	menuHelp->addAction(menuHelpUpdates);

	QAction *menuHelpWebsite = new QAction(QIcon(":icons/globe.png"), tr("Project website"), this);
	connect(menuHelpWebsite, SIGNAL(triggered()), this, SLOT(menuHelpWebsiteSlot()));
	menuHelp->addAction(menuHelpWebsite);

	QAction *menuHelpAbout = new QAction(QIcon(":help"), tr("About Fado"), this);
	connect(menuHelpAbout, SIGNAL(triggered()), this, SLOT(menuHelpAboutSlot()));
	menuHelp->addAction(menuHelpAbout);

	// Resetting everything
	menuFileNewSlot();
	tabChanged(0);
}


void MainWindow::menuFileCloseSlot() {

	foreach (Machine *m, core->machines) route->delMachine(m, false);
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
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Project"), settings.value("state/cwd").toString(), tr("Fado project files (*.fado)"));
	if (filename.isNull()) return;
	fileOpen(filename);
}


void MainWindow::menuFileOpenRecentSlot() {
	QAction *item = qobject_cast<QAction *>(QObject::sender());
	fileOpen(item->text());
}


void MainWindow::fileOpen(QString filename)
{
	QSettings settings;
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

		baseBpm->setValue(core->baseBpm);

		QStringList recentList = settings.value("state/recent").toStringList();
		recentList.prepend(filename);
		recentList.removeDuplicates();
		settings.setValue("state/recent", recentList);
		rebuildRecent();

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
		setWindowTitle(tr("%1 - Fado").arg(filename));
	}
}



void MainWindow::menuHelpAboutSlot()
{
	QMessageBox::about(this, tr("Fado"), tr("Synthetic music generator and sound processor"));
}


void MainWindow::menuHelpUpdatesSlot()
{
	Updater *updater = new Updater(core, this);
	if (updater->exec() == QDialog::Accepted) core->loadPlugins();
	delete updater;
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
	} else if (sender == "actionTabAnalyze") {
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
	switch (index) {
	case 0:
		toolbarPatterns->setEnabled(false);
		toolbarTracks->setEnabled(false);
		patternsMenu->setEnabled(false);
		sequencesMenu->setEnabled(false);
		break;
	case 1:
		toolbarPatterns->setEnabled(true);
		toolbarTracks->setEnabled(false);
		patternsMenu->setEnabled(true);
		sequencesMenu->setEnabled(false);
		break;
	case 2:
		toolbarPatterns->setEnabled(false);
		toolbarTracks->setEnabled(true);
		patternsMenu->setEnabled(false);
		sequencesMenu->setEnabled(true);
		break;
	case 3:
		toolbarPatterns->setEnabled(false);
		toolbarTracks->setEnabled(false);
		patternsMenu->setEnabled(false);
		sequencesMenu->setEnabled(false);
		break;
	}
}


void MainWindow::slotDisplayPatterns(Machine *m)
{
	pattern->displayPatterns(m);
	tabs->setCurrentIndex(1);
}


void MainWindow::playbackPlaySlot()
{
	core->start(false);
}



void MainWindow::playbackRecSlot()
{
	core->start(true);
}



void MainWindow::playbackStopSlot()
{
	core->stop();
	if (core->record) {
		QSettings settings;
		QString filename = settings.value("settings/tempFolder").toString()+"/fado.raw";
		QString savename = QFileDialog::getSaveFileName(this, tr("Save recorded file as..."), QString(), tr("Ogg Vorbis File (*.ogg)"));
		if (!savename.isNull()) {
			int res = Encoder::encode(filename, savename, 48000, 2, 0);
			if (res != 0) QMessageBox::critical(this, tr("Error"), tr("Error while saving audio file (%1)").arg(res));
		}
	}
}


void MainWindow::menuSettingsConfigureSlot()
{
	QDialog *settingsDialog = new Settings();
	if (settingsDialog->exec() == QDialog::Accepted) core->loadPlugins();
	delete settingsDialog;
}


void MainWindow::check()
{
	QSettings settings;

	QString tempFolder = settings.value("settings/tempFolder").toString();
	while (tempFolder.isEmpty()) {
		tempFolder = QFileDialog::getExistingDirectory(this, tr("Choose temporary file folder"), "/tmp");
		settings.setValue("settings/tempFolder", tempFolder);
	}

	QString pluginsFolder = settings.value("settings/pluginsFolder").toString();
	while (pluginsFolder.isEmpty()) {
		pluginsFolder = QFileDialog::getExistingDirectory(this, tr("Choose plugins folder"));
		settings.setValue("settings/pluginsFolder", pluginsFolder);
	}

}


void MainWindow::analyzeSlot()
{
	tabs->setCurrentIndex(3);
	analyze->buttonView();
}


void MainWindow::rebuildRecent()
{
	QSettings settings;
	menuFileRecent->clear();

	QStringList recentList = settings.value("state/recent").toStringList();

	if (recentList.length()) {
		foreach (QString recentItem, recentList) {
			menuFileRecent->addAction(recentItem, this, SLOT(menuFileOpenRecentSlot()));
		}
	} else {
		QAction *none = new QAction(tr("There are no recent files"), menuFileRecent);
		none->setDisabled(true);
		menuFileRecent->addAction(none);
	}
}


void MainWindow::menuHelpWebsiteSlot()
{
	QDesktopServices::openUrl(QUrl("http://saitfainder.altervista.org/fado/"));
}
