//Created by July IGHOR
//http://trader.uax.co
//Bitcoin Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc

#ifdef Q_OS_WIN
#define USING_QTSINGLEAPPLICATION //QtSingleApplication uses only to prevent starting two programs at time. You can remove this line to not use this class.
#endif
#include <QPlastiqueStyle>
#include "main.h"
#include <QtGui/QApplication>
#include <QFileInfo>
#include <QSettings>
#include "bitcointraderupdater.h"
#include "passworddialog.h"
#include "newpassworddialog.h"
#include "julyaes256.h"
#include <QTextCodec>

#ifdef USING_QTSINGLEAPPLICATION
#include "qtsingleapplication.h"//https://github.com/connectedtable/qtsingleapplication
#endif

QMap<QByteArray,QByteArray> *currencySignMap;
QMap<QByteArray,QByteArray> *currencyNamesMap;
QByteArray *bitcoinSign_;
QByteArray *currencySign_;
QByteArray *currencyStr_;
QByteArray *restKey_;
QByteArray *restSign_;
LogThread *logThread;
QtBitcoinTrader *mainWindow_;
quint64 *nonce_;
bool *logEnabled_;
QString *iniFileName_;
QString *logFileName_;
double *appVerReal_;
QByteArray *appVerStr_;
bool *validKeySign_;
bool *useSSL_;

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
	appVerStr_=new QByteArray("0.90");
	appVerReal_=new double(appVerStr_->toDouble());
	currencyStr_=new QByteArray();
	currencySign_=new QByteArray();
	validKeySign_=new bool(false);
	bitcoinSign_=new QByteArray("BTC");
	useSSL_=new bool(true);
	currencySignMap=new QMap<QByteArray,QByteArray>;
	currencyNamesMap=new QMap<QByteArray,QByteArray>;

#ifdef Q_OS_WIN
	if(argc>1)
	{
#ifdef USING_QTSINGLEAPPLICATION
		QtSingleApplication a("BitcoinTraderUpdater",argc,argv);
		if(a.isRunning())return 0;
#else
		QApplication a(argc,argv);
#endif
		if(a.arguments().last()=="/checkupdate")
		{
			BitcoinTraderUpdater updater;
			return a.exec();
		}
	}
#endif

#ifdef USING_QTSINGLEAPPLICATION
	QtSingleApplication a(argc, argv);
	if(a.isRunning())
	{
		a.sendMessage("ActivateWindow");
		return 0;
	}
#else
	QApplication a(argc,argv);
#endif
#ifndef Q_OS_WIN
	a.setStyle(new QPlastiqueStyle);
#endif
	{
	nonce_=new quint64(0);
	logEnabled_=new bool(false);
	QString appFileName=a.applicationDirPath()+"/"+QFileInfo(a.applicationFilePath()).completeBaseName();
	iniFileName_=new QString(appFileName+".ini");
	logFileName_=new QString(appFileName+".log");

	QSettings settings(iniFileName,QSettings::IniFormat);
	isLogEnabled=settings.value("LogEnabled",false).toBool();
	settings.setValue("LogEnabled",isLogEnabled);
	if(isLogEnabled)logThread=new LogThread;
	nonce=QDateTime::currentDateTime().toMSecsSinceEpoch();
	restKey_=new QByteArray;
	restSign_=new QByteArray;

	{
		QFile currencyFile("://Resources/Currencies.map");
		currencyFile.open(QIODevice::ReadOnly);
		QStringList currencyList=QString(currencyFile.readAll()).split("\r\n");
		currencyFile.close();
		for(int n=0;n<currencyList.count();n++)
		{
			QStringList currencyName=currencyList.at(n).split("=");
			if(currencyName.count()!=3)continue;
			currencyNamesMap->insert(currencyName.at(0).toAscii(),currencyName.at(1).toAscii());
			currencySignMap->insert(currencyName.at(0).toAscii(),currencyName.at(2).toAscii());
		}
	}

	bool tryDecrypt=true;
	while(tryDecrypt)
	{
		QByteArray cryptedData=QByteArray::fromBase64(settings.value("CryptedData","").toString().toAscii());
		QString tryPassword;
		restKey_->clear();
		restSign_->clear();

		if(cryptedData.isEmpty())
		{
			NewPasswordDialog newPassword;
			if(newPassword.exec()==QDialog::Rejected)return 0;
			tryPassword=newPassword.getPassword();
			restKey=newPassword.getRestKey().toAscii();
			restSign=QByteArray::fromBase64(newPassword.getRestSign().toAscii());
			cryptedData=JulyAES256::encrypt("Qt Bitcoin Trader\r\n"+restKey+"\r\n"+restSign_->toBase64(),tryPassword.toAscii());
			settings.setValue("CryptedData",QString(cryptedData.toBase64()));
			settings.remove("RestSign");
			settings.remove("RestKey");
			settings.sync();
		}
		if(tryPassword.isEmpty())
		{
			PasswordDialog enterPassword;
			if(enterPassword.exec()==QDialog::Rejected)return 0;
			if(enterPassword.resetData){settings.remove("CryptedData");settings.sync();continue;}
			tryPassword=enterPassword.getPassword();
		}
		if(!tryPassword.isEmpty())
		{
			QStringList decryptedList=QString(JulyAES256::decrypt(cryptedData,tryPassword.toAscii())).split("\r\n");
			if(decryptedList.count()==3&&decryptedList.first()=="Qt Bitcoin Trader")
			{
				restKey=decryptedList.at(1).toAscii();
				restSign=QByteArray::fromBase64(decryptedList.last().toAscii());
				tryDecrypt=false;
			}
		}
	}
	bitcoinSign=currencySignMap->value("BTC","BTC");
	currencyStr=settings.value("Currency","USD").toString().toAscii();
	currencySign=currencySignMap->value(currencyStr,"$");
	mainWindow_=new QtBitcoinTrader;

#ifdef USING_QTSINGLEAPPLICATION
	a.setActivationWindow(mainWindow_);
	QObject::connect(mainWindow_,SIGNAL(finished(int)),&a,SLOT(quit()));
#endif
	}
	mainWindow_->show();
	return a.exec();
}
