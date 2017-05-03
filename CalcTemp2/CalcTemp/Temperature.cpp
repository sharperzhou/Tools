#pragma once
#include "stdafx.h"
#include "Temperature.h"


Temperature::Temperature(double T, Type type):pvalues(0)
{
	this->SetValue(T, type);
}

Temperature::Temperature():valueC(-32767), valueF(-32767), valueK(-32767), valueR(-32767), pvalues(0)
{
}

void Temperature::SetValue(double T, Type type)
{
	if (T == -32767)
	{
		valueC = -32767;
		valueF = -32767;
		valueK = -32767;
		valueR = -32767;
		return;
	}
	switch (type)
	{
	case C:
		valueC = T;
		valueF = valueC * 1.8 + 32;
		valueK = valueC + 273.15;
		valueR = valueC * 1.8 + 32 + 459.67;
		break;
	case F:
		valueF = T;
		valueC = (valueF - 32) / 1.8;
		valueK = (valueF - 32) / 1.8 + 273.15;
		valueR = valueF + 459.67;
		break;
	case K:
		valueK = T;
		valueC = valueK - 273.15;
		valueF = (valueK - 273.15) * 1.8 + 32;
		valueR = (valueK - 273.15) * 1.8 + 32 + 459.67;
		break;
	case R:
		valueR = T;
		valueC = (valueR - 459.67 - 32) / 1.8;
		valueF = valueR - 459.67;
		valueK = (valueR - 459.67 - 32) / 1.8 + 273.15;
		break;
	default:
		break;
	}
}

double Temperature::GetValue(Type type)
{
	switch (type)
	{
	case C:
		return valueC;
	case F:
		return valueF;
	case K:
		return valueK;
	case R:
		return valueR;
	default:
		return -32767;
	}
}

double * Temperature::GetValues()
{
	pvalues = new double[4];
	pvalues[0] = valueC;
	pvalues[1] = valueF;
	pvalues[2] = valueK;
	pvalues[3] = valueR; 
	return pvalues;
}

Temperature::~Temperature(void)
{
	delete [] pvalues;
}
