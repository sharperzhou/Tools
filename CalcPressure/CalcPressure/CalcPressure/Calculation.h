#pragma once
#define   NA    1e30
#define   EPS   1e-10


enum PresType
{
	MPag,
	kPag,
	mmHg,
	kPa
};


class Calculation
{
public:
	Calculation(void);
	double value[4];
	void calc(PresType type, double input);
public:
	~Calculation(void);
};
