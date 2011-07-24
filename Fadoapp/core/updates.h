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

#ifndef CORE_UPDATES_H
#define CORE_UPDATES_H

#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDomDocument>
#include <QCoreApplication>

class Updates : public QObject
{
	Q_OBJECT

	public:
		Updates(QObject *parent = 0);
		void check();
		//! Compares two version numbers. Returns 1 if the second is more recent
		int versionCompare(QString v1, QString v2);

		enum UpdatesTypes { NoUpdates, AppUpdate, MachineUpdate, AppMachineUpdate };

		QNetworkAccessManager *manager;

	signals:
		void updateSignal(Updates::UpdatesTypes type, int num);

	public slots:
		void replyFinished(QNetworkReply*);
};

#endif
