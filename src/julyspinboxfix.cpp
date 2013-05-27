//Created by July IGHOR
//http://trader.uax.co
//Bitcoin Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc

#include "julyspinboxfix.h"
#include "main.h"

#include <QDebug>

JulySpinBoxFix::JulySpinBoxFix(QDoubleSpinBox *parentSB, int minWid)
	: QObject()
{
	pMinimumWidth=minWid;
	spinMargin=30;
	if(parentSB->buttonSymbols()==QDoubleSpinBox::NoButtons)spinMargin=10;
	parentSpinBox=parentSB;
	valueChanged(parentSB->text());
	if(!parentSB->suffix().isEmpty())
	{
		pMinimumWidth=parentSB->minimumWidth();
		parentSB->setMinimumWidth(pMinimumWidth);
    }

	parentSB->setMaximumWidth(110);
	connect(parentSB,SIGNAL(valueChanged(QString)),this,SLOT(valueChanged(QString)));
}

JulySpinBoxFix::~JulySpinBoxFix()
{
}

void JulySpinBoxFix::valueChanged(QString text)
{
	if(pMinimumWidth==0)
		parentSpinBox->setMinimumWidth(aFontMetrics.width(text)+spinMargin);
	else 
		parentSpinBox->setMinimumWidth(qMax(aFontMetrics.width(text)+spinMargin,pMinimumWidth));
}
