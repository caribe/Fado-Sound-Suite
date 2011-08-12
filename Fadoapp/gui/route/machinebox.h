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
 * aunsigned long with Fado Sound Suite.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
