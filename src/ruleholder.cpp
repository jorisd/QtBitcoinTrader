// Copyright (C) 2013 July IGHOR.
// I want to create Bitcoin Trader application that can be configured for any rule and strategy.
// If you want to help me please Donate: 1d6iMwjjNo8ZGYeJBZKXgcgVk9o7fXcjc
// For any questions please use contact form at http://trader.uax.co
// Or send e-mail directly to julyighor@gmail.com
//
// You may use, distribute and copy the Qt Bitcion Trader under the terms of
// GNU General Public License version 3

#include "ruleholder.h"
#include "main.h"

RuleHolder::RuleHolder(int moreLessEqual, double price, double bitcoins, uint guid, bool isBuy, double sellPrice, int rulePriceTp)
{
	invalidHolder=false;
	rulePriceType=rulePriceTp;
	rulePrice=sellPrice;
	ruleCheckPrice=price;
	ruleGuid=guid;
	ruleMoreLessEqual=moreLessEqual;
	ruleBtc=bitcoins;
	buying=isBuy;
	waitingGoodLag=false;
}

bool RuleHolder::isAchieved(double price)
{
	if(price<0.1)return false;
	if(waitingGoodLag)return true;
	if(ruleMoreLessEqual==-1&&ruleCheckPrice>price)return true;
	if(ruleMoreLessEqual==1&&ruleCheckPrice<price)return true;
	if(ruleMoreLessEqual==0&&ruleCheckPrice==price)return true;
	return false;
}

void RuleHolder::startWaitingLowLag()
{
	waitingGoodLag=true;
}

bool RuleHolder::isBuying()
{
	return buying;
}

QString RuleHolder::getDescriptionString()
{
		QString priceStr=currencySign+" "+QString::number(ruleCheckPrice,'f',5);
		if(ruleMoreLessEqual==1)
		{
			if(rulePriceType==1)return julyTr("IF_MARKET_LAST_MORE","If market last price goes more than %1").arg(priceStr);
			if(rulePriceType==2)return julyTr("IF_MARKET_BUY_MORE","If market buy price goes more than %1").arg(priceStr);
			if(rulePriceType==3)return julyTr("IF_MARKET_SELL_MORE","If market sell price goes more than %1").arg(priceStr);
			if(rulePriceType==4)return julyTr("IF_MARKET_HIGH_MORE","If market high price goes more than %1").arg(priceStr);
			if(rulePriceType==5)return julyTr("IF_MARKET_LOW_MORE","If market low price goes more than %1").arg(priceStr);
			if(rulePriceType==6)return julyTr("IF_MARKET_LAST_BUY_MORE","If orders last buy price goes more than %1").arg(priceStr);
			if(rulePriceType==7)return julyTr("IF_MARKET_LAST_SELL_MORE","If orders last sell price goes more than %1").arg(priceStr);
		}
		if(ruleMoreLessEqual==-1)
		{
			if(rulePriceType==1)return julyTr("IF_MARKET_LAST_LESS","If market last price goes less than %1").arg(priceStr);
			if(rulePriceType==2)return julyTr("IF_MARKET_BUY_LESS","If market buy price goes less than %1").arg(priceStr);
			if(rulePriceType==3)return julyTr("IF_MARKET_SELL_LESS","If market sell price goes less than %1").arg(priceStr);
			if(rulePriceType==4)return julyTr("IF_MARKET_HIGH_LESS","If market high price goes less than %1").arg(priceStr);
			if(rulePriceType==5)return julyTr("IF_MARKET_LOW_LESS","If market low price goes less than %1").arg(priceStr);
			if(rulePriceType==6)return julyTr("IF_MARKET_LAST_BUY_LESS","If orders last buy price goes less than %1").arg(priceStr);
			if(rulePriceType==7)return julyTr("IF_MARKET_LAST_SELL_LESS","If orders last sell price goes less than %1").arg(priceStr);
		}
		if(ruleMoreLessEqual==0)
		{
			if(rulePriceType==1)return julyTr("IF_MARKET_LAST_LESS","If market last price equal to %1").arg(priceStr);
			if(rulePriceType==2)return julyTr("IF_MARKET_BUY_LESS","If market buy price equal to %1").arg(priceStr);
			if(rulePriceType==3)return julyTr("IF_MARKET_SELL_LESS","If market sell price equal to %1").arg(priceStr);
			if(rulePriceType==4)return julyTr("IF_MARKET_HIGH_LESS","If market high price equal to %1").arg(priceStr);
			if(rulePriceType==5)return julyTr("IF_MARKET_LOW_LESS","If market low price equal to %1").arg(priceStr);
			if(rulePriceType==6)return julyTr("IF_MARKET_LAST_BUY_LESS","If orders last buy price equal to %1").arg(priceStr);
			if(rulePriceType==7)return julyTr("IF_MARKET_LAST_SELL_LESS","If orders last sell price equal to %1").arg(priceStr);
		}
		return priceStr;
}


QString RuleHolder::getSellOrBuyString()
{
	if(ruleBtc==-5.0)return julyTr("CANCEL_ALL_ORDERS","Cancel All Orders");
	if(buying)return julyTr("BUY","Buy");
	return julyTr("SELL","Sell");
}

QString RuleHolder::getBitcoinsString()
{
	if(ruleBtc==-1.0)return julyTr("SELL_ALL_BTC","Sell All my BTC");
	if(ruleBtc==-2.0)return julyTr("SELL_HALF_BTC","Sell Half my BTC");
	if(ruleBtc==-3.0)return julyTr("SELL_ALL_FUNDS","Spend All my Funds");
	if(ruleBtc==-4.0)return julyTr("SELL_HALF_FUNDS","Spend Half my Funds");
	if(ruleBtc==-5.0)return julyTr("NOT_USED","Not Used");

	return bitcoinSign+" "+QString::number(ruleBtc,'f',8);
}

QString RuleHolder::getPriceText()
{
	if(ruleBtc==-5.0)return julyTr("NOT_USED","Not Used");;

	if(rulePrice==-1.0)return julyTr("AS_MARKET_LAST","Market Last");
	if(rulePrice==-2.0)return julyTr("AS_MARKET_BUY","Market Buy");
	if(rulePrice==-3.0)return julyTr("AS_MARKET_SELL","Market Sell");
	if(rulePrice==-4.0)return julyTr("AS_MARKET_HIGH","Market High");
	if(rulePrice==-5.0)return julyTr("AS_MARKET_LOW","Market Low");
	if(rulePrice==-6.0)return julyTr("AS_ORDERS_LAST_BUY","Orders Last Buy");
	if(rulePrice==-7.0)return julyTr("AS_ORDERS_LAST_SELL","Orders Last Sell");
	if(rulePrice==-8.0)return julyTr("SAME_AS_RULE","Same as Rule Price");
	return currencySign+" "+QString::number(rulePrice,'f',5);
}

