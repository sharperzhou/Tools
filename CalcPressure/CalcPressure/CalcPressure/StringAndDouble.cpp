#include "stdafx.h"
#include "StringAndDouble.h"

StringAndDouble::StringAndDouble(void)
{
}

StringAndDouble::~StringAndDouble(void)
{
}

bool StringAndDouble::StrtoD(const char *resource, double &result) //字符串转double重载一：char*作为输入
{
	char *pEnd;
	double temp;
	if (! *resource)
	{
		result = DBL_MIN;
		return false;
	}
	temp = strtod(resource, &pEnd);
	if (! *pEnd)
	{
		if (fabs(temp) > DBL_MAX)
		{
			result = (temp > 0)? DBL_MAX: -DBL_MAX;
			return false;
		}
		else if (fabs(temp) < DBL_MIN)
		{
			result = 0;
			return true;
		}
		result = temp;
		return true;
	}
	else
	{
		result = DBL_MIN;
		return false;
	}
}

bool StringAndDouble::StrtoD(const wchar_t *resource, double &result) //字符串转double重载二：wchar_t*作为输入
{
	wchar_t *pEnd;
	double temp;
	if (! *resource)
	{
		result = DBL_MIN;
		return false;
	}
	temp = wcstod(resource, &pEnd);
	if (! *pEnd)
	{
		if (fabs(temp) > DBL_MAX)
		{
			result = (temp > 0)? DBL_MAX: -DBL_MAX;
			return false;
		}
		else if (fabs(temp) < DBL_MIN)
		{
			result = 0;
			return true;
		}
		result = temp;
		return true;
	}
	else
	{
		result = DBL_MIN;
		return false;
	}
}

bool StringAndDouble::StrtoD(const std::string &resource, double &result) //字符串转double重载三：STL string作为输入
{
	bool re = StrtoD(resource.c_str(), result);
	return re;
}

bool StringAndDouble::StrtoD(const CString &resource, double &result) //字符串转double重载四：CString作为输入
{
	CString temp = resource;
	bool re = StrtoD(temp.GetBuffer(temp.GetLength()), result);
	temp.ReleaseBuffer();
	return re;
}

bool StringAndDouble::DtoWholeStr(const double &resource, char *result) //double转完整字符串重载一：char*作为结果
{
	sprintf(result, "%.17g", resource);  //系统默认格式化字符串
	return true;
}

bool StringAndDouble::DtoWholeStr(const double &resource, wchar_t *result) //double转完整字符串重载二：wchar_t*作为结果
{
	swprintf(result, L"%.17g", resource);  //系统默认格式化字符串
	return true;
}

bool StringAndDouble::DtoWholeStr(const double &resource, std::string &result) //double转完整字符串重载三：STL string作为结果
{
	char pstr[25];
	bool re = DtoWholeStr(resource, pstr);
	result = pstr;
	return re;
}

bool StringAndDouble::DtoWholeStr(const double &resource, CString &result) //double转完整字符串重载四：CString作为结果
{
#ifdef UNICODE
	wchar_t pstr[25];
#else
	char pstr[25];
#endif
	bool re = DtoWholeStr(resource, pstr);
	result = pstr;
	return re;
}

bool StringAndDouble::DtoFormatStr(const double &resource, char *result, FormatType &formattype) //double转完指定格式字符串重载一：char*作为结果
{
	//if (formattype.digits > 16 || formattype.length > 16)  //格式化参数不正确时抛异常
	//	throw "OUT_OF_RANGE";
	//if (formattype.digits > formattype.length)
	//	throw "LOGIC_ERROR";

	switch (formattype.type)
	{
	case FixedDigit:
		char fmt[7];
		char tmp[5];
		if (fabs(resource) > 1e16) //超过1e16时以科学计数法显示
			sprintf(tmp, "%de", formattype.digits);
		else
			sprintf(tmp, "%dlf", formattype.digits);
		fmt[0] = '%'; fmt[1] = '.'; fmt[2] = '\0';
		strcat(fmt, tmp);
		if (fabs(resource) < pow((double)10, -(double)formattype.digits)) //小于预设的小数点保留精度内，直接输出零
			sprintf(result, fmt, 0);
		else
			sprintf(result, fmt, resource);
		return true;
	case FixedLength:
		StringAndDouble * p;
		p = new StringAndDouble;
		p->FormatFixedLengthStr(resource, result, formattype.length, formattype.digits);
		delete p;
		return true;
	default:
		result = NULL;
		return false;
	}
}

