//Created by July IGHOR
//Feel free to contact me: julyighor@gmail.com
//Bitcoin Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc

#ifndef ADDRULEWINDOW_H
#define ADDRULEWINDOW_H

#include <QDialog>
#include "ui_addrulewindow.h"
#include "ruleholder.h"

class AddRuleWindow : public QDialog
{
	Q_OBJECT

public:
	Ui::AddRuleWindow ui;
	AddRuleWindow(QWidget *parent = 0);
	~AddRuleWindow();
	QString getDescriptionString();
	QString getBitcoinsString();
	QString getSellOrBuy();
	QString getPrice();
	RuleHolder getRuleHolder();
	int getPriceType();
private:
	bool checkIsValidRule();
public slots:
	void amountChanged();
	void buttonAddRule();
};

#endif // ADDRULEWINDOW_H
