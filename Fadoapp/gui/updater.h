#ifndef UPDATER_H
#define UPDATER_H

#include <QDialog>
#include <QPushButton>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QStackedLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QDialogButtonBox>

#include "core/core.h"

class Updater : public QDialog
{
    Q_OBJECT
public:
	explicit Updater(Core *core, QWidget *parent = 0);

private:
	Core *core;
	QNetworkAccessManager *manager;
	bool searchMachine(Machine::MachineType type, const QString &author, const QString &name);
	int versionCompare(QString v1, QString v2);
	QUrl requestUrl;
	QListWidget *listListWidget;
	QStackedLayout *layout;

signals:
	void analyzeXmlResponseSignal(QNetworkReply *reply);

private slots:
	void replyFinished(QNetworkReply*);
	void check();
	void analyzeXmlResponse(QNetworkReply *reply);
	void downloadMachines();

public slots:

};

#endif // UPDATER_H
