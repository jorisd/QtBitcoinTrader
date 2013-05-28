//Created by July IGHOR
//Feel free to contact me: julyighor@gmail.com
//Bitcoin Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc

#ifndef QTBITCOINTRADER_H
#define QTBITCOINTRADER_H

#include <QtGui/QDialog>
#include "ui_gsgtrader.h"
#include <QHttp>
#include <QSslSocket>
#include "socketthread.h"
#include <QHttp>
#include <QCloseEvent>
#include "ruleholder.h"
#include "audioplayer.h"

class QtBitcoinTrader : public QDialog
{
	Q_OBJECT

public:
	void fixAllChildButtonsAndLabels(QWidget *par);

	QList<RuleHolder> rulesLastPrice;
	QList<RuleHolder> rulesMarketBuyPrice;
	QList<RuleHolder> rulesMarketSellPrice;
	QList<RuleHolder> rulesMarketHighPrice;
	QList<RuleHolder> rulesMarketLowPrice;
	QList<RuleHolder> rulesOrdersLastBuyPrice;
	QList<RuleHolder> rulesOrdersLastSellPrice;

	bool isValidSoftLag();

	void apiSell(double btc, double price);
	void apiBuy(double btc, double price);

	void checkAndExecuteRule(QList<RuleHolder> *ruleHolder, double price);

	Ui::gsgTraderClass ui;
	bool confirmBuySell;
	
	QByteArray getMidData(QString a, QString b,QByteArray *data);
	QTimer *secondTimer;
	QTimer *updateCheckTimer;
	SocketThread *socketThreadAuth;
	QtBitcoinTrader();
	~QtBitcoinTrader();

private:
	AudioPlayer *player;
	void postWorkAtTableItem(QTableWidgetItem *);
	void checkAllRules();

	void removeRuleByGuid(uint guid);
	bool removeRuleByGuidInRuleHolderList(uint guid, QList<RuleHolder> *ruleHolderList);

	double lastMarketLowPrice;
	double lastMarketHighPrice;

	bool logTextEditEmpty;
	bool constructorFinished;
	bool apiDownState;
	void setApiDown(bool);
	void closeEvent(QCloseEvent *event);
	void reject(){};
	QString clearData(QString data);
	QHttp *httpUpdate;

	QString appDir;
	bool authErrorOnce;
	bool showingMessage;
	bool ordersLogLoaded;
	void beep();

	void cancelOrderByOid(QByteArray);
	bool lastLagState;
	void setRuleStateBuGuid(quint64 guid, int state);
	void setRulesTableRowState(int row, int state);
	void setOrdersTableRowState(int row, int state);
	void setOrdersTableRowStateByText(int row, QByteArray text);

	double floatFee;
	double floatFeeDec;
	double floatFeeInc;

	bool balanceNotLoaded;
	bool marketPricesNotLoaded;
	void checkValidSellButtons();
	void checkValidBuyButtons();
	bool sellLockBtcToSell;
	bool sellLockPricePerCoin;
	bool sellLockAmountToReceive;

	bool buyLockTotalBtc;
	bool buyLockPricePerCoin;
	bool buyLockTotalSpend;

	QMap<QByteArray,QString> oidMap;
	void insertIntoTable(QByteArray,QString);
	QDateTime lastUpdate;
	QTime updateLogTime;
	bool profitSellThanBuyUnlocked;
	bool profitBuyThanSellUnlocked;
public slots:
	void zeroSellThanBuyProfit();
	void zeroBuyThanSellProfit();
	void profitSellThanBuy();
	void profitSellThanBuyChanged(double);
	void profitSellThanBuyCalc();
	void profitBuyThanSellCalc();
	void profitBuyThanSell();
	void profitBuyThanSellChanged(double);

	void buttonNewWindow();

	void checkValidRulesButtons();
	void aboutTranslationButton();
	void ordersLogGroupBoxToggled(bool);

	void currencyChanged(int);

	void apiDownSlot();
	void setSslEnabled(bool);
	void calcButtonClicked();
	void checkUpdate();
	void httpUpdateDone(bool);

	void saveSoundToggles();
	void ruleAddButton();
	void ruleRemove();
	void ruleRemoveAll();

	void copyDonateButton();

	void accountUSDChanged(double);
	void accountBTCChanged(double);
	void marketBuyChanged(double);
	void marketSellChanged(double);
	void marketLowChanged(double);
	void marketHighChanged(double);
	void marketLastChanged(double);
	void ordersLastBuyPriceChanged(double);
	void ordersLastSellPriceChanged(double);

	void balanceChanged(double);
	void updateLogTable();
	void ordersSelectionChanged();
	void mtgoxLagChanged(double);
	void ordersCancelSelected();
	void secondSlot();
	void dataReceivedAuth(QByteArray);
	void ordersCancelAll();
	void accountFeeChanged(double);

	void buyBtcToBuyChanged(double);
	void buyPricePerCoinChanged(double);
	void buyBtcToBuyAllIn();
	void buyBtcToBuyHalfIn();
	void buyPricePerCoinAsMarketPrice();
	void buyBitcoinsButton();
	void buyTotalToSpendInUsdChanged(double);

	void sellBitcoinButton();
	void sellAmountToReceiveChanged(double);
	void sellPricePerCoinInUsdChanged(double);
	void sellPricePerCoinAsMarketPrice();
	void sellTotalBtcToSellAllIn();
	void sellTotalBtcToSellHalfIn();
	void sellTotalBtcToSellChanged(double);
	void lastSoftLagChanged(double);
signals:
	void quit();
};

#endif // QTBITCOINTRADER_H
