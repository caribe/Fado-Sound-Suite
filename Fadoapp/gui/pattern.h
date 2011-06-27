#ifndef GUI_PATTERN_H
#define GUI_PATTERN_H

#include <QtGui>

class Pattern;

#include "mainwindow.h"
#include "gui/patterntable.h"

class Pattern : public QFrame
{
	Q_OBJECT
	
	public:
		MainWindow *mainWindow;
		Store *store;
		Machine *activeMachine;
		PatternTable *tableview;
		QComboBox *machineCombo;
		QComboBox *patternCombo;
		QComboBox *octaveCombo;

		QGroupBox *detail;
		QLabel *detailUnit;
		QLabel *detailMin;
		QLabel *detailMax;
		QLabel *detailDesc;

		Pattern(MainWindow *parent);
		void refreshMachines();
		void refreshPatterns();

		QList<QString> cols;
		int currentPattern();

	private slots:
		void machineChange(int index);
		void patternChange(int index);
	
	public slots:
		void activeCol(int col);
		void addPattern();
		void delPattern();
		void renPattern();
};

#endif
