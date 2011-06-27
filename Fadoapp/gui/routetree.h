#ifndef GUI_ROUTETREE_H
#define GUI_ROUTETREE_H

#include <QtGui>

class RouteTree;

#include "gui/route.h"
#include "../Fadolib/store.h"

class RouteTree : public QTreeWidget
{
	Q_OBJECT
	
	public:
		RouteTree(QWidget *parent, Route *route);

		Route *route;
		Store *store;

	private slots:
		void doubleclick(QTreeWidgetItem *item, int column);
};

#endif
