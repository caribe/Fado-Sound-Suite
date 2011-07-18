#ifndef GUI_TRACKS_H
#define GUI_TRACKS_H

#include <QSplitter>
#include <QListWidget>
#include <QGroupBox>
#include <QTableView>
#include <QDebug>

class Tracks;

#include "gears/master.h"
#include "tracksmodel.h"

class Tracks : public QSplitter
{
	Q_OBJECT

	public:
		Tracks(QWidget *parent, Core *core);
		
		Core *core;
		QListWidget *hintList;
		QTableView *tracksTable;
		TracksModel *tracksModel;

		Machine *currentMachine();
		void refreshMachines();

	private:
		void selectNext();

	public slots:
		void refreshPatterns();
		void addButtonSlot();
		void delButtonSlot();
		void frsButtonSlot();
		void lstButtonSlot();
		void deleteButtonSlot();
		void muteButtonSlot();
		void breakButtonSlot();

	private slots:
		void itemDoubleClickedSlot(QListWidgetItem *item);
};

#endif
