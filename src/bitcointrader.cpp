//Created by July IGHOR
//Feel free to contact me: julyighor@gmail.com
//Bitcoin Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc

#include <QMessageBox>
#include <QHttp>
#include <QSettings>
#include "main.h"
#include "julylightchanges.h"
#include "julyspinboxfix.h"
#include "addrulewindow.h"
#include <QFileInfo>
#include <QClipboard>
#include <QProcess>
#include "tempwindow.h"
#include "feecalculator.h"
#include <QFile>
#include <QSysInfo>
#include <QProcess>
#include <QDesktopWidget>

#ifdef Q_OS_WIN
#include "qtwin.h"
#endif

QtBitcoinTrader::QtBitcoinTrader()
	: QDialog()
{
	player=new AudioPlayer(this);

	logTextEditEmpty=true;
	constructorFinished=false;
	ordersLogLoaded=false;
	appDir=QApplication::applicationDirPath()+"/";
#ifdef Q_OS_WIN
	QFile::remove(appDir+QFileInfo(QApplication::applicationFilePath()).fileName()+".bkp");
#endif
	authErrorOnce=false;
	showingMessage=false;
	lastLagState=false;
	floatFee=0.0;
	floatFeeDec=0.0;
	floatFeeInc=0.0;
	confirmBuySell=true;
	apiDownState=true;
	marketPricesNotLoaded=true;
	balanceNotLoaded=true;
	sellLockBtcToSell=false;
	sellLockPricePerCoin=false;
	sellLockAmountToReceive=false;

	buyLockTotalBtc=false;
	buyLockPricePerCoin=false;
	buyLockTotalSpend=false;

	lastMarketLowPrice=0.0;
	lastMarketHighPrice=0.0;

	ui.setupUi(this);

	accountFeeChanged(ui.accountFee->value());
	setAttribute(Qt::WA_QuitOnClose,true);

	setWindowFlags(Qt::Window);

	{
		QPixmap btcPixmap("://Resources/BTC.png");
		ui.btcLabel2->setPixmap(btcPixmap);ui.btcLabel2->setToolTip("BTC");
		ui.btcLabel3->setPixmap(btcPixmap);ui.btcLabel3->setToolTip("BTC");
		ui.btcLabel4->setPixmap(btcPixmap);ui.btcLabel4->setToolTip("BTC");
		ui.btcLabel5->setPixmap(btcPixmap);ui.btcLabel5->setToolTip("BTC");
		ui.btcLabel6->setPixmap(btcPixmap);ui.btcLabel6->setToolTip("BTC");
		ui.btcLabel8->setPixmap(btcPixmap);ui.btcLabel8->setToolTip("BTC");
		ui.btcLabel11->setPixmap(btcPixmap);ui.btcLabel11->setToolTip("BTC");

		ui.usdLabel20->setPixmap(QPixmap(":/Resources/USD.png"));ui.usdLabel20->setToolTip("USD");
		ui.usdLabel21->setPixmap(QPixmap(":/Resources/USD.png"));ui.usdLabel21->setToolTip("USD");
	}
	{
	int lastCurrency=-1;
	QList<QByteArray> currencies=currencyNamesMap->keys();
	QList<QByteArray> currenciesSorted;
	currenciesSorted<<"USD"<<"EUR";
	for(int n=0;n<currencies.count();n++)
	{
		if(currencies.at(n)=="BTC")continue;
		if(currencies.at(n)=="USD")continue;
		if(currencies.at(n)=="EUR")continue;
		currenciesSorted<<currencies.at(n);
	}
	for(int n=0;n<currenciesSorted.count();n++)
	{
		if(currenciesSorted.at(n)==currencyStr)lastCurrency=n;
		ui.currencyComboBox->addItem(currenciesSorted.at(n));
	}
	if(lastCurrency>-1)ui.currencyComboBox->setCurrentIndex(lastCurrency);
	}

#ifdef Q_OS_WIN
	if(QtWin::isCompositionEnabled())
		QtWin::extendFrameIntoClientArea(this);
#endif

	setWindowIcon(QIcon(":/Resources/QtBitcoinTrader.png"));
	setApiDown(false);

	ui.ordersTableFrame->setVisible(false);

	ui.ordersTable->horizontalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
	ui.ordersTable->horizontalHeader()->setResizeMode(1,QHeaderView::ResizeToContents);
	ui.ordersTable->horizontalHeader()->setResizeMode(2,QHeaderView::Stretch);
	ui.ordersTable->horizontalHeader()->setResizeMode(3,QHeaderView::ResizeToContents);
	ui.ordersTable->horizontalHeader()->setResizeMode(4,QHeaderView::ResizeToContents);
	ui.ordersTable->horizontalHeader()->setResizeMode(5,QHeaderView::ResizeToContents);
	//ui.ordersTable->horizontalHeader()->setResizeMode(6,QHeaderView::ResizeToContents);
	//ui.ordersTable->horizontalHeader()->hideSection(6);

	ui.rulesNoMessage->setVisible(true);
	ui.rulesTable->setVisible(false);

	ui.rulesTable->horizontalHeader()->setResizeMode(0,QHeaderView::ResizeToContents);
	ui.rulesTable->horizontalHeader()->setResizeMode(1,QHeaderView::Stretch);
	ui.rulesTable->horizontalHeader()->setResizeMode(2,QHeaderView::ResizeToContents);
	ui.rulesTable->horizontalHeader()->setResizeMode(3,QHeaderView::ResizeToContents);
	ui.rulesTable->horizontalHeader()->setResizeMode(4,QHeaderView::ResizeToContents);

	QSettings settings(iniFileName,QSettings::IniFormat);

	setWindowTitle(windowTitle()+" v"+appVerStr+" ["+settings.value("ProfileName","Default Profile").toString()+"]");
#ifdef Q_OS_WIN
	useSSL=settings.value("OpenSSL",bool(QSysInfo::windowsVersion()>=0x0080)).toBool();
#else
	useSSL=settings.value("OpenSSL",true).toBool();
#endif

	settings.setValue("OpenSSL",useSSL);
	ui.sslCheck->setChecked(useSSL);

	ui.accountBTCBeep->setChecked(settings.value("AccountBTCBeep",false).toBool());
	ui.accountUSDBeep->setChecked(settings.value("AccountUSDBeep",false).toBool());
	ui.marketHighBeep->setChecked(settings.value("MarketHighBeep",false).toBool());
	ui.marketLowBeep->setChecked(settings.value("MarketLowBeep",false).toBool());

	ui.ordersLogGroupBox->setChecked(settings.value("LogExpanded",true).toBool());

	socketThreadAuth=new SocketThread(1);
	connect(socketThreadAuth,SIGNAL(dataReceived(QByteArray)),this,SLOT(dataReceivedAuth(QByteArray)));
	connect(socketThreadAuth,SIGNAL(apiDown()),this,SLOT(apiDownSlot()));

	ordersSelectionChanged();
	connect(ui.sslCheck,SIGNAL(toggled(bool)),this,SLOT(setSslEnabled(bool)));

	new JulyLightChanges(ui.marketVolume);
	new JulyLightChanges(ui.marketBuy);
	new JulyLightChanges(ui.marketSell);
	new JulyLightChanges(ui.marketBuy);
	new JulyLightChanges(ui.marketHigh);
	new JulyLightChanges(ui.marketLow);
	new JulyLightChanges(ui.accountBTC);
	new JulyLightChanges(ui.accountUSD);
	new JulyLightChanges(ui.marketLast);
	new JulyLightChanges(ui.ordersLastSellPrice);
	new JulyLightChanges(ui.ordersLastBuyPrice);

	foreach(QDoubleSpinBox* spinBox, findChildren<QDoubleSpinBox*>())new JulySpinBoxFix(spinBox);

	secondTimer=new QTimer;
	connect(secondTimer,SIGNAL(timeout()),this,SLOT(secondSlot()));
	secondTimer->setSingleShot(true);
	secondTimer->start(1000);

	httpUpdate=new QHttp("trader.uax.co",80,this);
	connect(httpUpdate,SIGNAL(done(bool)),this,SLOT(httpUpdateDone(bool)));
	updateCheckTimer=new QTimer(this);
	connect(updateCheckTimer,SIGNAL(timeout()),this,SLOT(checkUpdate()));
	checkUpdate();
	updateCheckTimer->start(3600000);

	constructorFinished=true;
	checkValidRulesButtons();
	currencyChanged(ui.currencyComboBox->currentIndex());

	fixAllChildButtonsAndLabels(this);

	int screenCount=QApplication::desktop()->screenCount();
	QPoint cursorPos=QCursor::pos();
	QRect currentDesktopRect(0,0,1024,720);
	for(int n=0;n<screenCount;n++)
	{
		if(QApplication::desktop()->screen(n)->rect().contains(cursorPos))
			currentDesktopRect=QApplication::desktop()->availableGeometry(n);
	}
    resize(qMax(minimumSizeHint().width(),qMin(1200,(int)(currentDesktopRect.width()*0.95))),qMin((int)(currentDesktopRect.height()*0.95),700));
}

