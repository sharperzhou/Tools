#ifndef __CALCULATION_PRESSURE__H__
#define __CALCULATION_PRESSURE__H__

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
    ~Calculation(void) {};
	
};

#endif //__CALCULATION_PRESSURE__H__