#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSettings>
#include <QPushButton>
#include <QDialogButtonBox>

class Settings : public QDialog
{
    Q_OBJECT

	QLineEdit *tempFolder, *pluginsFolder;

public:
    explicit Settings(QWidget *parent = 0);

signals:

public slots:
	void acceptAction();
};

#endif // SETTINGS_H
