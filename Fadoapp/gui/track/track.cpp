#include "track.h"

Track::Track(QWidget *parent, Core *core) : QWidget(parent)
{
	this->core = core;

	QGridLayout *grid = new QGridLayout(this);
	grid->setContentsMargins(3, 3, 3, 3);
	grid->setSpacing(3);

	// Toolbar
	
	QFrame *toolbar = new QFrame();
	QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbar);
	toolbarLayout->setContentsMargins(0, 0, 0, 0);
	toolbarLayout->setSpacing(3);

	QPushButton *addButton = new QPushButton(QIcon(":/plus.png"), "Add Row", toolbar);
	toolbarLayout->addWidget(addButton);
	connect(addButton, SIGNAL(clicked()), this, SLOT(addButtonSlot()));

	QPushButton *delButton = new QPushButton(QIcon(":/minus.png"), "Del Row", toolbar);
	toolbarLayout->addWidget(delButton);
	connect(delButton, SIGNAL(clicked()), this, SLOT(delButtonSlot()));

	QPushButton *frsButton = new QPushButton(QIcon("icons/control_start.png"), "First Row", toolbar);
	toolbarLayout->addWidget(frsButton);
	connect(frsButton, SIGNAL(clicked()), this, SLOT(frsButtonSlot()));

	QPushButton *lstButton = new QPushButton(QIcon("icons/control_end.png"), "Last Row", toolbar);
	toolbarLayout->addWidget(lstButton);
	connect(lstButton, SIGNAL(clicked()), this, SLOT(lstButtonSlot()));
	toolbarLayout->insertStretch(-1);

	grid->addWidget(toolbar, 0, 0);
	
	// Tracks

	// self.tracks = TrackTableView(self)
	tracksTable = new TrackTable(this);
	grid->addWidget(tracksTable, 1, 0);

	// Hint

	QGroupBox *hint = new QGroupBox(this);
	hint->setTitle("Patterns");
	hint->setMinimumWidth(200);
	hint->setMaximumWidth(200);

	QGridLayout *hintGrid = new QGridLayout(hint);

	hintList = new QListWidget(hint);
	connect(hintList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(itemDoubleClickedSlot(QListWidgetItem *)));
	hintGrid->addWidget(hintList, 0, 0);

	grid->addWidget(hint, 1, 1);
}


void Track::refreshMachines()
{
	tracksTable->refresh();
}


void Track::refreshPatterns()
{
	Machine *machine = currentMachine();
	if (machine == 0) return;

	hintList->clear();
	hintList->addItem(". <mute>");
	hintList->addItem(", <break>");

	mapping.clear();

	foreach (MachinePattern *pat, machine->patterns) {
		hintList->addItem(pat->name);
	}
}


void Track::itemDoubleClickedSlot(QListWidgetItem *item)
{
	int index = hintList->currentRow();

	if (index == 0 or index == 1) {
		tracksTable->setValue(index - 2);
	} else {
		tracksTable->setValue(item->data(Qt::UserRole).toInt());
	}
}


void Track::addButtonSlot()
{
	core->total_patterns++;
	tracksTable->refresh();
}


void Track::delButtonSlot()
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

	tracksTable->refresh();
}


void Track::frsButtonSlot()
{
	Master *m = (Master *)core->machines[0];
	int row = tracksTable->currentIndex().row();
	if (row == -1) row = 0;
	m->track_first = row;
	tracksTable->refresh();
}


void Track::lstButtonSlot()
{
	Master *m = (Master *)core->machines[0];
	int row = tracksTable->currentIndex().row();
	if (row == -1) row = core->total_patterns - 1;
	if (row < m->track_first) row = m->track_first;
	m->track_last = row;
	tracksTable->refresh();
}


Machine *Track::currentMachine()
{
	QModelIndex index = tracksTable->currentIndex();
	if (index.isValid()) return core->machines[index.row()];
	return 0;
}
