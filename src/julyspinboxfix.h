//Created by July IGHOR
//http://trader.uax.co
//Bitcoin Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc

#ifndef JULYSPINBOXFIX_H
#define JULYSPINBOXFIX_H

#include <QObject>
#include <QDoubleSpinBox>
#include <QFontMetrics>

class JulySpinBoxFix : public QObject
{
	Q_OBJECT

public:
	JulySpinBoxFix(QDoubleSpinBox *parent, int minimumWidth=0);
	~JulySpinBoxFix();

private:
	int pMinimumWidth;
	int spinMargin;
	QDoubleSpinBox *parentSpinBox;
private slots:
	void valueChanged(QString);
};

#endif // JULYSPINBOXFIX_H
