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

#include "settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent)
{
	QFormLayout *layout = new QFormLayout;

	QSettings settings;

	// Fields

	tempFolder = new QLineEdit(settings.value("settings/tempFolder", "/tmp").toString());
	QPushButton *tempFolderBrowse = new QPushButton(tr("Browse..."));
	QHBoxLayout *tempFolderLayout = new QHBoxLayout();
	tempFolderLayout->addWidget(tempFolder, 1);
	tempFolderLayout->addWidget(tempFolderBrowse);
	layout->addRow(tr("Temporary file folder"), tempFolderLayout);
	connect(tempFolderBrowse, SIGNAL(clicked()), SLOT(tempFolderBrowseSlot()));

	pluginsFolder = new QLineEdit(settings.value("settings/pluginsFolder", ".").toString());
	QPushButton *pluginsFolderBrowse = new QPushButton(tr("Browse..."));
	QHBoxLayout *pluginsFolderLayout = new QHBoxLayout();
	pluginsFolderLayout->addWidget(pluginsFolder, 1);
	pluginsFolderLayout->addWidget(pluginsFolderBrowse);
	layout->addRow(tr("Plugins folder"), pluginsFolderLayout);
	connect(pluginsFolderBrowse, SIGNAL(clicked()), SLOT(pluginsFolderBrowseSlot()));

	// Buttons

	QPushButton *saveButton = new QPushButton(QIcon(":/icons/tick.png"), tr("&Save"));
	saveButton->setDefault(true);

	QPushButton *cancelButton = new QPushButton(QIcon(":/icons/cross.png"), tr("&Cancel"));

	QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
	buttonBox->addButton(saveButton, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

	layout->addRow(buttonBox);

	connect(buttonBox, SIGNAL(accepted()), SLOT(acceptAction()));
	connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));

	// Finish

	setLayout(layout);
}


void Settings::acceptAction()
{
	QSettings settings;

	QFileInfo tempInfo(tempFolder->text());
	QFileInfo pluginsInfo(pluginsFolder->text());

	if (!tempInfo.isWritable()) {
		QMessageBox::critical(this, tr("Error"), tr("The choosen temporary file folder is not writeable or doesn't exists."));
	} else if (!pluginsInfo.isWritable()) {
		QMessageBox::critical(this, tr("Error"), tr("The choosen plugins folder is not writeable or doesn't exists."));
	} else {
		settings.setValue("settings/tempFolder", tempFolder->text());
		settings.setValue("settings/pluginsFolder", pluginsFolder->text());
		accept();
	}
}


void Settings::tempFolderBrowseSlot()
{
	QString folder = QFileDialog::getExistingDirectory(this, tr("Choose temporary file folder"), tempFolder->text());
	if (!folder.isEmpty()) tempFolder->setText(folder);
}


void Settings::pluginsFolderBrowseSlot()
{
	QString folder = QFileDialog::getExistingDirectory(this, tr("Choose plugins folder"), pluginsFolder->text());
	if (!folder.isEmpty()) pluginsFolder->setText(folder);
}
