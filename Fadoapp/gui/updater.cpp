#include "updater.h"

Updater::Updater(Core *core, QWidget *parent) : QDialog(parent)
{
	resize(320, 240);
	this->core = core;
	requestUrl.setUrl("http://saitfainder.altervista.org/fado/service.updates/");

	manager = new QNetworkAccessManager();
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

	// Confirm Widget

	QPushButton *initUpdateButton = new QPushButton(QIcon(":update"), tr("Update now!"));
	QPushButton *initCancelButton = new QPushButton(QIcon(":/icons/cross.png"), tr("Cancel"));

	QDialogButtonBox *initButtonBox = new QDialogButtonBox();
	initButtonBox->addButton(initUpdateButton, QDialogButtonBox::AcceptRole);
	initButtonBox->addButton(initCancelButton, QDialogButtonBox::RejectRole);
	connect(initButtonBox, SIGNAL(accepted()), SLOT(check()));
	connect(initButtonBox, SIGNAL(rejected()), SLOT(reject()));

	QVBoxLayout *initLayout = new QVBoxLayout();
	initLayout->addStretch(1);
	initLayout->addWidget(initButtonBox);

	QWidget *initWidget = new QWidget();
	initWidget->setLayout(initLayout);

	// Machine update list

	listListWidget = new QListWidget();

	QVBoxLayout *listLayout = new QVBoxLayout();
	listLayout->addWidget(listListWidget, 1);

	QPushButton *listUpdateButton = new QPushButton(QIcon(":update"), tr("Download"));
	QPushButton *listCancelButton = new QPushButton(QIcon(":/icons/cross.png"), tr("Cancel"));

	QDialogButtonBox *listButtonBox = new QDialogButtonBox();
	listButtonBox->addButton(listUpdateButton, QDialogButtonBox::AcceptRole);
	listButtonBox->addButton(listCancelButton, QDialogButtonBox::RejectRole);
	connect(listButtonBox, SIGNAL(accepted()), SLOT(check()));
	connect(listButtonBox, SIGNAL(rejected()), SLOT(reject()));

	listLayout->addWidget(listButtonBox);

	QWidget *listWidget = new QWidget();
	listWidget->setLayout(listLayout);

	// Layout

	layout = new QStackedLayout();
	layout->addWidget(initWidget);
	layout->addWidget(listWidget);

	layout->setCurrentIndex(0);

	setLayout(layout);

	// Connections

	connect(this, SIGNAL(analyzeXmlResponseSignal(QNetworkReply*)), SLOT(analyzeXmlResponse(QNetworkReply*)));
}


void Updater::check()
{
	QNetworkRequest req(requestUrl);
	manager->get(req);
}



void Updater::replyFinished(QNetworkReply *reply)
{
	if (reply->error() == QNetworkReply::NoError) {

		if (reply->url() == requestUrl) {
			emit analyzeXmlResponseSignal(reply);
		}
	}
}



void Updater::analyzeXmlResponse(QNetworkReply *reply)
{
	layout->setCurrentIndex(1);

	QByteArray xml = reply->readAll();

	QDomDocument doc;

	QString errorMsg;
	int errorLine, errorColumn;

	if (doc.setContent(xml, false, &errorMsg, &errorLine, &errorColumn) == false) {
		// QMessageBox::critical(0, "XML Error", errorMsg + " at line " + QString::number(errorLine) + " column " + QString::number(errorColumn));
		return;
	}

	QString version;
	int count, flag;

	QDomElement rootEl = doc.documentElement();
	for (QDomElement el = rootEl.firstChildElement(); !el.isNull(); el = el.nextSiblingElement()) {
		if (el.tagName() == "version") {
			if (versionCompare(QCoreApplication::applicationVersion(), el.text()) == 1) {
				QMessageBox::information(this, tr("Updates avaible"), tr("A new version of Fado is avaible. Go and download it now."));
			}
		} else if (el.tagName() == "machines") {
			for (QDomElement elMachine = el.firstChildElement("machine"); !elMachine.isNull(); elMachine = elMachine.nextSiblingElement("machine")) {
				Machine::MachineType type = Machine::MachineNull;
				QIcon icon;

				if (elMachine.attribute("type") == "generator") {
					type = Machine::MachineGenerator;
					icon = QIcon(":/icons/gear-small.png");
				} else if (elMachine.attribute("type") == "effect") {
					type = Machine::MachineEffect;
					icon = QIcon(":/icons/funnel-small.png");
				}

				if (type != Machine::MachineNull and searchMachine(type, elMachine.attribute("author"), elMachine.attribute("name")) == false) {
					QListWidgetItem *item = new QListWidgetItem(icon, elMachine.attribute("author")+" / "+elMachine.attribute("name"), listListWidget);
					item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
					item->setCheckState(Qt::Checked);
					item->setData(Qt::UserRole+1, elMachine.attribute("member_id"));
					item->setData(Qt::UserRole+2, elMachine.attribute("code"));
				}
			}

			if (listListWidget->count() == 0) {
				QMessageBox::Information(this, tr("No updates"), tr("There are no updatable machines"));
				this->reject();
			}
		}
	}

	reply->deleteLater();
}


void Updater::downloadMachines()
{
	layout->setCurrentIndex(2);
}


int Updater::versionCompare(QString v1, QString v2)
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


bool Updater::searchMachine(Machine::MachineType type, const QString &author, const QString &name)
{
	foreach (Machine *machine, core->machines) {
		if (machine->type == type and machine->author == author and machine->name == name) {
			return true;
		}
	}

	return false;
}
