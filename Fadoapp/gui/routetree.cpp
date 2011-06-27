#include "gui/routetree.h"

RouteTree::RouteTree(QWidget *parent, Route *route)
{
	this->route = route;
	setColumnCount(1);
	setHeaderLabel("Machines");

	store = route->store;

	foreach (QString t, store->factory.keys()) {
		QTreeWidgetItem *type = new QTreeWidgetItem(QStringList(t.left(1).toUpper()+t.mid(1)));
		type->setIcon(0, QIcon("icons/" + t + ".png"));
		foreach (QString g, store->factory[t].keys()) {
			QTreeWidgetItem *group = new QTreeWidgetItem(QStringList(g));
			foreach (QString i, store->factory[t][g].keys()) {
				group->addChild(new QTreeWidgetItem(QStringList(i)));
			}
			type->addChild(group);
		}
		addTopLevelItem(type);
	}

	QTreeWidgetItem *lineinput = new QTreeWidgetItem(QStringList("Input"));
	lineinput->setIcon(0, QIcon("icons/mic.png"));
	addTopLevelItem(lineinput);

	QTreeWidgetItem *fileinput = new QTreeWidgetItem(QStringList("File"));
	fileinput->setIcon(0, QIcon("icons/music.png"));
	addTopLevelItem(fileinput);

	connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem *, int)), this, SLOT(doubleclick(QTreeWidgetItem *, int)));
}


void RouteTree::doubleclick(QTreeWidgetItem *item, int column)
{
	QTreeWidgetItem *group = item->parent();
	if (group) {
		QTreeWidgetItem *type = group->parent();
		if (type) {
			route->newMachine(type->text(column), group->text(column), item->text(column));
		}
	} else if (indexOfTopLevelItem(item) == 2) {
		route->newMachine("generator", "Core", "lineinput");
	} else if (indexOfTopLevelItem(item) == 3) {
		route->newMachine("generator", "Core", "fileinput");
	}
}
