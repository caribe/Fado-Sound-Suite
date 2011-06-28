#include "gui/machinebox.h"

MachineBox::MachineBox(Route *parent, Machine *machine)
{
	/*
	this->m = machine;
	this->parent = parent;

	setRect(-50, -25, 100, 50);
	if (m->x == -1) m->x = 100;
	if (m->y == -1) m->y = 100;
	setPos(m->x, m->y);
	setZValue(1);

	QGraphicsTextItem *boxText = new QGraphicsTextItem(this);
	boxText->setPos(-50, -20);
	boxText->setTextWidth(100);

	if (m->author == "Core") {
		if (m->name == "output") {
			backgrounds << QColor(0x80, 0xb3, 0xff) << QColor(0xbf, 0xd9, 0xff);
			boxText->setHtml("<center><b>Master</b></center>");
			longname = "Master";
		} else {
			backgrounds << QColor(0xff, 0x80, 0xdf) << QColor(0xff, 0xbf, 0xf0);
			if (m->name == "lineinput") {
				boxText->setHtml("<center><b>Input</b></center>");
			} else if (m->name == "fileinput") {
				boxText->setHtml("<center><b>File</b></center>");
			}
			longname = m->name;
		}
	} else if (m->type == "generator") {
		backgrounds << QColor(0x80, 0xff, 0x80) << QColor(0xbf, 0xff, 0xbf);
		longname = m->author + "." + m->name;
		boxText->setHtml("<center>"+m->author+"<br/><b>"+m->name+"</b></center>");
	} else if (m->type == "effect") {
		backgrounds << QColor(0xff, 0xaa, 0x00) << QColor(0xff, 0xd5, 0x00);
		longname = m->author + "." + m->name;
		boxText->setHtml("<center>"+m->author+"<br/><b>"+m->name+"</b></center>");
	}

	setBrush(QBrush(backgrounds[0]));
	setFlag(QGraphicsItem::ItemIsFocusable, true);
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setCursor(Qt::OpenHandCursor);

	setAcceptHoverEvents(true);
	*/
}


void MachineBox::mousePressEvent(QGraphicsSceneMouseEvent *e) {
	/*
	if (e->button() == Qt::LeftButton) {
		if (e->modifiers() == Qt::ControlModifier) {
			dragMode = 2;
			parent->connectionStart();
			QPointF pos = e->scenePos();
			parent->connectionMove(m->x, m->y, pos.x(), pos.y());
		} else {
			QGraphicsRectItem::mousePressEvent(e);
			dragMode = 1;
			setCursor(Qt::ClosedHandCursor);
		}
	}
	*/
}


void MachineBox::mouseMoveEvent(QGraphicsSceneMouseEvent *e) {
	/*
	if (dragMode == 1) {
		QGraphicsRectItem::mouseMoveEvent(e);
		parent->moveConnection(this);
	} else if (dragMode == 2) {
		QPointF pos = e->scenePos();
		parent->connectionMove(m->x, m->y, pos.x(), pos.y());
	}
	*/
}


void MachineBox::mouseReleaseEvent(QGraphicsSceneMouseEvent *e) {
	/*
	if (dragMode == 1) {
		QGraphicsRectItem::mouseReleaseEvent(e);
		setCursor(Qt::OpenHandCursor);
		QPointF pos = this->pos();
		m->x = pos.x();
		m->y = pos.y();
	} else if (dragMode == 2) {
		QPointF pos = e->scenePos();
		parent->connectionFinish(this, pos.x(), pos.y());
	}
	*/
}


void MachineBox::keyPressEvent(QKeyEvent *e) {
	/*
	if (e->key() == Qt::Key_Delete and (m->author != "Core" or m->name != "output")) {
		if (QMessageBox::question(this->parent, "Are you sure?", "Are you sure to delete this machine?", QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {
			parent->delMachine(this);
		}
	} else {
		e->ignore();
	}
	*/
}


void MachineBox::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) 
{
	parent->mainWindow->tabs->setCurrentIndex(1);
	parent->mainWindow->pattern->machineCombo->setCurrentIndex(id());
}


void MachineBox::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	/*
	setBrush(QBrush(backgrounds[1]));
	setFocus(Qt::MouseFocusReason);
	parent->mainWindow->status->showMessage(m->type+"."+m->author+"."+m->name);
	*/
}


void MachineBox::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	/*
	setBrush(QBrush(backgrounds[0]));
	clearFocus();
	parent->mainWindow->status->clearMessage();
	*/
}


int MachineBox::id()
{
	/*
	return m->id;
	*/
}
