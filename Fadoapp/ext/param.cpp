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

#include "param.h"

const int Param::notes[] = {
	66, 70, 74, 78, 83, 88, 93, 98, 104, 110, 117, 124,
	131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, 247,
	262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 
	523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988, 
	1046, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865, 1976, 
	2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729, 3951,
	4186
};


Param::Param(QString name, QString description, Param::types type, int min, int max)
{
	this->name = name;
	this->description = description;
	this->type = type;
	this->min = min;
	this->max = max;
}


Param::Param(QString name, QString description, Param::types type)
{
	Param(name, description, type, 0, 0);
}


void Param::setFloat(float value)
{
	if (value > max) value = max;
	else if (value < min) value = min;
	floatValue = value;
}


void Param::setInt(int value)
{
	if (value > max) value = max;
	else if (value < min) value = min;
	intValue = value;
}


void Param::set(int value)
{
	if (this->type == ParamInt) {
		return setInt(value);
	} else if (this->type == ParamFloat) {
		return setFloat(value);
	}
}


void Param::set(double value)
{
	if (this->type == ParamInt) {
		return setInt(value);
	} else if (this->type == ParamFloat) {
		return setFloat(value);
	}
}


void Param::set(float value)
{
	if (this->type == ParamInt) {
		return setInt(value);
	} else if (this->type == ParamFloat) {
		return setFloat(value);
	}
}


void Param::set(QString value)
{
	if (this->type == ParamInt) {
		return setInt(value.toInt());
	} else if (this->type == ParamFloat) {
		return setFloat(value.toFloat());
	} else {
		strValue = value;
	}
}


int Param::getInt()
{
	return intValue;
}


float Param::getFloat()
{
	return floatValue;
}


QString Param::getString()
{
	return strValue;
}