QtBitcoinTrader::~QtBitcoinTrader()
{
}

void QtBitcoinTrader::fixAllChildButtonsAndLabels(QWidget *par)
{
	foreach(QPushButton* pushButtons, par->findChildren<QPushButton*>())pushButtons->setMinimumWidth(QFontMetrics(pushButtons->font()).width(pushButtons->text())+10);

	foreach(QCheckBox* checkBoxes, par->findChildren<QCheckBox*>())checkBoxes->setMinimumWidth(QFontMetrics(checkBoxes->font()).width(checkBoxes->text())+10);

	foreach(QLabel* labels, par->findChildren<QLabel*>())
	if(labels->text().length()&&labels->text().at(0)!='<')labels->setMinimumWidth(QFontMetrics(labels->font()).width(labels->text()));
}

void QtBitcoinTrader::currencyChanged(int val)
{
	if(!constructorFinished||val<0)return;
	currencyStr=ui.currencyComboBox->itemText(val).toAscii();
	currencySign=currencySignMap->value(currencyStr,"$");
	QSettings settings(iniFileName,QSettings::IniFormat);
	settings.setValue("Currency",QString(currencyStr));
	QPixmap curPix(":/Resources/"+currencyStr+".png");
	ui.usdLabel0->setPixmap(curPix);ui.usdLabel0->setToolTip(currencyStr);
	ui.usdLabel1->setPixmap(curPix);ui.usdLabel1->setToolTip(currencyStr);
	ui.usdLabel2->setPixmap(curPix);ui.usdLabel2->setToolTip(currencyStr);
	ui.usdLabel3->setPixmap(curPix);ui.usdLabel3->setToolTip(currencyStr);
	ui.usdLabel4->setPixmap(curPix);ui.usdLabel4->setToolTip(currencyStr);
	ui.usdLabel5->setPixmap(curPix);ui.usdLabel5->setToolTip(currencyStr);
	ui.usdLabel6->setPixmap(curPix);ui.usdLabel6->setToolTip(currencyStr);
	ui.usdLabel7->setPixmap(curPix);ui.usdLabel7->setToolTip(currencyStr);
	ui.usdLabel8->setPixmap(curPix);ui.usdLabel8->setToolTip(currencyStr);
	ui.usdLabel9->setPixmap(curPix);ui.usdLabel9->setToolTip(currencyStr);
	ui.usdLabel10->setPixmap(curPix);ui.usdLabel10->setToolTip(currencyStr);
	ui.usdLabel11->setPixmap(curPix);ui.usdLabel11->setToolTip(currencyStr);
	ui.usdLabel12->setPixmap(curPix);ui.usdLabel12->setToolTip(currencyStr);
	ui.usdLabel13->setPixmap(curPix);ui.usdLabel13->setToolTip(currencyStr);
	ui.usdLabel14->setPixmap(curPix);ui.usdLabel14->setToolTip(currencyStr);
	ui.usdLabel72->setPixmap(curPix);ui.usdLabel14->setToolTip(currencyStr);
	ui.accountUSD->setValue(0.0);
	ui.marketBuy->setValue(0.0);
	ui.marketSell->setValue(0.0);
	ui.marketHigh->setValue(0.0);
	ui.marketLow->setValue(0.0);
	ui.marketLast->setValue(0.0);
	ui.marketVolume->setValue(0.0);
	ui.buyTotalSpend->setValue(0.0);
	ui.sellTotalBtc->setValue(0.0);
	ui.buyPricePerCoin->setValue(0.01);
	ui.sellPricePerCoin->setValue(0.01);
	marketPricesNotLoaded=true;
	balanceNotLoaded=true;
	ordersLogLoaded=false;
}

void QtBitcoinTrader::profitLossChanged(double value)
{
	if(value<0)ui.profitLossSpinBox->setStyleSheet("QDoubleSpinBox {background: #ffaaaa;}");
	else
	ui.profitLossSpinBox->setStyleSheet("QDoubleSpinBox {background: #aaffaa;}");
}

void QtBitcoinTrader::setApiDown(bool on)
{
	bool apiRealyDown=on&&apiDownState;
	ui.lagValue->setVisible(!apiRealyDown);
	ui.apiDownLabel->setVisible(apiRealyDown);
	apiDownState=on;
}

void QtBitcoinTrader::apiDownSlot()
{
	setApiDown(true);
}

void QtBitcoinTrader::setSslEnabled(bool on)
{
	useSSL=on;
	socketThreadAuth->reconnectApi();
	QSettings settings(iniFileName,QSettings::IniFormat);
	settings.setValue("OpenSSL",on);
}

