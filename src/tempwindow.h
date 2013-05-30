// Copyright (C) 2013 July IGHOR.
// I want to create Bitcoin Trader application that can be configured for any rule and strategy.
// If you want to help me please Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc
// For any questions please use contact form at http://trader.uax.co
// Or send e-mail directly to julyighor@gmail.com
//
// You may use, distribute and copy the Qt Bitcion Trader under the terms of
// GNU General Public License version 3

#ifndef TEMPWINDOW_H
#define TEMPWINDOW_H

#include <QDialog>
#include "ui_tempwindow.h"

class TempWindow : public QDialog
{
	Q_OBJECT

public:
	TempWindow(QWidget *parent = 0);
	~TempWindow();

private:
	Ui::TempWindow ui;
private slots:
	void copyAddress();
};

#endif // TEMPWINDOW_H
