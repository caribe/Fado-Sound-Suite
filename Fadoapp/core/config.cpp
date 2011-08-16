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
 * aunsigned long with Fado Sound Suite.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "core/config.h"

int Config::load(QString filename, Core *core) {

	qDebug() << "Ready to load" << filename;

	// Salva la cartella corrente
	QFileInfo fileinfo(filename);
	QSettings settings;
	settings.setValue("state/cwd", fileinfo.path());

	// Caricamento file XML
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly) == false) return 1;
	QByteArray xml = file.readAll();
	file.close();

	qDebug() << "Ready to analyze";

	QDomDocument doc("config");
	
	QString errorMsg;
	int errorLine, errorColumn;
	if (doc.setContent(xml, false, &errorMsg, &errorLine, &errorColumn) == false) {
		qDebug() << errorMsg << " at line " << errorLine << " column " << errorColumn;
		return 2;
	}

	if (doc.documentElement().attribute("version") != "1.0") return 10;

	for (QDomElement rootElement = doc.documentElement().firstChildElement(); !rootElement.isNull(); rootElement = rootElement.nextSiblingElement()) {

		if (rootElement.tagName() == "settings") {

			QDomElement beatPerPattern = rootElement.firstChildElement("beat-per-pattern");
			if (!beatPerPattern.isNull()) core->beat_per_pattern = beatPerPattern.text().toInt();

			QDomElement beatPerMinute = rootElement.firstChildElement("beat-per-minute");
			if (!beatPerMinute.isNull()) core->baseBpm = beatPerMinute.text().toInt();

			QDomElement patterns = rootElement.firstChildElement("patterns");
			if (!patterns.isNull()) {
				core->total_patterns = patterns.text().toInt();
				core->track_first = patterns.attribute("first", "0").toInt();
				core->track_last  = patterns.attribute("last", patterns.text()).toInt();
			}

		} else if (rootElement.tagName() == "machine") {

			Machine *machine = NULL;

			QString type = rootElement.attribute("type");
			QString author = rootElement.attribute("author");
			QString name = rootElement.attribute("name");

			if (type == "master") {
				machine = (Machine *)new Master();
			} else if (type == "input") {
				if (name == "Line Input") {
					machine = (Machine *)new LineInput();
				} else if (name == "File Input") {
					machine = (Machine *)new FileInput();
				}
			} else {
				QStandardItem *generators;
				if (type == "generator") {
					generators = core->gearsTree->invisibleRootItem()->child(1);
				} else if (type == "effect") {
					generators = core->gearsTree->invisibleRootItem()->child(2);
				}

				if (generators->rowCount()) {
					for (int i = 0; i < generators->rowCount(); i++) {
						if (generators->child(i)->text() == author) {
							for (int j = 0; j < generators->child(i)->rowCount(); j++) {
								if (generators->child(i)->child(j)->text() == name) {
									int id = generators->child(i)->child(j)->data(Qt::UserRole + 1).toInt();
									machine = core->gears[id]->factory();
									break;
								}
							}
							break;
						}
					}
				}
			}

			if (machine == NULL) {
				qDebug() << "Machine not found";
				return 1;
			} else {

				machine->alias = rootElement.attribute("alias");
				machine->x = rootElement.attribute("x").toInt();
				machine->y = rootElement.attribute("y").toInt();

				core->machines << machine;

				QHash<QString, MachinePattern *> patternMap;

				QDomElement patternsEl = rootElement.firstChildElement("patterns");
				if (!patternsEl.isNull()) {
					for (QDomElement patternEl = patternsEl.firstChildElement("pattern"); !patternEl.isNull(); patternEl = patternEl.nextSiblingElement("pattern")) {
						MachinePattern *pattern = new MachinePattern();
						pattern->name = patternEl.attribute("name");
						patternMap[patternEl.attribute("id")] = pattern;
						for (QDomElement patternRowEl = patternEl.firstChildElement("row"); !patternRowEl.isNull(); patternRowEl = patternRowEl.nextSiblingElement("row")) {
							int row = patternRowEl.attribute("row", "0").toInt();
							pattern->params[row].empty();
							for (QDomElement patternRowValueEl = patternRowEl.firstChildElement("value"); !patternRowValueEl.isNull(); patternRowValueEl = patternRowValueEl.nextSiblingElement("value")) {
								int i = patternRowValueEl.attribute("i").toInt();
								pattern->params[row][i] = patternRowValueEl.text();
							}
						}
						machine->patterns << pattern;
					}
				}

				QDomElement trackEl = rootElement.firstChildElement("track");
				if (!trackEl.isNull()) {
					for (QDomElement patternEl = trackEl.firstChildElement("pattern"); !patternEl.isNull(); patternEl = patternEl.nextSiblingElement("pattern")) {
						int row = patternEl.attribute("row").toInt();
						QString type = patternEl.attribute("type");
						if (type == "mute") {
							machine->track[row] = core->mutePattern;
						} else if (type == "break") {
							machine->track[row] = core->breakPattern;
						} else {
							machine->track[row] = patternMap[patternEl.attribute("pattern")];
						}
					}
				}
			}

		} else if (rootElement.tagName() == "connections") {

			for (QDomElement connectionEl = rootElement.firstChildElement("connection"); !connectionEl.isNull(); connectionEl = connectionEl.nextSiblingElement("connection")) {
				Volume *volume = new Volume();
				volume->lx = connectionEl.attribute("lx").toInt();
				volume->rx = connectionEl.attribute("rx").toInt();
				Machine *src = core->machines[connectionEl.attribute("from").toInt()];
				Machine *dst = core->machines[connectionEl.attribute("to").toInt()];
				src->connectionDst[dst] = volume;
				dst->connectionSrc[src] = volume;
			}

		}
	}

	return 0;
}


