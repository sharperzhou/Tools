#pragma once

class ValueConvert
{
private:
	ValueConvert(void);
	~ValueConvert(void);
	const ValueConvert & operator=(const ValueConvert &);
	ValueConvert(const ValueConvert &);

	static char *cstr;
	static wchar_t *wcstr;
public:
	static double ToDouble(const char *);
	static char * ToChar(const wchar_t *);
};
