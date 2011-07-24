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

#ifndef GUI_ANALYZE_H
#define GUI_ANALYZE_H

#include <QSplitter>
#include <QFile>
#include <QTransform>
#include <QScrollBar>
#include <QGridLayout>
#include <QGraphicsView>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>

#include <cmath>
#include <fftw3.h>

class MainWindow;

#include "core/core.h"
#include "gui/analyze/timeanalyze.h"
#include "gui/analyze/freqanalyze.h"

class Analyze : public QWidget
{
	Q_OBJECT

	public:
		Analyze(QWidget *parent, Core *core);
		Core *core;
		QGraphicsScene *scenef, *scenet;
		QScrollBar *scrollBar;
		QFile *file;
		QGraphicsPolygonItem *tlx, *trx, *flx;

	private slots:
		void buttonView();
		void sliderValueChanged(int value);
};


#endif
