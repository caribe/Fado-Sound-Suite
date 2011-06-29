#include "pattern.h"

Pattern::Pattern(Store *store) {
	this->store = store;

	activeMachine = 0;

	QGridLayout *grid = new QGridLayout(this);
	grid->setSpacing(3);
	grid->setContentsMargins(3, 3, 3, 3);

	// Tools
	QFrame *toolbar = new QFrame();
	QHBoxLayout *toolbarLayout = new QHBoxLayout(toolbar);
	toolbarLayout->setContentsMargins(0, 0, 0, 0);
	toolbarLayout->setSpacing(3);

	QPushButton *addButton = new QPushButton(QIcon("icons/add.png"), "Add Pattern", toolbar);
	toolbarLayout->addWidget(addButton);
	connect(addButton, SIGNAL(clicked()), this, SLOT(addPattern()));
	QPushButton *delButton = new QPushButton(QIcon("icons/delete.png"), "Del Pattern", toolbar);
	toolbarLayout->addWidget(delButton);
	connect(delButton, SIGNAL(clicked()), this, SLOT(delPattern()));
	QPushButton *renButton = new QPushButton(QIcon("icons/standard/001_45.png"), "Rename Pattern", toolbar);
	toolbarLayout->addWidget(renButton);
	connect(renButton, SIGNAL(clicked()), this, SLOT(renPattern()));
	toolbarLayout->insertStretch(-1);

	grid->addWidget(toolbar, 0, 0);

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
	/*
	machineCombo->clear();
	foreach (int i, store->machines.keys()) {
		machineCombo->addItem(store->machines[i]->author+"."+store->machines[i]->name, QVariant(i));
	}
	*/
}


void Pattern::machineChange(int index) {
	/*
	int i = machineCombo->itemData(index).toInt();

	if (store->machines.contains(i)) {
		activeMachine = store->machines[i];
		cols.clear();
		foreach (QString name, activeMachine->params.keys()) cols << name;
		refreshPatterns();
	}
	*/
}


void Pattern::refreshPatterns() {
	/*
	if (!activeMachine) return;

	patternCombo->clear();

	foreach (int pattern, activeMachine->patterns.keys()) {
		if (activeMachine->patterns[pattern].contains(-1)) {
			patternCombo->addItem(activeMachine->patterns[pattern][-1]["name"], QVariant(pattern));
		} else {
			patternCombo->addItem(QString::number(pattern), QVariant(pattern));
		}
	}

	tableview->reset();

	activeCol(-1);
	*/
}


void Pattern::patternChange(int index) {
	tableview->refresh();
}

void Pattern::activeCol(int col)
{
	/*
	if (!activeMachine) return;

	if (col == -1) {
		detail->setTitle(activeMachine->name);
		detailDesc->setText(activeMachine->description);
		detailUnit->hide();
		detailMin->hide();
		detailMax->hide();
	} else {
		QString param = cols[col];
		detail->setTitle(param);
		detailDesc->setText(activeMachine->params[param]->description);
		detailUnit->show();
		detailUnit->setText("Unit: <b>" + QString::number(activeMachine->params[param]->type) + "</b>");
		detailMin->show();
		detailMin->setText("Min: <b>" + QString::number(activeMachine->params[param]->min) + "</b>");
		detailMax->show();
		detailMax->setText("Max: <b>" + QString::number(activeMachine->params[param]->max) + "</b>");
	}
	*/
}


void Pattern::addPattern()
{
	/*
	if (!activeMachine) return;

	int i;
	for (i = 0; i < 1024; i++) if (activeMachine->patterns.contains(i) == false) break;
	if (i < 1024) {
		activeMachine->patterns[i];
		refreshPatterns();
		patternCombo->setCurrentIndex(i);
		renPattern();
	} else {
		QMessageBox::critical(mainWindow, "Too much patterns", "You can have only 1024 patterns for each machine");
	}
	*/
}


void Pattern::renPattern()
{
	/*
	int index = patternCombo->currentIndex();
	if (!activeMachine or index < 0) return;
	
	QString oldName;
	int pattern = patternCombo->itemData(index).toInt();
	if (activeMachine->patterns[pattern].contains(-1) and activeMachine->patterns[pattern][-1].contains("name")) {
		oldName = activeMachine->patterns[pattern][-1]["name"];
	} else {
		oldName = QString::number(pattern);
	}

	bool ok;
	QString newName = QInputDialog::getText(this, "Rename Pattern", "Pattern Name:", QLineEdit::Normal, oldName, &ok);

	if (ok == true) {
		activeMachine->patterns[pattern][-1]["name"] = newName;
		mainWindow->refreshPatterns();
		patternCombo->setCurrentIndex(index);
	}
	*/
}


void Pattern::delPattern()
{
	/*
	if (!activeMachine or patternCombo->currentIndex() < 0) return;
	activeMachine->patterns.remove(patternCombo->itemData(patternCombo->currentIndex()).toInt());
	refreshPatterns();
	*/
}


int Pattern::currentPattern()
{
	return patternCombo->itemData(patternCombo->currentIndex()).toInt();
}
