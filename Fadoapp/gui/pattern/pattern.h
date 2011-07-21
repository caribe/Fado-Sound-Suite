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
