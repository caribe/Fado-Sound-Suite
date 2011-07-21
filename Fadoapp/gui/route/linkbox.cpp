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
 * along with Fado Sound Suite.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "linkbox.h"

LinkBox::LinkBox(Route *route, MachineBox *m1, MachineBox *m2)
{
	this->m1 = m1;
	this->m2 = m2;
	this->route = route;

	setAcceptHoverEvents(true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);

	backgrounds << QColor(0x00, 0x00, 0x00) << QColor(0x80, 0x80, 0x80);
	setBrush(QBrush(backgrounds[0]));
	setCursor(Qt::PointingHandCursor);

	basePath = QPainterPath(QPointF(10, 0));
	basePath.lineTo(-5, -10);
	basePath.lineTo(-5, 10);
	basePath.closeSubpath();

	px = py = 0;

	repos();
}


void LinkBox::repos()
{

	int x1m = m1->x();
	int y1m = m1->y();
	int x2m = m2->x();
	int y2m = m2->y();

	float dx = float(x2m - x1m);
	float dy = float(y2m - y1m);
	float dd = sqrt(dx * dx + dy * dy);
	
	float xx = x1m + dx / 2;
	float yy = y1m + dy / 2;
	
	float rad;

	if (dx != 0) {
		rad = atan(dy / dx);
	} else {
		rad = atan(dy / 0.00001);
	}

	if (dx < 0) {
		if (dx > 0) {
			rad += 3.14;
		} else {
			rad -= 3.14;
		}
	}

	QTransform *transform = new QTransform();
	transform->translate(xx, yy);
	transform->rotateRadians(rad);
	setTransform(*transform);

	QPainterPath *path = new QPainterPath();
	path->moveTo(10, 0);
	path->lineTo(dd / 2, 0);
	path->moveTo(-5, 0);
	path->lineTo(-dd / 2, 0);
	path->addPath(basePath);
	setPath(*path);
}


void LinkBox::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	setBrush(QBrush(backgrounds[1]));
	setFocus(Qt::MouseFocusReason);
	route->slotDisplayStatus("Connection "+QString::number(m1->m->connectionDst[m2->m]->rx)+"%");
}


void LinkBox::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	setBrush(QBrush(backgrounds[0]));
	clearFocus();
	route->slotClearStatus();
}


void LinkBox::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete) {
		route->delConnection(this);
	} else if (event->key() == Qt::Key_Plus) {
		Volume *volume = m1->m->connectionDst[m2->m];
		volume->rx++;
		if (volume->rx > 100) volume->rx = 100;
		route->slotDisplayStatus("Connection "+QString::number(volume->rx)+"%");
	} else if (event->key() == Qt::Key_Minus) {
		Volume *volume = m1->m->connectionDst[m2->m];
		volume->rx--;
		if (volume->rx < 0) volume->rx = 0;
		route->slotDisplayStatus("Connection "+QString::number(volume->rx)+"%");
	}
}


void LinkBox::wheelEvent(QGraphicsSceneWheelEvent *event) {

	if (event->orientation() == Qt::Vertical) {
		Volume *volume = m1->m->connectionDst[m2->m];

		if (event->delta() > 0) {
			volume->rx += 5;
			if (volume->rx > 100) volume->rx = 100;
		} else if (event->delta() < 0) {
			volume->rx -= 5;
			if (volume->rx < 0) volume->rx = 0;
		}

		route->slotDisplayStatus("Connection "+QString::number(volume->rx)+"%");
		event->accept();
	}
}
