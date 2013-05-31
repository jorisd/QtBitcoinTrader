// Copyright (C) 2013 July IGHOR.
// I want to create Bitcoin Trader application that can be configured for any rule and strategy.
// If you want to help me please Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc
// For any questions please use contact form at http://trader.uax.co
// Or send e-mail directly to julyighor@gmail.com
//
// You may use, distribute and copy the Qt Bitcion Trader under the terms of
// GNU General Public License version 3

#include "translationdialog.h"
#include "main.h"
#ifdef Q_OS_WIN
#include "qtwin.h"
#endif
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>

TranslationDialog::TranslationDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.buttonSaveAs->setEnabled(false);
	setWindowFlags(Qt::Window);
	setAttribute(Qt::WA_DeleteOnClose,true);
	//setFixedSize(size());

#ifdef GENERATE_LANGUAGE_FILE
	julyTranslator->loadMapFromUi(this);
	julyTranslator->saveToFile("LanguageDefault.lng");
#endif

#ifdef Q_OS_WIN
	if(QtWin::isCompositionEnabled())QtWin::extendFrameIntoClientArea(this);
#endif

	julyTranslator->translateUi(this);

	ui.languageName->setText(julyTr("LANGUAGE_NAME","Invalid"));
	ui.authorAbout->setText(julyTr("LANGUAGE_AUTHOR","Invalid"));

	gridLayout=new QGridLayout;
	fonWidget.setLayout(gridLayout);
	ui.scrollArea->setWidget(&fonWidget);

	fillLayoutByMap(&(julyTranslator->labelMap),"Label_");
	fillLayoutByMap(&(julyTranslator->groupBoxMap),"GroupBox_");
	fillLayoutByMap(&(julyTranslator->checkBoxMap),"CheckBox_");
	fillLayoutByMap(&(julyTranslator->buttonMap),"Button_");
	fillLayoutByMap(&(julyTranslator->spinBoxMap),"SpinBox_");
	fillLayoutByMap(&(julyTranslator->stringMap),"String_");

	fonWidget.setFixedHeight(fonWidget.minimumSizeHint().height());
	mainWindow.ui.aboutTranslationButton->setEnabled(false);
}

TranslationDialog::~TranslationDialog()
{
	if(gridLayout)delete gridLayout;
	mainWindow.ui.aboutTranslationButton->setEnabled(true);
}


void TranslationDialog::fillLayoutByMap(QMap<QString,QString>* cMap, QString subName)
{
	static int currentRow=0;
	QStringList currentIdList=cMap->keys();

	for(int n=0;n<currentIdList.count();n++)
	{
		if(currentIdList.at(n).startsWith("LANGUAGE_"))continue;
		QLineEdit *newEdit=new QLineEdit;
		newEdit->setToolTip(subName+currentIdList.at(n));
		newEdit->setText(cMap->value(currentIdList.at(n)));
		lineEdits<<newEdit;
		gridLayout->addWidget(newEdit, currentRow++, 0);
	}
}

void TranslationDialog::applyButton()
{
	bool isValid=true;
	QStringList resultList;
	for(int n=0;n<lineEdits.count();n++)
	{
		if(lineEdits.at(n)->text().isEmpty())
		{
			ui.buttonSaveAs->setEnabled(false);
			QMessageBox::warning(this,windowTitle(),julyTr("LANGUAGE_NOT_APPROVED","Please fill empty fields"));
			return;
		}
		resultList<<lineEdits.at(n)->toolTip()+"="+lineEdits.at(n)->text();
	}
	resultList<<"String_LANGUAGE_NAME="+ui.languageName->text();
	resultList<<"String_LANGUAGE_AUTHOR="+ui.authorAbout->text();
	resultList<<"String_LANGUAGE_LOCALE="+locale().name();
	QFile writeFile(appDataDir+"Language/Custom.lng");
	writeFile.open(QIODevice::WriteOnly|QIODevice::Truncate);
	writeFile.write(resultList.join("\r\n").toUtf8());
	writeFile.close();

	mainWindow.reloadLanguageList(appDataDir+"Language/Custom.lng");
	ui.buttonSaveAs->setEnabled(true);
}

void TranslationDialog::saveAsButton()
{
	applyButton();
	if(ui.buttonSaveAs->isEnabled()==false)return;

	QString fileName=QFileDialog::getSaveFileName(this, julyTr("SAVE_TRANSLATION","Save Translation"),QDesktopServices::storageLocation(QDesktopServices::DesktopLocation),"(*.lng)");
	if(fileName.isEmpty())return;
	QFile::remove(fileName);
	QFile::copy(julyTranslator->lastFile(),fileName);
}