bool StringAndDouble::DtoFormatStr(const double &resource, wchar_t *result, FormatType &formattype) //double转完指定格式字符串重载二：wchar_t*作为结果
{
	char pstr[25];
	size_t len = sizeof(pstr) / sizeof(char);
	bool re = DtoFormatStr(resource, pstr, formattype);
	size_t converted = 0;
	mbstowcs_s(&converted, result, len, pstr, _TRUNCATE);
	return re;
}

bool StringAndDouble::DtoFormatStr(const double &resource, std::string &result, FormatType &formattype) //double转完指定格式字符串重载三：STL string作为结果
{
	char pstr[25];
	bool re = DtoFormatStr(resource, pstr, formattype);
	result = pstr;
	return re;
}

bool StringAndDouble::DtoFormatStr(const double &resource, CString &result, FormatType &formattype) //double转完指定格式字符串重载四：CString作为结果
{
#ifdef UNICODE
	wchar_t pstr[25];
#else
	char pstr[25];
#endif
	bool re = DtoFormatStr(resource, pstr, formattype);
	result = pstr;
	return re;
}

void StringAndDouble::FormatFixedLengthStr(const double &input, char * output, const unsigned int &len, const unsigned int &dgts)
{
	double index = pow((double)10, (double)(len - dgts));
	if (input >= index) //大数字用科学计数法表示
	{
		double temp = input / index;
		int i = 0;
		while (temp >= 10)
		{
			temp /= 10;
			i++;
		}
		char fmt[12];
		char tmp[10];
		sprintf(tmp, "%dfe+%%03d", dgts + 1);
		fmt[0] = '%'; fmt[1] = '.'; fmt[2] = '\0';
		strcat(fmt, tmp);
		sprintf(output, fmt, temp, i + len - dgts);
		return;
	}
	else if (input >= 1 / index)  //在范围内用可变小数位数表示
	{
		for(int i = len - dgts -1; i > 0 ; i--)
		{
			if (input >= pow((double)10, (double)i))
			{
				sprintf(output, this->formatStr(len - i -1), input);
				return;
			}
		}
		sprintf(output, this->formatStr(len -1), input);
		return;
	}
	else if (input >= DBL_MIN)  //当较小的正数时也用科学计数法表示
	{
		double temp = input * index;
		int i = 0;
		while (temp < 1)
		{
			temp *= 10;
			i++;
		}
		char fmt[12];
		char tmp[10];
		sprintf(tmp, "%dfe-%%03d", dgts + 1);
		fmt[0] = '%'; fmt[1] = '.'; fmt[2] = '\0';
		strcat(fmt, tmp);
		sprintf(output, fmt, temp, i + len - dgts);
		return;
	}
	else if (input >= 0)  //0~DBL_MIN之间很接近0的数值等于0
	{
		sprintf(output, this->formatStr(len -1), input);
		return;
	}
	else  //负数表示方法
	{
		StringAndDouble::FormatFixedLengthStr(-input, output, len, dgts);
		int sz = strlen(output) + 1;
		output[sz] = '\0';
		for (int i = sz -1; i > 0; i--)
		{
			output[i] = output[i - 1];
		}
		output[0] = '-';
	}
}

char * StringAndDouble::formatStr(const unsigned int &dgts)
{
	char tmp[4];
	sprintf(tmp, "%df", dgts);
	_strfmt[0] = '%'; _strfmt[1] = '.'; _strfmt[2] = '\0';
	strcat(_strfmt, tmp);
	return _strfmt;
}
