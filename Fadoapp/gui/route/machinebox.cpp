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

	boxText = new QGraphicsTextItem(this);
	boxText->setPos(-50, -10);
	boxText->setTextWidth(100);

	if (m->type == Machine::MachineMaster) {
		backgrounds << QColor(0x80, 0xb3, 0xff) << QColor(0xbf, 0xd9, 0xff);
	} else if (m->type == Machine::MachineInput) {
		backgrounds << QColor(0xff, 0x80, 0xdf) << QColor(0xff, 0xbf, 0xf0);
	} else {
		if (m->type == Machine::MachineGenerator) {
			backgrounds << QColor(0x80, 0xff, 0x80) << QColor(0xbf, 0xff, 0xbf);
		} else if (m->type == Machine::MachineEffect) {
			backgrounds << QColor(0xff, 0xaa, 0x00) << QColor(0xff, 0xd5, 0x00);
		}
	}

	refreshName();

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
	if (e->key() == Qt::Key_Delete and m->type != Machine::MachineMaster) {
		if (QMessageBox::question(route->parentWidget(), "Are you sure?", "Are you sure to delete this machine?", QMessageBox::Ok, QMessageBox::Cancel) == QMessageBox::Ok) {
			route->delMachine(this);
		}
	} else {
		e->ignore();
	}
}


void MachineBox::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) 
{
	route->slotDisplayPatterns(m);
}


void MachineBox::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
	setBrush(QBrush(backgrounds[1]));
	setFocus(Qt::MouseFocusReason);

	QString typeName;
	if (m->type == Machine::MachineMaster) {
		typeName = "Master";
	} else if (m->type == Machine::MachineInput) {
		typeName = "Input";
	} else if (m->type == Machine::MachineGenerator) {
		typeName = "Generator";
	} else if (m->type == Machine::MachineEffect) {
		typeName = "Effect";
	}

	route->slotDisplayStatus(typeName+" / "+m->author+" / "+m->name);
}


void MachineBox::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
	setBrush(QBrush(backgrounds[0]));
	clearFocus();
	route->slotClearStatus();
}


void MachineBox::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
	QMenu menu;
	QAction *renameAction = menu.addAction(QIcon(":pencil.png"), route->tr("Rename"));
	QAction *deleteAction = menu.addAction(QIcon(":cross.png"), route->tr("Delete"));
	QAction *selectedAction = menu.exec(event->screenPos());

	if (selectedAction == deleteAction) {
		route->delMachine(this);
	} else if (selectedAction == renameAction) {
		route->renMachine(this);
	}
}


void MachineBox::refreshName()
{
	if (m->alias.isNull()) {
		if (m->type == Machine::MachineMaster) {
			boxText->setHtml("<center><b>Master</b></center>");
		} else if (m->type == Machine::MachineInput) {
			if (m->name == "lineinput") {
				boxText->setHtml("<center><b>Mic</b></center>");
			} else if (m->name == "fileinput") {
				boxText->setHtml("<center><b>File</b></center>");
			}
		} else {
			boxText->setHtml("<center><b>"+m->name+"</b></center>");
		}
	} else {
		boxText->setHtml("<center><b>"+m->alias+"</b></center>");
	}
}
