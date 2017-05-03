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
功能：用于字符串（char*, wchar_t*, STL string, CString）与数值转换，包括字符串转换为double，double转换为
	  完整的字符串，double转格式化的字符串。

自定义变量类型：1、枚举变量Type，用于设定格式化字符串的类型，固定长度指小数位数根据数值大小可变，固定小数
				位数根据四舍五入法。
			    2、结构体类型FormatType，用于指定固定长度及保留的小数位数。

公开成员函数：1、StrtoD，实现字符串转double，返回值标示转换成功与否（下同），输出结果为引用类型。
			  2、DtoFormatStr，实现double转成指定格式的字符串。
			  3、DtoWholeStr，double转成完整的字符串，与编译器一致，最多保留18位长度，可根据数值大小自动
			  转换为科学计数。

私有成员函数：1、FormatFixedLengthStr，固定长度格式化的处理子函数，无返回值。
			  2、formatStr，保留小数位数的格式化字符串，用于sprintf函数，返回值为char *。

公开数据成员：无。

私有数据成员：_strfmt[6]，保留小数位数的格式化字符串，用保存格式化字符串内容。
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////

class StringAndDouble
{
private:
	StringAndDouble(void);
	~StringAndDouble(void);
	StringAndDouble &operator=(const StringAndDouble &); //禁用赋值拷贝
	StringAndDouble(const StringAndDouble &); //禁用构造拷贝
	void FormatFixedLengthStr(const double &input, char * output, const unsigned int &len, const unsigned int &dgts); //固定长度格式化的处理子函数
	char * formatStr(const unsigned int &dgts); //保留小数位数的格式化字符串，用于sprintf函数
	char _strfmt[6]; //保留小数位数的格式化字符串，用保存格式化字符串内容
public:
	enum Type
	{
		FixedLength, //固定长度，小数位数随数值大小可变
		FixedDigit   //固定小数位数，长度随数值大小可变
	};

	struct FormatType
	{
		Type type;	//指定按固定长度还是固定小数位数格式化数值
		unsigned int length; //值的长度，但不包含小数点和指数部分。当Type为FixedDigit时该项为0
		unsigned int digits; //当Type为FixedDigit时，该项为小数位数；当为FixedLength时，该项为最少小数位数
	};

	static bool StrtoD(        //任意字符串转double
		const char *resource,  //输入的字符串
		double &result);       //输出的double类型结果

	static bool StrtoD(
		const wchar_t *resource,
		double &result);

	static bool StrtoD(
		const std::string &resource,
		double &result);

	static bool StrtoD(
		const CString &resource,
		double &result);
		

	static bool DtoFormatStr(   //double转成指定格式的字符串
		const double &resource, //输入的double类型值
		char *result,           //输出的字符串结果
		FormatType &formattype); //设置输出类型

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


	static bool DtoWholeStr(    //double转成完整的字符串，该有多少就保留多少位小数
		const double &resource, //输入的double类型值
		char *result);			//输出的字符串结果
	
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
