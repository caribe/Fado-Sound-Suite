#ifndef GUI_ABOUT_H
#define GUI_ABOUT_H

#include <QDialog>
#include <QLabel>

class About : public QDialog {
	Q_OBJECT

	public:
		About(QWidget *parent);
};

#endif
