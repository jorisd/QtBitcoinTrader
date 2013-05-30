#ifndef TRANSLATIONABOUT_H
#define TRANSLATIONABOUT_H

#include <QDialog>
#include "ui_translationabout.h"

class TranslationAbout : public QDialog
{
	Q_OBJECT

public:
	TranslationAbout(QWidget *parent = 0);
	~TranslationAbout();

private:
	Ui::TranslationAbout ui;
private slots:
	void createTranslation();
};

#endif // TRANSLATIONABOUT_H
