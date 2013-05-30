// Copyright (C) 2013 July IGHOR.
// I want to create Bitcoin Trader application that can be configured for any rule and strategy.
// If you want to help me please Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc
// For any questions please use contact form at http://trader.uax.co
// Or send e-mail directly to julyighor@gmail.com
//
// You may use, distribute and copy the Qt Bitcion Trader under the terms of
// GNU General Public License version 3

#include "tempwindow.h"
#include "main.h"

TempWindow::TempWindow(QWidget *)
	: QDialog()
{
	ui.setupUi(this);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowModality(Qt::ApplicationModal);
	setFixedSize(size());
	setAttribute(Qt::WA_DeleteOnClose,true);
}

TempWindow::~TempWindow()
{

}

void TempWindow::copyAddress()
{
	mainWindow.copyDonateButton();
}
