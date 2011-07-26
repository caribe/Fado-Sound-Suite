#include "settings.h"

Settings::Settings(QWidget *parent) : QDialog(parent)
{
	QFormLayout *layout = new QFormLayout;

	QSettings settings;

	// Fields

	tempFolder = new QLineEdit(settings.value("settings/tempFolder", "/tmp").toString());
	layout->addRow(tr("Temporary file folder"), tempFolder);

	pluginsFolder = new QLineEdit(settings.value("settings/pluginsFolder", ".").toString());
	layout->addRow(tr("Plugins folder"), pluginsFolder);

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
	settings.setValue("settings/tempFolder", tempFolder->text());
	settings.setValue("settings/pluginsFolder", pluginsFolder->text());

	accept();
}
