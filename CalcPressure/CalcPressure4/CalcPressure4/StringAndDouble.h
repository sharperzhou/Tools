#ifndef __STRING_AND_DOUBLE__H__
#define __STRING_AND_DOUBLE__H__
#include <tchar.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
���ܣ������ַ�������ֵת���������ַ���ת��Ϊdouble��doubleת��Ϊ�������ַ�����doubleת��ʽ�����ַ�����

a) Type = FixedDigit, digits = 3, e.g. 1.234, 12.345, 1.234e+020, -123.456
b) Type = FixedLength, length = 6, digits = 2, |n| >= 10000 or <= 1e-4 use exponent pattern,
   e.g. 1.23400, 12.3400, 1234.00, 1.23e+004, -1.23e-004, length not include ".", "e", "+", "-"
   required: length - digits > 3 and digits > 0 and length < 17


�Զ���������ͣ�1��ö�ٱ���Type�������趨��ʽ���ַ��������ͣ��̶�����ָС��λ��������ֵ��С�ɱ䣬�̶�С��
                λ�������������뷨��
                2���ṹ������FormatType������ָ���̶����ȼ�������С��λ����

������Ա������1��StrtoD��ʵ���ַ���תdouble������ֵ��ʾת���ɹ������ͬ����������Ϊ�������͡�
              2��DtoFormatStr��ʵ��doubleת��ָ����ʽ���ַ�����
              3��DtoWholeStr��doubleת���������ַ������������һ�£���ౣ��18λ���ȣ��ɸ�����ֵ��С�Զ�
              ת��Ϊ��ѧ������

˽�г�Ա������1��FormatFixedLengthStr���̶����ȸ�ʽ���Ĵ����Ӻ������޷���ֵ��
              2��formatStr������С��λ���ĸ�ʽ���ַ���������sprintf����������ֵΪchar *��

�������ݳ�Ա���ޡ�

˽�����ݳ�Ա��_strfmt[6]������С��λ���ĸ�ʽ���ַ������ñ����ʽ���ַ������ݡ�
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////

class StringAndDouble
{
private:
	StringAndDouble(void);
	~StringAndDouble(void);
	StringAndDouble &operator=(const StringAndDouble &); //���ø�ֵ����
	StringAndDouble(const StringAndDouble &); //���ù��쿽��
	void FormatFixedLengthStr(double input, TCHAR * output, unsigned int len, unsigned int dgts); //�̶����ȸ�ʽ���Ĵ����Ӻ���
	TCHAR * formatStr(unsigned int dgts); //����С��λ���ĸ�ʽ���ַ���������sprintf����
	TCHAR _strfmt[6]; //����С��λ���ĸ�ʽ���ַ������ñ����ʽ���ַ�������
public:
	enum Type
	{
        FixedDigit,   //�̶�С��λ������������ֵ��С�ɱ�
		FixedLength //�̶����ȣ�С��λ������ֵ��С�ɱ�
	};

	struct FormatType
	{
		Type type;	//ָ�����̶����Ȼ��ǹ̶�С��λ����ʽ����ֵ
		unsigned int length; //ֵ�ĳ��ȣ���������С�����ָ�����֡���TypeΪFixedDigitʱ����Ϊ0
		unsigned int digits; //��TypeΪFixedDigitʱ������ΪС��λ������ΪFixedLengthʱ������Ϊ����С��λ��
	};

	static bool StrtoD(        //�����ַ���תdouble
		const TCHAR *resource,  //������ַ���
		double &result);       //�����double���ͽ��

	static bool DtoFormatStr(   //doubleת��ָ����ʽ���ַ���
		double resource, //�����double����ֵ
		TCHAR *result,           //������ַ������
		FormatType &formattype); //�����������

	static bool DtoWholeStr(    //doubleת���������ַ��������ж��پͱ�������λС��
		double resource, //�����double����ֵ
		TCHAR *result);			//������ַ������
};

#endif  //__STRING_AND_DOUBLE__H__