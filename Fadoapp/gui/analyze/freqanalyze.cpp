#include "freqanalyze.h"

FreqAnalyze::FreqAnalyze(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent)
{
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}


void FreqAnalyze::resizeEvent(QResizeEvent *event)
{
	resetTransform();
	scale(event->size().width() / 512.0, event->size().height() / 240.0);
}
