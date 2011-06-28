#ifndef PARAM_H
#define PARAM_H

#include <QString>

class Param
{
	public:
		enum types { int_t, float_t, freq_t, note_t, string_t };
		static const int notes[];

		QString name, description;
		int type;
		int min;
		int max;
		float floatValue;
		int intValue;
		QString strValue;

		Param(QString name, QString description, int type, int min, int max);
		Param(QString name, QString description, int type);

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
