#ifndef GUI_MACHINEBOX_H
#define GUI_MACHINEBOX_H

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QKeyEvent>
#include <QBrush>
#include <QCursor>

class MachineBox;

#include "route.h"
#include "ext/machine.h"

class MachineBox : public QGraphicsRectItem
{
	public:
		MachineBox(Route *route, Machine *machine);

		enum DragMode {DragBox, DragConnection};

		Route *route;
		Machine *m;
		QColor color;
		int dragMode;
		int ix, iy;
		QGraphicsTextItem *boxText;
		
		QHash<int, QGraphicsLineItem *> connections;
		QList<QColor> backgrounds;

		void mousePressEvent(QGraphicsSceneMouseEvent *e);
		void mouseMoveEvent(QGraphicsSceneMouseEvent *e);
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);
		void keyPressEvent(QKeyEvent *e);
		void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);

		void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
		void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

		void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

		void refreshName();
};

#endif
