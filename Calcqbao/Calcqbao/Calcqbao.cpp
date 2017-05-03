//#include<stdio.h>
//bool isTrueNum(const double&);
//
//void main(void)
//{	
//	while(true)
//	{
//		printf("收益/保证金/天数：");
//		double vals[3]={0,0,0};
//		try
//		{
//			scanf("%lf%lf%lf",&vals[0],&vals[1],&vals[2]);
//			if(vals[0]==0||vals[1]==0||vals[2]==0) throw "error1";
//			double wfrsy=vals[0]/vals[1]/vals[2]*10000;
//			double nll=vals[0]/vals[1]/vals[2]*365*100;
//			if (!(isTrueNum(wfrsy) ||isTrueNum(nll))) throw "error2";
//			printf("日万份收益：%f元\t（基准：3，余额保：1.25）\n"
//				"年收益利率：%f%%\t（余额保：4.4%%）\n-----------------\n\n",wfrsy,nll);
//		}
//		catch(...)
//		{printf("计算出错!\n-----------------\n\n");fflush(stdin);}
//	}
//}
//bool isTrueNum(const double& d)
//{
//	return (d<=-1e30 || d>=1e30 || (d>0 && d<1e-30) || (d<0 && d>-1e-30))?false:true;
//}



#include <iostream>
#include <String>
#include <stdio.h>
#define CLS        "cls"
#define SEPARATOR  " /,，;；\\"
using namespace std;
string strline;

int main()
{
	while (true)
	{
		cout<<"收益/保证金/天数：";
		getline(cin, strline, '\n');  //获取整行字符串
		char *tmp = new char[strline.length() + 1];   //创建临时变量，来处理那行字符串
		strcpy(tmp, strline.c_str());
		int i = -1;
		while (tmp[i++])
			tmp[i] = tolower(tmp[i]);

		if (strcmp(tmp, CLS) == 0)
		{
			system("cls");
			continue;
		}

		const char *separator = SEPARATOR;
		char *ptr;
		char *next_token;
		double vals[3] = {0,0,0};
		ptr = strtok_s(tmp, separator, &next_token);
		for (int i = 0; i < sizeof(vals) / sizeof(double); i++)
		{
			if (ptr != NULL)
			{
				vals[i] = atof(ptr);
				ptr = strtok_s(NULL, separator, &next_token);
			}
		}
		delete []tmp;

		try
		{
			if(vals[0]==0||vals[1]==0||vals[2]==0) throw 0;
			double wfrsy=vals[0]/vals[1]/vals[2]*10000;
			double nll=vals[0]/vals[1]/vals[2]*365*100;
			printf("日万份收益：%f元\t（基准：3，余额保：1.25）\n"
				"年收益利率：%f%%\t（余额保：4.4%%）\n-----------------\n\n",wfrsy,nll);
		}
		catch(int)
		{printf("计算出错!\n-----------------\n\n");}
	}
	return 0;
}