int Config::save(QString filename, Core *core) {

	// Salva la cartella corrente
	QFileInfo fileinfo(filename);
	QSettings qsettings;
	qsettings.setValue("state/cwd", fileinfo.path());

	QDomDocument doc("config");

	QDomElement root = doc.createElement("fado");
	root.setAttribute("version", "1.0");
	doc.appendChild(root);

	QDomElement settings = doc.createElement("settings");
	root.appendChild(settings);

	QDomElement settingsPatterns = doc.createElement("patterns");
	settingsPatterns.setAttribute("first", QString::number(core->track_first));
	settingsPatterns.setAttribute("last", QString::number(core->track_last));
	settingsPatterns.appendChild(doc.createTextNode(QString::number(core->total_patterns)));
	settings.appendChild(settingsPatterns);

	QDomElement settingsBPP = doc.createElement("beat-per-pattern");
	settingsBPP.appendChild(doc.createTextNode(QString::number(core->beat_per_pattern)));
	settings.appendChild(settingsBPP);

	QDomElement settingsBPM = doc.createElement("beat-per-minute");
	settingsBPM.appendChild(doc.createTextNode(QString::number(core->baseBpm)));
	settings.appendChild(settingsBPM);

	QHash<Machine *, int> machineMapping;
	int nextMachineId = 0;

	foreach (Machine *machine, core->machines) {
		machineMapping[machine] = nextMachineId++;
	}

	QDomElement connectionsEl = doc.createElement("connections");

	foreach (Machine *machine, core->machines) {
		QDomElement machine_el = doc.createElement("machine");
		machine_el.setAttribute("id", machineMapping[machine]);
		machine_el.setAttribute("x", machine->x);
		machine_el.setAttribute("y", machine->y);
		if (machine->type == Machine::MachineMaster) {
			machine_el.setAttribute("type", "master");
		} else if (machine->type == Machine::MachineInput) {
			machine_el.setAttribute("type", "input");
		} else if (machine->type == Machine::MachineGenerator) {
			machine_el.setAttribute("type", "generator");
		} else if (machine->type == Machine::MachineEffect) {
			machine_el.setAttribute("type", "effect");
		}
		machine_el.setAttribute("author", machine->author);
		machine_el.setAttribute("name", machine->name);
		if (!machine->alias.isNull()) machine_el.setAttribute("alias", machine->alias);

		QDomElement patterns_el = doc.createElement("patterns");
		int nextPatternId = 0;
		QHash<MachinePattern *, QString> patternMapping;

		foreach (MachinePattern *pattern, machine->patterns) {
			QString patternId = QString("%1-%2").arg(machineMapping[machine]).arg(nextPatternId++);
			patternMapping[pattern] = patternId;

			QDomElement pattern_el = doc.createElement("pattern");
			pattern_el.setAttribute("id", patternId);
			pattern_el.setAttribute("name", pattern->name);

			if (pattern->type == MachinePattern::StandardPattern) {
				foreach (int row, pattern->params.keys()) {
					QDomElement patternrow_el = doc.createElement("row");
					patternrow_el.setAttribute("row", row);
					foreach (int paramKey, pattern->params[row].keys()) {
						QDomElement value_el = doc.createElement("value");
						value_el.setAttribute("i", QString::number(paramKey));
						value_el.appendChild(doc.createTextNode(pattern->params[row][paramKey]));
						patternrow_el.appendChild(value_el);
					}
					pattern_el.appendChild(patternrow_el);
				}
			}
			patterns_el.appendChild(pattern_el);
		}

		machine_el.appendChild(patterns_el);

		// Tracks
		QDomElement tracksEl = doc.createElement("track");
		foreach (int track, machine->track.keys()) {
			MachinePattern *pattern = machine->track[track];
			if (pattern == NULL) continue;

			QDomElement trackEl = doc.createElement("pattern");
			trackEl.setAttribute("row", track);

			if (pattern->type == MachinePattern::MutePattern) {
				trackEl.setAttribute("type", "mute");
			} else if (pattern->type == MachinePattern::BreakPattern) {
				trackEl.setAttribute("type", "break");
			} else {
				trackEl.setAttribute("pattern", patternMapping[pattern]);
			}

			tracksEl.appendChild(trackEl);
		}
		machine_el.appendChild(tracksEl);

		// Connections
		foreach (Machine *dst, machine->connectionDst.keys()) {
			QDomElement connectionEl = doc.createElement("connection");
			connectionEl.setAttribute("from", machineMapping[machine]);
			connectionEl.setAttribute("to", machineMapping[dst]);
			connectionEl.setAttribute("lx", machine->connectionDst[dst]->lx);
			connectionEl.setAttribute("rx", machine->connectionDst[dst]->rx);
			connectionsEl.appendChild(connectionEl);
		}

		root.appendChild(machine_el);
	}

	root.appendChild(connectionsEl);

	QFile file(filename);
	file.open(QIODevice::WriteOnly);
	file.write(doc.toByteArray());
	file.close();

	return 0;
}
