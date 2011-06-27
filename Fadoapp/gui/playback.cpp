#include "gui/playback.h"

Playback::Playback(MainWindow *parent)
{
	this->parent = parent;

	QGridLayout *grid = new QGridLayout(this);
	grid->setSpacing(3);
	grid->setContentsMargins(3, 3, 3, 3);

	QPushButton *play = new QPushButton(QIcon("icons/playback/play-icon"), "Play", this);
	QPushButton *rec = new QPushButton(QIcon("icons/playback/record-icon"), "Rec", this);
	QPushButton *stop = new QPushButton(QIcon("icons/playback/stop-icon"), "Stop", this);
	QPushButton *view = new QPushButton("View", this);

	connect(play, SIGNAL(clicked()), this, SLOT(buttonPlay()));
	connect(rec,  SIGNAL(clicked()), this, SLOT(buttonRec()));
	connect(stop, SIGNAL(clicked()), this, SLOT(buttonStop()));
	connect(view, SIGNAL(clicked()), this, SLOT(buttonView()));

	grid->setColumnStretch(0, 1);
	grid->addWidget(play, 0, 1);
	grid->addWidget(rec, 0, 2);
	grid->addWidget(stop, 0, 3);
	grid->addWidget(view, 0, 4);
	grid->setColumnStretch(6, 1);

	file = new QFile();

	scrollBar = new QScrollBar(Qt::Horizontal);
	scrollBar->setMinimum(0);
	scrollBar->setPageStep(512);
	scrollBar->setMaximum(1024);
	scrollBar->setValue(0);
	connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));
	grid->addWidget(scrollBar, 3, 0, 1, -1);

	scenet = new QGraphicsScene();

	QGraphicsView *gview = new QGraphicsView(scenet, this);
	grid->addWidget(gview, 1, 0, 1, -1);
	grid->setRowStretch(1, 1);

	tlx = new QGraphicsPolygonItem();
	tlx->setPen(QPen(QColor(0xff, 0, 0), 1));
	tlx->setBrush(QBrush(QColor(0xff, 0x80, 0x80)));
	scenet->addItem(tlx);

	trx = new QGraphicsPolygonItem();
	trx->setPen(QPen(QColor(0, 0xff, 0), 1));
	trx->setBrush(QBrush(QColor(0x80, 0xff, 0x80)));
	scenet->addItem(trx);

	scenef = new QGraphicsScene();

	QGraphicsView *fview = new QGraphicsView(scenef, this);
	grid->addWidget(fview, 2, 0, 1, -1);
	grid->setRowStretch(2, 1);

	flx = new QGraphicsPolygonItem();
	flx->setPen(QPen(QColor(0xff, 0, 0), 1));
	flx->setBrush(QBrush(QColor(0xff, 0x80, 0x80)));
	scenef->addItem(flx);

	state = 0;
}



void Playback::buttonPlay()
{
	if (parent->core->start(false) != 0) {
		QMessageBox::critical(this, "Cannot start", parent->core->errstr);
		state = 0;
	} else {
		state = 1;
	}
}



void Playback::buttonRec()
{
	if (parent->core->start(true) != 0) {
		QMessageBox::critical(this, "Cannot start", parent->core->errstr);
		state = 0;
	} else {
		state = 2;
	}
}



void Playback::buttonStop()
{
	if (parent->core->stop() != 0) {
		QMessageBox::critical(this, "Cannot stop", parent->core->errstr);
	}
	state = 0;
}


void Playback::buttonView()
{
	short *buffer = new short[2048];

	if (file->isOpen() == false) {
		file->setFileName("/tmp/record.raw");
		file->open(QIODevice::ReadOnly);
		scrollBar->setMaximum((file->size() / 4) - 1024);
		scrollBar->setValue(0);
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
	fftw_execute(p); /* repeat as needed */

	*lx << QPointF(0, -1);
	max = 0;
	for (int i = 0; i < 512; i++) if (fabs(out[i][0]) > max) max = fabs(out[i][0]);
	for (int i = 0; i < 512; i++) *lx << QPointF(i, (fabs(out[i][0]) * -240 / max) - 1);
	*lx << QPointF(512, -1);

/*
	for (int i = 0; i < 2048; i+=2) {
		in[i/2][0] = buffer[i+1] / 32768.0;
		in[i/2][1] = 0;
	}
	fftw_execute(p); // repeat as needed

	*rx << QPointF(0, 1);
	for (int i = 0; i < 512; i++) if (fabs(out[i][0]) > max) max = fabs(out[i][0]);
	for (int i = 0; i < 512; i++) *rx << QPointF(i, (fabs(out[i][0]) * 240 / max) + 1);
	*rx << QPointF(512, 1);
*/

	fftw_destroy_plan(p);
	fftw_free(in);
	fftw_free(out);

	flx->setPolygon(*lx);
	// scenet->addPolygon(*rx, QPen(QColor(0, 0xff, 0), 1), QBrush(QColor(0x80, 0xff, 0x80)));
}


void Playback::sliderValueChanged(int value)
{
	buttonView();
}
