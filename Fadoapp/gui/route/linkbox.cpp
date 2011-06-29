#include "linkbox.h"

LinkBox::LinkBox(MachineBox *m1, MachineBox *m2)
{
	this->m1 = m1;
	this->m2 = m2;

	setAcceptHoverEvents(true);
	setFlag(QGraphicsItem::ItemIsFocusable, true);

	// backgrounds << QColor(0x00, 0x00, 0x00) << QColor(0x33, 0x33, 0x33);
	backgrounds << QColor(0x00, 0x00, 0x00) << QColor(0x80, 0x80, 0x80);
	setBrush(QBrush(backgrounds[0]));
	setCursor(Qt::PointingHandCursor);

	basePath = new QPainterPath(QPointF(10, 0));
	basePath->lineTo(-5, -10);
	basePath->lineTo(-5, 10);
	basePath->closeSubpath();

	px = py = 0;
	value = 100;

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
	path->addPath(*basePath);
	setPath(*path);
}


void LinkBox::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	/*
	setBrush(QBrush(backgrounds[1]));
	setFocus(Qt::MouseFocusReason);
	route->mainWindow->status->showMessage("Connection "+QString::number(store->connections[m1->m->id][m2->m->id])+"%");
	*/
}


void LinkBox::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	/*
	setBrush(QBrush(backgrounds[0]));
	clearFocus();
	route->mainWindow->status->clearMessage();
	*/
}


void LinkBox::keyPressEvent(QKeyEvent *event)
{
	/*
	if (event->key() == Qt::Key_Delete) {
		store->flipConnection(m1->id(), m2->id());
		route->delConnection(m1, m2);
	} else if (event->key() == Qt::Key_Plus) {
		int value = store->connections[m1->m->id][m2->m->id];
		value++;
		if (value > 100) value = 100;
		route->mainWindow->status->showMessage("Connection "+QString::number(value)+"%");
		store->connections[m1->m->id][m2->m->id] = value;
	} else if (event->key() == Qt::Key_Minus) {
		int value = store->connections[m1->m->id][m2->m->id];
		value--;
		if (value < 0) value = 0;
		route->mainWindow->status->showMessage("Connection "+QString::number(value)+"%");
		store->connections[m1->m->id][m2->m->id] = value;
	}
	*/
}


void LinkBox::wheelEvent(QGraphicsSceneWheelEvent *event) {
	/*
	if (event->orientation() == Qt::Vertical) {
		int value = store->connections[m1->m->id][m2->m->id];
		if (event->delta() > 0) {
			value += 5;
			if (value > 100) value = 100;
		} else if (event->delta() < 0) {
			value -= 5;
			if (value < 0) value = 0;
		}
		route->mainWindow->status->showMessage("Connection "+QString::number(value)+"%");
		store->connections[m1->m->id][m2->m->id] = value;
		event->accept();
	}
	*/
}
