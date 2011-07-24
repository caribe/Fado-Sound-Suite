#include "timeanalyze.h"

TimeAnalyze::TimeAnalyze(QGraphicsScene *scene, QWidget *parent) : QGraphicsView(scene, parent)
{
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}


void TimeAnalyze::resizeEvent(QResizeEvent *event)
{
	resetTransform();
	scale(event->size().width() / 1024.0, event->size().height() / 480.0);
}
