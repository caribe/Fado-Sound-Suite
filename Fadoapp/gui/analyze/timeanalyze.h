#ifndef TIMEANALYZE_H
#define TIMEANALYZE_H

#include <QGraphicsView>
#include <QResizeEvent>

class TimeAnalyze : public QGraphicsView
{
    Q_OBJECT

	QTransform gtransform;

public:
	explicit TimeAnalyze(QGraphicsScene *scene, QWidget *parent = 0);
	void resizeEvent(QResizeEvent * event);

signals:

public slots:

};

#endif // TIMEANALYZE_H
