#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

#include <QtXml/QDomDocument>

#include "core/store.h"
#include "ext/machine.h"
#include "gears/master.h"
#include "gears/lineinput.h"
#include "gears/fileinput.h"

class Config {
	public:
		static int load(QString filename, Store *store);
		static int save(QString filename, Store *store);
};

#endif