void QtBitcoinTrader::checkUpdate()
{
	httpUpdate->get("/API.php?Thread=Call&Object=General&Method=CheckUpdate");
}

QString QtBitcoinTrader::clearData(QString data)
{
	while(data.count()&&(data.at(0)=='{'||data.at(0)=='['||data.at(0)=='\"'))data.remove(0,1);
	while(data.count()&&(data.at(data.length()-1)=='}'||data.at(data.length()-1)==']'||data.at(data.length()-1)=='\"'))data.remove(data.length()-1,1);
	return data;
}

void QtBitcoinTrader::httpUpdateDone(bool on)
{
	if(on)return;
	QString allData=httpUpdate->readAll();
	if(allData.length()<3)return;
	QString errorString;
	QString versionString;
	QStringList allDataList=allData.split(",");
	for(int n=0;n<allDataList.count();n++)
	{
		QStringList pairList=allDataList.at(n).split(":");
		if(pairList.count()==2)
		{
			QString name=clearData(pairList.first());
			QString value=clearData(pairList.last());
			if(name=="Error")errorString=value;
			if(name=="Version")versionString=value;
		}
	}
	if(errorString.isEmpty())
	{
		if(versionString.toDouble()>appVerReal)
		{
#ifdef Q_OS_WIN
			QProcess proc;
			proc.startDetached(QApplication::applicationFilePath()+" /checkupdate");
			proc.waitForStarted();
#else
			QMessageBox::information(this,"Qt Bitcoin Trader","Update released "+versionString+". Please download from https://sourceforge.net/projects/bitcointrader/");
#endif
		}
	}
}

void QtBitcoinTrader::saveSoundToggles()
{
	if(!constructorFinished)return;
	QSettings settings(iniFileName,QSettings::IniFormat);
	settings.setValue("AccountBTCBeep",ui.accountBTCBeep->isChecked());
	settings.setValue("AccountUSDBeep",ui.accountUSDBeep->isChecked());
	settings.setValue("MarketHighBeep",ui.marketHighBeep->isChecked());
	settings.setValue("MarketLowBeep",ui.marketLowBeep->isChecked());
}

void QtBitcoinTrader::accountFeeChanged(double val)
{
	floatFee=val/100;
	floatFeeDec=1.0-floatFee;
	floatFeeInc=1.0+floatFee;
}

QByteArray QtBitcoinTrader::getMidData(QString a, QString b,QByteArray *data)
{
	QByteArray rez;
	if(b.isEmpty())b="\",";
	int startPos=data->indexOf(a,0);
	if(startPos>-1)
	{
	int endPos=data->indexOf(b,startPos+a.length());
	if(endPos>-1)rez=data->mid(startPos+a.length(),endPos-startPos-a.length());
	}
	return rez;
}

void QtBitcoinTrader::secondSlot()
{
	if(width()<minimumSizeHint().width())resize(minimumSizeHint().width(),height());
	ui.lastUpdate->setValue((double)lastUpdate.msecsTo(QDateTime::currentDateTime())/1000);
	if(!ordersLogLoaded||logTextEditEmpty)updateLogTable();
	secondTimer->start(1000);
}

void QtBitcoinTrader::mtgoxLagChanged(double val)
{
	if(val>=1.0)
		ui.lagValue->setStyleSheet("QDoubleSpinBox {background: #ffaaaa;}");
	else
		ui.lagValue->setStyleSheet("");
}

void QtBitcoinTrader::balanceChanged(double)
{
	updateLogTable();
}

