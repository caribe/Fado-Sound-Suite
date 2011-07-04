#include "tracktable.h"

TrackTable::TrackTable(Track *track)
{
	this->track = track;
	this->core = track->core;

	setFont(QFont("TypeWriter", 6));
	// setStyleSheet("QTableView::item { border: 0px; padding: 0px; font: 6px TypeWriter }");

	model = new TrackTableData(track, core);
	setModel(model);
	setSelectionMode(QAbstractItemView::SingleSelection);

	QItemSelectionModel *selModel = selectionModel();
	connect(selModel, SIGNAL(currentColumnChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(currentColumnChangedSlot(const QModelIndex &, const QModelIndex &)));
}


void TrackTable::refresh()
{
	model->reload();
}


void TrackTable::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key_Delete:
			setValue(-3);
			break;
		case Qt::Key_Comma:
			setValue(-1);
			break;
		case Qt::Key_Period:
			setValue(-2);
			break;
		default:
			bool ok;
			int val = event->text().toInt(&ok);
			if (ok) {
				setValue(val);
			} else {
				QTableView::keyPressEvent(event);
			}
	}
}


void TrackTable::setValue(int value)
{
	Machine *machine = track->currentMachine();
	if (machine == 0) return;

	QModelIndex index = this->currentIndex();

	int row = index.row();
	int row2 = (row + 1) % model->rowCount();

	if (value == -3) {
		machine->track.remove(row);
	} else {
		// machine->track[row]-> = value;
	}

	setCurrentIndex(index.sibling(row2, index.column()));
}


void TrackTable::currentColumnChangedSlot(const QModelIndex &current, const QModelIndex &previous)
{
	track->refreshPatterns();
}
