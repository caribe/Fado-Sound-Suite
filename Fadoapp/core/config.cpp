#include "core/config.h"

int Config::load(QString filename, Store *store) {
	QTextStream out(stdout);

	MachineFactory tm;
	Machine *tx;
	int track_first, track_last;

	// Caricamento file XML
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly) == false) return 1;
	QByteArray xml = file.readAll();
	file.close();

	QDomDocument doc("config");
	
	QString errorMsg;
	int errorLine, errorColumn;
	if (doc.setContent(xml, false, &errorMsg, &errorLine, &errorColumn) == false) {
		out << errorMsg << " at line " << errorLine << " column " << errorColumn << endl;
		return 2;
	}

	if (doc.documentElement().attribute("version") != "1.0") return 10;

	QDomNodeList rootNodes = doc.documentElement().childNodes();
	for (int i = 0; i < rootNodes.length(); i++) if (rootNodes.item(i).isElement()) {
		QDomElement rootElement = rootNodes.item(i).toElement();
		if (rootElement.tagName() == "settings") {
			QDomNodeList settingNodes = rootElement.childNodes();
			for (int j = 0; j < settingNodes.length(); j++) if (settingNodes.item(j).isElement()) {
				QDomElement settingElement = settingNodes.item(j).toElement();
				if (settingElement.tagName() == "beat-per-pattern") {
					store->beat_per_pattern = settingElement.text().toInt();
				} else if (settingElement.tagName() == "patterns") {
					store->total_patterns = settingElement.text().toInt();
					track_first = settingElement.attribute("first", "0").toInt();
					track_last  = settingElement.attribute("last", settingElement.text()).toInt();
				}
			}
		} else if (rootElement.tagName() == "machines") {
			QDomNodeList machineNodes = rootElement.childNodes();
			for (int j = 0; j < machineNodes.length(); j++) if (machineNodes.item(j).isElement()) {
				QDomElement machineElement = machineNodes.item(j).toElement();
				if (machineElement.tagName() == "machine") {

					QString type = machineElement.attribute("type");
					QString author = machineElement.attribute("author");
					QString name = machineElement.attribute("name");

					if (author == "Core") {
						if (name == "output") {
							tx = (Machine *)new Master();
						} else if (name == "lineinput") {
							tx = (Machine *)new LineInput();
						} else if (name == "fileinput") {
							tx = (Machine *)new FileInput();
						}
						tx->id = machineElement.attribute("id").toInt();
						tx->x = machineElement.attribute("x").toInt();
						tx->y = machineElement.attribute("y").toInt();
						tx->store = store;
						store->machines[tx->id]= tx;
					} else {
						if (store->factory[type].contains(author) && store->factory[type][author].contains(name)) {
							tx = store->factory[type][author][name]();
							tx->id = machineElement.attribute("id").toInt();
							tx->x = machineElement.attribute("x").toInt();
							tx->y = machineElement.attribute("y").toInt();
							tx->store = store;
							store->machines[tx->id]= tx;
						} else {
							out << "No machine called " << type << "." << author << "." << name << "\n";
							continue;
						}
					}

					QDomNodeList machineChilds = machineElement.childNodes();

					for (int k = 0; k < machineChilds.length(); k++) if (machineChilds.item(k).isElement()) {
						QDomElement machineChild = machineChilds.item(k).toElement();
						if (machineChild.tagName() == "patterns") {
							QDomNodeList patternChilds = machineChild.childNodes();
							for (int l = 0; l < patternChilds.length(); l++) if (patternChilds.item(l).isElement()) {
								QDomElement patternElement = patternChilds.item(l).toElement();
								if (patternElement.tagName() == "pattern") {
									QString patternName = patternElement.attribute("name", "");
									if (patternName != "") tx->patterns[patternElement.attribute("id").toInt()][-1]["name"] = patternName;
									QDomNodeList patternValues = patternElement.childNodes();
									for (int m = 0; m < patternValues.length(); m++) if (patternValues.item(m).isElement()) {
										QDomElement patternValue = patternValues.item(m).toElement();
										if (patternValue.tagName() == "row") {
											QDomNodeList patternParams = patternValue.childNodes();
											for (int n = 0; n < patternParams.length(); n++) if (patternParams.item(n).isElement()) {
												tx->patterns[patternElement.attribute("id").toInt()][patternValue.attribute("id").toInt()][patternParams.item(n).toElement().tagName()] = patternParams.item(n).toElement().text();
											}
										}
									}
								}
							}
						} else if (machineChild.tagName() == "tracks") {
							QDomNodeList tracksChilds = machineChild.childNodes();
							for (int l = 0; l < tracksChilds.length(); l++) if (tracksChilds.item(l).isElement()) {
								QDomElement trackElement = tracksChilds.item(l).toElement();
								if (trackElement.tagName() == "pattern") {
									tx->track[trackElement.attribute("row").toInt()] = trackElement.text().toInt();
								}
							}
						}
					}
				}
			}
		} else if (rootElement.tagName() == "connections") {
			QDomNodeList connectionNodes = rootElement.childNodes();
			for (int j = 0; j < connectionNodes.length(); j++) if (connectionNodes.item(j).isElement()) {
				QDomElement connectionElement = connectionNodes.item(j).toElement();
				if (connectionElement.tagName() == "connection") {
					store->connections[connectionElement.attribute("from").toInt()][connectionElement.attribute("to").toInt()] = connectionElement.attribute("value", "100").toInt();
				}
			}
		}
	}


	foreach (int i, store->machines.keys()) {
		Machine *m = store->machines[i];
		out << "Tracks " << m->name << endl;
		for (int j = 0; j < m->track.keys().length(); j++) {
			out << "\t" << m->track.keys().value(j) << "\t" << m->track.value(m->track.keys().value(j)) << endl;
		}
	}

	foreach (int i, store->machines.keys()) {
		Machine *m = store->machines[i];
		out << "Patterns " << m->name << endl;
		m->patterns.size();

		for (int j = 0; j < m->patterns.keys().length(); j++) {
			out << "\t" << m->patterns.keys().value(j) << endl;
		}
	}

	Master *master = (Master *)store->machines[0];
	master->track_first = track_first;
	master->track_last = track_last;

	out << "Beat per pattern: " << store->beat_per_pattern << endl;

	return 0;
}