void QtBitcoinTrader::dataReceivedAuth(QByteArray data)
{
	if(isLogEnabled)logThread->writeLog("Received: "+data);
	setApiDown(false);
	if(getMidData("{\"result\":\"","\",",&data)=="success")
	{
		QDateTime newDate=QDateTime::currentDateTime();
		ui.lastUpdate->setValue((double)lastUpdate.msecsTo(newDate)/1000);
		lastUpdate=newDate;

		if(data.size()>27)data.remove(0,28);
		if(data.size()&&data.at(data.size()-1)=='\"')data.remove(data.size()-1,1);

		if(data.startsWith("\"Login"))
		{
			QByteArray login=getMidData("Login\":\"","\",",&data);
			if(!login.isEmpty())ui.accountLoginLabel->setText(login);

			QByteArray btcBalance=getMidData("BTC\":{\"Balance\":{\"value\":\"","",&data);
			if(!btcBalance.isEmpty())ui.accountBTC->setValue(btcBalance.toDouble());

			QByteArray usdBalance=getMidData(currencyStr+"\":{\"Balance\":{\"value\":\"","",&data);
			ui.accountUSD->setValue(usdBalance.toDouble());

			QByteArray monthValue=getMidData("Monthly_Volume\":{\"value\":\"","\",",&data);
			if(!monthValue.isEmpty())ui.accountVolume->setValue(monthValue.toDouble());

			QByteArray tradeFee=getMidData("Trade_Fee\":","}",&data);
			if(!tradeFee.isEmpty())ui.accountFee->setValue(tradeFee.toDouble());

			if(balanceNotLoaded)
			{
				balanceNotLoaded=false;
				sellTotalBtcToSellAllIn();
				buyBtcToBuyAllIn();
			}
		}
		else
		if(data.startsWith("\"high"))
		{
			ui.marketHigh->setValue(getMidData("high\":{\"value\":\"","",&data).toDouble());
			ui.marketLow->setValue(getMidData("low\":{\"value\":\"","",&data).toDouble());
			ui.marketSell->setValue(getMidData("sell\":{\"value\":\"","",&data).toDouble());
			ui.marketLast->setValue(getMidData("last\":{\"value\":\"","",&data).toDouble());
			ui.marketBuy->setValue(getMidData("buy\":{\"value\":\"","",&data).toDouble());
			ui.marketVolume->setValue(getMidData("vol\":{\"value\":\"","",&data).toDouble());

			if(marketPricesNotLoaded)
			{
				marketPricesNotLoaded=false;
				sellPricePerCoinAsMarketPrice();
				buyPricePerCoinAsMarketPrice();
			}
		}
		else
		if(data.startsWith("\"lag"))
		{
			ui.lagValue->setValue(getMidData("lag_secs\":",",\"",&data).toDouble());
		}
		else
		if(data=="]}")//Orders is empty
		{
			if(ui.ordersTable->rowCount())
			{
				if(isLogEnabled)logThread->writeLog("Order table cleared");
				oidMap.clear();
				ui.ordersTable->clearContents();
				ui.ordersTable->setRowCount(0);
				ui.ordersTableFrame->setVisible(false);
				ui.noOpenedOrdersLabel->setVisible(true);
				ui.noOpenedOrdersLabel->setVisible(true);
			}
		}
		else
		if(data.startsWith("{\"oid\":"))//Have orders
		{
			QByteArray currentOrder=getMidData("oid","\"actions\":",&data);
			QMap<QByteArray,bool> activeOrders;
			while(currentOrder.size())
			{
				QByteArray oid=getMidData("\":\"","\",\"",&currentOrder);
				QByteArray itemType=getMidData("\"type\":\"","\",\"",&currentOrder);
				QByteArray itemAmount=getMidData("\"amount\":{\"value\":\"","\",\"",&currentOrder);
				QByteArray itemPrice=getMidData("\"price\":{\"value\":\"","\",\"",&currentOrder);
				QByteArray itemStatus=getMidData("\"status\":\"","\",\"",&currentOrder);
				QByteArray itemDate=getMidData(",\"date\":",",",&currentOrder);
				QByteArray orderSign=currencySignMap->value(getMidData("\"currency\":\"","\",\"",&currentOrder),"$");
				QString oidData=itemDate+";"+itemType+";"+itemStatus+";"+itemAmount+";"+itemPrice+";"+orderSign;
				QString findedData=oidMap.value(oid,"");

				if(findedData!="")//Update
				{
					if(findedData!=oidData)
						for(int n=0;n<ui.ordersTable->rowCount();n++)
							if(ui.ordersTable->item(n,0)->data(Qt::UserRole).toByteArray()==oid)
							{
								if(ui.ordersTable->item(n,2)->text()!="canceled")
								{
									ui.ordersTable->item(n,2)->setText(itemStatus);
									ui.ordersTable->item(n,3)->setText(bitcoinSign+" "+itemAmount);
									ui.ordersTable->item(n,4)->setText(orderSign+" "+itemPrice);
									ui.ordersTable->item(n,5)->setText(orderSign+" "+QString::number(itemAmount.toDouble()*itemPrice.toDouble(),'f',5));
									setOrdersTableRowStateByText(n,itemStatus);
									oidMap[oid]=oidData;
								}
								break;
							}
				}
				else//Insert
				{
					insertIntoTable(oid,oidData);
					oidMap[oid]=oidData;
				}
				activeOrders[oid]=1;
				if(data.size()>currentOrder.size())data.remove(0,currentOrder.size());
				currentOrder=getMidData("oid","\"actions\"",&data);
			}

			for(int n=0;n<ui.ordersTable->rowCount();n++)
			{
				if(n<0)break;
				QByteArray curOid=ui.ordersTable->item(n,0)->data(Qt::UserRole).toByteArray();
				if(activeOrders.values(curOid).count()==0)
				{
					if(isLogEnabled)logThread->writeLog("Deleted: "+curOid);
					oidMap.remove(curOid);
					ui.ordersTable->removeRow(n--);
					ui.ordersTableFrame->setVisible(ui.ordersTable->rowCount());
					ui.noOpenedOrdersLabel->setVisible(!ui.ordersTable->isVisible());
				}
			}
		}
		else
		if(data.startsWith("\"oid\":"))
		{
			if(isLogEnabled)logThread->writeLog("Removed: "+data);

			QByteArray oid=getMidData("oid\":\"","\",\"",&data);
			if(!oid.isEmpty())
				for(int n=0;n<ui.ordersTable->rowCount();n++)
					if(ui.ordersTable->item(n,0)->data(Qt::UserRole).toByteArray()==oid)
					{
						ui.ordersTable->item(n,2)->setText("canceled");
						setOrdersTableRowState(n,0);
						break;
					}
		}
		else
		if(data.startsWith("\"oid\""))
		{
			if(isLogEnabled)logThread->writeLog("Canceled: "+data);
		}
		else
		if(data.right(2)=="\"}"&&data.size()>36&&data.size()<40)
		{
			if(isLogEnabled)logThread->writeLog("Added: "+data);
		}
		else
		if(data.startsWith("\"records\""))
		{
			ordersLogLoaded=true;
			double lastBuyPrice=0.0;
			double lastSellPrice=0.0;
			QString newLog;
			QStringList dataList=QString(data).split("\"Index\"");
			for(int n=0;n<dataList.count();n++)
			{
				QByteArray curLog(dataList.at(n).toAscii());
				QByteArray logType=getMidData("\"Type\":\"","\",\"",&curLog);
				int logTypeInt=0;
				if(logType=="out"){logTypeInt=1;logType="<font color=\"red\">(Sold)</font>";}
				else 
				if(logType=="in"){logTypeInt=2;logType="<font color=\"blue\">(Bought)</font>";}
				else 
				if(logType=="fee"){logTypeInt=3;logType.clear();}
				else 
				if(logType=="deposit"){logTypeInt=4;logType.clear();logType="<font color=\"green\">(Deposit)</font>";}
				if(logTypeInt)
				{
					QByteArray logValue=getMidData("\"Value\":{\"value\":\"","\",\"",&curLog);
					QByteArray logDate=getMidData("\"Date\":",",\"",&curLog);
					QByteArray logText=getMidData(" at ","\",\"",&curLog);

					if((logTypeInt==1||logTypeInt==2)&&(lastSellPrice==0.0||lastBuyPrice==0.0))
					{
						QByteArray priceValue;
						for(int n=0;n<logText.size();n++)
							if(QChar(logText.at(n)).isDigit()||logText.at(n)=='.')priceValue.append(logText.at(n));
						QByteArray priceCurrency=logText.left(logText.size()-priceValue.size());
						if(lastSellPrice==0.0&&logTypeInt==1)
						{
							lastSellPrice=priceValue.toDouble();
							if(ui.usdLabel20->toolTip()!=priceCurrency)
							{
								ui.usdLabel20->setPixmap(QPixmap(":/Resources/"+currencySignMap->key(priceCurrency,"$")+".png"));
								ui.usdLabel20->setToolTip(priceCurrency);
							}
						}
						if(lastBuyPrice==0.0&&logTypeInt==2)
						{
							lastBuyPrice=priceValue.toDouble();
							if(ui.usdLabel21->toolTip()!=priceCurrency)
							{
								ui.usdLabel21->setPixmap(QPixmap(":/Resources/"+currencySignMap->key(priceCurrency,"$")+".png"));
								ui.usdLabel21->setToolTip(priceCurrency);
							}
						}
					}
					if(!logText.isEmpty())logText=" at <font color=\"darkgreen\">"+logText+"</font>";
					curLog=getMidData("\"Info\":\"","\",\"",&curLog);
	
				newLog.append("<font color=\"gray\">"+QDateTime::fromTime_t(logDate.toUInt()).toString("yyyy-MM-dd HH:mm:ss")+"</font>  <font color=\"#996515\">"+bitcoinSign+logValue+"</font>"+logText+" "+logType+"<br>");
				}
			}
			if(logTextEditEmpty&&newLog.size())logTextEditEmpty=false;
			ui.logTextEdit->setHtml(newLog);
			ui.ordersLastSellPrice->setValue(lastSellPrice);
			ui.ordersLastBuyPrice->setValue(lastBuyPrice);
			if(isLogEnabled)logThread->writeLog("Log Table Updated");
		}
		else
		{
			if(data.size()>0)QMessageBox::warning(0,"Unknown Data",data);
			if(isLogEnabled)logThread->writeLog("Unknown Data: "+data);
		}
	}
	else
	{
		QString errorString=getMidData("error\":\"","\"",&data);
		
		if(errorString.isEmpty())return;

		if(errorString=="Order not found")
		{
			//QMessageBox::warning(0,"Mt.Gox Error",errorString);
		}
		else
		if(errorString.startsWith("Identification required"))
		{
			if(!showingMessage)
			{
				if(!showingMessage&&!authErrorOnce)
				{
				authErrorOnce=true;
				showingMessage=true;
				QMessageBox::warning(this,"Mt.Gox Error","Identification required to access private API.\nPlease enter valid API key and Secret.\n\nTo get new API Keys:\nGo http://mtgox.com => \"Security Center\"  => \"Advanced API Key Creation\"\nCheck \"Info\" and \"Trade\"\nPress Create Key\nCopy and paste RestKey and RestSign to program.");
				if(isLogEnabled)logThread->writeLog("Mt.Gox Error: Identification required to access private API\nPlease restart application, and enter valid API key and Secret");
				showingMessage=false;
				}
			}
		}
		else
		if(errorString.startsWith("Invalid request"))
		{
			if(isLogEnabled)logThread->writeLog("Mt.Gox Error: Invalid request method for this API");
		}
		else
		if(errorString.isEmpty())
		{
			if(!showingMessage)
			{
			showingMessage=true;
			QMessageBox::warning(this,"Mt.Gox API",data+"\n\nPlease Restart Application");
			showingMessage=false;
			}

			if(isLogEnabled)logThread->writeLog("Mt.Gox Error: "+data);
		}
		else
		{
			if(!showingMessage)
			{
				showingMessage=true;
				QMessageBox::warning(this,"Mt.Gox API",errorString);
				showingMessage=false;
			}

		if(isLogEnabled)logThread->writeLog("Mt.Gox Error: "+errorString.toAscii());
		}
	}
}

