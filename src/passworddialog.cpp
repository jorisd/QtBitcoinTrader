// Copyright (C) 2013 July IGHOR.
// I want to create Bitcoin Trader application that can be configured for any rule and strategy.
// If you want to help me please Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc
// For any questions please use contact form at http://trader.uax.co
// Or send e-mail directly to julyighor@gmail.com
//
// You may use, distribute and copy the Qt Bitcion Trader under the terms of
// GNU General Public License version 3

#include "passworddialog.h"
#include "main.h"

#ifdef Q_OS_WIN
#include "qtwin.h"
#endif
#include <QDir>
#include <QSettings>
#include <QMessageBox>

PasswordDialog::PasswordDialog(QWidget *parent)
	: QDialog(parent)
{
	resetData=false;
	newProfile=false;
	ui.setupUi(this);
	setWindowTitle(windowTitle()+" v"+appVerStr);
	setFixedSize(350,minimumSizeHint().height());
	setWindowFlags(Qt::WindowCloseButtonHint|Qt::WindowStaysOnTopHint);
	ui.okButton->setEnabled(false);
#ifdef Q_OS_WIN
	if(QtWin::isCompositionEnabled())
		QtWin::extendFrameIntoClientArea(this);
#endif

	QStringList settingsList=QDir(appDataDir,"*.ini").entryList();
	for(int n=0;n<settingsList.count();n++)
		ui.profileComboBox->addItem(QSettings(appDataDir+settingsList.at(n),QSettings::IniFormat).value("ProfileName",QFileInfo(settingsList.at(n)).completeBaseName()).toString(),settingsList.at(n));
	if(ui.profileComboBox->count()==0)ui.profileComboBox->addItem(julyTr("DEFAULT_PROFILE_NAME","Default Profile"));

#ifdef GENERATE_LANGUAGE_FILE
	julyTranslator->loadMapFromUi(this);
	julyTranslator->saveToFile("LanguageDefault.lng");
#endif

	julyTranslator->translateUi(this);
}

PasswordDialog::~PasswordDialog()
{

}

QString PasswordDialog::getIniFilePath()
{
	int currIndex=ui.profileComboBox->currentIndex();
	if(currIndex==-1)return appDataDir+"QtBitcoinTrader.ini";
	return appDataDir+ui.profileComboBox->itemData(currIndex).toString();
}

void PasswordDialog::addNewProfile()
{
	newProfile=true;
	accept();
}

QString PasswordDialog::getPassword()
{
	return ui.passwordEdit->text();
}

void PasswordDialog::resetDataSlot()
{
	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Question);
	msgBox.setWindowTitle(windowTitle());
	msgBox.setText(julyTr("CONFIRM_DELETE_PROFILE","Are you sure to delete \"%1\" profile?").arg(ui.profileComboBox->currentText()));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if(msgBox.exec()!=QMessageBox::Yes)return;

	resetData=true;
	accept();
}

void PasswordDialog::checkToEnableButton(QString pass)
{
	if(pass.length()<8){ui.okButton->setEnabled(pass.length()>=8);return;}

	static QString allowedChars="!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
	bool containsLetter=false;
	bool containsDigit=false;
	bool containsSpec=false;
	for(int n=0;n<pass.length();n++)
	{
		if(!containsLetter&&pass.at(n).isLetter())containsLetter=true;
		if(!containsDigit&&pass.at(n).isDigit())containsDigit=true;
		if(!containsSpec&&allowedChars.contains(pass.at(n)))containsSpec=true;
		if(containsSpec&&containsDigit&&containsSpec)break;
	}
	ui.okButton->setEnabled(containsLetter&&containsDigit&&containsSpec);
}