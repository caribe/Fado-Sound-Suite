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
