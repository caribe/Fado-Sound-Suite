#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QSettings>
#include <QDebug>

//! MainWindow class
class MainWindow;

#include "core/core.h"

#include "gui/route/route.h"
#include "gui/track/track.h"
#include "gui/pattern/pattern.h"
#include "gui/playback/playback.h"

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

		void refreshPatterns();
		
		QTabWidget *tabs;
		QStatusBar *status;
		void closeEvent(QCloseEvent *event);

		void critical(const QString & s);

	private:
		void settingsSave();
		void settingsLoad();

	private slots:
		void setTabByAction();
		void messageCritical(const QString &title, const QString &text);
		void tabChanged(int index);
		void refreshMachines();

		void menuFileCloseSlot();
		void menuFileNewSlot();
		void menuFileOpenSlot();
		void menuFileSaveSlot();
		void menuFileSaveAsSlot();
		void menuHelpAboutSlot();
		void menuHelpUpdatesSlot();
};

#endif
