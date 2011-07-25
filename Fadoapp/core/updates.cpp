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

#include "core/updates.h"

Updates::Updates(QObject *parent) : QObject(parent)
{
	manager = new QNetworkAccessManager();
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}



void Updates::check()
{
	QUrl url("http://saitfainder.altervista.org/fado/updates.php");
	QNetworkRequest req(url);
	manager->get(req);
}


void Updates::replyFinished(QNetworkReply *reply)
{
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray xml = reply->readAll();

		QDomDocument doc;

		QString errorMsg;
		int errorLine, errorColumn;

		if (doc.setContent(xml, false, &errorMsg, &errorLine, &errorColumn) == false) {
			// QMessageBox::critical(0, "XML Error", errorMsg + " at line " + QString::number(errorLine) + " column " + QString::number(errorColumn));
			return;
		}

		QString version;
		QHash<QString, QHash<QString, QHash<QString, QString> > > machines;

		QDomElement rootEl = doc.documentElement();
		for (QDomElement el = rootEl.firstChildElement(); !el.isNull(); el = el.nextSiblingElement()) {
			if (el.tagName() == "version") {
				version = el.text();
			} else if (el.tagName() == "machines") {
				QDomNodeList machinesNodes = el.childNodes();
				for (unsigned int i = 0; i < machinesNodes.length(); i++) if (machinesNodes.item(i).isElement()) {
					QDomElement machinesElement = machinesNodes.item(i).toElement();
					if (machinesElement.tagName() == "machine") {
						machines[machinesElement.attribute("type")][machinesElement.attribute("author")][machinesElement.attribute("name")] = machinesElement.text();
					}
				}
			}
		}

		int count = 0, flag = 0;
		flag = versionCompare(QCoreApplication::applicationVersion(), version) == 1 ? 1 : 0;
/*
		foreach (QString type, machines.keys()) {
			if (core->store->info.contains(type)) foreach (QString author, machines[type].keys()) {
				if (core->store->info[type].contains(author)) foreach (QString name, machines[type][author].keys()) {
					if (core->store->info[type][author].contains(name)) {
						QStringList machineInfo = core->store->info[type][author][name]();
						if (versionCompare(machineInfo[4], machines[type][author][name]) == 1) count++;
					} else qDebug() << "Unkown machine `" << type << "." << author << "." << name << "`";
				} else qDebug() << "Unkown author `" << type << "." << author << "`";
			} else qDebug() << "Unkown type `" << type << "`";
		}
*/

		if (flag and count) {
			emit updateSignal(AppMachineUpdate, count);
		} else if (flag) {
			emit updateSignal(AppUpdate, 0);
		} else if (count) {
			emit updateSignal(MachineUpdate, count);
		} else {
			emit updateSignal(NoUpdates, 0);
		}
	}
}


int Updates::versionCompare(QString v1, QString v2)
{
	QStringList l1 = v1.split(".");
	QStringList l2 = v2.split(".");

	if (l1.size() > 0 and l2.size() > 0) {
		if (l1[0].toInt() > l2[0].toInt()) {
			return -1;
		} else if (l1[0].toInt() < l2[0].toInt()) {
			return 1;
		}
	}

	if (l1.size() > 1 and l2.size() > 1) {
		if (l1[1].toInt() > l2[1].toInt()) {
			return -1;
		} else if (l1[1].toInt() < l2[1].toInt()) {
			return 1;
		}
	}

	if (l1.size() > 2 and l2.size() > 2) {
		if (l1[2].toInt() > l2[2].toInt()) {
			return -1;
		} else if (l1[2].toInt() < l2[2].toInt()) {
			return 1;
		}
	}

	return 0;
}
