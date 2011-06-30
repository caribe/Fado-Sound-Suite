#include "machinebox.h"

MachineBox::MachineBox(Route *route, Machine *machine)
{
	this->route = route;
	this->m = machine;

	// Box

	setRect(-50, -25, 100, 50);
	if (m->x == -1) m->x = 100;
	if (m->y == -1) m->y = 100;
	setPos(m->x, m->y);
	setZValue(1);

	// Label

	QGraphicsTextItem *boxText = new QGraphicsTextItem(this);
	boxText->setPos(-50, -20);
	boxText->setTextWidth(100);

	if (m->type == Machine::MachineMaster) {
		backgrounds << QColor(0x80, 0xb3, 0xff) << QColor(0xbf, 0xd9, 0xff);
		boxText->setHtml("<center><b>Master</b></center>");
		longname = "Master";
	} else if (m->type == Machine::MachineInput) {
		backgrounds << QColor(0xff, 0x80, 0xdf) << QColor(0xff, 0xbf, 0xf0);
		if (m->name == "lineinput") {
			boxText->setHtml("<center><b>Input</b></center>");
		} else if (m->name == "fileinput") {
			boxText->setHtml("<center><b>File</b></center>");
		}
		longname = m->name;
	} else if (m->type == Machine::MachineGenerator) {
		backgrounds << QColor(0x80, 0xff, 0x80) << QColor(0xbf, 0xff, 0xbf);
		longname = m->author + "." + m->name;
		boxText->setHtml("<center>"+m->author+"<br/><b>"+m->name+"</b></center>");
	} else if (m->type == Machine::MachineEffect) {
		backgrounds << QColor(0xff, 0xaa, 0x00) << QColor(0xff, 0xd5, 0x00);
		longname = m->author + "." + m->name;
		boxText->setHtml("<center>"+m->author+"<br/><b>"+m->name+"</b></center>");
	}

	// Style

	setBrush(QBrush(backgrounds[0]));
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setCursor(Qt::OpenHandCursor);

	// Events

	setAcceptHoverEvents(true);
}


void MachineBox::mousePressEvent(QGraphicsSceneMouseEvent *e) {
	if (e->button() == Qt::LeftButton) {
		if (e->modifiers() == Qt::ControlModifier) {
			dragMode = DragConnection;
			route->connectionStart();
			QPointF pos = e->scenePos();
			route->connectionMove(m->x, m->y, pos.x(), pos.y());
		} else {
			QGraphicsRectItem::mousePressEvent(e);
			dragMode = DragBox;
			setCursor(Qt::ClosedHandCursor);
		}
	}
}


void MachineBox::mouseMoveEvent(QGraphicsSceneMouseEvent *e) {
	if (dragMode == DragBox) {
		QGraphicsRectItem::mouseMoveEvent(e);
		route->moveConnection(this);
	} else if (dragMode == DragConnection) {
		QPointF pos = e->scenePos();
		route->connectionMove(m->x, m->y, pos.x(), pos.y());
	}
}


void MachineBox::mouseReleaseEvent(QGraphicsSceneMouseEvent *e) {
	if (dragMode == DragBox) {
		QGraphicsRectItem::mouseReleaseEvent(e);
		setCursor(Qt::OpenHandCursor);
		QPointF pos = this->pos();
		m->x = pos.x();
		m->y = pos.y();
	} else if (dragMode == DragConnection) {
		QPointF pos = e->scenePos();
		route->connectionFinish(this, pos.x(), pos.y());
	}
}


void MachineBox::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_Delete and (m->author != "Core" or m->name != "output")) {
		if (QMessageBox::question(route, "Are you sure?", "Are you sure to delete this machine?", QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {
			route->delMachine(this);
		}
	} else {
		e->ignore();
	}
}


void MachineBox::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) 
{
	// parent->mainWindow->tabs->setCurrentIndex(1);
	// parent->mainWindow->pattern->machineCombo->setCurrentIndex(id());
}


void MachineBox::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	setBrush(QBrush(backgrounds[1]));
	setFocus(Qt::MouseFocusReason);
	// parent->mainWindow->status->showMessage(m->type+"."+m->author+"."+m->name);
}


void MachineBox::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	setBrush(QBrush(backgrounds[0]));
	clearFocus();
	// parent->mainWindow->status->clearMessage();
}
