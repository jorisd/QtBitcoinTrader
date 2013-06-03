// Copyright (C) 2013 July IGHOR.
// I want to create Bitcoin Trader application that can be configured for any rule and strategy.
// If you want to help me please Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc
// For any questions please use contact form at http://trader.uax.co
// Or send e-mail directly to julyighor@gmail.com
//
// You may use, distribute and copy the Qt Bitcion Trader under the terms of
// GNU General Public License version 3

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
	QGridLayout *gridLayout;
	void fillLayoutByMap(QMap<QString,QString>*, QString subName);
	QList<QLineEdit*> lineEdits;
	QWidget fonWidget;
	Ui::TranslationDialog ui;
public slots:
	void searchLang(QString);
	void applyButton();
	void saveAsButton();
};

#endif // TRANSLATIONDIALOG_H
