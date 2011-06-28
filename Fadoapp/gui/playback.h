#ifndef GUI_PLAYBACK_H
#define GUI_PLAYBACK_H

#include <QFrame>
#include <QFile>
#include <QScrollBar>

#include <cmath>
#include <fftw3.h>

class Playback;
class MainWindow;

#include "mainwindow.h"

class Playback : public QFrame
{
	Q_OBJECT

	public:
		Playback(MainWindow *parent);
		MainWindow *parent;
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
