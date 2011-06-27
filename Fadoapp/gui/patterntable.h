#ifndef GUI_PATTERTABLE_H
#define GUI_PATTERTABLE_H

#include <QtGui>

class PatternTable;

#include "gui/pattern.h"
#include "gui/patterntabledata.h"

class PatternTable : public QTableView
{
	Q_OBJECT

	public:
		Pattern *pattern;
		PatternTable(Pattern *pattern);

		PatternTableData *model;
		void refresh();

		void keyPressEvent(QKeyEvent *event);
		
		QList<int> keyMap;
		QList<QString> notes;

	private slots:
		void currentColumnChangedSlot(const QModelIndex &current, const QModelIndex &previous);
};

#endif