void QtBitcoinTrader::insertIntoTable(QByteArray oid, QString data)
{
	QStringList dataList=data.split(";");
	if(dataList.count()!=6)return;
	int curRow=ui.ordersTable->rowCount();
	QByteArray orderSign=dataList.at(5).toAscii();
	ui.ordersTable->setRowCount(curRow+1);
	ui.ordersTable->setRowHeight(curRow,30);
	ui.ordersTable->setItem(curRow,0,new QTableWidgetItem(QDateTime::fromTime_t(dataList.at(0).toUInt()).toString("yyyy-MM-dd HH:mm:ss")));postWorkAtTableItem(ui.ordersTable->item(curRow,0));ui.ordersTable->item(curRow,0)->setData(Qt::UserRole,oid);
	ui.ordersTable->setItem(curRow,1,new QTableWidgetItem(dataList.at(1)));postWorkAtTableItem(ui.ordersTable->item(curRow,1));
	ui.ordersTable->setItem(curRow,2,new QTableWidgetItem(dataList.at(2)));postWorkAtTableItem(ui.ordersTable->item(curRow,2));
	ui.ordersTable->setItem(curRow,3,new QTableWidgetItem(bitcoinSign+" "+dataList.at(3)));ui.ordersTable->item(curRow,3)->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
	ui.ordersTable->setItem(curRow,4,new QTableWidgetItem(orderSign+" "+dataList.at(4)));ui.ordersTable->item(curRow,4)->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
	ui.ordersTable->setItem(curRow,5,new QTableWidgetItem(orderSign+" "+QString::number(dataList.at(3).toDouble()*dataList.at(4).toDouble(),'f',5)));ui.ordersTable->item(curRow,5)->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);

	setOrdersTableRowStateByText(curRow,dataList.at(2).toAscii());
	ordersSelectionChanged();
	ui.ordersTableFrame->setVisible(true);
	ui.noOpenedOrdersLabel->setVisible(false);
}

void QtBitcoinTrader::setOrdersTableRowStateByText(int row, QByteArray text)
{
	if(text=="invalid")setOrdersTableRowState(row,4);
	else 
	if(text.contains("pending"))setOrdersTableRowState(row,1);
	else
	setOrdersTableRowState(row,2);
}

void QtBitcoinTrader::setOrdersTableRowState(int row, int state)
{
	QColor nColor(255,255,255);
	switch(state)
	{
		case 0: nColor=Qt::lightGray; break;
		case 1: nColor.setRgb(255,255,200); break; //Yellow
		case 2: nColor.setRgb(200,255,200); break; //Green
		case 3: nColor.setRgb(255,200,200); break; //Red
		case 4: nColor.setRgb(200,200,255); break; //Blue
		default: break;
	}
	for(int n=0;n<ui.ordersTable->columnCount();n++)ui.ordersTable->item(row,n)->setBackgroundColor(nColor);
}

void QtBitcoinTrader::setRuleStateBuGuid(quint64 guid, int state)
{
	for(int n=0;n<ui.rulesTable->rowCount();n++)
		if(ui.rulesTable->item(n,0)->data(Qt::UserRole).toUInt()==guid)
		{
			setRulesTableRowState(n,state);
			return;
		}
}

void QtBitcoinTrader::setRulesTableRowState(int row, int state)
{
	QColor nColor(255,255,255);
	QString nText;
	switch(state)
	{
	case 0: nColor=Qt::lightGray; break;
	case 1: nColor.setRgb(255,255,200); nText="pending"; break; //Yellow
	case 2: nColor.setRgb(200,255,200); nText="done"; break; //Green
	case 3: nColor.setRgb(255,200,200); nText="disabled";break; //Red
	case 4: nColor.setRgb(200,200,255); break; //Blue
	default: break;
	}
	ui.rulesTable->item(row,0)->setText(nText);
	for(int n=0;n<ui.rulesTable->columnCount();n++)ui.rulesTable->item(row,n)->setBackgroundColor(nColor);
}

void QtBitcoinTrader::ordersCancelAll()
{
	for(int n=0;n<ui.ordersTable->rowCount();n++)
		if(ui.ordersTable->item(n,2)->text()!="canceled")
			cancelOrderByOid(ui.ordersTable->item(n,0)->data(Qt::UserRole).toByteArray());
}

void QtBitcoinTrader::cancelOrderByOid(QByteArray oid)
{
	socketThreadAuth->sendToApi("BTC"+currencyStr+"/money/order/cancel","&oid="+oid);
	if(isLogEnabled)logThread->writeLog("BTC"+currencyStr+"/money/order/cancel?nonce=*&oid="+oid);
}

