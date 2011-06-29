#ifndef CORE_UPDATES_H
#define CORE_UPDATES_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QCoreApplication>

class Updates;

#include "core/core.h"

class Updates : public QObject
{
	Q_OBJECT

	public:
		Updates(Core *core);
		int check();
		//! Compares two version numbers. Returns 1 if the second is more recent
		int versionCompare(QString v1, QString v2);

		Core *core;

		QNetworkAccessManager *manager;
		
	public slots:
		void replyFinished(QNetworkReply*);
};

#endif
