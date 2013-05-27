//Created by July IGHOR
//Feel free to contact me: julyighor@gmail.com
//Bitcoin Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc

#include "addrulewindow.h"
#include "main.h"
#include "tempwindow.h"
#include "julyspinboxfix.h"
#ifdef Q_OS_WIN
#include "qtwin.h"
#endif
#include <QMessageBox>

AddRuleWindow::AddRuleWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.thanValue->setValue(mainWindow.ui.marketLast->value());
	ui.exactPriceValue->setValue(mainWindow.ui.marketLast->value());
	ui.btcValue->setValue(mainWindow.ui.accountBTC->value());
	ui.exactPriceValue->setEnabled(false);
	ui.label_53->setEnabled(false);
	setWindowFlags(Qt::WindowCloseButtonHint);
	setWindowIcon(QIcon(":/Resources/QtBitcoinTrader.png"));
	amountChanged();

#ifdef Q_OS_WIN
	if(QtWin::isCompositionEnabled())QtWin::extendFrameIntoClientArea(this);
#endif

	QPixmap curPix(":/Resources/"+currencyStr+".png");
	ui.labelUSD1->setPixmap(curPix);ui.labelUSD1->setToolTip(currencyStr);
	ui.label_53->setPixmap(curPix);ui.label_53->setToolTip(currencyStr);

	QPixmap btcPix(":/Resources/BTC.png");
	ui.btcLabel->setPixmap(curPix);ui.btcLabel->setToolTip("BTC");

	new JulySpinBoxFix(ui.thanValue);
	new JulySpinBoxFix(ui.btcValue);
	new JulySpinBoxFix(ui.exactPriceValue);

	foreach(QPushButton* pushButtons, findChildren<QPushButton*>())pushButtons->setMinimumWidth(QFontMetrics(pushButtons->font()).width(pushButtons->text())+10);

	foreach(QCheckBox* checkBoxes, findChildren<QCheckBox*>())checkBoxes->setMinimumWidth(QFontMetrics(checkBoxes->font()).width(checkBoxes->text())+10);

	resize(minimumSizeHint());
	setMinimumSize(size());
	setMaximumSize(width()+100,height());
}

AddRuleWindow::~AddRuleWindow()
{

}

QString AddRuleWindow::getPrice()
{
	QString ruleSellPrice=ui.exactPriceValue->text();

	if(ui.checkLastPrice_2->isChecked())ruleSellPrice=ui.checkLastPrice_2->text().replace(" Price","");else
	if(ui.checkMarketBuy_2->isChecked())ruleSellPrice=ui.checkMarketBuy_2->text().replace(" Price","");else
	if(ui.checkMarketSell_2->isChecked())ruleSellPrice=ui.checkMarketSell_2->text().replace(" Price","");else
	if(ui.checkMarketHigh_2->isChecked())ruleSellPrice=ui.checkMarketHigh_2->text().replace(" Price","");else
	if(ui.checkMarketLow_2->isChecked())ruleSellPrice=ui.checkMarketLow_2->text().replace(" Price","");else
	if(ui.checkOrdersLastBuyPrice_2->isChecked())ruleSellPrice=ui.checkOrdersLastBuyPrice_2->text().replace(" Price","");else
	if(ui.checkOrdersLastSellPrice_2->isChecked())ruleSellPrice=ui.checkOrdersLastSellPrice_2->text().replace(" Price","");else
	ruleSellPrice.prepend(currencySign+" ");
	return ruleSellPrice;
}

void AddRuleWindow::buttonAddRule()
{
	if(checkIsValidRule())accept();
	else QMessageBox::warning(this,windowTitle(),"This rule will be executed instantly.\nThis means that you make a mistake.\nPlease check values you entered.");
}

bool AddRuleWindow::checkIsValidRule()
{
	if(ui.checkLastPrice->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.marketLast->value()))return false;
	if(ui.checkMarketBuy->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.marketBuy->value()))return false;
	if(ui.checkMarketSell->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.marketSell->value()))return false;
	if(ui.checkMarketHigh->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.marketHigh->value()))return false;
	if(ui.checkMarketLow->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.marketLow->value()))return false;
	if(ui.checkOrdersLastBuyPrice->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.ordersLastBuyPrice->value()))return false;
	if(ui.checkOrdersLastSellPrice->isChecked()&&getRuleHolder().isAchieved(mainWindow.ui.ordersLastSellPrice->value()))return false;
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
	QString priceStr=currencySign+" "+ui.thanValue->text();
	if(ui.checkGoesAbove->isChecked())
	{
	QString strA="If market last price goes more than "+priceStr;//Temporary values before Translation engine release
	QString strB="If market buy price goes more than "+priceStr;
	QString strC="If market sellprice goes more than "+priceStr;
	QString strD="If market high price goes more than "+priceStr;
	QString strE="If market low price goes more than "+priceStr;
	QString strF="If orders last buy price goes more than "+priceStr;
	QString strG="If orders last sell price goes more than "+priceStr;

	if(ui.checkLastPrice->isChecked())return strA;
	if(ui.checkMarketBuy->isChecked())return strB;
	if(ui.checkMarketSell->isChecked())return strC;
	if(ui.checkMarketHigh->isChecked())return strD;
	if(ui.checkMarketLow->isChecked())return strE;
	if(ui.checkOrdersLastBuyPrice->isChecked())return strF;
	if(ui.checkOrdersLastSellPrice->isChecked())return strG;
	return ui.checkSellAmount->text();
	}

	QString strA="If market last price goes less than "+priceStr;
	QString strB="If market buy price goes less than "+priceStr;
	QString strC="If market sellprice goes less than "+priceStr;
	QString strD="If market high price goes less than "+priceStr;
	QString strE="If market low price goes less than "+priceStr;
	QString strF="If orders last buy price goes less than "+priceStr;
	QString strG="If orders last sell price goes less than "+priceStr;

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

	double ruleSellPrice=ui.exactPriceValue->value();

	if(ui.checkLastPrice_2->isChecked())ruleSellPrice=-1.0;
	if(ui.checkMarketBuy_2->isChecked())ruleSellPrice=-2.0;
	if(ui.checkMarketSell_2->isChecked())ruleSellPrice=-3.0;
	if(ui.checkMarketHigh_2->isChecked())ruleSellPrice=-4.0;
	if(ui.checkMarketLow_2->isChecked())ruleSellPrice=-5.0;
	if(ui.checkOrdersLastBuyPrice_2->isChecked())ruleSellPrice=-6.0;
	if(ui.checkOrdersLastSellPrice_2->isChecked())ruleSellPrice=-7.0;

	static uint ruleGuid=0;
	return RuleHolder(ui.checkGoesAbove->isChecked(), ui.thanValue->value(), btcValue, ++ruleGuid, isBuying, ruleSellPrice);
}

int AddRuleWindow::getRulePriceType()
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