void QtBitcoinTrader::ordersCancelSelected()
{
	for(int n=0;n<ui.ordersTable->rowCount();n++)
		if(ui.ordersTable->item(n,2)->isSelected()&&ui.ordersTable->item(n,2)->text()!="canceled")
			cancelOrderByOid(ui.ordersTable->item(n,0)->data(Qt::UserRole).toByteArray());
}

void QtBitcoinTrader::calcButtonClicked()
{
	(new FeeCalculator)->show();
}

void QtBitcoinTrader::checkValidSellButtons()
{
	ui.sellBitcoinsButton->setEnabled(ui.sellTotalBtc->value()>=0.01);
}

void QtBitcoinTrader::sellPricePerCoinAsMarketPrice()
{
	ui.sellPricePerCoin->setValue(ui.marketBuy->value());
}

void QtBitcoinTrader::sellTotalBtcToSellAllIn()
{
	ui.sellTotalBtc->setValue(ui.accountBTC->value());
}

void QtBitcoinTrader::sellTotalBtcToSellHalfIn()
{
	ui.sellTotalBtc->setValue(ui.accountBTC->value()/2.0);
}

void QtBitcoinTrader::lastSoftLagChanged(double)
{
	bool lastL=lastLagState;
	if(!isValidSoftLag())
	{
		lastLagState=false;
		ui.lastUpdate->setStyleSheet("QDoubleSpinBox {background: #ffaaaa;}");
	}
	else
	{
		ui.lastUpdate->setStyleSheet("");
		lastLagState=true;
	}
	if(lastL!=lastLagState)
	{
		checkValidSellButtons();
		checkValidBuyButtons();
		ui.ordersControls->setEnabled(lastLagState);
		ui.buyButtonBack->setEnabled(lastLagState);
		ui.sellButtonBack->setEnabled(lastLagState);
		QString toolTip;
		if(!lastLagState)toolTip="API Lag is to High";

		ui.ordersControls->setToolTip(toolTip);
		ui.buyButtonBack->setToolTip(toolTip);
		ui.sellButtonBack->setToolTip(toolTip);

		if(lastLagState)checkAllRules();
	}
}

void QtBitcoinTrader::checkAllRules()
{
	marketBuyChanged(ui.marketBuy->value());
	marketSellChanged(ui.marketSell->value());
	marketLowChanged(ui.marketLow->value());
	marketHighChanged(ui.marketHigh->value());
	marketLastChanged(ui.marketLast->value());
	ordersLastBuyPriceChanged(ui.ordersLastBuyPrice->value());
	ordersLastSellPriceChanged(ui.ordersLastSellPrice->value());
}

void QtBitcoinTrader::sellTotalBtcToSellChanged(double)
{
	if(sellLockBtcToSell)return;
	sellLockBtcToSell=true;

	sellLockAmountToReceive=true;
	ui.sellAmountToReceive->setValue(ui.sellPricePerCoin->value()*ui.sellTotalBtc->value()*floatFeeDec);

	sellLockAmountToReceive=false;

	checkValidSellButtons();
	sellLockBtcToSell=false;
}

void QtBitcoinTrader::sellPricePerCoinInUsdChanged(double)
{
	if(!sellLockPricePerCoin)
	{
	sellLockPricePerCoin=true;
	sellTotalBtcToSellChanged(ui.sellTotalBtc->value());
	sellLockPricePerCoin=false;
	}
	ui.sellNextMaxBuyPrice->setValue(ui.sellPricePerCoin->value()*floatFeeDec*floatFeeDec);
	ui.sellNextMaxBuyStep->setValue(ui.sellPricePerCoin->value()-ui.sellNextMaxBuyPrice->value());
	checkValidSellButtons();
}

void QtBitcoinTrader::sellAmountToReceiveChanged(double val)
{
	ui.profitLossSpinBox->setValue(ui.sellAmountToReceive->value()-ui.buyTotalSpend->value());
	if(sellLockAmountToReceive)return;
	sellLockAmountToReceive=true;

	sellLockBtcToSell=true;
	sellLockPricePerCoin=true;

	ui.sellTotalBtc->setValue(val/ui.sellPricePerCoin->value()/floatFeeDec);

	sellLockPricePerCoin=false;
	sellLockBtcToSell=false;

	sellLockAmountToReceive=false;
	checkValidSellButtons();
}

void QtBitcoinTrader::sellBitcoinButton()
{
	checkValidSellButtons();
	if(ui.sellBitcoinsButton->isEnabled()==false)return;

	if(confirmBuySell)
	{
	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Question);
	msgBox.setWindowTitle("Please confirm transaction");
	msgBox.setText("Are you sure to sell "+bitcoinSign+" "+ui.sellTotalBtc->text()+" at "+currencySign+" "+ui.sellPricePerCoin->text()+" ?\n\nNote: If total orders amount of your Bitcoins exceeds your balance, Mt.Gox will remove this order immediately.");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if(msgBox.exec()!=QMessageBox::Yes)return;
	}

	apiSell(ui.sellTotalBtc->value(),ui.sellPricePerCoin->value());
}

void QtBitcoinTrader::buyTotalToSpendInUsdChanged(double)
{
	ui.profitLossSpinBox->setValue(ui.sellAmountToReceive->value()-ui.buyTotalSpend->value());
	if(buyLockTotalSpend)return;
	buyLockTotalSpend=true;

	buyLockTotalBtc=true;
	ui.buyTotalBtc->setValue(ui.buyTotalSpend->value()/ui.buyPricePerCoin->value());
	buyLockTotalBtc=false;

	buyLockTotalSpend=false;
	checkValidBuyButtons();
}


void QtBitcoinTrader::buyBtcToBuyChanged(double val)
{
	ui.buyTotalBtcResult->setValue(val*floatFeeDec);
	if(buyLockTotalBtc)return;
	buyLockTotalBtc=true;

	buyLockTotalSpend=true;
	ui.buyTotalSpend->setValue(ui.buyTotalBtc->value()*ui.buyPricePerCoin->value());
	buyLockTotalSpend=false;

	buyLockTotalBtc=false;
	checkValidBuyButtons();
}

void QtBitcoinTrader::buyPricePerCoinChanged(double)
{
	if(!buyLockPricePerCoin)
	{
	buyLockPricePerCoin=true;
	buyTotalToSpendInUsdChanged(ui.buyTotalSpend->value());
	buyLockPricePerCoin=false;
	}
	ui.buyNextInSellPrice->setValue(ui.buyPricePerCoin->value()*floatFeeInc*floatFeeInc);
	ui.buyNextMinBuyStep->setValue(ui.buyNextInSellPrice->value()-ui.buyPricePerCoin->value());
	checkValidBuyButtons();
}

