//Created by July IGHOR
//http://trader.uax.co
//Bitcoin Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc

#include "julylightchanges.h"

JulyLightChanges::JulyLightChanges(QDoubleSpinBox *parent, QString colL, QString colH)
	: QObject()
{
	lastValue=0.0;
	colorL=colL;
	colorH=colH;
	parentSpinBox=parent;
	setParent(parentSpinBox);
	changeTimer=new QTimer;
	connect(changeTimer,SIGNAL(timeout()),this,SLOT(changeTimerSlot()));
	changeTimer->setSingleShot(true);
	valueChanged(parentSpinBox->value());
	connect(parent,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
}

JulyLightChanges::~JulyLightChanges()
{
	if(changeTimer)delete changeTimer;
}

void JulyLightChanges::changeTimerSlot()
{
	parentSpinBox->setStyleSheet("QDoubleSpinBox:disabled{color:black; background: \"white\";} QDoubleSpinBox {color:black;background: \"white\";}");
}

void JulyLightChanges::valueChanged(double val)
{
	changeTimer->stop();
	if(lastValue<=val)
		parentSpinBox->setStyleSheet("QDoubleSpinBox:disabled{color:black; background: \""+colorH+"\";} QDoubleSpinBox {color:black;background: \""+colorH+"\";}");
	else
		parentSpinBox->setStyleSheet("QDoubleSpinBox:disabled{color:black; background: \""+colorL+"\";} QDoubleSpinBox {color:black;background: \""+colorL+"\";}");
	lastValue=val;
	changeTimer->start(2000);
}