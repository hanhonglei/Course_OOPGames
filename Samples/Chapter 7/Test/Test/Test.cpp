// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;
class Point	//����Point�������
{
public:
	void InitP(float xx=0, float yy=0)
	{X=xx;Y=yy;}
	void Move(float xOff, float yOff)
	{X+=xOff;Y+=yOff;}
	float GetX() {return X;}
	float GetY() {return Y;}
private:	//˽�����ݳ�Ա
	float X,Y;
};
class Rectangle: public Point  //����������
{
public:	//�������к�����Ա
	void InitR(float x, float y, float w, float h)
	{InitP(x,y);W=w;H=h;
	}//���û��๫�г�Ա����
	float GetH() {return H;}
	float GetW() {return W;}
private:	//����˽�����ݳ�Ա
	float W,H;
};
class Circle: public Point
{
public:
	void InitC(float x, float y, float r)
	{InitP(x,y);R = r;
	}//���û��๫�г�Ա����
	float GetR(){return R;}
private:
	float R;
};
int _tmain(int argc, _TCHAR* argv[])
{
	Rectangle rect;
	rect.InitR(2,3,20,10);
	Circle circle;
	circle.InitC(0, 0, 10);
	Point *p;
	p = &rect;
	cout<< "Rect before move:"
		<<p->GetX()<<','
		<<p->GetY()<<endl;
	p->Move(3, 3);
	cout<< "After move:"
		<< p->GetX()<<','
		<<p->GetY()<<endl;
	p = &circle;
	cout<< "Circle before move"
		<< p->GetX()<<','
		<<p->GetY()<<endl;
	p->Move(3, 3);
	cout<<"After move:"
		<<p->GetX()<<','
		<<p->GetY()<<endl;


	char c;
	cin >> c;
	return 0;
}

