#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

#include <QDomDocument>
#include <QDebug>

class Config;

#include "ext/machine.h"
#include "gears/master.h"
#include "gears/lineinput.h"
#include "gears/fileinput.h"

class Config {
	public:
		static int load(QString filename, Core *core);
		static int save(QString filename, Core *core);
};

#endif
