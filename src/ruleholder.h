#ifndef RULEHOLDER_H
#define RULEHOLDER_H

#include <QObject>

class RuleHolder
{
public:
	RuleHolder(bool moreLess, double price, double bitcoins, uint guid, bool isBuy);
	bool isAchieved(double price);
	bool isBuying();
	double getRuleBtc(){return ruleBtc;}
	uint getRuleGuid(){return ruleGuid;}
	void startWaitingLowLag();
private:
	bool waitingGoodLag;
	bool buying;
	double ruleBtc;
	uint ruleGuid;
	bool ruleMoreThanPrice;
	double rulePrice;
};

#endif // RULEHOLDER_H
