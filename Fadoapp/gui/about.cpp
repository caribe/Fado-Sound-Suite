#include "gui/about.h"

About::About(QWidget *parent)
{
	setWindowTitle("About Fado");

	QLabel *label = new QLabel(this);
	label->setText("Fado\nSynthetic music generator and sound processor\n<a href='http://saitfainder.altervista.org/fado/'>Checkout our site</a>");

	show();
}
