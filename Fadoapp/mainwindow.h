#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

//! MainWindow class
class MainWindow;

#include "core/core.h"

#include "gui/route.h"
#include "gui/track.h"
#include "gui/pattern.h"
#include "gui/playback.h"
#include "gui/about.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		MainWindow();
		Core *core;

		Route *route;
		Track *track;
		Pattern *pattern;
		Playback *playback;

		Store *getStore();
		void optimizeMachines();
		void refreshMachines();
		void refreshPatterns();
		
		QTabWidget *tabs;
		QStatusBar *status;
		void keyPressEvent(QKeyEvent *event);
		void closeEvent(QCloseEvent *event);

		void critical(const QString & s);

	private slots:
		void menuFileCloseSlot();
		void menuFileNewSlot();
		void menuFileOpenSlot();
		void menuFileSaveSlot();
		void menuFileSaveAsSlot();
		void menuHelpAboutSlot();
		void menuHelpUpdatesSlot();
		void menuDebugDumpSlot();
};

#endif
