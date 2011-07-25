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

#include "analyze.h"

Analyze::Analyze(QWidget *parent, Core *core) : QWidget(parent)
{
	this->core = core;
	file = new QFile();

	// Scrollbar

	scrollBar = new QScrollBar(Qt::Horizontal);
	scrollBar->setMinimum(0);
	scrollBar->setPageStep(512);
	scrollBar->setMaximum(1024);
	scrollBar->setValue(0);
	connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));

	// Time graph

	scenet = new QGraphicsScene();
	tlx = new QGraphicsPolygonItem();
	tlx->setPen(QPen(QColor(0xff, 0, 0), 1));
	tlx->setBrush(QBrush(QColor(0xff, 0x80, 0x80)));
	scenet->addItem(tlx);

	trx = new QGraphicsPolygonItem();
	trx->setPen(QPen(QColor(0, 0xff, 0), 1));
	trx->setBrush(QBrush(QColor(0x80, 0xff, 0x80)));
	scenet->addItem(trx);

	TimeAnalyze *gview = new TimeAnalyze(scenet, this);

	// Freq graph

	scenef = new QGraphicsScene();

	flx = new QGraphicsPolygonItem();
	flx->setPen(QPen(QColor(0xff, 0, 0), 1));
	flx->setBrush(QBrush(QColor(0xff, 0x80, 0x80)));
	scenef->addItem(flx);

	FreqAnalyze *fview = new FreqAnalyze(scenef, this);

	// Layout

	QSplitter *splitter = new QSplitter(Qt::Vertical);
	splitter->addWidget(gview);
	splitter->addWidget(fview);

	QVBoxLayout *vbox = new QVBoxLayout();
	vbox->addWidget(splitter, 1);
	vbox->addWidget(scrollBar);

	setLayout(vbox);
}



void Analyze::buttonView()
{
	short *buffer = new short[2048];

	QSettings settings;
	QString filename = settings.value("tempdir", "/tmp").toString()+"/fado.raw";

	if (file->isOpen() == false) {
		file->setFileName(filename);
		file->open(QIODevice::ReadOnly);
		scrollBar->setMaximum((file->size() / 4) - 1024);
		scrollBar->setValue(0);
	}

	if (file->isOpen() == false) {
		qDebug() << "Cannot open" << filename;
		return;
	}

	file->seek(scrollBar->value() * 4);
	file->read((char *)buffer, 1024 * 4);

	// Time analysis
	
	QPolygonF *lx = new QPolygonF();
	QPolygonF *rx = new QPolygonF();

	*lx << QPointF(0, -120);
	*rx << QPointF(0, 120);

	for (int i = 0; i < 2048; i+=2) {
		*lx << QPointF(i / 2, (buffer[i] * 120 / 32768.0) - 120);
		*rx << QPointF(i / 2, (buffer[i+1] * 120 / 32768.0) + 120);
	}

	*lx << QPointF(1023, -120);
	*rx << QPointF(1023, 120);
	
	tlx->setPolygon(*lx);
	trx->setPolygon(*rx);

	// Frequency analysis

	fftw_complex *in, *out;
	fftw_plan p;
	int N = 1024;
	float max;

	lx->clear();
	rx->clear();

	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	for (int i = 0; i < 2048; i+=2) {
		in[i/2][0] = buffer[i] / 32768.0;
		in[i/2][1] = 0;
	}
	fftw_execute(p); // repeat as needed

	*lx << QPointF(0, -1);
	max = 0;
	for (int i = 0; i < 512; i++) if (fabs(out[i][0]) > max) max = fabs(out[i][0]);
	for (int i = 0; i < 512; i++) *lx << QPointF(i, (fabs(out[i][0]) * -240 / max) - 1);
	*lx << QPointF(512, -1);


	for (int i = 0; i < 2048; i+=2) {
		in[i/2][0] = buffer[i+1] / 32768.0;
		in[i/2][1] = 0;
	}
	fftw_execute(p); // repeat as needed

	*rx << QPointF(0, 1);
	for (int i = 0; i < 512; i++) if (fabs(out[i][0]) > max) max = fabs(out[i][0]);
	for (int i = 0; i < 512; i++) *rx << QPointF(i, (fabs(out[i][0]) * 240 / max) + 1);
	*rx << QPointF(512, 1);


	fftw_destroy_plan(p);
	fftw_free(in);
	fftw_free(out);

	flx->setPolygon(*lx);
	// scenet->addPolygon(*rx, QPen(QColor(0, 0xff, 0), 1), QBrush(QColor(0x80, 0xff, 0x80)));
}


void Analyze::sliderValueChanged(int value)
{
	buttonView();
}
