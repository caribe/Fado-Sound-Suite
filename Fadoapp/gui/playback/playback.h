#ifndef GUI_PLAYBACK_H
#define GUI_PLAYBACK_H

#include <QFrame>
#include <QFile>
#include <QScrollBar>
#include <QGridLayout>
#include <QGraphicsView>
#include <QPushButton>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>

#include <cmath>
#include <fftw3.h>

class Playback;
class MainWindow;

#include "core/core.h"

class Playback : public QFrame
{
	Q_OBJECT

	public:
		Playback(Core *core);
		Core *core;
		QGraphicsScene *scenef, *scenet;
		QScrollBar *scrollBar;
		int state;
		QFile *file;
		QGraphicsPolygonItem *tlx, *trx, *flx;

	private slots:
		void buttonPlay();
		void buttonRec();
		void buttonStop();
		void buttonView();
		void sliderValueChanged(int value);
};


#endif