int Config::save(QString filename, Store *store) {
	QTextStream out(stdout);
	Master *master = (Master *)store->machines[0];

	QDomDocument doc("config");

	QDomElement root = doc.createElement("fado");
	root.setAttribute("version", "1.0");
	doc.appendChild(root);

	QDomElement settings = doc.createElement("settings");
	root.appendChild(settings);

	QDomElement settingsPatterns = doc.createElement("patterns");
	settingsPatterns.setAttribute("first", QString::number(master->track_first));
	settingsPatterns.setAttribute("last", QString::number(master->track_last));
	settingsPatterns.appendChild(doc.createTextNode(QString::number(store->total_patterns)));
	settings.appendChild(settingsPatterns);

	QDomElement settingsBPM = doc.createElement("beat-per-pattern");
	settingsBPM.appendChild(doc.createTextNode(QString::number(store->beat_per_pattern)));
	settings.appendChild(settingsBPM);

	QDomElement machines = doc.createElement("machines");
	root.appendChild(machines);

	foreach (Machine *machine, store->machines.values()) {
		QDomElement machine_el = doc.createElement("machine");
		machine_el.setAttribute("id", machine->id);
		machine_el.setAttribute("x", machine->x);
		machine_el.setAttribute("y", machine->y);
		machine_el.setAttribute("type", machine->type);
		machine_el.setAttribute("author", machine->author);
		machine_el.setAttribute("name", machine->name);

		QDomElement patterns_el = doc.createElement("patterns");
		foreach (int pattern, machine->patterns.keys()) {
			QDomElement pattern_el = doc.createElement("pattern");
			pattern_el.setAttribute("id", pattern);
			foreach (int row, machine->patterns[pattern].keys()) {
				if (row == -1) {
					pattern_el.setAttribute("name", machine->patterns[pattern][row]["name"]);
				} else {
					QDomElement row_el = doc.createElement("row");
					row_el.setAttribute("id", row);
					pattern_el.appendChild(row_el);
					foreach (QString value, machine->patterns[pattern][row].keys()) {
						QDomElement value_el = doc.createElement(value);
						value_el.appendChild(doc.createTextNode(machine->patterns[pattern][row][value]));
						row_el.appendChild(value_el);
					}
				}
			}
			patterns_el.appendChild(pattern_el);
		}
		machine_el.appendChild(patterns_el);

		QDomElement tracks_el = doc.createElement("tracks");
		foreach (int track, machine->track.keys()) {
			QDomElement track_el = doc.createElement("pattern");
			track_el.setAttribute("row", track);
			track_el.appendChild(doc.createTextNode(QString::number(machine->track[track])));
			tracks_el.appendChild(track_el);
		}
		machine_el.appendChild(tracks_el);

		machines.appendChild(machine_el);
	}

	QDomElement connections_el = doc.createElement("connections");
	foreach (int from, store->connections.keys()) {
		foreach (int to, store->connections[from].keys()) {
			QDomElement connection_el = doc.createElement("connection");
			connection_el.setAttribute("from", from);
			connection_el.setAttribute("to", to);
			connection_el.setAttribute("value", store->connections[from][to]);
			connections_el.appendChild(connection_el);
		}
	}
	root.appendChild(connections_el);

	QFile file(filename);
	file.open(QIODevice::WriteOnly);
	file.write(doc.toByteArray());
	file.close();

	return 0;
}
