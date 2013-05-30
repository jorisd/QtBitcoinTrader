// Copyright (C) 2013 July IGHOR.
// I want to create Bitcoin Trader application that can be configured for any rule and strategy.
// If you want to help me please Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc
// For any questions please use contact form at http://trader.uax.co
// Or send e-mail directly to julyighor@gmail.com
//
// You may use, distribute and copy the Qt Bitcion Trader under the terms of
// GNU General Public License version 3

#ifndef BITCOINTRADERUPDATER_H
#define BITCOINTRADERUPDATER_H

#include <QThread>
#include <QHttp>

class BitcoinTraderUpdater : public QThread
{
	Q_OBJECT

public:
	BitcoinTraderUpdater();

private:
	QByteArray md5Value;
	bool downloadingFile;
	QString clearData(QString data);
	QHttp *httpUpdate;
	void run();
	void quitApp();
public slots:
	void httpUpdateDone(bool);
	
};

#endif // BITCOINTRADERUPDATER_H
