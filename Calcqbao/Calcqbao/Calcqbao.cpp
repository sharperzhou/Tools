//#include<stdio.h>
//bool isTrueNum(const double&);
//
//void main(void)
//{	
//	while(true)
//	{
//		printf("����/��֤��/������");
//		double vals[3]={0,0,0};
//		try
//		{
//			scanf("%lf%lf%lf",&vals[0],&vals[1],&vals[2]);
//			if(vals[0]==0||vals[1]==0||vals[2]==0) throw "error1";
//			double wfrsy=vals[0]/vals[1]/vals[2]*10000;
//			double nll=vals[0]/vals[1]/vals[2]*365*100;
//			if (!(isTrueNum(wfrsy) ||isTrueNum(nll))) throw "error2";
//			printf("��������棺%fԪ\t����׼��3������1.25��\n"
//				"���������ʣ�%f%%\t������4.4%%��\n-----------------\n\n",wfrsy,nll);
//		}
//		catch(...)
//		{printf("�������!\n-----------------\n\n");fflush(stdin);}
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
#define SEPARATOR  " /,��;��\\"
using namespace std;
string strline;

int main()
{
	while (true)
	{
		cout<<"����/��֤��/������";
		getline(cin, strline, '\n');  //��ȡ�����ַ���
		char *tmp = new char[strline.length() + 1];   //������ʱ�����������������ַ���
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
			printf("��������棺%fԪ\t����׼��3������1.25��\n"
				"���������ʣ�%f%%\t������4.4%%��\n-----------------\n\n",wfrsy,nll);
		}
		catch(int)
		{printf("�������!\n-----------------\n\n");}
	}
	return 0;
}