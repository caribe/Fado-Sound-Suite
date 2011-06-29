#ifndef GUI_PATTERNTABLEDATA_H
#define GUI_PATTERNTABLEDATA_H

#include <QAbstractTableModel>

class PatternTableData;

#include "pattern.h"

class PatternTableData : public QAbstractTableModel
{
	Q_OBJECT

	public:
		Pattern *pattern;

		PatternTableData(Pattern *pattern);
		int rowCount(const QModelIndex& parent = QModelIndex()) const;
		int columnCount(const QModelIndex& parent = QModelIndex()) const;
		QVariant data(const QModelIndex& index, int role) const;
		bool setData(const QModelIndex& index, const QVariant & value, int role);

		void reload();

		QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		Qt::ItemFlags flags(const QModelIndex & index) const;
};

#endif
