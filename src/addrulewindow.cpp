//Created by July IGHOR
//Feel free to contact me: julyighor@gmail.com
//Bitcoin Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc

#include "addrulewindow.h"
#include "main.h"
#include "tempwindow.h"
#ifdef Q_OS_WIN
#include "qtwin.h"
#endif
#include <QMessageBox>

AddRuleWindow::AddRuleWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.thanValue->setValue(mainWindow_->ui.marketLast->value());
	ui.btcValue->setValue(mainWindow_->ui.accountBTC->value());
	resize(minimumSizeHint());
	setMinimumSize(size());
	setMaximumSize(width()+100,height());
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowIcon(QIcon(":/Resources/QtBitcoinTrader.png"));
	amountChanged();

#ifdef Q_OS_WIN
	if(QtWin::isCompositionEnabled())QtWin::extendFrameIntoClientArea(this);
#endif

	resize(minimumSizeHint());
}

AddRuleWindow::~AddRuleWindow()
{

}

QString AddRuleWindow::getPrice()
{
	return currencySign+" "+ui.thanValue->text();
}

void AddRuleWindow::buttonAddRule()
{
	if(checkIsValidRule())accept();
	else QMessageBox::warning(this,windowTitle(),"This rule will be executed instantly.\nThis means that you make a mistake.\nPlease check values you entered.");
}

bool AddRuleWindow::checkIsValidRule()
{
	if(ui.checkLastPrice->isChecked()&&getRuleHolder().isAchieved(mainWindow_->ui.marketLast->value()))return false;
	if(ui.checkMarketBuy->isChecked()&&getRuleHolder().isAchieved(mainWindow_->ui.marketBuy->value()))return false;
	if(ui.checkMarketSell->isChecked()&&getRuleHolder().isAchieved(mainWindow_->ui.marketSell->value()))return false;
	if(ui.checkMarketHigh->isChecked()&&getRuleHolder().isAchieved(mainWindow_->ui.marketHigh->value()))return false;
	if(ui.checkMarketLow->isChecked()&&getRuleHolder().isAchieved(mainWindow_->ui.marketLow->value()))return false;
	if(ui.checkOrdersLastBuyPrice->isChecked()&&getRuleHolder().isAchieved(mainWindow_->ui.ordersLastBuyPrice->value()))return false;
	if(ui.checkOrdersLastSellPrice->isChecked()&&getRuleHolder().isAchieved(mainWindow_->ui.ordersLastSellPrice->value()))return false;
	return true;
}

void AddRuleWindow::amountChanged()
{
	bool btcVisible=ui.checkSellAmount->isChecked()||ui.checkBuyAmount->isChecked();
	ui.btcValue->setVisible(btcVisible);
	ui.btcLabel->setVisible(btcVisible);
	ui.labelSellAll->setVisible(ui.checkSellAllIn->isChecked());
	ui.labelSellHalf->setVisible(ui.checkSellHalfIn->isChecked());
	ui.labelSpendAll->setVisible(ui.checkBuyAllIn->isChecked());
	ui.labelSpendHalf->setVisible(ui.checkBuyHalfIn->isChecked());
	ui.labelCancelAllOrders->setVisible(ui.checkCancelAllOrders->isChecked());
}

QString AddRuleWindow::getDescriptionString()
{
	if(ui.checkGoesAbove->isChecked())
	{
	QString strA="If market last price goes more than";//Temporary values before Translation engine release
	QString strB="If market buy price goes more than";
	QString strC="If market sellprice goes more than";
	QString strD="If market high price goes more than";
	QString strE="If market low price goes more than";
	QString strF="If orders last buy price goes more than";
	QString strG="If orders last sell price goes more than";

	if(ui.checkLastPrice->isChecked())return strA;
	if(ui.checkMarketBuy->isChecked())return strB;
	if(ui.checkMarketSell->isChecked())return strC;
	if(ui.checkMarketHigh->isChecked())return strD;
	if(ui.checkMarketLow->isChecked())return strE;
	if(ui.checkOrdersLastBuyPrice->isChecked())return strF;
	if(ui.checkOrdersLastSellPrice->isChecked())return strG;
	return ui.checkSellAmount->text();
	}

	QString strA="If market last price goes less than";
	QString strB="If market buy price goes less than";
	QString strC="If market sellprice goes less than";
	QString strD="If market high price goes less than";
	QString strE="If market low price goes less than";
	QString strF="If orders last buy price goes less than";
	QString strG="If orders last sell price goes less than";

	if(ui.checkLastPrice->isChecked())return strA;
	if(ui.checkMarketBuy->isChecked())return strB;
	if(ui.checkMarketSell->isChecked())return strC;
	if(ui.checkMarketHigh->isChecked())return strD;
	if(ui.checkMarketLow->isChecked())return strE;
	if(ui.checkOrdersLastBuyPrice->isChecked())return strF;
	if(ui.checkOrdersLastSellPrice->isChecked())return strG;
	return ui.checkSellAmount->text();
}

QString AddRuleWindow::getBitcoinsString()
{
	if(ui.checkSellAllIn->isChecked())return ui.checkSellAllIn->text();
	if(ui.checkSellHalfIn->isChecked())return ui.checkSellHalfIn->text();
	if(ui.checkBuyAllIn->isChecked())return ui.checkBuyAllIn->text();
	if(ui.checkBuyHalfIn->isChecked())return ui.checkBuyHalfIn->text();
	if(ui.checkCancelAllOrders->isChecked())return ui.checkCancelAllOrders->text();
	return ui.btcLabel->text()+" "+ui.btcValue->text();
}

QString AddRuleWindow::getSellOrBuy()
{
	bool sell=ui.checkSellAmount->isChecked()||ui.checkSellAllIn->isChecked()||ui.checkSellHalfIn->isChecked();
	QString buyStr="Buy";
	QString sellStr="Sell";
	if(sell)return sellStr;
	return buyStr;
}

RuleHolder AddRuleWindow::getRuleHolder()
{
	double btcValue=ui.btcValue->value();
	bool isBuying=ui.checkBuyAmount->isChecked();
	if(ui.checkSellAllIn->isChecked())btcValue=-1.0;
	if(ui.checkSellHalfIn->isChecked())btcValue=-2.0;
	if(ui.checkBuyAllIn->isChecked()){isBuying=true;btcValue=-3.0;}
	if(ui.checkBuyHalfIn->isChecked()){isBuying=true;btcValue=-4.0;}
	if(ui.checkCancelAllOrders->isChecked())btcValue=-5.0;
	static uint ruleGuid=0;
	return RuleHolder(ui.checkGoesAbove->isChecked(), ui.thanValue->value(), btcValue, ++ruleGuid, isBuying);
}

int AddRuleWindow::getPriceType()
{
	if(ui.checkLastPrice->isChecked())return 1;
	if(ui.checkMarketBuy->isChecked())return 2;
	if(ui.checkMarketSell->isChecked())return 3;
	if(ui.checkMarketHigh->isChecked())return 4;
	if(ui.checkMarketLow->isChecked())return 5;
	if(ui.checkOrdersLastBuyPrice->isChecked())return 6;
	if(ui.checkOrdersLastSellPrice->isChecked())return 7;
	return 0;
}