#include "stdafx.h"
#include "StringAndDouble.h"

StringAndDouble::StringAndDouble(void)
{
}

StringAndDouble::~StringAndDouble(void)
{
}

bool StringAndDouble::StrtoD(const char *resource, double &result)
{
	char *pEnd;
	double temp;
	if (! *resource)
	{
		result = -32767;
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
		result = -32767;
		return false;
	}
}

bool StringAndDouble::StrtoD(const wchar_t *resource, double &result)
{
	wchar_t *pEnd;
	double temp;
	if (! *resource)
	{
		result = -32767;
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
		result = -32767;
		return false;
	}
}

bool StringAndDouble::DtoWholeStr(const double &resource, char *result)
{
	sprintf(result, "%.15g", resource);  //ϵͳĬ�ϸ�ʽ���ַ���
	return true;
}

bool StringAndDouble::DtoWholeStr(const double &resource, wchar_t *result)
{
	swprintf(result, L"%.15g", resource);  //ϵͳĬ�ϸ�ʽ���ַ���
	return true;
}

bool StringAndDouble::DtoFormatStr(const double &resource, char *result, StringAndDouble::FormatType &formattype)
{
	//if (formattype.digits > 16 || formattype.length > 16)  //��ʽ����������ȷʱ���쳣
	//	throw "OUT_OF_RANGE";
	//if (formattype.digits > formattype.length)
	//	throw "LOGIC_ERROR";

	switch (formattype.type)
	{
	case FixedDigit:
		char fmt[7];
		char tmp[5];
		if (fabs(resource) > 1e16) //����1e16ʱ�Կ�ѧ��������ʾ
			sprintf(tmp, "%de", formattype.digits);
		else
			sprintf(tmp, "%dlf", formattype.digits);
		fmt[0] = '%'; fmt[1] = '.'; fmt[2] = '\0';
		strcat(fmt, tmp);
		if (fabs(resource) < pow((double)10, -(double)formattype.digits)) //С��Ԥ���С���㱣�������ڣ�ֱ�������
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

bool StringAndDouble::DtoFormatStr(const double &resource, wchar_t *result, StringAndDouble::FormatType &formattype)
{
	char pstr[25];
	size_t len = sizeof(pstr) / sizeof(char);
	bool re = DtoFormatStr(resource, pstr, formattype);
	size_t converted = 0;
	mbstowcs_s(&converted, result, len, pstr, _TRUNCATE);
	return re;
}

void StringAndDouble::FormatFixedLengthStr(const double &input, char * output, const unsigned int &len, const unsigned int &dgts)
{
	double index = pow((double)10, (double)(len - dgts));
	if (input >= index) //�������ÿ�ѧ��������ʾ
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
	else if (input >= 1 / index)  //�ڷ�Χ���ÿɱ�С��λ����ʾ
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
	else if (input >= DBL_MIN)  //����С������ʱҲ�ÿ�ѧ��������ʾ
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
	else if (input >= 0)  //0~DBL_MIN֮��ܽӽ�0����ֵ����0
	{
		sprintf(output, this->formatStr(len -1), input);
		return;
	}
	else  //������ʾ����
	{
		StringAndDouble::FormatFixedLengthStr(-input, output, len, dgts);
		size_t sz = strlen(output) + 1;
		output[sz] = '\0';
		for (size_t i = sz -1; i > 0; i--)
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
