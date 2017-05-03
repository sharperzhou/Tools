#pragma once
#include "stdafx.h"

enum Type
{
	C,  //�����¶�
	F,  //�����¶�
	K,  //�����¶�
	R   //�����¶�
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

