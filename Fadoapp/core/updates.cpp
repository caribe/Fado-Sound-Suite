#include "core/updates.h"

Updates::Updates(Core *core)
{
	this->core = core;

	manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

int Updates::check()
{
	QUrl url("http://saitfainder.altervista.org/fado/updates.php");
	QNetworkRequest req(url);
	manager->get(req);
}


void Updates::replyFinished(QNetworkReply *reply)
{
	/*
	if (reply->error() == QNetworkReply::NoError) {
		QByteArray xml = reply->readAll();

		QDomDocument doc("");

		QString errorMsg;
		int errorLine, errorColumn;

		if (doc.setContent(xml, false, &errorMsg, &errorLine, &errorColumn) == false) {
			QMessageBox::critical(0, "XML Error", errorMsg + " at line " + QString::number(errorLine) + " column " + QString::number(errorColumn));
			return;
		}

		QString version;
		QHash<QString, QHash<QString, QHash<QString, QString> > > machines;

		QDomNodeList rootNodes = doc.documentElement().childNodes();
		for (int i = 0; i < rootNodes.length(); i++) if (rootNodes.item(i).isElement()) {
			QDomElement el = rootNodes.item(i).toElement();
			if (el.tagName() == "version") {
				version = el.text();
			} else if (el.tagName() == "machines") {
				QDomNodeList machinesNodes = el.childNodes();
				for (int i = 0; i < machinesNodes.length(); i++) if (machinesNodes.item(i).isElement()) {
					QDomElement machinesElement = machinesNodes.item(i).toElement();
					if (machinesElement.tagName() == "machine") {
						machines[machinesElement.attribute("type")][machinesElement.attribute("author")][machinesElement.attribute("name")] = machinesElement.text();
					}
				}
			}
		}

		int count = 0, flag = 0;
		flag = versionCompare(QCoreApplication::applicationVersion(), version) == 1 ? 1 : 0;

		foreach (QString type, machines.keys()) {
			if (core->store->info.contains(type)) foreach (QString author, machines[type].keys()) {
				if (core->store->info[type].contains(author)) foreach (QString name, machines[type][author].keys()) {
					if (core->store->info[type][author].contains(name)) {
						QStringList machineInfo = core->store->info[type][author][name]();
						if (versionCompare(machineInfo[4], machines[type][author][name]) == 1) count++;
					} else qDebug() << "Unkown machine `" << type << "." << author << "." << name << "`" << endl;
				} else qDebug() << "Unkown author `" << type << "." << author << "`" << endl;
			} else qDebug() << "Unkown type `" << type << "`" << endl;
		}

		if (flag) {
			QMessageBox::information(0, "Updates avaible", "A new version of Fado is avaible. Go and download it now.");
		}

		if (count) {
			QMessageBox::information(0, "Updates avaible", "New machines are avaible. Should i try to download them?");
		}
	}
	*/
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
