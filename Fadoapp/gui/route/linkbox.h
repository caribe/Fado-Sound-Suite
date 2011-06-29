#ifndef GUI_LINKBOX_H
#define GUI_LINKBOX_H

#include <QGraphicsPathItem>
#include <QStatusBar>

#include <cmath>

class LinkBox;

#include "core/store.h"
#include "machinebox.h"

class LinkBox : public QGraphicsPathItem
{
	public:
		LinkBox(MachineBox *m1, MachineBox *m2);
		void repos();

		MachineBox *m1, *m2;
		QList<QColor> backgrounds;
		int value;
		QPainterPath *basePath;
		float px, py, pr;

		void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
		void keyPressEvent(QKeyEvent *event);
		void wheelEvent(QGraphicsSceneWheelEvent *event);
};

#endif
