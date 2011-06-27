#include "core/settings.h"

Settings::Settings(Core *core)
{
	this->core = core;
	filename = "settings.xml";
	version = "1.0";
}

int Settings::load()
{
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly) == false) return 1;
	QByteArray xml = file.readAll();
	file.close();

	QDomDocument doc("settings");

	QString errorMsg;
	int errorLine, errorColumn;
	if (doc.setContent(xml, false, &errorMsg, &errorLine, &errorColumn) == false) return 2;

	if (doc.documentElement().attribute("version") != version) return 10;

	QDomNodeList rootNodes = doc.documentElement().childNodes();

	for (int i = 0; i < rootNodes.length(); i++) if (rootNodes.item(i).isElement()) {
		QDomElement el = rootNodes.item(i).toElement();
		if (el.tagName() == "window") {
			int m = el.attribute("m", "0").toInt();

			if (m) {
				core->mainWindow->showMaximized();
			} else {
				int x = el.attribute("x", "0").toInt();
				int y = el.attribute("y", "0").toInt();
				int w = el.attribute("w", "800").toInt();
				int h = el.attribute("h", "600").toInt();
				core->mainWindow->move(x, y);
				core->mainWindow->resize(w, h);
			}
		}
	}
}

void Settings::save()
{
	QDomDocument doc("settings");

	QDomElement root = doc.createElement("fado-settings");
	root.setAttribute("version", version);
	doc.appendChild(root);

	QDomElement el;
	
	el = doc.createElement("window");
	
	if (core->mainWindow->isMaximized()) {
		el.setAttribute("m",  1);
	} else {
		el.setAttribute("x", core->mainWindow->x());
		el.setAttribute("y", core->mainWindow->y());
		el.setAttribute("w", core->mainWindow->width());
		el.setAttribute("h", core->mainWindow->height());
	}

	root.appendChild(el);

	QFile file(filename);
	file.open(QIODevice::WriteOnly);
	file.write(doc.toByteArray());
	file.close();
}
