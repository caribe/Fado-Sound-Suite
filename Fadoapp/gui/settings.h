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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSettings>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QListWidget>
#include <QDebug>

#include "portaudio.h"

class Settings : public QDialog
{
    Q_OBJECT

	QLineEdit *tempFolder, *pluginsFolder;
	QTableWidget *devicesInputWidget, *devicesOutputWidget;

public:
    explicit Settings(QWidget *parent = 0);

signals:

private slots:
	void tempFolderBrowseSlot();
	void pluginsFolderBrowseSlot();

public slots:
	void acceptAction();
};

#endif // SETTINGS_H
