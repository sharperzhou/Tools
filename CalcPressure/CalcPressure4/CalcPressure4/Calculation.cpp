#include "Calculation.h"
#include <memory.h>

Calculation::Calculation(void)
{
	memset(value, 0, 4);
}

void Calculation::calc(PresType type, double input)
{
	switch (type)
	{
	case MPag:
		value[0] = input;
		value[1] = NA;
		if (value[0] < 0)
		{
			value[2] = NA; value[3] = NA;
		}
		else
		{
			value[3] = value[0] * 1000 + 101.325;
			value[2] = value[3] / 101.325 * 760;
		}
		break;
	case kPag:
		value[1] = input;
		value[0] = NA;
		if (value[1] > 0 || value[1] < -101.325)
		{
			value[2] = NA; value[3] = NA;
		}
		else
		{
			value[3] = value[1] + 101.325;
			value[2] = value[3] / 101.325 * 760;
		}
		break;
	case mmHg:
		value[2] = input;
		if (value[2] < 0)
		{
			value[0] = NA; value[1] = NA; value[3] = NA;
		}
		else if (value[2] >= 0 && value[2] < 760)
		{
			value[0] = NA;
			value[3] = value[2] / 760 * 101.325;
			value[1] = value[3] - 101.325;
		}
		else if (value[2] == 760)
		{
			value[0] = 0; value[1] = 0; value[3] = 101.325;
		}
		else
		{
			value[1] = NA;
			value[3] = value[2] / 760 * 101.325;
			value[0] = (value[3] - 101.325) / 1000;
		}
		break;
	case kPa:
		value[3] = input;
		if (value[3] < 0)
		{
			value[0] = NA; value[1] = NA; value[2] = NA;
		}
		else if (value[3] >= 0 && value[3] < 101.325)
		{
			value[0] = NA;
			value[1] = value[3] - 101.325;
			value[2] = value[3] /101.325 * 760;
		}
		else if (value[3] == 101.325)
		{
			value[0] = 0; value[1] = 0; value[2] = 760;
		}
		else
		{
			value[1] = NA;
			value[0] = (value[3] - 101.325) / 1000;
			value[2] = value[3] / 101.325 * 760;
		}
		break;
	default :
		break;
	}
}
