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

#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QSettings>
#include <QToolBar>
#include <QDebug>

//! MainWindow class
class MainWindow;

#include "core/core.h"
#include "core/encoder.h"

#include "gui/route/route.h"
#include "gui/track/tracks.h"
#include "gui/pattern/pattern.h"
#include "gui/analyze/analyze.h"
#include "gui/settings.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow();
		Core *core;

		Route *route;
		Tracks *track;
		Pattern *pattern;
		Analyze *analyze;

		void refreshPatterns();

		QTabWidget *tabs;
		QStatusBar *status;
		void closeEvent(QCloseEvent *event);

		void critical(const QString & s);

	private:
		void settingsSave();
		void settingsLoad();

		QToolBar *toolbarPatterns, *toolbarTracks, *toolbarPlayback, *toolbarAnalyze;
		QMenu *patternsMenu, *sequencesMenu, *playbackMenu, *analyzeMenu;

	private slots:
		void setTabByAction();
		void messageCritical(const QString &title, const QString &text);
		void tabChanged(int index);
		void refreshMachines();
		void slotDisplayPatterns(Machine *m);

		void menuFileCloseSlot();
		void menuFileNewSlot();
		void menuFileOpenSlot();
		void menuFileSaveSlot();
		void menuFileSaveAsSlot();
		void menuHelpAboutSlot();
		void menuHelpUpdatesSlot();

		void playbackPlaySlot();
		void playbackRecSlot();
		void playbackStopSlot();

		void menuSettingsConfigureSlot();
};

#endif
