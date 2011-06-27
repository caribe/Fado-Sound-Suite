#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

#include <QtXml/QDomDocument>

#include "../Fadolib/store.h"
#include "../Fadolib/machine.h"
#include "core/master.h"
#include "core/lineinput.h"
#include "core/fileinput.h"

class Config {
	public:
		static int load(QString filename, Store *store);
		static int save(QString filename, Store *store);
};

#endif
