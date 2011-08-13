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

#include "settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent)
{
	QSettings settings;

	QVBoxLayout *layout = new QVBoxLayout();
	QTabWidget *tabs = new QTabWidget();

	// Directories

	QFormLayout *dirLayout = new QFormLayout;

	tempFolder = new QLineEdit(settings.value("settings/tempFolder", "/tmp").toString());
	QPushButton *tempFolderBrowse = new QPushButton(tr("Browse..."));
	QHBoxLayout *tempFolderLayout = new QHBoxLayout();
	tempFolderLayout->addWidget(tempFolder, 1);
	tempFolderLayout->addWidget(tempFolderBrowse);
	dirLayout->addRow(tr("Temporary file folder"), tempFolderLayout);
	connect(tempFolderBrowse, SIGNAL(clicked()), SLOT(tempFolderBrowseSlot()));

	pluginsFolder = new QLineEdit(settings.value("settings/pluginsFolder", ".").toString());
	QPushButton *pluginsFolderBrowse = new QPushButton(tr("Browse..."));
	QHBoxLayout *pluginsFolderLayout = new QHBoxLayout();
	pluginsFolderLayout->addWidget(pluginsFolder, 1);
	pluginsFolderLayout->addWidget(pluginsFolderBrowse);
	dirLayout->addRow(tr("Plugins folder"), pluginsFolderLayout);
	connect(pluginsFolderBrowse, SIGNAL(clicked()), SLOT(pluginsFolderBrowseSlot()));

	QWidget *dirWidget = new QWidget();
	dirWidget->setLayout(dirLayout);
	tabs->addTab(dirWidget, tr("Directories"));

	// Output Devices

	int devices = Pa_GetDeviceCount();

	devicesOutputWidget = new QTableWidget();
	devicesOutputWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	devicesOutputWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	devicesOutputWidget->setColumnCount(2);
	devicesOutputWidget->setRowCount(devices);

	devicesOutputWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Device")));
	devicesOutputWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Name")));

	QFont italic = QFont();
	italic.setItalic(true);

	int outputDevicesCount = 0;
	int selectedOutputDevice = settings.value("settings/outputDevice", QVariant(Pa_GetDefaultOutputDevice())).toInt();

	for (int i = 0; i < devices; i++) {
		const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);

		if (deviceInfo->maxOutputChannels > 0) {
			devicesOutputWidget->setItem(outputDevicesCount, 0, new QTableWidgetItem(QString::number(i)));

			QTableWidgetItem *itemName = new QTableWidgetItem(deviceInfo->name);
			if (i == Pa_GetDefaultOutputDevice()) itemName->setFont(italic);
			devicesOutputWidget->setItem(outputDevicesCount, 1, itemName);

			if (i == selectedOutputDevice) devicesOutputWidget->selectRow(outputDevicesCount);

			outputDevicesCount++;
		}
	}

	devicesOutputWidget->setRowCount(outputDevicesCount);

	tabs->addTab(devicesOutputWidget, tr("Output Devices"));

	// Output Devices

	devicesInputWidget = new QTableWidget();
	devicesInputWidget->setSelectionMode(QAbstractItemView::SingleSelection);
	devicesInputWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	devicesInputWidget->setColumnCount(2);
	devicesInputWidget->setRowCount(devices);

	devicesInputWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("Device")));
	devicesInputWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Name")));

	int inputDevicesCount = 0;
	int selectedInputDevice = settings.value("settings/inputDevice", QVariant(Pa_GetDefaultInputDevice())).toInt();

	for (int i = 0; i < devices; i++) {
		const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);

		if (deviceInfo->maxInputChannels > 0) {
			devicesInputWidget->setItem(inputDevicesCount, 0, new QTableWidgetItem(QString::number(i)));

			QTableWidgetItem *itemName = new QTableWidgetItem(deviceInfo->name);
			if (i == Pa_GetDefaultInputDevice()) itemName->setFont(italic);
			devicesInputWidget->setItem(inputDevicesCount, 1, itemName);

			if (i == selectedInputDevice) devicesInputWidget->selectRow(inputDevicesCount);

			inputDevicesCount++;
		}
	}

	devicesInputWidget->setRowCount(inputDevicesCount);

	devicesInputWidget->selectRow(settings.value("settings/inputDevice", QVariant(Pa_GetDefaultInputDevice())).toInt());

	tabs->addTab(devicesInputWidget, tr("Input Devices"));

	// Buttons

	QPushButton *saveButton = new QPushButton(QIcon(":/icons/tick.png"), tr("&Save"));
	saveButton->setDefault(true);

	QPushButton *cancelButton = new QPushButton(QIcon(":/icons/cross.png"), tr("&Cancel"));

	QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
	buttonBox->addButton(saveButton, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

	connect(buttonBox, SIGNAL(accepted()), SLOT(acceptAction()));
	connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));

	// Finish

	layout->addWidget(tabs, 1);
	layout->addWidget(buttonBox);
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
		QTableWidgetItem *outputDevice = devicesOutputWidget->selectedItems().at(0);
		settings.setValue("settings/outputDevice", outputDevice->text());
		QTableWidgetItem *inputDevice = devicesInputWidget->selectedItems().at(0);
		settings.setValue("settings/inputDevice", inputDevice->text());
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
