#include "tracks.h"

Tracks::Tracks(QWidget *parent, Core *core) : QSplitter(parent)
{
	this->core = core;

	// Tracks

	tracksModel = new TracksModel(this, core);

	tracksTable = new QTableView(this);
	tracksTable->setFont(QFont("TypeWriter", 6));
	// tracksTable->setStyleSheet("QTableView::item { border: 0px; padding: 0px; font: 6px TypeWriter }");

	tracksTable->setModel(tracksModel);
	tracksTable->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(tracksTable->selectionModel(), SIGNAL(currentColumnChanged(QModelIndex,QModelIndex)), SLOT(refreshPatterns()));

	addWidget(tracksTable);

	// Hint

	hintList = new QListWidget();
	connect(hintList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), SLOT(itemDoubleClickedSlot(QListWidgetItem *)));
	addWidget(hintList);
}


void Tracks::refreshMachines()
{
	tracksModel->reload();
}


void Tracks::refreshPatterns()
{
	Machine *machine = currentMachine();
	if (machine == 0) return;

	hintList->clear();

	foreach (MachinePattern *pat, machine->patterns) {
		hintList->addItem(pat->name);
	}
}


void Tracks::itemDoubleClickedSlot(QListWidgetItem *item)
{
	Machine *machine = currentMachine();
	if (machine == 0) return;

	QModelIndex index = tracksTable->currentIndex();

	int currentRow = index.row();
	int nextRow = (currentRow + 1) % tracksModel->rowCount();

	machine->track[currentRow] = machine->patterns[hintList->currentRow()];

	tracksTable->setCurrentIndex(index.sibling(nextRow, index.column()));
}


void Tracks::addButtonSlot()
{
	core->total_patterns++;
	tracksTable->reset();
}


void Tracks::delButtonSlot()
{
	int row = tracksTable->currentIndex().row();
	if (row == -1) row = core->total_patterns - 1;
	if (core->total_patterns <= 1) return;
	core->total_patterns--;

	foreach (Machine *m, core->machines) {
		QList<int> tracks = m->track.keys();
		qSort(tracks);
		foreach (int i, tracks) {
			if (i == row) {
				m->track.remove(i);
			} else if (i > row) {
				m->track[i-1] = m->track[i];
				m->track.remove(i);
			}
		}
	}

	Master *m = (Master *)core->machines[0];
	if (m->track_first > row) m->track_first--;
	if (m->track_last > row) m->track_last--;
	if (m->track_first >= core->total_patterns) m->track_first = core->total_patterns - 1;
	if (m->track_last  >= core->total_patterns) m->track_last  = core->total_patterns - 1;

	tracksTable->reset();
}


void Tracks::frsButtonSlot()
{
	Master *m = (Master *)core->machines[0];
	int row = tracksTable->currentIndex().row();
	if (row == -1) row = 0;
	m->track_first = row;
	tracksTable->reset();
}


void Tracks::lstButtonSlot()
{
	Master *m = (Master *)core->machines[0];
	int row = tracksTable->currentIndex().row();
	if (row == -1) row = core->total_patterns - 1;
	if (row < m->track_first) row = m->track_first;
	m->track_last = row;
	tracksTable->reset();
}


void Tracks::deleteButtonSlot() {}
void Tracks::muteButtonSlot() {}
void Tracks::breakButtonSlot() {}



Machine *Tracks::currentMachine()
{
	QModelIndex index = tracksTable->currentIndex();
	if (index.isValid()) return core->machines[index.row()];
	return 0;
}
