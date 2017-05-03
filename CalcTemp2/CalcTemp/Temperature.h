#pragma once
#include "stdafx.h"

enum Type
{
	C,  //摄氏温度
	F,  //华氏温度
	K,  //绝对温度
	R   //兰氏温度
};

class Temperature
{
public:
	Temperature(double T, Type type);
	Temperature();
	void SetValue(double T, Type type);
	double GetValue(Type type);
	double * GetValues();
public:
	~Temperature(void);
private:
	double valueC, valueF, valueK, valueR;
	double * pvalues;
};

