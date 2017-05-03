#pragma once
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
���ܣ������ַ�����char*, wchar_t*, STL string, CString������ֵת���������ַ���ת��Ϊdouble��doubleת��Ϊ
	  �������ַ�����doubleת��ʽ�����ַ�����

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
	void FormatFixedLengthStr(const double &input, char * output, const unsigned int &len, const unsigned int &dgts); //�̶����ȸ�ʽ���Ĵ����Ӻ���
	char * formatStr(const unsigned int &dgts); //����С��λ���ĸ�ʽ���ַ���������sprintf����
	char _strfmt[6]; //����С��λ���ĸ�ʽ���ַ������ñ����ʽ���ַ�������
public:
	enum Type
	{
		FixedLength, //�̶����ȣ�С��λ������ֵ��С�ɱ�
		FixedDigit   //�̶�С��λ������������ֵ��С�ɱ�
	};

	struct FormatType
	{
		Type type;	//ָ�����̶����Ȼ��ǹ̶�С��λ����ʽ����ֵ
		unsigned int length; //ֵ�ĳ��ȣ���������С�����ָ�����֡���TypeΪFixedDigitʱ����Ϊ0
		unsigned int digits; //��TypeΪFixedDigitʱ������ΪС��λ������ΪFixedLengthʱ������Ϊ����С��λ��
	};

	static bool StrtoD(        //�����ַ���תdouble
		const char *resource,  //������ַ���
		double &result);       //�����double���ͽ��

	static bool StrtoD(
		const wchar_t *resource,
		double &result);

	static bool StrtoD(
		const std::string &resource,
		double &result);

	static bool StrtoD(
		const CString &resource,
		double &result);
		

	static bool DtoFormatStr(   //doubleת��ָ����ʽ���ַ���
		const double &resource, //�����double����ֵ
		char *result,           //������ַ������
		FormatType &formattype); //�����������

	static bool DtoFormatStr(
		const double &resource,
		wchar_t *result,
		FormatType &formattype);

	static bool DtoFormatStr(
		const double &resource,
		std::string &result,
		FormatType &formattype);

	static bool DtoFormatStr(
		const double &resource,
		CString &result,
		FormatType &formattype);


	static bool DtoWholeStr(    //doubleת���������ַ��������ж��پͱ�������λС��
		const double &resource, //�����double����ֵ
		char *result);			//������ַ������
	
	static bool DtoWholeStr(
		const double &resource,
		wchar_t *result);

	static bool DtoWholeStr(
		const double &resource,
		std::string &result);

	static bool DtoWholeStr(
		const double &resource,
		CString &result);
};
