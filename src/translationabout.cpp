// Copyright (C) 2013 July IGHOR.
// I want to create Bitcoin Trader application that can be configured for any rule and strategy.
// If you want to help me please Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc
// For any questions please use contact form at https://sourceforge.net/projects/bitcointrader/
// Or send e-mail directly to julyighor@gmail.com
//
// You may use, distribute and copy the Qt Bitcion Trader under the terms of
// GNU General Public License version 3

#include "translationabout.h"
#include "main.h"
#ifdef Q_OS_WIN
#include "qtwin.h"
#endif
#include "translationdialog.h"

TranslationAbout::TranslationAbout(QWidget *)
	: QDialog()
{
	ui.setupUi(this);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowModality(Qt::ApplicationModal);
	setAttribute(Qt::WA_DeleteOnClose,true);
	//setFixedSize(size());

#ifdef Q_OS_WIN
	if(QtWin::isCompositionEnabled())
		QtWin::extendFrameIntoClientArea(this);
#endif

#ifdef GENERATE_LANGUAGE_FILE
	julyTranslator->loadMapFromUi(this);
	julyTranslator->saveToFile("LanguageDefault.lng");
#endif

	julyTranslator->translateUi(this);
	ui.languageField->setText(julyTr("LANGUAGE_NAME","Invalid Language"));
	ui.textEdit->setText(julyTr("LANGUAGE_AUTHOR","Invalid About"));
}

TranslationAbout::~TranslationAbout()
{

}

void TranslationAbout::createTranslation()
{
	accept();
	(new TranslationDialog)->show();
}