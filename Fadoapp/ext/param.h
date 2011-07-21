/*
 * Fado Sound Suite - Modular synthetic music generator and sound processor
 *
 * Copyright (C) 2011 Vincenzo Buttazzo <vbuttazzo@yahoo.com>
 *
 * This file is part of Fado Sound Suite.
 *
 * Fado Sound Suite is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fado Sound Suite is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fado Sound Suite.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef PARAM_H
#define PARAM_H

#include <QString>

class Param
{
	public:
		enum types { ParamInt, ParamFloat, ParamFreq, ParamNote, ParamString };
		static const int notes[];

		QString name, description;
		types type;
		int min;
		int max;
		float floatValue;
		int intValue;
		QString strValue;

		Param(QString name, QString description, types type, int min, int max);
		Param(QString name, QString description, types type);
		Param() {}

		void setInt(int value);
		void setFloat(float value);
		
		void set(int value);
		void set(float value);
		void set(double value);
		void set(QString value);

		int getInt();
		float getFloat();
		QString getString();
};

#endif