void QtBitcoinTrader::checkValidBuyButtons()
{
	ui.buyBitcoinsButton->setEnabled(ui.buyTotalBtc->value()>=0.01);
}


void QtBitcoinTrader::checkValidRulesButtons()
{
	bool isTableSelected=ui.rulesTable->selectedItems().count();
	ui.ruleSelectNone->setEnabled(isTableSelected);
	ui.ruleRemove->setEnabled(isTableSelected);
	ui.ruleRemoveAll->setEnabled(ui.rulesTable->rowCount());

	ui.currencyComboBox->setEnabled(ui.rulesTable->rowCount()==0);
}

void QtBitcoinTrader::buyBtcToBuyAllIn()
{
	ui.buyTotalSpend->setValue(ui.accountUSD->value());
}

void QtBitcoinTrader::buyBtcToBuyHalfIn()
{
	ui.buyTotalSpend->setValue(ui.accountUSD->value()/2.0);
}

void QtBitcoinTrader::buyPricePerCoinAsMarketPrice()
{
	ui.buyPricePerCoin->setValue(ui.marketSell->value());
}

void QtBitcoinTrader::ordersSelectionChanged()
{
	ui.ordersCancelAllButton->setEnabled(ui.ordersTable->rowCount());
	ui.ordersSelectNone->setEnabled(ui.ordersTable->selectedItems().count());
	ui.ordersCancelSelected->setEnabled(ui.ordersSelectNone->isEnabled());
}

void QtBitcoinTrader::closeEvent(QCloseEvent *event)
{
	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Question);
	msgBox.setWindowTitle("Qt Bitcoin Trader");
	msgBox.setText("Are you sure to close Application?\nActive rules works only while application is running.\nNote: rules table will be cleared.");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if(msgBox.exec()!=QMessageBox::Yes){event->ignore();return;}
	emit quit();
	event->accept();
}

void QtBitcoinTrader::apiSell(double apiBtcToSell, double apiPriceToSell)
{
	QByteArray params="&type=ask&amount_int="+QByteArray::number(apiBtcToSell*100000000,'f',0)+"&price_int="+QByteArray::number(apiPriceToSell*100000,'f',0);
	socketThreadAuth->sendToApi("BTC"+currencyStr+"/money/order/add",params);
	if(isLogEnabled)logThread->writeLog("BTC"+currencyStr+"/money/order/add?nonce=*"+params);

}

void QtBitcoinTrader::apiBuy(double apiBtcToBuy, double apiPriceToBuy)
{
	QByteArray params="&type=bid&amount_int="+QByteArray::number(apiBtcToBuy*100000000,'f',0)+"&price_int="+QByteArray::number(apiPriceToBuy*100000,'f',0);
	socketThreadAuth->sendToApi("BTC"+currencyStr+"/money/order/add",params);
	if(isLogEnabled)logThread->writeLog("BTC"+currencyStr+"/money/order/add?nonce=*"+params);
}

void QtBitcoinTrader::buyBitcoinsButton()
{
	checkValidBuyButtons();
	if(ui.buyBitcoinsButton->isEnabled()==false)return;
	if(confirmBuySell)
	{
	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Question);
	msgBox.setWindowTitle("Please confirm new order");
	msgBox.setText("Are you sure to buy "+bitcoinSign+" "+ui.buyTotalBtc->text()+" at "+currencySign+" "+ui.buyPricePerCoin->text()+" ?\n\nNote: If total orders amount of your funds exceeds your balance, Mt.Gox will remove this order immediately.");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if(msgBox.exec()!=QMessageBox::Yes)return;
	} 
	apiBuy(ui.buyTotalBtc->value(),ui.buyPricePerCoin->value());
}

void QtBitcoinTrader::updateLogTable()
{
	if(updateLogTime.elapsed()<1000)return;
	socketThreadAuth->sendToApi("money/wallet/history","&currency=BTC");
	if(isLogEnabled)logThread->writeLog("money/wallet/history?nonce=*&currency=BTC");
	updateLogTime.restart();
}

void QtBitcoinTrader::copyDonateButton()
{
	QApplication::clipboard()->setText(ui.bitcoinAddress->text());
	QMessageBox::information(this,"Qt Bitcoin Trader","Bitcoin address copied to clipboard.\nThank you for support!");
}

void QtBitcoinTrader::postWorkAtTableItem(QTableWidgetItem *item)
{
	item->setTextAlignment(Qt::AlignCenter);
	item->setToolTip(item->text());
}

void QtBitcoinTrader::ruleAddButton()
{
	AddRuleWindow addRule;
	if(addRule.exec()!=QDialog::Accepted)return;

	RuleHolder newRuleHolder=addRule.getRuleHolder();

	int curRow=ui.rulesTable->rowCount();
	ui.rulesTable->setRowCount(curRow+1);
	ui.rulesTable->setRowHeight(curRow,30);
	ui.rulesTable->setItem(curRow,0,new QTableWidgetItem());postWorkAtTableItem(ui.rulesTable->item(curRow,0));ui.rulesTable->item(curRow,0)->setData(Qt::UserRole,newRuleHolder.getRuleGuid());
	ui.rulesTable->setItem(curRow,1,new QTableWidgetItem(addRule.getDescriptionString()));postWorkAtTableItem(ui.rulesTable->item(curRow,1));
	ui.rulesTable->setItem(curRow,2,new QTableWidgetItem(addRule.getSellOrBuy()));postWorkAtTableItem(ui.rulesTable->item(curRow,2));
	ui.rulesTable->setItem(curRow,3,new QTableWidgetItem(addRule.getBitcoinsString()));postWorkAtTableItem(ui.rulesTable->item(curRow,3));
	ui.rulesTable->setItem(curRow,4,new QTableWidgetItem(addRule.getPrice()));postWorkAtTableItem(ui.rulesTable->item(curRow,4));
	setRulesTableRowState(curRow,1);
	switch(addRule.getRulePriceType())
	{
	case 1: rulesLastPrice<<newRuleHolder; break;
	case 2: rulesMarketBuyPrice<<newRuleHolder; break;
	case 3: rulesMarketSellPrice<<newRuleHolder; break;
	case 4: rulesMarketHighPrice<<newRuleHolder; break;
	case 5: rulesMarketLowPrice<<newRuleHolder; break;
	case 6: rulesOrdersLastBuyPrice<<newRuleHolder; break;
	case 7: rulesOrdersLastSellPrice<<newRuleHolder; break;
	default: break;
	}

	ui.rulesNoMessage->setVisible(false);
	ui.rulesTable->setVisible(true);
	checkValidRulesButtons();
	checkAllRules();
}

