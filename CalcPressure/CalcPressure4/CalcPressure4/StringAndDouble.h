#ifndef __STRING_AND_DOUBLE__H__
#define __STRING_AND_DOUBLE__H__
#include <tchar.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
功能：用于字符串与数值转换，包括字符串转换为double，double转换为完整的字符串，double转格式化的字符串。

a) Type = FixedDigit, digits = 3, e.g. 1.234, 12.345, 1.234e+020, -123.456
b) Type = FixedLength, length = 6, digits = 2, |n| >= 10000 or <= 1e-4 use exponent pattern,
   e.g. 1.23400, 12.3400, 1234.00, 1.23e+004, -1.23e-004, length not include ".", "e", "+", "-"
   required: length - digits > 3 and digits > 0 and length < 17


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
	void FormatFixedLengthStr(double input, TCHAR * output, unsigned int len, unsigned int dgts); //固定长度格式化的处理子函数
	TCHAR * formatStr(unsigned int dgts); //保留小数位数的格式化字符串，用于sprintf函数
	TCHAR _strfmt[6]; //保留小数位数的格式化字符串，用保存格式化字符串内容
public:
	enum Type
	{
        FixedDigit,   //固定小数位数，长度随数值大小可变
		FixedLength //固定长度，小数位数随数值大小可变
	};

	struct FormatType
	{
		Type type;	//指定按固定长度还是固定小数位数格式化数值
		unsigned int length; //值的长度，但不包含小数点和指数部分。当Type为FixedDigit时该项为0
		unsigned int digits; //当Type为FixedDigit时，该项为小数位数；当为FixedLength时，该项为最少小数位数
	};

	static bool StrtoD(        //任意字符串转double
		const TCHAR *resource,  //输入的字符串
		double &result);       //输出的double类型结果

	static bool DtoFormatStr(   //double转成指定格式的字符串
		double resource, //输入的double类型值
		TCHAR *result,           //输出的字符串结果
		FormatType &formattype); //设置输出类型

	static bool DtoWholeStr(    //double转成完整的字符串，该有多少就保留多少位小数
		double resource, //输入的double类型值
		TCHAR *result);			//输出的字符串结果
};

#endif  //__STRING_AND_DOUBLE__H__