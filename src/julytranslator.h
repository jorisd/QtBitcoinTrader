// Copyright (C) 2013 July IGHOR.
// I want to create Bitcoin Trader application that can be configured for any rule and strategy.
// If you want to help me please Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc
// For any questions please use contact form at http://trader.uax.co
// Or send e-mail directly to julyighor@gmail.com
//
// You may use, distribute and copy the Qt Bitcion Trader under the terms of
// GNU General Public License version 3

#ifndef JULYTRANSLATOR_H
#define JULYTRANSLATOR_H

#include <QObject>
#include <QMap>
#include <QWidget>

class JulyTranslator : public QObject
{
	Q_OBJECT

public:
	int loadFromFile(const QString &fileName);
	int saveToFile(const QString &fileName);
	void clearMaps(){labelMap.clear();groupBoxMap.clear();checkBoxMap.clear();buttonMap.clear();}
	void loadMapFromUi(QWidget *wid);
	void translateUi(QWidget *wid);

	QString translateButton(const QString &tid, const QString &defaultText);
	QString translateLabel(const QString &tid, const QString &defaultText);
	QString translateCheckBox(const QString &tid, const QString &defaultText);
	QString translateGroupBox(const QString &tid, const QString &defaultText);
	QString translateSpinBox(const QString &tid, const QString &defaultText);
	QString translateString(const QString &tid, const QString &defaultText);
private:
	bool fillMapsFromLine(QMap<QString,QString> *map, QString line, const QString &prefix);
	void fillMapsFromList(const QStringList &list);
	QStringList getMapList(QMap<QString,QString> *map, QString prefix);
	QMap<QString,QString>labelMap;
	QMap<QString,QString>groupBoxMap;
	QMap<QString,QString>checkBoxMap;
	QMap<QString,QString>buttonMap;
	QMap<QString,QString>spinBoxMap;
	QMap<QString,QString>stringMap;
signals:
	void languageChanged();
};

#endif // JULYTRANSLATOR_H
