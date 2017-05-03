#include "StdAfx.h"
#include "ValueConvert.h"

ValueConvert::ValueConvert(void)
{
}

ValueConvert::~ValueConvert(void)
{
	delete[] cstr;
	delete[] wcstr;
}

double ValueConvert::ToDouble(const char * pchar)
{
	if (*pchar == '\0') return -32767;
	char * endpos;
	double temp = strtod(pchar, &endpos);
	return (*endpos == 0)?temp:-32767;
}

char * ValueConvert::ToChar(const wchar_t * pwchar)
{
	size_t len = (wcslen(pwchar) + 1);
	size_t converted = 0;
	ValueConvert::cstr = new char[len];
	wcstombs_s(&converted, cstr, len, pwchar, _TRUNCATE);
	return cstr;
}

char * ValueConvert::cstr = 0;
wchar_t * ValueConvert::wcstr = 0;
