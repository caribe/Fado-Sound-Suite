#include "pattern.h"

Pattern::Pattern(QWidget *widget, Core *core) : QWidget(widget) {
	this->core = core;

	QGridLayout *grid = new QGridLayout(this);
	grid->setSpacing(3);
	grid->setContentsMargins(3, 3, 3, 3);

	// Area centrale
	QFrame *menu = new QFrame(this);
	QGridLayout *menuGrid = new QGridLayout(menu);
	menuGrid->setHorizontalSpacing(3);
	menuGrid->setContentsMargins(3, 3, 3, 3);

	// Etichetta 'Machine'
	QLabel *l1 = new QLabel("Machine", menu);
	menuGrid->addWidget(l1, 0, 0);

	// Select delle macchine instanziate
	machineCombo = new QComboBox(menu);
	connect(machineCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(machineChange(int)));
	menuGrid->addWidget(machineCombo, 0, 1);
	menuGrid->setColumnStretch(1, 1);

	// Etichetta 'Pattern'
	QLabel *l2 = new QLabel("Pattern", menu);
	menuGrid->addWidget(l2, 0, 2);

	// Select dei pattern per ogni macchina
	patternCombo = new QComboBox(menu);
	connect(patternCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(patternChange(int)));
	menuGrid->addWidget(patternCombo, 0, 3);
	menuGrid->setColumnStretch(3, 1);

	// Etichetta 'Base octave'
	QLabel *l3 = new QLabel("Base octave", menu);
	menuGrid->addWidget(l3, 0, 4);

	// Select dell'ottava di base
	octaveCombo = new QComboBox(menu);
	for (int i = 2; i < 8; i++) octaveCombo->insertItem(i, QString::number(i));
	octaveCombo->setCurrentIndex(2);
	menuGrid->addWidget(octaveCombo, 0, 5);

	menuGrid->setColumnStretch(6, 1);

	grid->addWidget(menu, 1, 0, 1, 2);


	// Colonna di destra

	detail = new QGroupBox(this);
	detail->setTitle("Description");
	detail->setMinimumWidth(200);
	detail->setMaximumWidth(200);

	QGridLayout *detailGrid = new QGridLayout(detail);

	detailUnit = new QLabel(detail);
	detailGrid->addWidget(detailUnit, 0, 0);
	
	detailMin = new QLabel(detail);
	detailGrid->addWidget(detailMin, 1, 0);
	
	detailMax = new QLabel(detail);
	detailGrid->addWidget(detailMax, 2, 0);

	detailDesc = new QLabel(detail);
	detailDesc->setWordWrap(true);
	detailGrid->addWidget(detailDesc, 3, 0);

	detailGrid->setRowStretch(4, 1);
	detail->setLayout(detailGrid);

	grid->addWidget(detail, 2, 1);


	// Tabella centrale
	tableview = new PatternTable(this);
	grid->addWidget(tableview, 2, 0);


	setLayout(grid);
}


void Pattern::refreshMachines()
{
	patternCombo->clear();
	machineCombo->clear();
	foreach (Machine *machine, core->machines) {
		machineCombo->addItem(machine->getName());
	}
}



void Pattern::machineChange(int index) {
	if (index > -1) refreshPatterns();
}



void Pattern::refreshPatterns()
{
	patternCombo->clear();

	Machine *machine = currentMachine();
	if (machine == 0) return;

	foreach (MachinePattern *pattern, machine->patterns) {
		patternCombo->addItem(pattern->name);
	}

	tableview->reset();

	activeCol(-1);
}



void Pattern::patternChange(int index) {
	tableview->refresh();
}



void Pattern::activeCol(int col)
{
	Machine *machine = currentMachine();
	if (machine == 0) return;

	if (col == -1) {
		detail->setTitle(machine->name);
		detailDesc->setText(machine->description);
		detailUnit->hide();
		detailMin->hide();
		detailMax->hide();
	} else {
		Param *param = machine->params[col];
		detail->setTitle(param->name);
		detailDesc->setText(param->description);
		detailUnit->show();
		detailUnit->setText("Unit: <b>" + QString::number(param->type) + "</b>");
		detailMin->show();
		detailMin->setText("Min: <b>" + QString::number(param->min) + "</b>");
		detailMax->show();
		detailMax->setText("Max: <b>" + QString::number(param->max) + "</b>");
	}
}


void Pattern::addPattern()
{
	Machine *machine = currentMachine();
	if (machine == 0) return;

	machine->addPattern(tr("New pattern"));

	refreshPatterns();
	patternCombo->setCurrentIndex(patternCombo->count()-1);
	renPattern();
}


void Pattern::renPattern()
{
	Machine *machine = currentMachine();
	if (machine == 0) return;

	int index = patternCombo->currentIndex();

	QString oldName = patternCombo->currentText();

	bool ok;
	QString newName = QInputDialog::getText(this, "Rename Pattern", "Pattern Name:", QLineEdit::Normal, oldName, &ok);

	if (ok == true) {
		machine->patterns[index]->name = newName;
		patternCombo->setItemText(index, newName);
	}
}


void Pattern::delPattern()
{
	Machine *machine = currentMachine();
	if (machine == 0) return;

	int index = patternCombo->currentIndex();

	if (index < 0) return;

	patternCombo->removeItem(index);
	machine->patterns.removeAt(index);

	refreshPatterns();
}


MachinePattern *Pattern::currentPattern()
{
	Machine *machine = currentMachine();
	if (machine == 0) return 0;

	int index = patternCombo->currentIndex();

	if (index > -1 and index < machine->patterns.length()) {
		return machine->patterns[index];
	} else {
		return 0;
	}
}



Machine *Pattern::currentMachine()
{
	int index = machineCombo->currentIndex();
	if (index == -1) {
		return 0;
	} else {
		return core->machines[index];
	}
}


void Pattern::displayPatterns(Machine *m)
{
	int i = core->machines.indexOf(m);
	if (i > -1) {
		machineCombo->setCurrentIndex(i);
	}
}
