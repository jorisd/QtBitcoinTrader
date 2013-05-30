#ifndef TRANSLATIONDIALOG_H
#define TRANSLATIONDIALOG_H

#include <QDialog>
#include "ui_translationdialog.h"

class TranslationDialog : public QDialog
{
	Q_OBJECT

public:
	TranslationDialog(QWidget *parent = 0);
	~TranslationDialog();

private:
	Ui::TranslationDialog ui;
};

#endif // TRANSLATIONDIALOG_H
