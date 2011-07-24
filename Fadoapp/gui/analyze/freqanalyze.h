#ifndef FREQANALYZE_H
#define FREQANALYZE_H

#include <QGraphicsView>
#include <QResizeEvent>

class FreqAnalyze : public QGraphicsView
{
    Q_OBJECT

	QTransform gtransform;

public:
	explicit FreqAnalyze(QGraphicsScene *scene, QWidget *parent = 0);
	void resizeEvent(QResizeEvent * event);

signals:

public slots:

};

#endif // TIMEANALYZE_H
