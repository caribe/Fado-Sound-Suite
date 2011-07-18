#ifndef GUI_PATTERN_H
#define GUI_PATTERN_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QInputDialog>
#include <QLineEdit>

class Pattern;

#include "core/core.h"
#include "patterntable.h"

class Pattern : public QWidget
{
	Q_OBJECT
	
	public:
		Pattern(QWidget *widget, Core *core);

		Core *core;
		PatternTable *tableview;
		QComboBox *machineCombo;
		QComboBox *patternCombo;
		QComboBox *octaveCombo;

		QGroupBox *detail;
		QLabel *detailUnit;
		QLabel *detailMin;
		QLabel *detailMax;
		QLabel *detailDesc;

		void refresh();
		void refreshPatterns();

		Machine *currentMachine();
		MachinePattern *currentPattern();

	private slots:
		void machineChange(int index);
		void patternChange(int index);
	
	public slots:
		void refreshMachines();
		void displayPatterns(Machine *);

		void activeCol(int col);
		void addPattern();
		void delPattern();
		void renPattern();

	signals:
		void messageCritical(const QString &title, const QString &text);
};

#endif
