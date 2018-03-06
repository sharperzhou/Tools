#include "StringAndDouble.h"
#include <float.h>
#include <math.h>

#pragma warning(disable: 4996)

StringAndDouble::StringAndDouble(void)
{
}

StringAndDouble::~StringAndDouble(void)
{
}

bool StringAndDouble::StrtoD(const TCHAR *resource, double &result) //�ַ���תdouble����һ��char*��Ϊ����
{
	TCHAR *pEnd;
	double temp;
	if (! *resource)
	{
		result = DBL_MIN;
		return false;
	}
	temp = _tcstod(resource, &pEnd);
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

bool StringAndDouble::DtoWholeStr(double resource, TCHAR *result) //doubleת�����ַ�������һ��char*��Ϊ���
{
	_stprintf(result, _T("%.17g"), resource);  //ϵͳĬ�ϸ�ʽ���ַ���
	return true;
}

bool StringAndDouble::DtoFormatStr(double resource, TCHAR *result, FormatType &formattype) //doubleת��ָ����ʽ�ַ�������һ��char*��Ϊ���
{
	//if (formattype.digits > 16 || formattype.length > 16)  //��ʽ����������ȷʱ���쳣
	//	throw "OUT_OF_RANGE";
	//if (formattype.digits > formattype.length)
	//	throw "LOGIC_ERROR";

	switch (formattype.type)
	{
	case FixedDigit:
		TCHAR fmt[7];
		TCHAR tmp[5];
		if (fabs(resource) > 1e16) //����1e16ʱ�Կ�ѧ��������ʾ
			_stprintf(tmp, _T("%de"), formattype.digits);
		else
			_stprintf(tmp, _T("%dlf"), formattype.digits);
		fmt[0] = _T('%'); fmt[1] = _T('.'); fmt[2] = _T('\0');
		_tcscat(fmt, tmp);
		if (fabs(resource) < pow((double)10, -(double)formattype.digits)) //С��Ԥ���С���㱣�������ڣ�ֱ�������
			_stprintf(result, fmt, 0);
		else
			_stprintf(result, fmt, resource);
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

void StringAndDouble::FormatFixedLengthStr(double input, TCHAR * output, unsigned int len, unsigned int dgts)
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
		TCHAR fmt[12];
		TCHAR tmp[10];
		_stprintf(tmp, _T("%dfe+%%03d"), dgts + 1);
		fmt[0] = _T('%'); fmt[1] = _T('.'); fmt[2] = _T('\0');
		_tcscat(fmt, tmp);
		_stprintf(output, fmt, temp, i + len - dgts);
		return;
	}
	else if (input >= 1 / index)  //�ڷ�Χ���ÿɱ�С��λ����ʾ
	{
		for(int i = len - dgts -1; i > 0 ; i--)
		{
			if (input >= pow((double)10, (double)i))
			{
				_stprintf(output, this->formatStr(len - i -1), input);
				return;
			}
		}
		_stprintf(output, this->formatStr(len -1), input);
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
		TCHAR fmt[12];
		TCHAR tmp[10];
		_stprintf(tmp, _T("%dfe-%%03d"), dgts + 1);
		fmt[0] = _T('%'); fmt[1] = _T('.'); fmt[2] = _T('\0');
		_tcscat(fmt, tmp);
		_stprintf(output, fmt, temp, i + len - dgts);
		return;
	}
	else if (input >= 0)  //0~DBL_MIN֮��ܽӽ�0����ֵ����0
	{
		_stprintf(output, this->formatStr(len -1), input);
		return;
	}
	else  //������ʾ����
	{
		StringAndDouble::FormatFixedLengthStr(-input, output, len, dgts);
		size_t sz = _tcslen(output) + 1;
		output[sz] = _T('\0');
		for (size_t i = sz -1; i > 0; i--)
		{
			output[i] = output[i - 1];
		}
		output[0] = _T('-');
	}
}

TCHAR * StringAndDouble::formatStr(unsigned int dgts)
{
	TCHAR tmp[4];
	_stprintf(tmp, _T("%df"), dgts);
	_strfmt[0] = _T('%'); _strfmt[1] = _T('.'); _strfmt[2] = _T('\0');
	_tcscat(_strfmt, tmp);
	return _strfmt;
}
