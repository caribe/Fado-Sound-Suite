#include "patterntabledata.h"

PatternTableData::PatternTableData(Pattern *pattern)
{
	this->pattern = pattern;
}


void PatternTableData::reload() {
	reset();
}


int PatternTableData::columnCount(const QModelIndex& parent) const
{
	/*
	if (pattern->activeMachine) {
		return pattern->activeMachine->params.size();
	} else {
		return 0;
	}
	*/
}



int PatternTableData::rowCount(const QModelIndex& parent) const
{
	return 16;
}



QVariant PatternTableData::data(const QModelIndex& index, int role) const
{
	/*
	if (index.isValid() == false) {
		return QVariant();
	} else if (role != Qt::DisplayRole and role != Qt::EditRole) { 
		return QVariant();
	}

	if (!pattern->activeMachine) return QVariant();

	int pat = pattern->currentPattern();
	int row = index.row();
	int col = index.column();
	QString colName = headerData(col, Qt::Horizontal, Qt::DisplayRole).toString();

	if (pattern->activeMachine->patterns.contains(pat)
		and pattern->activeMachine->patterns[pat].contains(row)
		and pattern->activeMachine->patterns[pat][row].contains(colName)
	) {
		return QVariant(pattern->activeMachine->patterns[pat][row][colName]);
	}
	*/
	return QVariant();
}


Qt::ItemFlags PatternTableData::flags(const QModelIndex & index) const
{
	/*
	QString paramName = pattern->cols[index.column()];

	if (pattern->activeMachine->params[paramName]->type == pattern->activeMachine->params[paramName]->note_t) {
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else {
		return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
	}
	*/
}


bool PatternTableData::setData(const QModelIndex& index, const QVariant & value, int role)
{
	/*
	int pat = pattern->currentPattern();
	int row = index.row();
	int col = index.column();

	QString colName = headerData(col, Qt::Horizontal, Qt::DisplayRole).toString();
	QString strValue = value.toString();

	strValue = pattern->activeMachine->validate(colName, strValue);

	if (strValue == "") {
		if (pattern->activeMachine->patterns[pat].contains(row)
			and pattern->activeMachine->patterns[pat][row].contains(colName)
		) {
			pattern->activeMachine->patterns[pat][row].remove(colName);
			if (pattern->activeMachine->patterns[pat][row].size() == 0) {
				pattern->activeMachine->patterns[pat].remove(row);
			}
		}
	} else {
		pattern->activeMachine->patterns[pat][row][colName] = strValue;
	}
	*/
}


QVariant PatternTableData::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole) { 
		return QVariant();
	} else if (orientation == Qt::Horizontal) {
		return QVariant(pattern->cols[section]);
	} else if (orientation == Qt::Vertical) {
		return QVariant(section);
	}
}

