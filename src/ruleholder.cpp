#include "ruleholder.h"
#include <QDebug>

RuleHolder::RuleHolder(bool moreLess, double price, double bitcoins, uint guid, bool isBuy, double sellPrice)
{
	rulePrice=sellPrice;
	ruleCheckPrice=price;
	ruleGuid=guid;
	ruleMoreThanPrice=moreLess;
	ruleBtc=bitcoins;
	buying=isBuy;
	waitingGoodLag=false;
}

bool RuleHolder::isAchieved(double price)
{
	if(price<0.1)return false;
	if(waitingGoodLag)return true;
	if(ruleMoreThanPrice&&ruleCheckPrice<=price)return true;
	if(!ruleMoreThanPrice&&ruleCheckPrice>=price)return true;
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