void QtBitcoinTrader::ruleRemove()
{
	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Question);
	msgBox.setWindowTitle("Qt Bitcoin Trader");
	msgBox.setText("Are you sure to remove this rule?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if(msgBox.exec()!=QMessageBox::Yes)return;

	for(int n=ui.rulesTable->rowCount()-1;n>=0;n--)
		if(ui.rulesTable->item(n,0)->isSelected())
		{
			removeRuleByGuid(ui.rulesTable->item(n,0)->data(Qt::UserRole).toUInt());
			ui.rulesTable->removeRow(n--);
		}
	checkValidRulesButtons();
}

void QtBitcoinTrader::removeRuleByGuid(uint guid)
{
	if(removeRuleByGuidInRuleHolderList(guid,&rulesLastPrice))return;
	if(removeRuleByGuidInRuleHolderList(guid,&rulesMarketBuyPrice))return;
	if(removeRuleByGuidInRuleHolderList(guid,&rulesMarketSellPrice))return;
	if(removeRuleByGuidInRuleHolderList(guid,&rulesMarketHighPrice))return;
	if(removeRuleByGuidInRuleHolderList(guid,&rulesMarketLowPrice))return;
	if(removeRuleByGuidInRuleHolderList(guid,&rulesOrdersLastBuyPrice))return;
	removeRuleByGuidInRuleHolderList(guid,&rulesOrdersLastSellPrice);
}

bool QtBitcoinTrader::removeRuleByGuidInRuleHolderList(uint guid, QList<RuleHolder> *ruleHolderList)
{
	for(int n=0;n<ruleHolderList->count();n++)
		if((*ruleHolderList)[n].getRuleGuid()==guid)
		{
			ruleHolderList->removeAt(n);
			return true;
		}
	return false;
}

void QtBitcoinTrader::ruleRemoveAll()
{
	QMessageBox msgBox(this);
	msgBox.setIcon(QMessageBox::Question);
	msgBox.setWindowTitle("Qt Bitcoin Trader");
	msgBox.setText("Are you sure to remove all rules?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	if(msgBox.exec()!=QMessageBox::Yes)return;

	for(int n=0;n<ui.rulesTable->rowCount();n++)
		removeRuleByGuid(ui.rulesTable->item(n,0)->data(Qt::UserRole).toUInt());
	ui.rulesTable->clearContents();
	ui.rulesTable->setRowCount(0);
	checkValidRulesButtons();
}

void QtBitcoinTrader::beep()
{
	//QApplication::beep();
	player->beep();
}

void QtBitcoinTrader::accountUSDChanged(double)
{
	if(ui.accountUSDBeep->isChecked())beep();
}

void QtBitcoinTrader::accountBTCChanged(double)
{
	if(ui.accountBTCBeep->isChecked())beep();
}

void QtBitcoinTrader::marketLowChanged(double val)
{
	checkAndExecuteRule(&rulesMarketLowPrice,val);
	if(ui.marketLowBeep->isChecked()&&lastMarketLowPrice!=val)
	{
		lastMarketLowPrice=val;
		beep();
	}
}

void QtBitcoinTrader::marketHighChanged(double val)
{
	checkAndExecuteRule(&rulesMarketHighPrice,val);
	if(ui.marketHighBeep->isChecked()&&lastMarketHighPrice!=val)
	{
		lastMarketHighPrice=val;
		beep();
	}
}

void QtBitcoinTrader::marketBuyChanged(double val)			{checkAndExecuteRule(&rulesMarketBuyPrice,val);}
void QtBitcoinTrader::marketSellChanged(double val)			{checkAndExecuteRule(&rulesMarketSellPrice,val);}
void QtBitcoinTrader::marketLastChanged(double val)			{checkAndExecuteRule(&rulesLastPrice,val);}
void QtBitcoinTrader::ordersLastBuyPriceChanged(double val)	{checkAndExecuteRule(&rulesOrdersLastBuyPrice,val);}
void QtBitcoinTrader::ordersLastSellPriceChanged(double val){checkAndExecuteRule(&rulesOrdersLastSellPrice,val);}

void QtBitcoinTrader::checkAndExecuteRule(QList<RuleHolder> *ruleHolder, double price)
{
	for(int n=0;n<ruleHolder->count();n++)
		if((*ruleHolder)[n].isAchieved(price))
		{
			if(!isValidSoftLag()){(*ruleHolder)[n].startWaitingLowLag();continue;}

			double ruleBtc=(*ruleHolder)[n].getRuleBtc();
			uint ruleGuid=(*ruleHolder)[n].getRuleGuid();
			bool isBuying=(*ruleHolder)[n].isBuying();
			double priceToExec=(*ruleHolder)[n].getRulePrice();
			ruleHolder->removeAt(n--);

			setRuleStateBuGuid(ruleGuid,2);
			if(ruleBtc<0)
			{
				if(ruleBtc==-1.0)ruleBtc=ui.accountBTC->value();
				if(ruleBtc==-2.0)ruleBtc=ui.accountBTC->value()/2.0;
				if(ruleBtc==-3.0)ruleBtc=ui.buyTotalSpend->value()/ui.buyPricePerCoin->value();
				if(ruleBtc==-4.0)ruleBtc=ui.buyTotalSpend->value()/ui.buyPricePerCoin->value()/2.0;
				if(ruleBtc==-5.0){ordersCancelAll();beep();continue;}
			}

			if(priceToExec<0)
			{
				if(priceToExec==-1.0)priceToExec=ui.marketLast->value();
				if(priceToExec==-2.0)priceToExec=ui.marketBuy->value();
				if(priceToExec==-3.0)priceToExec=ui.marketSell->value();
				if(priceToExec==-4.0)priceToExec=ui.marketHigh->value();
				if(priceToExec==-5.0)priceToExec=ui.marketLow->value();
				if(priceToExec==-6.0)priceToExec=ui.ordersLastBuyPrice->value();
				if(priceToExec==-7.0)priceToExec=ui.ordersLastSellPrice->value();
			}			

			if(ruleBtc>=0.01)
			{
			if(isBuying)apiBuy(ruleBtc,priceToExec);
			else apiSell(ruleBtc,priceToExec);
			}
			beep();
		}
}

bool QtBitcoinTrader::isValidSoftLag()
{
	return ui.lastUpdate->value()<2.0;
}

void QtBitcoinTrader::ordersLogGroupBoxToggled(bool on)
{
	ui.ordersLogGroupBox->setMaximumHeight(on?16777215:60);
	ui.logTextEdit->setVisible(on);

	QSettings settings(iniFileName,QSettings::IniFormat);
	settings.setValue("LogExpanded",on);
}

void QtBitcoinTrader::aboutTranslationButton()
{
	(new TempWindow(this))->show();
}

void QtBitcoinTrader::buttonNewWindow()
{
	QProcess proc;
	proc.startDetached(QApplication::applicationFilePath());
    //proc.waitForStarted();
}
