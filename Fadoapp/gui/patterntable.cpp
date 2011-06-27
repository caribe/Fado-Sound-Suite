#include "gui/patterntable.h"

PatternTable::PatternTable(Pattern *pattern) {
	this->pattern = pattern;

	keyMap << Qt::Key_Z << Qt::Key_S << Qt::Key_X << Qt::Key_D << Qt::Key_C << Qt::Key_V << Qt::Key_G << Qt::Key_B << Qt::Key_H << Qt::Key_N << Qt::Key_J << Qt::Key_M;
	notes << "C" << "C#" << "D" << "D#" << "E" << "F" << "F#" << "G" << "G#" << "A" << "A#" << "B";

	setSelectionMode(QAbstractItemView::SingleSelection);

	setFont(QFont("TypeWriter", 6));
	
	model = new PatternTableData(pattern);
	setModel(model);

	QItemSelectionModel *selModel = selectionModel();
	connect(selModel, SIGNAL(currentColumnChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(currentColumnChangedSlot(const QModelIndex &, const QModelIndex &)));
}


void PatternTable::refresh()
{
	model->reload();
}


void PatternTable::currentColumnChangedSlot(const QModelIndex &current, const QModelIndex &previous)
{
	pattern->activeCol(current.column());
}


void PatternTable::keyPressEvent(QKeyEvent *event)
{
	int i = keyMap.indexOf(event->key());
	QModelIndex index = currentIndex();

	if (i >= 0) {
		model->setData(index, QVariant(notes[i]+pattern->octaveCombo->currentText()), 0);
		update(index);
		setCurrentIndex(index.sibling((index.row()+1) % model->rowCount(), index.column()));
	} else if (event->key() == Qt::Key_Delete) {
		model->setData(index, QVariant(""), 0);
		update(index);
		setCurrentIndex(index.sibling((index.row()+1) % model->rowCount(), index.column()));
	} else {
		QTableView::keyPressEvent(event);
	}
}
